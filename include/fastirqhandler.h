#ifndef __FASTIRQHANDLER_H__
#define __FASTIRQHANDLER_H__

#include <functional>
#include <map>
#include <mutex>
#include <thread>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#include <signal.h>
#include <onion-debug.h>

#include "fastgpiotypes.h"
#include "fastirqmetadata.h"

#ifndef SIG_GPIO_IRQ
  #define SIG_GPIO_IRQ 42
#endif

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
	bool checkDebounce(FastIRQMetaData*);
    void run();
    static void irqHandler(int, siginfo_t*, void*);

	std::map<int, FastIRQMetaData*> m_metadata;
	std::mutex m_mutex;
	std::thread *m_thread;
    sigset_t m_sigset;
    siginfo_t m_siginfo;
	bool m_enabled;

	FastIRQHandler *m_instance;
};
#endif
