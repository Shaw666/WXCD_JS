/*
 * sim_spi.c
 *
 *  Created on: 2017-3-24
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "HK_all_include.h"
void SPIGpio_Init(void)
{
	EALLOW;
	//GPIO寄存器受保护
	//SPI SCK SDI SDO CS --- GPIO21 GPIO20 GPIO23 GPIO24
	//SDI
	GpioCtrlRegs.GPAMUX2.bit.GPIO20  = 0;	//GPIO20作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO20   = 1;		//GPIO20方向为输出
	GpioDataRegs.GPASET.bit.GPIO20   = 1;		//GPIO20输出高电平
	//SCK
	GpioCtrlRegs.GPAMUX2.bit.GPIO21  = 0;	//GPIO21作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO21   = 1;		//GPIO21方向为输出
	GpioDataRegs.GPASET.bit.GPIO21   = 1;		//GPIO21输出高电平
	//SDO
	GpioCtrlRegs.GPAMUX2.bit.GPIO23  = 0;	//GPIO23作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO23   = 0;		//GPIO23方向为输入
	GpioCtrlRegs.GPAPUD.bit.GPIO23   = 1;   //上拉23
	//CS
	GpioCtrlRegs.GPAMUX2.bit.GPIO24  = 0;	//GPIO24作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO24   = 1;		//GPIO24方向为输出
	GpioDataRegs.GPASET.bit.GPIO24   = 1;		//GPIO24输出高电平
	EDIS;
}
void SPI_INit(void)
{
	SPIGpio_Init();//初始化GPIO
	//初始化FLASH
	//...................
}
