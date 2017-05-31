/*
 * main.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "Module_Project.h"

extern Uint16 RamfuncsLoadSize;
Uint16 checkOrderflag =0;
void main(void)
{
   InitSysCtrl();
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();
   memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);

	GPIO_INit();   //初始化
	Init_GlobalVariable();
//	ExInt_Init(); //初始化外部中断 急停按钮
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC =0;
	EDIS;
	ADC_Config(); //初始化ADC 区别收发
	Timer0_init(); //初始化定时器0用于定时处理相关任务
//	Timer1_init();
	SCI_Init(57600);   //初始化SCI用于调试串口
//	SetupSCI(115200);
//	open_uart_debug();
//	I2CA_Init();                //HW IIC初始化，100KHz
//	SPI_INit();
	ZM5168_INit();              //初始化zm5168_P0模块
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC =1;
	EDIS;
//	AdcRegs.ADCSOCFRC1.all = 0Xffff; //软件触发AD 的 SOC0--SOC3采样
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	EINT;  // Enable Global interrupt INTM
	ERTM;
//	AdcRegs.ADCSOCFRC1.all = 0X9FDE;
//	DIP_Scan();   //开机扫描一次拨码开关
	DealRxLenth = 5;
	AdcRegs.ADCSOCFRC1.all = 0X9FDE;
    while(1) 
    {
   	if(timer0Base.msCounter == 1){
   		timer0Base.msCounter = 0;
				checkOrderflag++;
				if(checkOrderflag>=1000){
					checkOrderflag=0;
					scia_xmit(0x11);
					scia_xmit(0x22);
					scia_xmit(0x33);
				}else{
				SendRequestSCI(0xA2);//发送输出电压
 //   			Upper_Uart();
				}
    		}
}									
}
//===========================================================================
// No more.
//===========================================================================

 
