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

#ifndef _MODULE_H_
#define _MODULE_H_

#include <functional>
#include <vector>
#include <map>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <onion-debug.h>

#include "fastgpiotypes.h"

class FastGPIO {
public:
	FastGPIO(int pin);
	~FastGPIO();

	std::function<void()> callback() { return m_callback; }
	void setCallback(std::function<void()> cbk) { m_callback = cbk; }

	suseconds_t time() { return m_lastMillis; }
	void setTime(suseconds_t m) { m_lastMillis = m; }
	
	int fd() { return m_fd; }
	void setFd(int f) { m_fd = f; }
	
	int pin() { return m_pin; }
	void setPin(int p) { m_pin = p; }

	virtual unsigned long debounce() { return m_debounce; }
	virtual void setDebounce(unsigned long d) { m_debounce = d; }
	virtual GPIO_Irq_Type type() { return m_type; }
	virtual void setType(GPIO_Irq_Type t) { m_type = t; }
	virtual void setDirection(GPIO_Pin_Direction) = 0;
	virtual GPIO_Pin_Direction direction() = 0;
	virtual void setValue(GPIO_Pin_State) = 0;
	virtual GPIO_Pin_State value() = 0;

protected:
	int setupAddress(unsigned long int, unsigned long int);

	void writeReg(unsigned long int, unsigned long int);
	unsigned long int readReg(unsigned long int);
	void setBit(unsigned long int&, int, int);
	int getBit(unsigned long int, int);

private:
	unsigned long int *m_regAddress;
   	std::function<void()> m_callback;
	GPIO_Irq_Type m_type;
	unsigned long m_debounce;
	suseconds_t m_lastMillis;
    int m_fd;
    int m_pin;
};

#endif 	// _MODULE_H_
