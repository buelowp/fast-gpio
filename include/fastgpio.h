#ifndef _MODULE_H_
#define _MODULE_H_

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
	FastGPIO(void);
	~FastGPIO(void);

	int pin() { return m_pin; }

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

	int m_pin;

private:
	unsigned long int *m_regAddress;
};

#endif 	// _MODULE_H_
