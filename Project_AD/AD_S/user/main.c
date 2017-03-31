/*
 * main.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */

//GpioDataRegs.GPASET.bit.GPIO5 = 1;   //������¶ȹ������ߴ򿪷���
//GpioDataRegs.GPASET.bit.GPIO11 = 1;
//����˼�⵽�����ѹ����350V���ߣ��պϼ̵���
//���ն� ֱ������ڱ��� �͵�ƽ�Ͽ��̵�������

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
   //ʵ����RAM�����д���İ��ˣ�����ʹ�ùٷ���DELAY_US
   memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);

	InitLED();
	D401ON();
	D402LOW();

	LED_INit();   //��ʼ��LED �����շ�
	KEY_Init();   //��ʼ�����뿪�� �����շ�
	ExInt_Init(); //��ʼ���ⲿ�ж� ��ͣ��ť ����

	Timer0_init(); //��ʼ����ʱ��0���ڶ�ʱ�����������
//	Timer1_init(); //��ʼ����ʱ��1���ڶ�ʱ����PID �����շ�

	SCI_Init(128000);   //��ʼ��SCI���ڵ��Դ���
	ZM5168_INit();              //��ʼ��zm5168_P0ģ��
	open_uart_debug();

	//TM1650 IIC��ʼ��
	InitI2C_Gpio();             //io ��ʼ��ΪIIC
	I2CA_Init();                //HW IIC��ʼ����100KHz

	EPWM1_Config(1500);         //��ʼ��PWM20k 1us���� �����շ�
//	EPWM2_Config(3000);

	ADC_Config(); //��ʼ��ADC �����շ�
	AdcRegs.ADCSOCFRC1.all = 0Xffff; //�������AD �� SOC0--SOC3����
	//�ж����ò���-----5
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	EINT;  // Enable Global interrupt INTM
	ERTM;

	DIP_Scan();   //����ɨ��һ�β��뿪��

    while(1) 
    {

    	if(timer0Base.Mark_Para.Status_Bits.OnemsdFlag == 1)
    	{
    		timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 0;
    		if(STOP_Scan()==Excep_jygy){//��ѹģ�������ѹ ֪ͨǰ�˽�������
    		//����˷�����Ϣ ����¼״̬
    		//�жϼ̵���
    		}
    		if(timer0Base.msCounter>=200){

    		switch(key){
    		case 0x01:  {GpioDataRegs.GPASET.bit.GPIO4 = 1;	break;}	//GPIO10����ߵ�ƽ
    		case 0x02:	{GpioDataRegs.GPASET.bit.GPIO5 = 1;	break;}	//GPIO10����ߵ�ƽ
    		case 0x04:	{GpioDataRegs.GPASET.bit.GPIO4 = 0;	break;}	//GPIO10����ߵ�ƽ
    		case 0x08:	{GpioDataRegs.GPASET.bit.GPIO5 = 0;	break;}	//GPIO10����ߵ�ƽ
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
//��ʼ��LED
//D400�� GPIO41
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

 
