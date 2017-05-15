/*
 * at24c02.c
 *
 *  Created on: 2014-8-7
 *      Author: M新动力
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Module_Project.h"

char keyVal,LigntVal;
I2CSlaveMSG MCP79411;
__interrupt void i2c_int1a_isr(void);

//Uint16 IntSRC[10];            //中断源            测试用
//Uint16 IntSRCCnt;             //中断源记录  测试用
//Uint16 MasterStatus;			//自行软件定义的IIC状态
//Uint16 SlavePHYAddress;	    //IIC物理地址（硬件地址）
//Uint16 LogicAddr;             //类似于EEPROM，需要提供逻辑地址
//Uint16 Len;			        //操作数据的长度（不含物理地址）
//Uint16 MsgInBuffer[I2C_MAX_BUFFER_SIZE]; //发送数组，最大4个深度
//Uint16 MsgOutBuffer[I2C_MAX_BUFFER_SIZE];//接收数组，最大4个深度
//Uint16 IIC_TimerOUT;          //软件定义的超时变量

// Interrupt Source Messages
//#define I2C_NO_ISRC             0x0000
//#define I2C_ARB_ISRC            0x0001
//#define I2C_NACK_ISRC           0x0002
//#define I2C_ARDY_ISRC           0x0003
//#define I2C_RX_ISRC             0x0004
//#define I2C_TX_ISRC             0x0005
//#define I2C_SCD_ISRC            0x0006
//#define I2C_AAS_ISRC            0x0007

//中断源注释
char *IIC_ISR_String[] = {"No","ARB",
		        "NACK","ARDY",
		        "RX","Tx",
		        "SCD","AAS"};


I2CSlaveMSG TM1650;//定义一个TM1650结构体
I2CSlaveMSG AT24C02Msg;
I2CSlaveMSG *PtrMsg[]={&TM1650,&AT24C02Msg};//定义一个指针数组，存放I2C结构体，这里包含TM1650和EEPROM

uint8_t SEG7Table[]={

    //共阴极
    0x3f,0x06,0x5b,0x4f,
    0x66,0x6d,0x7d,0x07,
    0x7f,0x6f,0x77,0x7c,
    0x39,0x5e,0x79,0x71

    //共阳极
//    0xc0,0xf9,0xa4,0xb0,
//    0x99,0x92,0x82,0xf8,
//    0x80,0x90,0x88,0x83,
//    0xc6,0xa1,0x86,0x8e,
};


/************************************************************************/
//IIC 的IO口定义
/************************************************************************/
void InitI2C_Gpio()
{
   EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)

	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation
    EDIS;
}


//IIC中断函数，中断源为P32 IIC手册说明
__interrupt void i2c_int1a_isr(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}


#define IICFRE 100000L   //100K IIC频率
/************************************************************************/
// Initialize I2C
//100K
//使能FIFO
//不采用中断
/************************************************************************/
void I2CA_Init(void)
{
	InitI2C_Gpio();

   I2caRegs.I2CPSC.all = 5;		    //对系统时钟分频给IIC内部模块， Prescaler - need 7-12 Mhz on module clk
//SYSCLK/(I2caRegs.I2CPSC.all+1)/IICFRE/2-5
//45为100KHz
   I2caRegs.I2CCLKL = SYSCLK/(I2caRegs.I2CPSC.all+1)/IICFRE/2-5;// NOTE: must be non zero
   I2caRegs.I2CCLKH = SYSCLK/(I2caRegs.I2CPSC.all+1)/IICFRE/2-5;// NOTE: must be non zero

//   I2caRegs.I2CCLKL = 10;// NOTE: must be non zero
//   I2caRegs.I2CCLKH = 10;// NOTE: must be non zero

   //P21 《sprufz9d--IIC.pdf》中断源
   //P28  I2CIER寄存器说明
//   I2caRegs.I2CIER.all = 0x22;		// Enable SCD & ARDY interrupts & NACK

   I2caRegs.I2CMDR.all = 0x0020;	// Take I2C out of reset
   									// Stop I2C when suspended
   I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
   I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

   EALLOW;	// This is needed to write to EALLOW protected registers
   PieVectTable.I2CINT1A = &i2c_int1a_isr;
   EDIS;   // This is needed to disable write to EALLOW protected registers

// Enable I2C interrupt 1 in the PIE: Group 8 interrupt 1
//   PieCtrlRegs.PIEIER8.bit.INTx1 = 1;

// Enable CPU INT8 whih is connected to PIE group 8
   IER |= M_INT8;
   EINT;

   return;
}



