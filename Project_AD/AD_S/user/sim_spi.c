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
	//GPIO�Ĵ����ܱ���
	//SPI SCK SDI SDO CS --- GPIO21 GPIO20 GPIO23 GPIO24
	//SDI
	GpioCtrlRegs.GPAMUX2.bit.GPIO20  = 0;	//GPIO20��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO20   = 1;		//GPIO20����Ϊ���
	GpioDataRegs.GPASET.bit.GPIO20   = 1;		//GPIO20����ߵ�ƽ
	//SCK
	GpioCtrlRegs.GPAMUX2.bit.GPIO21  = 0;	//GPIO21��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO21   = 1;		//GPIO21����Ϊ���
	GpioDataRegs.GPASET.bit.GPIO21   = 1;		//GPIO21����ߵ�ƽ
	//SDO
	GpioCtrlRegs.GPAMUX2.bit.GPIO23  = 0;	//GPIO23��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO23   = 0;		//GPIO23����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO23   = 1;   //����23
	//CS
	GpioCtrlRegs.GPAMUX2.bit.GPIO24  = 0;	//GPIO24��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO24   = 1;		//GPIO24����Ϊ���
	GpioDataRegs.GPASET.bit.GPIO24   = 1;		//GPIO24����ߵ�ƽ
	EDIS;
}
void SPI_INit(void)
{
	SPIGpio_Init();//��ʼ��GPIO
	//��ʼ��FLASH
	//...................
}
