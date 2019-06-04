#include <module.h>

Module::Module(void)
{
	// not verbose by default
	m_verbosityLevel = 0;

	// not in debug mode by default
	m_debuglevel = 0;
}

Module::~Module(void)
{
}

int Module::setupAddress(unsigned long int blockBaseAddr, unsigned long int blockSize)
{
	int  m_mfd;
	int page_size;
	if (m_debuglevel == 0)
	{
		if ((m_mfd = open("/dev/mem", O_RDWR)) < 0)
		{
			ONION_ERROR("Unable to open /dev/mem: %d", errno)
			return EXIT_FAILURE;	// maybe return -1
		}
		m_regAddress = (unsigned long int*)mmap(NULL,
												1024, 
												PROT_READ|PROT_WRITE, 
												MAP_FILE|MAP_SHARED, 
												m_mfd, 
												blockBaseAddr);
		close(m_mfd);
		if (m_regAddress == MAP_FAILED)
		{
			ONION_ERROR("Error mapping GPIO memory: %d", errno);
			return EXIT_FAILURE;	// maybe return -2
		}
	}

	return EXIT_SUCCESS;	// m_regAddress is now populated
}

void Module::writeReg(unsigned long int registerOffset, unsigned long int value)
{
	ONION_DEBUG("Writing register 0x%08lx with data 0x%08lx \n", (m_regAddress + registerOffset), value);

	*(m_regAddress + registerOffset) = value;
}

unsigned long int Module::readReg(unsigned long int registerOffset)
{
	unsigned long int 	value = 0x0;
	// read the value 
	value = *(m_regAddress + registerOffset);

	ONION_DEBUG("Read register 0x%08lx, data: 0x%08lx \n", (m_regAddress + registerOffset), value);

	return(value);
}

// change the value of a single bit
void Module::setBit(unsigned long int &regVal, int bitNum, int value)
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
int Module::getBit(unsigned long int regVal, int bitNum)
{
	int value;

	// isolate the specific bit
	value = ((regVal >> bitNum) & 0x1);

	return (value);
}
