/*
 * at24c02.c
 *
 *  Created on: 2014-8-7
 *      Author: M�¶���
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Module_Project.h"

char keyVal,LigntVal;
I2CSlaveMSG MCP79411;
__interrupt void i2c_int1a_isr(void);

//Uint16 IntSRC[10];            //�ж�Դ            ������
//Uint16 IntSRCCnt;             //�ж�Դ��¼  ������
//Uint16 MasterStatus;			//������������IIC״̬
//Uint16 SlavePHYAddress;	    //IIC�����ַ��Ӳ����ַ��
//Uint16 LogicAddr;             //������EEPROM����Ҫ�ṩ�߼���ַ
//Uint16 Len;			        //�������ݵĳ��ȣ����������ַ��
//Uint16 MsgInBuffer[I2C_MAX_BUFFER_SIZE]; //�������飬���4�����
//Uint16 MsgOutBuffer[I2C_MAX_BUFFER_SIZE];//�������飬���4�����
//Uint16 IIC_TimerOUT;          //�������ĳ�ʱ����

// Interrupt Source Messages
//#define I2C_NO_ISRC             0x0000
//#define I2C_ARB_ISRC            0x0001
//#define I2C_NACK_ISRC           0x0002
//#define I2C_ARDY_ISRC           0x0003
//#define I2C_RX_ISRC             0x0004
//#define I2C_TX_ISRC             0x0005
//#define I2C_SCD_ISRC            0x0006
//#define I2C_AAS_ISRC            0x0007

//�ж�Դע��
char *IIC_ISR_String[] = {"No","ARB",
		        "NACK","ARDY",
		        "RX","Tx",
		        "SCD","AAS"};


I2CSlaveMSG TM1650;//����һ��TM1650�ṹ��
I2CSlaveMSG AT24C02Msg;
I2CSlaveMSG *PtrMsg[]={&TM1650,&AT24C02Msg};//����һ��ָ�����飬���I2C�ṹ�壬�������TM1650��EEPROM

uint8_t SEG7Table[]={

    //������
    0x3f,0x06,0x5b,0x4f,
    0x66,0x6d,0x7d,0x07,
    0x7f,0x6f,0x77,0x7c,
    0x39,0x5e,0x79,0x71

    //������
//    0xc0,0xf9,0xa4,0xb0,
//    0x99,0x92,0x82,0xf8,
//    0x80,0x90,0x88,0x83,
//    0xc6,0xa1,0x86,0x8e,
};


/************************************************************************/
//IIC ��IO�ڶ���
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


//IIC�жϺ������ж�ԴΪP32 IIC�ֲ�˵��
__interrupt void i2c_int1a_isr(void)
{
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}


