#include <fastgpio.h>

FastGPIO::FastGPIO()
{
}

FastGPIO::~FastGPIO()
{
}

int FastGPIO::setupAddress(unsigned long int blockBaseAddr, unsigned long int blockSize)
{
	int  m_mfd;
	int page_size;

	if ((m_mfd = open("/dev/mem", O_RDWR)) < 0) {
			onionPrint(ONION_SEVERITY_FATAL, "Unable to open /dev/mem: %d", errno);
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
		onionPrint(ONION_SEVERITY_FATAL, "Error mapping GPIO memory: %d", errno);
		return EXIT_FAILURE;	// maybe return -2
	}

	return EXIT_SUCCESS;	// m_regAddress is now populated
}

void FastGPIO::writeReg(unsigned long int registerOffset, unsigned long int value)
{
	onionPrint(ONION_SEVERITY_DEBUG, "Writing register 0x%08lx with data 0x%08lx \n", (m_regAddress + registerOffset), value);

	*(m_regAddress + registerOffset) = value;
}

unsigned long int FastGPIO::readReg(unsigned long int registerOffset)
{
	unsigned long int 	value = 0x0;
	// read the value 
	value = *(m_regAddress + registerOffset);

	onionPrint(ONION_SEVERITY_DEBUG, "Read register 0x%08lx, data: 0x%08lx \n", (m_regAddress + registerOffset), value);

	return(value);
}

// change the value of a single bit
void FastGPIO::setBit(unsigned long int &regVal, int bitNum, int value)
{
	if (value == 1) {
		regVal |= (1 << bitNum);
	}
	else {
		regVal &= ~(1 << bitNum);
	}

	// try this out
	// regVal ^= (-value ^ regVal) & (1 << bitNum);
}

// find the value of a single bit
int FastGPIO::getBit(unsigned long int regVal, int bitNum)
{
	int value;

	// isolate the specific bit
	value = ((regVal >> bitNum) & 0x1);

	return (value);
}
