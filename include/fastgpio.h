#ifndef _FAST_GPIO_H_
#define _FAST_GPIO_H_

#include <module.h>
#include <string.h> 

#define DEVICE_TYPE			"omega2"

class FastGpio : protected Module {
public:
	virtual int setDirection(int, GPIO_Pin_Direction)=0;
	virtual int getDirection(int, GPIO_Pin_Direction&)=0;

	virtual int set(int, GPIO_Pin_State)=0;
	virtual int read(int, GPIO_Pin_State&)=0;

protected:
	int m_pin;
};


#endif 	// _FAST_GPIO_H_
