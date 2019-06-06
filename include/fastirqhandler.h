/*
 * Fast GPIO Library for the Onion.io Omega2+ board
 * Copyright (C) 2019  Peter Buelow <goballstate at gmail>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __FASTIRQHANDLER_H__
#define __FASTIRQHANDLER_H__

#include <functional>
#include <map>
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>

#include <signal.h>
#include <onion-debug.h>

#include "fastgpiotypes.h"
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
	bool set(FastGPIO*);		// Pin and Rise/Fall, debounce set to 0
	int clear(int);					// Turn IRQ off for pin
	int interruptsActive() { return m_metadata.size(); }
	bool enabled() { return m_enabled; }
	void setEnabled(bool e) { m_enabled = e; }
	FastGPIO* findPin(int);
	bool checkDebounce(FastGPIO*);
	void start();
    void stop();
    void run();

	static FastIRQHandler* instance()
	{
		static FastIRQHandler instance;

        std::cout << "Handler pointer: " << &instance << std::endl;
		return &instance;
	}

private:
	FastIRQHandler() { m_enabled = false; }
	~FastIRQHandler() {};
	FastIRQHandler& operator=(FastIRQHandler const&) {};
	FastIRQHandler(FastIRQHandler&);

    bool exportPin(int);
    bool unexportPin(int);
    bool gpioEdge(int, GPIO_Irq_Type);
    int gpioValue(int);

	std::map<int, FastGPIO*> m_metadata;
	std::mutex m_mutex;
	std::thread *m_thread;
	bool m_enabled;
};
#endif
