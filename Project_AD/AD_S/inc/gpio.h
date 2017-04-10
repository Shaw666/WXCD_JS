/*
 * led.h
 *
 *  Created on: 2017-3-21
 *      Author: Shaw
 */

#ifndef GPIO_H_
#define GPIO_H_

//接收端读取按键宏定义
#define DIP0() GpioDataRegs.GPADAT.bit.GPIO0
#define DIP1() GpioDataRegs.GPADAT.bit.GPIO1
#define DIP2() GpioDataRegs.GPADAT.bit.GPIO3
#define DIP3() GpioDataRegs.GPADAT.bit.GPIO22
//物理充电开关 状态
#define ISStart() GpioDataRegs.GPADAT.bit.GPIO12
#define Excep_kgdk 0x02
//急停开关 中断处理
//继电器开关
#define RELAYON()  GpioDataRegs.GPASET.bit.GPIO11 = 1	//GPIO11输出高电平  闭合
#define RELAYOFF() GpioDataRegs.GPACLEAR.bit.GPIO11 = 1		//GPIO11输出低电平  断开
//降压模块过压保护
#define ISOver() GpioDataRegs.GPADAT.bit.GPIO8
#define Excep_jygy 0x03 //降压模块过压
//********************分隔符***********************************

//发射端读取按键宏定义
//#define DIP0() GpioDataRegs.GPADAT.bit.GPIO7
//#define DIP1() GpioDataRegs.GPADAT.bit.GPIO12
//#define DIP2() GpioDataRegs.GPADAT.bit.GPIO18
//#define DIP3() GpioDataRegs.GPADAT.bit.GPIO22
//#define qdyckey() GpioDataRegs.GPADAT.bit.GPIO3
//#define Excep_qdyc 0x01 //驱动异常
//风扇开关
extern u8 key;

void LED_INit(void);
void KEY_Init(void);
void DIP_Scan(void);
u8 STOP_Scan(void);
#endif /* LED_H_ */
