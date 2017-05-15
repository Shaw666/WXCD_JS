/*
 * ad.h
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Module_Project.h"

void ExInt_Init(void);
interrupt void xint1_isr(void);


//###########################################################################
//外部中断，下降沿检测
//开启XINT1外部中断
//急停中断
//---------------------------------------------------------------------------------------------
void ExInt_Init(void)
{
	//中断配置步骤-----2，重映射中断服务函数
	EALLOW;	
    PieVectTable.XINT1 = &xint1_isr;
	EDIS;
	//中断配置步骤-----3，连接CPU中断Y
	IER |= M_INT1;
	//中断配置步骤-----4，连接Y中断里的第几位
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Gropu 1 INT4   //XINT1
    // Enable CPU INT1
	EALLOW;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 6;
	EDIS;
	XIntruptRegs.XINT1CR.bit.POLARITY = 1;      // 1为上升沿，0和2为下降沿，3为双边沿
	//中断配置步骤-----1,开启模块中断使能，
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;         // Enable XINT1
}

//外部急停按钮 GPIO4
interrupt void xint1_isr(void)
{
	if(ISStart()==1){
		//用户按下按钮
	}
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

