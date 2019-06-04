#ifndef _MODULE_H_
#define _MODULE_H_

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

class Module {
public:
	Module(void);
	~Module(void);

protected:
	// protected functions
	int stupAddress(unsigned long int, unsigned long int);
	void writeReg(unsigned long int, unsigned long int);
	unsigned long int readReg(unsigned long int);
	
	void setBit(unsigned long int&, int, int);
	int getBit(unsigned long int, int);

private:
	unsigned long int *m_regAddress;
};

#endif 	// _MODULE_H_
