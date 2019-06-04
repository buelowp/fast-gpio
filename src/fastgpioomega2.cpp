#include <fastgpioomega2.h>

FastGpioOmega2::FastGpioOmega2(void)
{
	// setup the memory address space
	setupAddress(REG_BLOCK_ADDR, REG_BLOCK_SIZE);
}

FastGpioOmega2::~FastGpioOmega2()
{
}

void FastGpioOmega2::setGpioOffset(int gpio)
{
	int mod;
	mod = gpio / 32;
	switch (mod)
	case 0:
		this->ctrlOffset = REGISTER_CTRL0_OFFSET;
		this->dataOffset = REGISTER_DATA0_OFFSET;
		this->dataSetOffset = REGISTER_DSET0_OFFSET;
		this->dataClrOffset = REGISTER_DCLR0_OFFSET;
		break;
	case 1:
		this->ctrlOffset = REGISTER_CTRL1_OFFSET;
		this->dataOffset = REGISTER_DATA1_OFFSET;
		this->dataSetOffset = REGISTER_DSET1_OFFSET;
		this->dataClrOffset = REGISTER_DCLR1_OFFSET;
		break;
	default:
		this->ctrlOffset = REGISTER_CTRL2_OFFSET;
		this->dataOffset = REGISTER_DATA2_OFFSET;
		this->dataSetOffset = REGISTER_DSET2_OFFSET;
		this->dataClrOffset = REGISTER_DCLR2_OFFSET;
		break;
	}
}

int FastGpioOmega2::setDirection(int pinNum, GPIO_Pin_Direction bOutput)
{
	unsigned long int regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;
	// read the current input and output settings
	regVal = readReg(ctrlOffset);
	ONION_DEBUG("Direction setting read: 0x%08lx\n", regVal);

	// set the OE for this pin
	setBit(regVal, gpio, static_case<int>(bOutput));
	ONION_DEBUG("Direction setting write: 0x%08lx\n", regVal);

	// write the new register value
	writeReg(ctrlOffset, regVal);

	return (EXIT_SUCCESS);
}

int FastGpioOmega2::getDirection(int pinNum, GPIO_Pin_Direction &bOutput)
{
	unsigned long int regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;
	// read the current input and output settings
	regVal 	= readReg(ctrlOffset);
	ONION_DEBUG("Direction setting read: 0x%08lx\n", regVal);

	bOutput = static_case<GPIO_Pin_Direction>(getBit(regVal, gpio));

	return (EXIT_SUCCESS);
}

int FastGpioOmega2::set(int pinNum, GPIO_Pin_State value)
{
	unsigned long int 	regAddr;
	unsigned long int 	regVal;
	setGpioOffset(pinNum);
	int gpio = pinNum % 32;

	if (value == 0)	{
		// write to the clear register
		regAddr = dataClrOffset;
	}
	else {
		// write to the set register
		regAddr = dataSetOffset;
	}

	// put the desired pin value into the register 
	regVal = (0x1 << gpio);

	// write to the register
	writeReg (regAddr, regVal);

	return EXIT_SUCCESS;
}

int FastGpioOmega2::read(int pinNum, GPIO_Pin_State &value)
{
	unsigned long int 	regVal;
	setGpioOffset(pinNum);
	int gpio;
	gpio = pinNum % 32;
	// read the current value of all pins
	regVal = readReg (dataOffset);

	// find the value of the specified pin
	value = static_cast<GPIO_Pin_State>(getBit(regVal, gpio));

	return EXIT_SUCCESS;
}