#define IICFRE 100000L   //100K IICƵ��
/************************************************************************/
// Initialize I2C
//100K
//ʹ��FIFO
//�������ж�
/************************************************************************/
void I2CA_Init(void)
{
	InitI2C_Gpio();

   I2caRegs.I2CPSC.all = 5;		    //��ϵͳʱ�ӷ�Ƶ��IIC�ڲ�ģ�飬 Prescaler - need 7-12 Mhz on module clk
//SYSCLK/(I2caRegs.I2CPSC.all+1)/IICFRE/2-5
//45Ϊ100KHz
   I2caRegs.I2CCLKL = SYSCLK/(I2caRegs.I2CPSC.all+1)/IICFRE/2-5;// NOTE: must be non zero
   I2caRegs.I2CCLKH = SYSCLK/(I2caRegs.I2CPSC.all+1)/IICFRE/2-5;// NOTE: must be non zero

//   I2caRegs.I2CCLKL = 10;// NOTE: must be non zero
//   I2caRegs.I2CCLKH = 10;// NOTE: must be non zero

   //P21 ��sprufz9d--IIC.pdf���ж�Դ
   //P28  I2CIER�Ĵ���˵��
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



//IIC�ײ㷢�ͺ���
//IIC Maste Mode������ģʽ
//I2caRegs.I2CCNT Ϊ���͵��ֽڳ��ȣ��������������������ַ
//���͸�ʽ��START ADDRESS CONTROL_BYTE DA_BYTE STOP
//��д��I2caRegs.I2CMDR.all = 0x2E20��Ӳ��IIC���в�����STOP�źŽ���
Uint16 I2CA_Tx_STOP(struct I2CSlaveMSG *msg)
{
	Uint16 i;
	if(I2caRegs.I2CMDR.bit.STP==1)//�ȴ�STOP�ź�
	{
	  return I2C_STP_NOT_READY_ERROR;
	}
    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
       return I2C_BUS_BUSY_ERROR;
    }

//    I2caRegs.I2CSAR = msg->SlavePHYAddress;//���÷��ʴ�������ַ�������ַ

    //FIFOΪ4���ֽڣ����������� 4�� ����
    for(i=0;i<msg->Len;i++)     // С�ڵ���4��byte������
    {
    	I2caRegs.I2CDXR = msg->MsgOutBuffer[i];
    }
 	I2caRegs.I2CCNT = msg->Len; //���÷��ͳ��ȣ����Ϊ4��byte
//    I2caRegs.I2CMDR.all = 0x2E20;		// Send restart as master receiver
    I2caRegs.I2CMDR.all = 0x6E20;
    //TRX  = 1 ,����ģʽ�� STP = 1 ����ֹͣλ
    return I2C_SUCCESS;
}


//IIC�ײ���պ���
//�ȷ��ʹ������������ַ���ٽ��� I2CCNT ���ֽ�
//IIC Maste Mode������ģʽ
//I2caRegs.I2CCNT Ϊ���͵��ֽڳ��ȣ��������������������ַ��
//��ʽ(����4���ֽ�Ϊ��)��START ADDRESS DATA_BYTE1 DATA_BYTE2 DATA_BYTE3 DATA_BYTE4  STOP //DATAΪ��ȡ������
//��д��I2caRegs.I2CMDR.all = 0x2C20��Ӳ��IIC���в�����Ӳ��STOP���ͺ󣬽��յ�������FIFO��
Uint16 I2CA_Rxdata_STOP(struct I2CSlaveMSG *msg)
{
//	I2caRegs.I2CSAR = msg->SlavePHYAddress;
 	I2caRegs.I2CCNT = msg->Len;	      // Setup how many bytes to expect
    I2caRegs.I2CMDR.all = 0x2C20;		      //  master receiver
    //TRX  = 0 ,����ģʽ�� STP = 1 ����ֹͣλ
    return I2C_SUCCESS;
}



u8 MCP79411_INit(void)
{
	MCP79411.Len=4;
	MCP79411.MsgOutBuffer[0]=0xde;//RTC�Ĵ���д
	MCP79411.MsgOutBuffer[1]=0x01;//��ַ����
	MCP79411.MsgOutBuffer[2]=0x45;//45����
    MCP79411.MsgOutBuffer[3]=0x31;//00110001 24��AM��10+1Сʱ
	I2CA_Tx_STOP(&MCP79411);
	MCP79411.Len=4;
	MCP79411.MsgOutBuffer[0]=0x0A;//�ⲿ��ع���+���ڶ�
	MCP79411.MsgOutBuffer[1]=0x14;//14 of the month
	MCP79411.MsgOutBuffer[2]=0x04;//4��
	MCP79411.MsgOutBuffer[3]=0x17;//2017
	I2CA_Tx_STOP(&MCP79411);
	MCP79411.Len=3;
	MCP79411.MsgOutBuffer[0]=0xde;//�ⲿ��ع���+���ڶ�
	MCP79411.MsgOutBuffer[1]=0x00;//��ַ ��
	MCP79411.MsgOutBuffer[2]=0xA3;//��+st= 1��������  1010 0011 23 ��
	I2CA_Tx_STOP(&MCP79411);
	return RTC_SUCCESS;
}

