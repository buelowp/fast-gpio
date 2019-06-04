#include <fastirqhandler.h>

FastIRQHandler::FastIRQHandler()
{
}

FastIRQHandler::~FastIRQHandler()
{
}

void FastIRQHandler::irqHandler(int n, siginfo_t *info, void *unused)
{
    int pin = -1;
    int val = 0;
    GPIO_Irq_Type type;

    if (!m_enabled) {
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

    auto cbk_it = m_
}

int clear(int pin)
{
	int fd;

	std::lock_guard<std::mutex> guard(m_mutex);

	if (m_eventHandler.find(pin) == m_eventHandler.end()) {
		ONION_DEBUG("Pin %d is not active", pin);
		return m_eventHandler.size();
	}

    if ((fd = open("/sys/kernel/debug/gpio-irq", O_WRONLY)) > 0) {
        sprintf(buf, "- %d %i", pinNum, getpid());

         if (write(fd, buf, strlen(buf) + 1) > 0) {
             auto it = m_eventHandler.find(pin);
             m_eventHandler.erase(it);
             it = m_irqType.find(pin);
             m_irqType.erase(it);
             it = m_debouce.find(pin);
             m_debouce.erase(it);
         }
         else {
        	 ONION_ERROR("Error writing to gpio-irq: %d", errno);
        	 return m_eventHandler.size();
         }
    }
    else {
    	ONION_ERROR("Error opening gpio-irq: %d", errno);
    }

    return m_eventHandler.size();
}

bool FastIRQHandler::set(int pin, unsigned long debounce, GPIO_Irq_Type type, std::function<void()> handler)
{
	int fd;

	std::lock_guard<std::mutex> guard(m_mutex);

	if (m_eventHandler.find(pin) != m_eventHandler.end()) {
		ONION_DEBUG("Pin %d is already active, cancel first", pin);
		return false;
	}

	if (!m_enabled)
		enable();

	if ((fd = open("/sys/kernel/debug/gpio-irq", O_WRONLY)) > 0) {
		sprintf(buf, "+ %d %i", pinNum, getpid());
		if (write(fd, buf, strlen(buf) + 1) < 0) {
			ONION_ERROR("Error enabling IRQ in sysfs: %d", errno);
			close(fd);
			return false;
		}

		FastIRQMetaData *md = new FastIRQMetaData(type, debounce, handler);
		m_metadata.insert(std::make_pair(pin, md));
	}
	return true;
}

bool FastIRQHandler::set(int pin, GPIO_Irq_Type type, std::function<void()> handler)
{
	return set(pin, 0L, type, handler);
}

void FastIRQHandler::enable()
{
    struct sigaction sig;
    sig.sa_sigaction = FastIRQHandler::irqHandler;

    sig.sa_flags = SA_SIGINFO | SA_NODEFER;
    sigaction(SIG_GPIO_IRQ, &sig, NULL);

    sigemptyset(&irqSigset);

    sigprocmask(SIG_BLOCK, &irqSigset, NULL);
    m_enabled = true;
}
