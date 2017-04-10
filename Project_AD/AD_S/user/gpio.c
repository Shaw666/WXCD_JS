/*
 * led.c
 *
 *  Created on: 2017-3-21
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "HK_all_include.h"
///*
//���ն�LED��ʼ��
void GPIO_INit(void)
{
	EALLOW;
	//GPIO�Ĵ����ܱ���   LED1  2   3  GPIO 4 5 10
	GpioCtrlRegs.GPAMUX1.bit.GPIO4  = 0;	//GPIO4��Ϊ��ͨIO
	GpioCtrlRegs.GPAMUX1.bit.GPIO5  = 0;	//GPIO5��Ϊ��ͨIO
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;	//GPIO10��Ϊ��ͨIO

	GpioCtrlRegs.GPADIR.bit.GPIO4  = 1;		//GPIO4����Ϊ���
	GpioCtrlRegs.GPADIR.bit.GPIO5  = 1;		//GPIO5����Ϊ���
	GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;		//GPIO10����Ϊ���

	GpioDataRegs.GPASET.bit.GPIO4 = 1;		//GPIO4����ߵ�ƽ
	GpioDataRegs.GPASET.bit.GPIO5 = 1;  	//GPIO5����ߵ�ƽ
	GpioDataRegs.GPASET.bit.GPIO10 = 1;		//GPIO10����ߵ�ƽ
	//�̵������Ƴ�ʼ��   �̵����ߵ�ƽ�պ� �ڽ�ѹģ��汾ʱ��δʹ��
	GpioCtrlRegs.GPAMUX1.bit.GPIO11  = 0;	//GPIO11��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO11  = 1;		//GPIO11����Ϊ���
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;		//GPIO11����͵�ƽ

	EDIS;
}
//*/

/*
//�����LED��ʼ��
void GPIO_INit(void)
{
	EALLOW;
	//GPIO�Ĵ����ܱ���   LED 1 2 3 4 --- GPIO9 16 17 34  ��ʼ��1111
	GpioCtrlRegs.GPAMUX1.bit.GPIO9  = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO16  = 0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;

	GpioCtrlRegs.GPADIR.bit.GPIO9  = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO16  = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;

	GpioDataRegs.GPASET.bit.GPIO9 = 1;
	GpioDataRegs.GPASET.bit.GPIO16 = 1;
	GpioDataRegs.GPASET.bit.GPIO17 = 1;
	GpioDataRegs.GPBSET.bit.GPIO34 = 1;
	//�̵������Ƴ�ʼ��   �̵����ߵ�ƽ�պ�  ��ʼ��0
	GpioCtrlRegs.GPAMUX1.bit.GPIO11  = 0;	//GPIO11��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO11  = 1;		//GPIO11����Ϊ���
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;		//GPIO11����͵�ƽ
	//���ȿ��Ƴ�ʼ��  �ߵ�ƽ��Ч�򿪷���  ��ʼ��0
	GpioCtrlRegs.GPAMUX1.bit.GPIO5  = 0;	//GPIO11��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO5  = 1;		//GPIO11����Ϊ���
	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;		//GPIO11����͵�ƽ
	EDIS;
}
*/

///*
//���ն˲��밴����ʼ��
void KEY_Init(void)
{
	EALLOW;	//GPIO�Ĵ����ܱ���   ���뿪�� 1  2  3  4 --- GPIO0 1 3 22
	GpioCtrlRegs.GPAMUX2.bit.GPIO22= 0;		//GPIO27��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO22 = 0;		//GPIO27����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 0;		//GPIO0��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO0   = 0;		//GPIO0����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO0   = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO0 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 0;		//GPIO1��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO1   = 0;		//GPIO1����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO1   = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 0;		//GPIO3��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO3   = 0;		//GPIO3����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO3   = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 0;   // ���Ų�����ϵͳʱ��ͬ��
//	//�û���ť  �ߵ�ƽ��Ч����   �����ⲿ�ж�
//	GpioCtrlRegs.GPAMUX1.bit.GPIO12  = 0;		//GPIO3��Ϊ��ͨIO
//	GpioCtrlRegs.GPADIR.bit.GPIO12   = 0;		//GPIO3����Ϊ����
//	GpioCtrlRegs.GPAPUD.bit.GPIO12   = 1;		//�����ڲ�����  �ⲿ����
//	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;   // ���Ų�����ϵͳʱ��ͬ��
	//��ѹģ���ѹ������ʼ��  �͵�ƽ����
	GpioCtrlRegs.GPAMUX1.bit.GPIO8  = 0;		//GPIO3��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO8   = 0;		//GPIO3����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO8   = 1;		//�����ڲ�����  �ⲿ����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO8 = 0;   // ���Ų�����ϵͳʱ��ͬ��
	EDIS;
}
//*/

/*
//����˲��밴����ʼ��
void KEY_Init(void)
{
	EALLOW;	//GPIO�Ĵ����ܱ���   ���뿪�� 1  2  3  4 --- 7 12 18 22
	//����ģ���쳣����  Ĭ��Ϊ�ߣ��쳣������͵�ƽ
	GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 0;		//GPIO3��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO3   = 0;		//GPIO3����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO3   = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX1.bit.GPIO7= 0;		//GPIO7��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO7 = 0;		//GPIO7����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX1.bit.GPIO12  = 0;		//GPIO12��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO12   = 0;		//GPIO12����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO12   = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX2.bit.GPIO18  = 0;		//GPIO18��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO18   = 0;		//GPIO18����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO18   = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX2.bit.GPIO22  = 0;		//GPIO22��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO22   = 0;		//GPIO22����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO22   = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	EDIS;
}
*/
/*-----------------------------------------------------------
 *�����Ͱ�������ɨ��
 *����һ��ɨ�����ڵ���ʱ��Ӧ
 *����GPIO 7 12 18 22 ---------- 0 1 2 3
 *����GPIO 0 1 3 22 ---------- 0 1 2 3
 *---------------------------------------------------------*/
void DIP_Scan(void)
{
	key=0x00;
	if((DIP0()==0||DIP1()==0||DIP2()==0||DIP3()==0))
	{
		if(DIP0()==0)     {
			key|=0x01; //0
		}
		else if(DIP1()==0){
			key|=0x02; //1
		}
		else if(DIP2()==0){
			key|=0x04; //3
		}
		else if(DIP3()==0){
			key|=0x08; //22
		}
	}else{
		key=0x00;
	}
}
//����˿���ɨ��
//u8 STOP_Scan(void)
//{
////����������쳣���
//	if(qdyckey()==0){
//		return Excep_qdyc;
//	}
//	return 0;
//}
//���ն˿���ɨ��
u8 STOP_Scan(void)
{
//���ն˽�ѹ��ѹ�������
	if(ISOver()==0){
		return Excep_jygy;
	}
	return 0;
}

