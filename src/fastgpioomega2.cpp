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

#include <fastgpioomega2.h>

FastGpioOmega2::FastGpioOmega2(int pin) : FastGPIO(pin)
{
	setupAddress(OMEGA2_REG_BLOCK_ADDR, OMEGA2_REG_BLOCK_SIZE);
	m_gpio = pin % 32;
	m_offset = pin / 32;

	switch (m_offset) {
	case 0:
		this->m_ctrlOffset = OMEGA2_REGISTER_CTRL0_OFFSET;
		this->m_dataOffset = OMEGA2_REGISTER_DATA0_OFFSET;
		this->m_dataSetOffset = OMEGA2_REGISTER_DSET0_OFFSET;
		this->m_dataClrOffset = OMEGA2_REGISTER_DCLR0_OFFSET;
		break;
	case 1:
		this->m_ctrlOffset = OMEGA2_REGISTER_CTRL1_OFFSET;
		this->m_dataOffset = OMEGA2_REGISTER_DATA1_OFFSET;
		this->m_dataSetOffset = OMEGA2_REGISTER_DSET1_OFFSET;
		this->m_dataClrOffset = OMEGA2_REGISTER_DCLR1_OFFSET;
		break;
	default:
		this->m_ctrlOffset = OMEGA2_REGISTER_CTRL2_OFFSET;
		this->m_dataOffset = OMEGA2_REGISTER_DATA2_OFFSET;
		this->m_dataSetOffset = OMEGA2_REGISTER_DSET2_OFFSET;
		this->m_dataClrOffset = OMEGA2_REGISTER_DCLR2_OFFSET;
		break;
	}
}

FastGpioOmega2::~FastGpioOmega2()
{
}

void FastGpioOmega2::setDirection(GPIO_Pin_Direction dir)
{
	unsigned long int regVal = readReg(m_ctrlOffset);
	onionPrint(ONION_SEVERITY_DEBUG, "%s:%d: Direction setting read: 0x%08lx\n", __FUNCTION__, __LINE__, regVal);

	// set the OE for this pin
	setBit(regVal, m_gpio, static_cast<int>(dir));
	onionPrint(ONION_SEVERITY_DEBUG, "%s:%d: Direction setting write: 0x%08lx\n", __FUNCTION__, __LINE__, regVal);

	// write the new register value
	writeReg(m_ctrlOffset, regVal);
}

GPIO_Pin_Direction FastGpioOmega2::direction()
{
	unsigned long int regVal;

	// read the current input and output settings
	regVal 	= readReg(m_ctrlOffset);
	onionPrint(ONION_SEVERITY_DEBUG, "%s:%d: Direction setting read: 0x%08lx\n", __FUNCTION__, __LINE__, regVal);

	return static_cast<GPIO_Pin_Direction>(getBit(regVal, m_gpio));
}

void FastGpioOmega2::setValue(GPIO_Pin_State value)
{
	unsigned long int regAddr;
	unsigned long int regVal;

	if (value == 0)	{
		// write to the clear register
		regAddr = m_dataClrOffset;
	}
	else {
		// write to the set register
		regAddr = m_dataSetOffset;
	}

	// put the desired pin value into the register 
	regVal = (0x1 << m_gpio);

	// write to the register
	writeReg (regAddr, regVal);
}

GPIO_Pin_State FastGpioOmega2::value()
{
	unsigned long int regVal = readReg(m_dataOffset);

	// find the value of the specified pin
	return static_cast<GPIO_Pin_State>(getBit(regVal, m_gpio));
}

