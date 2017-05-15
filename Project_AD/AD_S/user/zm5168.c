/*
 * zm5168.c
 *
 *  Created on: 2017-3-24
 *      Author: Shaw
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Module_Project.h"


void SetupSCI(Uint32 buad)
{
	Uint16 brr_reg = (1875000 / buad) - 1;	//15000000/8 = 1875000
	//Allow write to protected registers
	EALLOW;

	LinaRegs.SCIGCR0.bit.RESET = 0; //Into reset
	LinaRegs.SCIGCR0.bit.RESET = 1; //Out of reset

	LinaRegs.SCIGCR1.bit.SWnRST = 0; //Into software reset

	//SCI Configurations
	LinaRegs.SCIGCR1.bit.COMMMODE = 0;   //Idle-Line Mode
	LinaRegs.SCIGCR1.bit.TIMINGMODE = 1; //Asynchronous Timing
	LinaRegs.SCIGCR1.bit.PARITYENA = 0;  //No Parity Check
	LinaRegs.SCIGCR1.bit.PARITY = 0;	 //Odd Parity
	LinaRegs.SCIGCR1.bit.STOP = 0;		 //One Stop Bit
	LinaRegs.SCIGCR1.bit.CLK_MASTER = 1; //Enable SCI Clock
	LinaRegs.SCIGCR1.bit.LINMODE = 0;	 //SCI Mode
	LinaRegs.SCIGCR1.bit.SLEEP = 0;      //Ensure Out of Sleep
	LinaRegs.SCIGCR1.bit.MBUFMODE = 0;	 //No Buffers Mode
	LinaRegs.SCIGCR1.bit.LOOPBACK = 0;   //External Loopback
	LinaRegs.SCIGCR1.bit.CONT = 1;		 //Continue on Suspend
	LinaRegs.SCIGCR1.bit.RXENA = 1;		 //Enable RX
	LinaRegs.SCIGCR1.bit.TXENA = 1;		 //Enable TX

	//Ensure IODFT is disabled
    LinaRegs.IODFTCTRL.bit.IODFTENA = 0x0;

    //Set transmission length
    LinaRegs.SCIFORMAT.bit.CHAR = 7;	 //Eight bits
    LinaRegs.SCIFORMAT.bit.LENGTH = 0;   //One byte

	//Set baudrate
    LinaRegs.BRSR.bit.SCI_LIN_PSL =  brr_reg & 0x00FF; //0XC2-->9600 ; 97--> 19200 ;0x30-->38400;14-->128000
    LinaRegs.BRSR.bit.SCI_LIN_PSH = (brr_reg >> 8) & 0x00FF;
    // baud = LSPCLK/8/((BRR+1)

    LinaRegs.BRSR.bit.M = 5;

    LinaRegs.SCIGCR1.bit.SWnRST = 1;  //bring out of software reset

	//Disable write to protected registers
	EDIS;
	//等待准备完毕
	while(LinaRegs.SCIFLR.bit.IDLE == 1);
//	//Wait for a charachter to by typed
//	if(LinaRegs.SCIFLR.bit.RXRDY == 1)
//	{
//		ReceivedChar = LinaRegs.SCIRD;
//
//		//Wait for the module to be ready to transmit
//		while(LinaRegs.SCIFLR.bit.TXRDY == 0);
//		//Begin transmission
//		LinaRegs.SCITD = ReceivedChar;
//	}
}

/*
 * 发射接收共用zm5168无线zigbee驱动函数
 * GPIO 2 6
 * DSPTX---- RX13
 * DSPRX---- TX12
 */
void ZM5168_INit(void)
{
	//初始化lina为uart模式
//	SetupSCI();
	EALLOW;
	//GPIO寄存器受保护  reset WAKE RX TX ----- GPIO2 GPIO6 LINTX LINRX
	GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 0;	//GPIO2作为普通IO  1ms低电平有效
	GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 0;	//GPIO6作为普通IO

	GpioCtrlRegs.GPADIR.bit.GPIO2  = 1;		//GPIO2方向为输出
	GpioCtrlRegs.GPADIR.bit.GPIO6  = 1;		//GPIO6方向为输出

	GpioDataRegs.GPASET.bit.GPIO2 = 1;		//GPIO2输出高电平
	GpioDataRegs.GPASET.bit.GPIO6 = 1;  	//GPIO6输出高电平
	EDIS;

//	GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;  	//GPIO6输出低电平
	DELAY_US(1024);                        //延时1024us

	ReadLocalConf();

}
//发送协议标志
u8 SendAgreementFlag()
{
	u8 res=0;
	scia_xmit(Agreement_1);
	scia_xmit(Agreement_2);
	scia_xmit(Agreement_3);
	return res;
}

DEV_INFO zm5168_p2={0};
Uint16 x,y,z;
void LocalConfDeal()
{
	Uint16 i,j=4,temp[80];
	for(i=0,j=4;i<8;i++,j++){
		zm5168_p2.DevName[i] = 0xff00&(temp[j]<<8);
	    zm5168_p2.DevName[i] |= 0x00ff&temp[++j];
	}
	for(i=0;i<8;i++,j++){
		zm5168_p2.DevPwd[i] = 0xff00&(temp[j]<<8);
		zm5168_p2.DevPwd[i] |= 0x00ff&temp[++j];
	}
	zm5168_p2.DevMode_Chan = temp[36]<<8|temp[37];
	zm5168_p2.PanID = temp[38]<<8|temp[39];
	zm5168_p2.MyAddr = temp[40]<<8|temp[41];
	for(i=0,j=42;i<4;i++,j++){
		zm5168_p2.MyIEEE[i] = 0xff00&(temp[j]<<8);
		zm5168_p2.MyIEEE[i] |= 0x00ff&temp[++j];
	}
	zm5168_p2.DstAddr = temp[50]<<8|temp[51];
	for(i=0,j=52;i<4;i++,j++){
		zm5168_p2.DstIEEE[i] = 0xff00&(temp[j]<<8);
		zm5168_p2.DstIEEE[i] |= 0x00ff&temp[++j];
	}
	zm5168_p2.Reserve_PowerLevel = temp[60]<<8|temp[61];
	zm5168_p2.RetryNum_TranTimeout = temp[62]<<8|temp[63];
	zm5168_p2.Serial_Rate_Serial_DataB = temp[64]<<8|temp[65];
	zm5168_p2.Serial_StopB_Serial_ParityB = temp[66]<<8|temp[67];
	zm5168_p2.SendMode = temp[68];
}

//读取本地配置
u8 ReadLocalConf()
{
	u8 res=0x00;
//	u8 retry=0;
	SendAgreementFlag();
	scia_xmit(RLocalConf);
	res=0x0ff&(Agreement_1+Agreement_2+Agreement_3+RLocalConf);
	scia_xmit(res);
	res=0x00;
	return res;
}
//设置通道号
u8 SetChanelNum(u8 chanelnum)
{
	u8 res=0;
	return res;
}
//搜索
u8 SearchDevice()
{
	u8 res=0;
	return res;
}
//获取远程配置信息
u8 ReadRemoteConf(u16 remoteadress)
{
	u8 res=0;
	return res;
}
//修改配置命令
u8 WriteConf(u16 deviceadress)
{
	u8 res=0;
	return res;
}
//复位设备
u8 ResetDevice(u16 deviceadress, u16 devicetype)
{
	u8 res=0;
	return res;
}
//回复出厂设置
u8 RestoreDevice(u16 deviceadress, u16 devicetype)
{
	u8 res=0;
	return res;
}

