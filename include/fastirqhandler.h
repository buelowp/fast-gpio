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
#include "fastgpio.h"

#ifndef SIG_GPIO_IRQ
  #define SIG_GPIO_IRQ 42
#endif

/**
 * \class FastIRQHandler
 * Provides a singleton for handling GPIO IRQ events for specified pins
 */
class FastIRQHandler {
public:
	bool set(FastGPIO*, GPIO_Irq_Type, std::function<void()>);		// Pin and Rise/Fall, debounce set to 0
	bool set(FastGPIO*, unsigned long, GPIO_Irq_Type, std::function<void()>); // Pin, debouce value, Rise/Fall
	int clear(int);					// Turn IRQ off for pin
	int interruptsActive() { return m_metadata.size(); }
	bool enabled() { return m_enabled; }
	FastIRQMetaData* findMetaData(int);
	bool checkDebounce(FastIRQMetaData*);

	static FastIRQHandler* instance()
	{
		static FastIRQHandler instance;

		return &instance;
	}

    sigset_t m_sigset;
    siginfo_t m_siginfo;

private:
	FastIRQHandler();
	~FastIRQHandler();
	FastIRQHandler& operator=(FastIRQHandler const&) {};
	FastIRQHandler(FastIRQHandler&);

	void enable();
    void run();

	std::map<int, FastIRQMetaData*> m_metadata;
	std::mutex m_mutex;
	std::thread *m_thread;
	bool m_enabled;
	int m_pin;
};
#endif
