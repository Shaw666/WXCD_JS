/*
 * main.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "Module_Project.h"

Uint16 Error;
Uint16 Tmp = 0;
u8 key=0;
extern Uint16 RamfuncsLoadSize;

void main(void)
{
   InitSysCtrl();
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();
   memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);

	GPIO_INit();   //��ʼ��
	Init_GlobalVariable();
//	ExInt_Init(); //��ʼ���ⲿ�ж� ��ͣ��ť
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC =0;
	EDIS;
	ADC_Config(); //��ʼ��ADC �����շ�
//	Timer0_init(); //��ʼ����ʱ��0���ڶ�ʱ�����������
//	Timer1_init();
	SCI_Init(115200);   //��ʼ��SCI���ڵ��Դ���
//	SetupSCI(115200);
//	open_uart_debug();
//	InitI2C_Gpio();             //io ��ʼ��ΪIIC
//	I2CA_Init();                //HW IIC��ʼ����100KHz
//	SPI_INit();
//	ZM5168_INit();              //��ʼ��zm5168_P0ģ��
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC =1;
	EDIS;
//	AdcRegs.ADCSOCFRC1.all = 0Xffff; //�������AD �� SOC0--SOC3����
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	EINT;  // Enable Global interrupt INTM
	ERTM;
	AdcRegs.ADCSOCFRC1.all = 0X9FDE;
//	DIP_Scan();   //����ɨ��һ�β��뿪��
    while(1) 
    {
    	GpioDataRegs.GPASET.bit.GPIO4 = 0;
		if(0){
			LocalConfDeal();
			//break;
		}
    	if(timer0Base.Mark_Para.Status_Bits.OnemsdFlag == 1)
    	{
    		timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 0;
    		if(STOP_Scan()==Excep_jygy){//��ѹģ�������ѹ ֪ͨǰ�˽�������
    		//����˷�����Ϣ ����¼״̬
    		//�жϼ̵���
    		}

    		if(timer0Base.msCounter>=200){
				timer0Base.msCounter = 0;


    		switch(0x09){
    		case 0x01:  {GpioDataRegs.GPASET.bit.GPIO4 = 1;	break;}	//GPIO10����ߵ�ƽ
    		case 0x02:	{GpioDataRegs.GPASET.bit.GPIO5 = 1;	break;}	//GPIO10����ߵ�ƽ
    		case 0x04:	{GpioDataRegs.GPASET.bit.GPIO4 = 0;	break;}	//GPIO10����ߵ�ƽ
    		case 0x08:	{GpioDataRegs.GPASET.bit.GPIO5 = 0;	break;}	//GPIO10����ߵ�ƽ
    		}
    		}
    	  }
    	GpioDataRegs.GPASET.bit.GPIO4 = 0;
    }
}									

//===========================================================================
// No more.
//===========================================================================

 