//IIC底层发送函数
//IIC Maste Mode，发送模式
//I2caRegs.I2CCNT 为发送的字节长度，不包括从器件的物理地址
//发送格式：START ADDRESS CONTROL_BYTE DA_BYTE STOP
//当写入I2caRegs.I2CMDR.all = 0x2E20，硬件IIC进行操作，STOP信号结束
Uint16 I2CA_Tx_STOP(struct I2CSlaveMSG *msg)
{
	Uint16 i;
	if(I2caRegs.I2CMDR.bit.STP==1)//等待STOP信号
	{
	  return I2C_STP_NOT_READY_ERROR;
	}
    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
       return I2C_BUS_BUSY_ERROR;
    }

//    I2caRegs.I2CSAR = msg->SlavePHYAddress;//设置访问从器件地址，物理地址

    //FIFO为4个字节，所以最大放入 4个 数据
    for(i=0;i<msg->Len;i++)     // 小于等于4个byte的数据
    {
    	I2caRegs.I2CDXR = msg->MsgOutBuffer[i];
    }
 	I2caRegs.I2CCNT = msg->Len; //设置发送长度，最大为4个byte
//    I2caRegs.I2CMDR.all = 0x2E20;		// Send restart as master receiver
    I2caRegs.I2CMDR.all = 0x6E20;
    //TRX  = 1 ,发送模式； STP = 1 发送停止位
    return I2C_SUCCESS;
}


//IIC底层接收函数
//先发送从器件的物理地址，再接收 I2CCNT 个字节
//IIC Maste Mode，接收模式
//I2caRegs.I2CCNT 为发送的字节长度，不包括从器件的物理地址，
//格式(接收4个字节为例)：START ADDRESS DATA_BYTE1 DATA_BYTE2 DATA_BYTE3 DATA_BYTE4  STOP //DATA为读取的数据
//当写入I2caRegs.I2CMDR.all = 0x2C20，硬件IIC进行操作，硬件STOP发送后，接收的数据在FIFO中
Uint16 I2CA_Rxdata_STOP(struct I2CSlaveMSG *msg)
{
//	I2caRegs.I2CSAR = msg->SlavePHYAddress;
 	I2caRegs.I2CCNT = msg->Len;	      // Setup how many bytes to expect
    I2caRegs.I2CMDR.all = 0x2C20;		      //  master receiver
    //TRX  = 0 ,接收模式； STP = 1 发送停止位
    return I2C_SUCCESS;
}



u8 MCP79411_INit(void)
{
	MCP79411.Len=4;
	MCP79411.MsgOutBuffer[0]=0xde;//RTC寄存器写
	MCP79411.MsgOutBuffer[1]=0x01;//地址分钟
	MCP79411.MsgOutBuffer[2]=0x45;//45分钟
    MCP79411.MsgOutBuffer[3]=0x31;//00110001 24、AM、10+1小时
	I2CA_Tx_STOP(&MCP79411);
	MCP79411.Len=4;
	MCP79411.MsgOutBuffer[0]=0x0A;//外部电池供电+星期二
	MCP79411.MsgOutBuffer[1]=0x14;//14 of the month
	MCP79411.MsgOutBuffer[2]=0x04;//4月
	MCP79411.MsgOutBuffer[3]=0x17;//2017
	I2CA_Tx_STOP(&MCP79411);
	MCP79411.Len=3;
	MCP79411.MsgOutBuffer[0]=0xde;//外部电池供电+星期二
	MCP79411.MsgOutBuffer[1]=0x00;//地址 秒
	MCP79411.MsgOutBuffer[2]=0xA3;//秒+st= 1启动振荡器  1010 0011 23 秒
	I2CA_Tx_STOP(&MCP79411);
	return RTC_SUCCESS;
}

