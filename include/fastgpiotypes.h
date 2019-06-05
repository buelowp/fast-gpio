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

//Define Macros in derived class. 
#define OMEGA2_REG_BLOCK_ADDR			0x10000000
#define OMEGA2_REG_BLOCK_SIZE			0x6AC
//DIRECTION CONTROL REGISTERS

//GPIO_CTRL_0 10000600(Directions for GPIO0-GPIO31)
#define OMEGA2_REGISTER_CTRL0_OFFSET		384
//GPIO_CTRL_1 10000604(Directions for GPIO32-GPIO63)
#define OMEGA2_REGISTER_CTRL1_OFFSET		385
//GPIO_CTRL_2 10000608(Directions for GPIO64-GPIO95)
#define OMEGA2_REGISTER_CTRL2_OFFSET		386

//DATA REGISTERS: STATES OF GPIOS 

//GPIO_DATA_0 10000620(GPIO0-31)
#define OMEGA2_REGISTER_DATA0_OFFSET		392
//GPIO_DATA_1 10000624(GPIO32-63)
#define OMEGA2_REGISTER_DATA1_OFFSET		393
//GPIO_DATA_2 10000628(GPIO64-95)
#define OMEGA2_REGISTER_DATA2_OFFSET		394

//DATA SET REGISTERS: SET STATES OF GPIO_DATA_x registers

//GPIO_DSET_0 10000630(GPIO0-31)
#define OMEGA2_REGISTER_DSET0_OFFSET		396
//GPIO_DSET_1 10000634(GPIO31-63)
#define OMEGA2_REGISTER_DSET1_OFFSET		397
//GPIO_DSET_2 10000638(GPIO64-95)
#define OMEGA2_REGISTER_DSET2_OFFSET		398

//DATA CLEAR REGISTERS: CLEAR BITS OF GPIO_DATA_x registers

//GPIO_DCLR_0 10000640(GPIO0-31)
#define OMEGA2_REGISTER_DCLR0_OFFSET		400
//GPIO_DCLR_1 10000644(GPIO31-63)
#define OMEGA2_REGISTER_DCLR1_OFFSET		401
//GPIO_DCLR_2 10000648(GPIO64-95)
#define OMEGA2_REGISTER_DCLR2_OFFSET		402
#endif
