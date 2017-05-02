/*
 * zm5168.c
 *
 *  Created on: 2017-3-24
 *      Author: Shaw
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "HK_all_include.h"

void Init_LinaGpio()
{
   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;		// Enable pull-up for GPIO22 (LIN TX)
	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;		// Enable pull-up for GPIO23 (LIN RX)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.
	GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (LINRXA)

/* Configure LIN-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be LIN pins.
// Only one set of pins should be enabled at any time for LIN operation.
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;   // Configure GPIO19 for LIN TX operation	 (3-Enable,0-Disable)
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO23 for LIN RX operation (3-Enable,0-Disable)

    EDIS;
}

void SetupSCI(Uint32 buad)
{
	Uint16 brr_reg = (1875000 / buad) - 1;	//15000000/8 = 1875000

	Init_LinaGpio();//22 23 TX RX
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
	//�ȴ�׼�����
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
 * ������չ���zm5168����zigbee��������
 * GPIO 2 6
 * DSPTX---- RX13
 * DSPRX---- TX12
 */
void ZM5168_INit(void)
{
	//��ʼ��linaΪuartģʽ
//	SetupSCI();
	EALLOW;
	//GPIO�Ĵ����ܱ���  reset WAKE RX TX ----- GPIO2 GPIO6 LINTX LINRX
	GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 0;	//GPIO2��Ϊ��ͨIO  1ms�͵�ƽ��Ч
	GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 0;	//GPIO6��Ϊ��ͨIO

	GpioCtrlRegs.GPADIR.bit.GPIO2  = 1;		//GPIO2����Ϊ���
	GpioCtrlRegs.GPADIR.bit.GPIO6  = 1;		//GPIO6����Ϊ���

	GpioDataRegs.GPASET.bit.GPIO2 = 1;		//GPIO2����ߵ�ƽ
	GpioDataRegs.GPASET.bit.GPIO6 = 1;  	//GPIO6����ߵ�ƽ
	EDIS;

	GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;  	//GPIO6����͵�ƽ
	DELAY_US(1024);                        //��ʱ1024us

	ReadLocalConf();

}
//����Э���־
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
	Uint16 i,j=4;
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

//��ȡ��������
u8 ReadLocalConf()
{
	u8 res=0x00;
//	u8 retry=0;
	UartRxLEN=73;
	SendAgreementFlag();
	scia_xmit(RLocalConf);
	res=0x0ff&(Agreement_1+Agreement_2+Agreement_3+RLocalConf);
	scia_xmit(res);
	res=0x00;
	return res;
}
//����ͨ����
u8 SetChanelNum(u8 chanelnum)
{
	u8 res=0;
	return res;
}
//����
u8 SearchDevice()
{
	u8 res=0;
	return res;
}
//��ȡԶ��������Ϣ
u8 ReadRemoteConf(u16 remoteadress)
{
	u8 res=0;
	return res;
}
//�޸���������
u8 WriteConf(u16 deviceadress)
{
	u8 res=0;
	return res;
}
//��λ�豸
u8 ResetDevice(u16 deviceadress, u16 devicetype)
{
	u8 res=0;
	return res;
}
//�ظ���������
u8 RestoreDevice(u16 deviceadress, u16 devicetype)
{
	u8 res=0;
	return res;
}

