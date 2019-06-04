#ifndef __FASTGPIOTYPES_H__
#define __FASTGPIOTYPES_H__

enum GPIO_Irq_Type {
    GPIO_IRQ_NONE = 0,
    GPIO_IRQ_RISING = 1,
    GPIO_IRQ_FALLING = 2,
    GPIO_IRQ_BOTH = 3
};

enum GPIO_Pin_Direction {
	GPIO_DIRECTION_IN = 0,
	GPIO_DIRECTION_OUT = 1
};

enum GPIO_Pin_State {
	GPIO_PIN_LOW = 0,
	GPIO_PIN_HIGH = 1
};

#endif
