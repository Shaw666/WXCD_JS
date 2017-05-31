/*
 * led.c
 *
 *  Created on: 2017-3-21
 *      Author: Shaw
 */
#include "Module_Project.h"

//���ն�LED��ʼ��
void GPIO_INit(void) {
	EALLOW;
	//GPIO�Ĵ����ܱ���   LED1  2   3  GPIO 4 5 10
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;	//GPIO4��Ϊ��ͨIO
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;	//GPIO5��Ϊ��ͨIO
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;	//GPIO10��Ϊ��ͨIO

	GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;		//GPIO4����Ϊ���
	GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;		//GPIO5����Ϊ���
	GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;		//GPIO10����Ϊ���

	GpioDataRegs.GPASET.bit.GPIO4 =  1;		//GPIO4����ߵ�ƽ
	GpioDataRegs.GPASET.bit.GPIO5 =  1;  	//GPIO5����ߵ�ƽ
	GpioDataRegs.GPASET.bit.GPIO10 = 1;		//GPIO10����ߵ�ƽ
	//�̵������Ƴ�ʼ��   �̵����ߵ�ƽ�պ� �ڽ�ѹģ��汾ʱ��δʹ��
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;	//GPIO11��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;		//GPIO11����Ϊ���
	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;		//GPIO11����͵�ƽ
	DELAY_US(900000);
	GpioDataRegs.GPASET.bit.GPIO11 = 1;
    //ZigBee TX  RX ----28 29
	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation


	//GPIO18 19---USART_TX  USART_RX
	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	// Enable pull-up for GPIO22 (LIN TX)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	// Enable pull-up for GPIO23 (LIN RX)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO23 (LINRXA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2; // Configure GPIO19 for LIN TX operation	 (3-Enable,0-Disable)
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;  // Configure GPIO23 for LIN RX operati

	//GPIO�Ĵ����ܱ���   ���뿪�� 1  2  3  4 --- GPIO0 1 3 22
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;		//GPIO27��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO22 = 0;		//GPIO27����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;		//GPIO0��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO0 = 0;		//GPIO0����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO0 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;		//GPIO1��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;		//GPIO1����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;		//GPIO3��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO3 = 0;		//GPIO3����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 0;   // ���Ų�����ϵͳʱ��ͬ��
//	//�û���ť  �ߵ�ƽ��Ч����   �����ⲿ�ж�
	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;		//GPIO12��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;		//GPIO12����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;		//�����ڲ�����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;   // ���Ų�����ϵͳʱ��ͬ��
	//��ѹģ���ѹ������ʼ��  �͵�ƽ����
	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;		//GPIO3��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO8 = 0;		//GPIO3����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;		//�����ڲ�����  �ⲿ����
	GpioCtrlRegs.GPAQSEL1.bit.GPIO8 = 0;   // ���Ų�����ϵͳʱ��ͬ��

	EDIS;
}
/*-----------------------------------------------------------
 *�����Ͱ�������ɨ��
 *����һ��ɨ�����ڵ���ʱ��Ӧ
 *����GPIO 7 12 18 22 ---------- 0 1 2 3
 *����GPIO 0 1 3 22 ---------- 0 1 2 3
 *---------------------------------------------------------*/
void DIP_Scan(void) {
	Uint16 key = 0x00;
	if ((DIP0() == 0 || DIP1() == 0 || DIP2() == 0 || DIP3() == 0)) {
		if (DIP0() == 0) {
			key |= 0x01; //0
		} else if (DIP1() == 0) {
			key |= 0x02; //1
		} else if (DIP2() == 0) {
			key |= 0x04; //3
		} else if (DIP3() == 0) {
			key |= 0x08; //22
		}
	} else {
		key = 0x00;
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
u8 STOP_Scan(void) {
//���ն˽�ѹ��ѹ�������
	if (ISOver() == 0) {
		return Excep_jygy;
	}
	return 0;
}

