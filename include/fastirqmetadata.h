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

#ifndef __FASTGPIOIRQMETADATA_H__
#define __FASTGPIOIRQMETADATA_H__

#include <functional>

#include <signal.h>
#include <sys/time.h>

#include "fastgpiotypes.h"

class FastIRQMetaData {
public:
	FastIRQMetaData()
	{
		m_type = GPIO_IRQ_NONE;
		m_debounce = 0;
		m_lastMillis = 0;
        m_fd = -1;
	}
	FastIRQMetaData(GPIO_Irq_Type t, unsigned long d, std::function<void()> cbk)
	{
		setType(t);
		setDebounce(d);
		setCallback(cbk);
		m_lastMillis = 0;
        m_fd = -1;
	}
	~FastIRQMetaData() {};

	std::function<void()> callback() { return m_callback; }
	void setCallback(std::function<void()> cbk) { m_callback = cbk; }

	GPIO_Irq_Type type() { return m_type; }
	void setType(GPIO_Irq_Type t) { m_type = t; }

	unsigned long debounce() { return m_debounce; }
	void setDebounce(unsigned long d) { m_debounce = d; }

	suseconds_t time() { return m_lastMillis; }
	void setTime(suseconds_t m) { m_lastMillis = m; }
	
	int fd() { return m_fd; }
	void setFd(int f) { m_fd = f; }
	
	int pin() { return m_pin; }
	void setPin(int p) { m_pin = p; }

private:
	std::function<void()> m_callback;
	GPIO_Irq_Type m_type;
	unsigned long m_debounce;
	suseconds_t m_lastMillis;
    int m_fd;
    int m_pin;
};

#endif
