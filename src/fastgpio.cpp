void FastGpio::irqHandler(int, siginfo *info, void*)
{
    int pin = -1;
    int val = 0;
    GPIO_Irq_Type type = GPIO_IRQ_NONE;

    if (!m_irqEnabled) {
        return;
    }

    // Get type of change and pin it applies to
    pin = (info->si_int >> 24) & 0xff;
    val = info->si_int & 0xff;
    if (val == 0) {
        type = GPIO_IRQ_FALLING;
    } else {
        type = GPIO_IRQ_RISING;
        val = 1;
    }

}

void * FastGpio::irqThreadRunner(void * unused) {
    return runIRQ();
}

void * FastGpio::runIRQ() {
    irqRunning = true;
    while (irqRunning) {
        sigwaitinfo(&irqSigset, &irqSiginfo);
    }
    return NULL;
}

