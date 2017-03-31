/*
 * main.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */

//GpioDataRegs.GPASET.bit.GPIO5 = 1;   //发射端温度过高拉高打开风扇
//GpioDataRegs.GPASET.bit.GPIO11 = 1;
//发射端检测到输入电压高于350V拉高，闭合继电器
//接收端 直流接入口保护 低电平断开继电器保护

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "HK_all_include.h"

void InitLED(void);

Uint16 Error;
Uint16 Tmp = 0;
u8 key=0;
extern Uint16 RamfuncsLoadSize;

void main(void) {

// Step 1. Initialize System Control:b
   InitSysCtrl();
// InitGpio();  // Skipped for this example
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();
   // Copy time critical code and Flash setup code to RAM
   // The  RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart
   // symbols are created by the linker. Refer to the F28035v1.cmd file.
   //实现在RAM中运行代码的搬运，例如使用官方的DELAY_US
   memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);

	InitLED();
	D401ON();
	D402LOW();

	LED_INit();   //初始化LED 区别收发
	KEY_Init();   //初始化拨码开关 区别收发
	ExInt_Init(); //初始化外部中断 急停按钮 发射

	Timer0_init(); //初始化定时器0用于定时处理相关任务
//	Timer1_init(); //初始化定时器1用于定时处理PID 区别收发

	SCI_Init(128000);   //初始化SCI用于调试串口
	ZM5168_INit();              //初始化zm5168_P0模块
	open_uart_debug();

	//TM1650 IIC初始化
	InitI2C_Gpio();             //io 初始化为IIC
	I2CA_Init();                //HW IIC初始化，100KHz

	EPWM1_Config(1500);         //初始化PWM20k 1us死区 区别收发
//	EPWM2_Config(3000);

	ADC_Config(); //初始化ADC 区别收发
	AdcRegs.ADCSOCFRC1.all = 0Xffff; //软件触发AD 的 SOC0--SOC3采样
	//中断配置步骤-----5
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	EINT;  // Enable Global interrupt INTM
	ERTM;

	DIP_Scan();   //开机扫描一次拨码开关

    while(1) 
    {

    	if(timer0Base.Mark_Para.Status_Bits.OnemsdFlag == 1)
    	{
    		timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 0;
    		if(STOP_Scan()==Excep_jygy){//降压模块输出过压 通知前端结束传输
    		//向发射端发送消息 并记录状态
    		//切断继电器
    		}
    		if(timer0Base.msCounter>=200){

    		switch(key){
    		case 0x01:  {GpioDataRegs.GPASET.bit.GPIO4 = 1;	break;}	//GPIO10输出高电平
    		case 0x02:	{GpioDataRegs.GPASET.bit.GPIO5 = 1;	break;}	//GPIO10输出高电平
    		case 0x04:	{GpioDataRegs.GPASET.bit.GPIO4 = 0;	break;}	//GPIO10输出高电平
    		case 0x08:	{GpioDataRegs.GPASET.bit.GPIO5 = 0;	break;}	//GPIO10输出高电平
    		}
    		}
			if(timer0Base.msCounter >= 1000)//ms
			{
				timer0Base.msCounter = 0;

			}
    	  }

    }
}									

//#################################################
//初始化LED
//D400用 GPIO41
//-----------------------------------------------
void InitLED()
{
	EALLOW;
	D400MUX = 0;
	D400DIR = 1;
	D401MUX = 0;
	D401DIR = 1;
	D402MUX = 0;
	D402DIR = 1;

	RGB_R_MUX = 0;
	RGB_R_DIR = 1;
	RGB_G_MUX = 0;
	RGB_G_DIR = 1;
	RGB_B_MUX = 0;
	RGB_B_DIR = 1;
	EDIS;
}

//===========================================================================
// No more.
//===========================================================================

 
