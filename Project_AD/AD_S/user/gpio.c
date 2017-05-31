/*
 * led.c
 *
 *  Created on: 2017-3-21
 *      Author: Shaw
 */
#include "Module_Project.h"

//接收端LED初始化
void GPIO_INit(void) {
	EALLOW;
	//GPIO寄存器受保护   LED1  2   3  GPIO 4 5 10
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;	//GPIO4作为普通IO
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;	//GPIO5作为普通IO
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;	//GPIO10作为普通IO

	GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;		//GPIO4方向为输出
	GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;		//GPIO5方向为输出
	GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;		//GPIO10方向为输出

	GpioDataRegs.GPASET.bit.GPIO4 =  1;		//GPIO4输出高电平
	GpioDataRegs.GPASET.bit.GPIO5 =  1;  	//GPIO5输出高电平
	GpioDataRegs.GPASET.bit.GPIO10 = 1;		//GPIO10输出高电平
	//继电器控制初始化   继电器高电平闭合 在降压模块版本时并未使用
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;	//GPIO11作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;		//GPIO11方向为输出
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;		//GPIO11输出低电平
	DELAY_US(900000);
	GpioDataRegs.GPASET.bit.GPIO11 = 1;
    //ZigBee TX  RX ----28 29
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation


	//GPIO18 19---USART_TX  USART_RX
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	// Enable pull-up for GPIO22 (LIN TX)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	// Enable pull-up for GPIO23 (LIN RX)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO23 (LINRXA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2; // Configure GPIO19 for LIN TX operation	 (3-Enable,0-Disable)
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;  // Configure GPIO23 for LIN RX operati

	//GPIO寄存器受保护   拨码开关 1  2  3  4 --- GPIO0 1 3 22
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;		//GPIO27作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO22 = 0;		//GPIO27方向为输入
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;		//开启内部上拉
	GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 0;   // 引脚采样与系统时钟同步

	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;		//GPIO0作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO0 = 0;		//GPIO0方向为输入
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;		//开启内部上拉
	GpioCtrlRegs.GPAQSEL1.bit.GPIO0 = 0;   // 引脚采样与系统时钟同步

	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;		//GPIO1作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;		//GPIO1方向为输入
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;		//开启内部上拉
	GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;   // 引脚采样与系统时钟同步

	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;		//GPIO3作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO3 = 0;		//GPIO3方向为输入
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;		//开启内部上拉
	GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 0;   // 引脚采样与系统时钟同步
//	//用户按钮  高电平有效锁死   采用外部中断
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;		//GPIO12作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;		//GPIO12方向为输入
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;		//禁用内部上拉
	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;   // 引脚采样与系统时钟同步
	//降压模块过压保护初始化  低电平保护
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;		//GPIO3作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO8 = 0;		//GPIO3方向为输入
	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;		//禁用内部上拉  外部下拉
	GpioCtrlRegs.GPAQSEL1.bit.GPIO8 = 0;   // 引脚采样与系统时钟同步

	EDIS;
}
/*-----------------------------------------------------------
 *自锁型按键慢速扫描
 *会有一个扫描周期的延时反应
 *发射GPIO 7 12 18 22 ---------- 0 1 2 3
 *接收GPIO 0 1 3 22 ---------- 0 1 2 3
 *---------------------------------------------------------*/
void DIP_Scan(void) {
	Uint16 key = 0x00;
	if ((DIP0() == 0 || DIP1() == 0 || DIP2() == 0 || DIP3() == 0)) {
		if (DIP0() == 0) {
			key |= 0x01; //0
		} else if (DIP1() == 0) {
			key |= 0x02; //1
		} else if (DIP2() == 0) {
			key |= 0x04; //3
		} else if (DIP3() == 0) {
			key |= 0x08; //22
		}
	} else {
		key = 0x00;
	}
}
//发射端快速扫描
//u8 STOP_Scan(void)
//{
////发射端驱动异常检测
//	if(qdyckey()==0){
//		return Excep_qdyc;
//	}
//	return 0;
//}
//接收端快速扫描
u8 STOP_Scan(void) {
//接收端降压过压保护检测
	if (ISOver() == 0) {
		return Excep_jygy;
	}
	return 0;
}

