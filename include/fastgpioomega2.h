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

#ifndef _FAST_GPIO_OMEGA2_H_
#define _FAST_GPIO_OMEGA2_H_

#include <functional>
#include <signal.h>

#include "fastgpio.h"
#include "fastgpiotypes.h"

class FastGpioOmega2 : public FastGPIO {
public:
	FastGpioOmega2(int);
	~FastGpioOmega2();

	void setDirection(GPIO_Pin_Direction) override;
	GPIO_Pin_Direction direction() override;

	void setValue(GPIO_Pin_State) override;
	GPIO_Pin_State value() override;

private:
	int	m_ctrlOffset;
	int	m_dataOffset;
	int m_dataSetOffset;
	int m_dataClrOffset;
	int m_gpio;
	int m_offset;
};


#endif 	// _FAST_GPIO_OMEGA2_H_
