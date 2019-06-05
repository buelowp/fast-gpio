#ifndef _FAST_GPIO_OMEGA2_H_
#define _FAST_GPIO_OMEGA2_H_

#include <functional>
#include <signal.h>

#include "fastgpio.h"
#include "fastgpiotypes.h"


//Define Macros in derived class. 
#define REG_BLOCK_ADDR			0x10000000
#define REG_BLOCK_SIZE			0x6AC
//DIRECTION CONTROL REGISTERS

//GPIO_CTRL_0 10000600(Directions for GPIO0-GPIO31)
#define REGISTER_CTRL0_OFFSET		384
//GPIO_CTRL_1 10000604(Directions for GPIO32-GPIO63)
#define REGISTER_CTRL1_OFFSET		385
//GPIO_CTRL_2 10000608(Directions for GPIO64-GPIO95)
#define REGISTER_CTRL2_OFFSET		386

//DATA REGISTERS: STATES OF GPIOS 

//GPIO_DATA_0 10000620(GPIO0-31)
#define REGISTER_DATA0_OFFSET		392
//GPIO_DATA_1 10000624(GPIO32-63)
#define REGISTER_DATA1_OFFSET		393
//GPIO_DATA_2 10000628(GPIO64-95)
#define REGISTER_DATA2_OFFSET		394

//DATA SET REGISTERS: SET STATES OF GPIO_DATA_x registers

//GPIO_DSET_0 10000630(GPIO0-31)
#define REGISTER_DSET0_OFFSET		396
//GPIO_DSET_1 10000634(GPIO31-63)
#define REGISTER_DSET1_OFFSET		397
//GPIO_DSET_2 10000638(GPIO64-95)
#define REGISTER_DSET2_OFFSET		398

//DATA CLEAR REGISTERS: CLEAR BITS OF GPIO_DATA_x registers

//GPIO_DCLR_0 10000640(GPIO0-31)
#define REGISTER_DCLR0_OFFSET		400
//GPIO_DCLR_1 10000644(GPIO31-63)
#define REGISTER_DCLR1_OFFSET		401
//GPIO_DCLR_2 10000648(GPIO64-95)
#define REGISTER_DCLR2_OFFSET		402

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
