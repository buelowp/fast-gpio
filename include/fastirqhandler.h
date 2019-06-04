#ifndef __FASTIRQHANDLER_H__
#define __FASTIRQHANDLER_H__

#include <functional>
#include <map>
#include <mutex>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fastgpiotypes.h"
#include "fastirqmetadata.h"

/**
 * \class FastIRQHandler
 * Provides a singleton for handling GPIO IRQ events for specified pins
 */
class FastIRQHandler {
public:
	bool set(int, GPIO_Irq_Type, std::function<void()>);		// Pin and Rise/Fall, debounce set to 0
	bool set(int, unsigned long, GPIO_Irq_Type, std::function<void()>); // Pin, debouce value, Rise/Fall
	int clear(int);					// Turn IRQ off for pin
	int interruptsActive() { return m_metadata.size(); }

	FastIRQHandler* instance()
	{
		if (!m_instance)
			m_instance = new FastIRQHandler();

		return m_instance;
	}

private:
	FastIRQHandler();
	~FastIRQHandler();

	void enable();

	std::map<int, FastIRQMetaData*> m_metadata;
	std::mutex m_mutex;

	bool m_enabled;

	FastIRQHandler *m_instance;

    static void * irqThreadRunner(void * unused);
    static void * runIRQ();
    static void irqHandler(int n, siginfo_t *info, void *unused);

    static pthread_t irqThread;
    static sigset_t irqSigset;
    static siginfo_t irqSiginfo;
};
#endif
