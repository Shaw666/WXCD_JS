/*
 * led.h
 *
 *  Created on: 2017-3-21
 *      Author: Shaw
 */

#ifndef GPIO_H_
#define GPIO_H_

//���ն˶�ȡ�����궨��
#define DIP0() GpioDataRegs.GPADAT.bit.GPIO0
#define DIP1() GpioDataRegs.GPADAT.bit.GPIO1
#define DIP2() GpioDataRegs.GPADAT.bit.GPIO3
#define DIP3() GpioDataRegs.GPADAT.bit.GPIO22
//�����翪�� ״̬
#define ISStart() GpioDataRegs.GPADAT.bit.GPIO12
#define Excep_kgdk 0x02
//��ͣ���� �жϴ���
//�̵�������
#define RELAYON()  GpioDataRegs.GPASET.bit.GPIO11 = 1	//GPIO11����ߵ�ƽ  �պ�
#define RELAYOFF() GpioDataRegs.GPACLEAR.bit.GPIO11 = 1		//GPIO11����͵�ƽ  �Ͽ�
//��ѹģ���ѹ����
#define ISOver() GpioDataRegs.GPADAT.bit.GPIO8
#define Excep_jygy 0x03 //��ѹģ���ѹ
//********************�ָ���***********************************

//����˶�ȡ�����궨��
//#define DIP0() GpioDataRegs.GPADAT.bit.GPIO7
//#define DIP1() GpioDataRegs.GPADAT.bit.GPIO12
//#define DIP2() GpioDataRegs.GPADAT.bit.GPIO18
//#define DIP3() GpioDataRegs.GPADAT.bit.GPIO22
//#define qdyckey() GpioDataRegs.GPADAT.bit.GPIO3
//#define Excep_qdyc 0x01 //�����쳣
//���ȿ���
extern u8 key;

void GPIO_INit(void);
void KEY_Init(void);
void DIP_Scan(void);
u8 STOP_Scan(void);
#endif /* LED_H_ */
