#include <fastgpioomega2.h>

FastGpioOmega2::FastGpioOmega2(int pin)
{
	setupAddress(REG_BLOCK_ADDR, REG_BLOCK_SIZE);
	m_pin = pin;
	m_gpio = m_pin % 32;
	m_offset = m_pin / 32;

	switch (m_offset) {
	case 0:
		this->m_ctrlOffset = REGISTER_CTRL0_OFFSET;
		this->m_dataOffset = REGISTER_DATA0_OFFSET;
		this->m_dataSetOffset = REGISTER_DSET0_OFFSET;
		this->m_dataClrOffset = REGISTER_DCLR0_OFFSET;
		break;
	case 1:
		this->m_ctrlOffset = REGISTER_CTRL1_OFFSET;
		this->m_dataOffset = REGISTER_DATA1_OFFSET;
		this->m_dataSetOffset = REGISTER_DSET1_OFFSET;
		this->m_dataClrOffset = REGISTER_DCLR1_OFFSET;
		break;
	default:
		this->m_ctrlOffset = REGISTER_CTRL2_OFFSET;
		this->m_dataOffset = REGISTER_DATA2_OFFSET;
		this->m_dataSetOffset = REGISTER_DSET2_OFFSET;
		this->m_dataClrOffset = REGISTER_DCLR2_OFFSET;
		break;
	}
}

FastGpioOmega2::~FastGpioOmega2()
{
}

void FastGpioOmega2::setDirection(GPIO_Pin_Direction dir)
{
	unsigned long int regVal = readReg(m_ctrlOffset);
	onionPrint(ONION_SEVERITY_DEBUG, "Direction setting read: 0x%08lx\n", regVal);

	// set the OE for this pin
	setBit(regVal, m_gpio, static_cast<int>(dir));
	onionPrint(ONION_SEVERITY_DEBUG, "Direction setting write: 0x%08lx\n", regVal);

	// write the new register value
	writeReg(m_ctrlOffset, regVal);
}

GPIO_Pin_Direction FastGpioOmega2::getDirection()
{
	unsigned long int regVal;

	// read the current input and output settings
	regVal 	= readReg(m_ctrlOffset);
	onionPrint(ONION_SEVERITY_DEBUG, "Direction setting read: 0x%08lx\n", regVal);

	return static_cast<GPIO_Pin_Direction>(getBit(regVal, m_gpio));
}

void FastGpioOmega2::set(GPIO_Pin_State value)
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

GPIO_Pin_State FastGpioOmega2::get()
{
	unsigned long int regVal = readReg(m_dataOffset);

	// find the value of the specified pin
	return static_cast<GPIO_Pin_State>(getBit(regVal, m_gpio));
}

