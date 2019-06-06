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

#include <fastgpio.h>

FastGPIO::FastGPIO(int pin)
{
    m_fd = 0;
    m_pin = pin;
    m_debounce = 0;
    m_lastMillis = 0;
    m_type = GPIO_IRQ_NONE;
}

FastGPIO::~FastGPIO()
{
}

int FastGPIO::setupAddress(unsigned long int blockBaseAddr, unsigned long int blockSize)
{
	int  m_mfd;
	int page_size;

	if ((m_mfd = open("/dev/mem", O_RDWR)) < 0) {
			onionPrint(ONION_SEVERITY_FATAL, "open(/dev/mem): %s(%d)\n", strerror(errno));
			return EXIT_FAILURE;	// maybe return -1
	}

	m_regAddress = (unsigned long int*)mmap(NULL,
												1024, 
												PROT_READ|PROT_WRITE, 
												MAP_FILE|MAP_SHARED, 
												m_mfd, 
												blockBaseAddr);
	close(m_mfd);
	if (m_regAddress == MAP_FAILED)	{
		onionPrint(ONION_SEVERITY_FATAL, "mmap: %s(%d)\n", strerror(errno), errno);
		return EXIT_FAILURE;	// maybe return -2
	}

	return EXIT_SUCCESS;	// m_regAddress is now populated
}

void FastGPIO::writeReg(unsigned long int registerOffset, unsigned long int value)
{
	onionPrint(ONION_SEVERITY_DEBUG, "%s:%d: Writing register 0x%08lx with data 0x%08lx\n", __FUNCTION__, __LINE__, (m_regAddress + registerOffset), value);

	*(m_regAddress + registerOffset) = value;
}

unsigned long int FastGPIO::readReg(unsigned long int registerOffset)
{
	unsigned long int 	value = 0x0;
	// read the value 
	value = *(m_regAddress + registerOffset);

	onionPrint(ONION_SEVERITY_DEBUG, "%s:%d: Read register 0x%08lx, data: 0x%08lx \n", __FUNCTION__, __LINE__, (m_regAddress + registerOffset), value);

	return(value);
}

// change the value of a single bit
void FastGPIO::setBit(unsigned long int &regval, int bitnum, int value)
{
    onionPrint(ONION_SEVERITY_DEBUG, "%s:%d: Set bit %d of register 0x%08lx to %d\n", __FUNCTION__, __LINE__, bitnum, regval, value); 
	if (value == 1) {
		regval |= (1 << bitnum);
	}
	else {
		regval &= ~(1 << bitnum);
	}
}

// find the value of a single bit
int FastGPIO::getBit(unsigned long int regVal, int bitNum)
{
	return ((regVal >> bitNum) & 0x1);
}
