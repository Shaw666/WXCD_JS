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
	Timer0_init(); //��ʼ����ʱ��0���ڶ�ʱ�����������
//	Timer1_init();
	SCI_Init(57600);   //��ʼ��SCI���ڵ��Դ���
	SetupSCI(115200);
//	open_uart_debug();
//	I2CA_Init();                //HW IIC��ʼ����100KHz
//	SPI_INit();
	ZM5168_INit();              //��ʼ��zm5168_P0ģ��
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC =1;
	EDIS;
//	AdcRegs.ADCSOCFRC1.all = 0Xffff; //�������AD �� SOC0--SOC3����
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	EINT;  // Enable Global interrupt INTM
	ERTM;
	AdcRegs.ADCSOCFRC1.all = 0X9FDE;
//	DIP_Scan();   //����ɨ��һ�β��뿪��
	DealRxLenth = 5;
    while(1) 
    {
  //  	if(timer0Base.Mark_Para.Status_Bits.OnemsdFlag == 1)

//    		if(STOP_Scan()==Excep_jygy){//��ѹģ�������ѹ ֪ͨǰ�˽�������
//    		//����˷�����Ϣ ����¼״̬
//    		//�жϼ̵���
//    		}
    	if(timer0Base.msCounter>=50){
//    		GpioDataRegs.GPATOGGLE.bit.GPIO5 = 1;
				timer0Base.msCounter = 0;
				SendRequestSCI(0xA2);//���������ѹ
				AdcRegs.ADCSOCFRC1.all = 0X9FDE;
 //   			Upper_Uart();
    		}
}									
}
//===========================================================================
// No more.
//===========================================================================

 
