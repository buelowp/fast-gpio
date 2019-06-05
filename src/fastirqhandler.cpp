#include <fastirqhandler.h>

void run()
{
    while (FastIRQHandler::instance()->enabled()) {
        sigwaitinfo(&FastIRQHandler::instance()->m_sigset, &FastIRQHandler::instance()->m_siginfo);
    }
}

void irqHandler(int, siginfo_t *info, void*)
{
    int pin = -1;
    int val = 0;
    GPIO_Irq_Type type;

    if (!FastIRQHandler::instance()->enabled()) {
    	onionPrint(ONION_SEVERITY_DEBUG, "No IRQ has been enabled, this shouldn't happen");
        return;
    }

    pin = (info->si_int >> 24) & 0xff;
    val = info->si_int & 0xff;
    if (val == 0) {
        type = GPIO_IRQ_FALLING;
    }
    else {
        type = GPIO_IRQ_RISING;
        val = 1;
    }

    onionPrint(ONION_SEVERITY_DEBUG, "Got IRQ for pin %d, value %d", pin, val);
    FastIRQMetaData *md = FastIRQHandler::instance()->findMetaData(pin);
    if (md != nullptr) {
    	if (md->type() == type || md->type() == GPIO_IRQ_BOTH) {
       		if (!FastIRQHandler::instance()->checkDebounce(md))
       			return;

       		try {
    			std::function<void()> func = md->callback();
    			func();
    	    } catch(const std::bad_function_call& e) {
    	        onionPrint(ONION_SEVERITY_FATAL, "Exception attempting to execute callback for pin %d", pin);
    	        return;
    	    }
    	}
    }
}

FastIRQHandler::FastIRQHandler()
{
	m_enabled = false;
}

FastIRQHandler::~FastIRQHandler()
{
	m_enabled = false;
}

bool FastIRQHandler::checkDebounce(FastIRQMetaData *md)
{
    timeval timeNow;
    gettimeofday(&timeNow, NULL);
    suseconds_t nowMs = (timeNow.tv_sec * 1000L) + (timeNow.tv_usec / 1000L);

    suseconds_t timeDiff = nowMs - md->time();

    // Don't want it if insufficient time has elapsed for debounce
    if (timeDiff < md->debounce())
        return false;

    md->setTime(nowMs);
    return true;
}

FastIRQMetaData* FastIRQHandler::findMetaData(int pin)
{
	auto it = m_metadata.find(pin);
	if (it != m_metadata.end()){
		return &(*it->second);
	}

	return nullptr;
}

int FastIRQHandler::clear(int pin)
{
	int fd;
	char buf[128];

	std::lock_guard<std::mutex> guard(m_mutex);

	if (m_metadata.find(pin) == m_metadata.end()) {
		onionPrint(ONION_SEVERITY_DEBUG, "Pin %d is not active", pin);
		return m_metadata.size();
	}

    if ((fd = open("/sys/kernel/debug/gpio-irq", O_WRONLY)) > 0) {
        sprintf(buf, "- %d %i", pin, getpid());

         if (write(fd, buf, strlen(buf) + 1) > 0) {
             auto it = m_metadata.find(pin);
             m_metadata.erase(it);
          }
         else {
        	 onionPrint(ONION_SEVERITY_FATAL, "Error writing to gpio-irq: %d", errno);
        	 return m_metadata.size();
         }
    }
    else {
    	onionPrint(ONION_SEVERITY_FATAL, "Error opening gpio-irq: %d", errno);
    }

    if (m_metadata.size() == 0)
    	m_enabled = false;

    return m_metadata.size();
}

bool FastIRQHandler::set(FastGPIO *pin, unsigned long debounce, GPIO_Irq_Type type, std::function<void()> handler)
{
	int fd;
	char buf[128];

	std::lock_guard<std::mutex> guard(m_mutex);

	if (pin->direction() != GPIO_DIRECTION_IN) {
		onionPrint(ONION_SEVERITY_DEBUG, "Pin is set as output, cannot continue");
		return false;
	}

	if (m_metadata.find(pin->pin()) != m_metadata.end()) {
		onionPrint(ONION_SEVERITY_DEBUG, "Pin %d is already active, cancel first", pin);
		return false;
	}

	if (!m_enabled)
		enable();

	if ((fd = open("/sys/kernel/debug/gpio-irq", O_WRONLY)) > 0) {
		sprintf(buf, "+ %d %i", pin->pin(), getpid());
		onionPrint(ONION_SEVERITY_DEBUG, "Writing %s to gpio-irq", buf);
		if (write(fd, buf, strlen(buf) + 1) < 0) {
			onionPrint(ONION_SEVERITY_FATAL, "Error enabling IRQ in sysfs: %d", errno);
			close(fd);
			return false;
		}

		FastIRQMetaData *md = new FastIRQMetaData(type, debounce, handler);
		m_metadata.insert(std::make_pair(pin->pin(), md));
	}
	return true;
}

bool FastIRQHandler::set(FastGPIO *pin, GPIO_Irq_Type type, std::function<void()> handler)
{
	return set(pin, 0L, type, handler);
}

void FastIRQHandler::enable()
{
    struct sigaction sig;
    sig.sa_sigaction = irqHandler;

    sig.sa_flags = SA_SIGINFO | SA_NODEFER;
    sigaction(SIG_GPIO_IRQ, &sig, NULL);

    sigemptyset(&m_sigset);

    sigprocmask(SIG_BLOCK, &m_sigset, NULL);
    std::thread(run);
    m_enabled = true;
}

