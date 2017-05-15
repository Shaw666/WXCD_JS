/*
 * ad.h
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Module_Project.h"

void ExInt_Init(void);
interrupt void xint1_isr(void);


//###########################################################################
//�ⲿ�жϣ��½��ؼ��
//����XINT1�ⲿ�ж�
//��ͣ�ж�
//---------------------------------------------------------------------------------------------
void ExInt_Init(void)
{
	//�ж����ò���-----2����ӳ���жϷ�����
	EALLOW;	
    PieVectTable.XINT1 = &xint1_isr;
	EDIS;
	//�ж����ò���-----3������CPU�ж�Y
	IER |= M_INT1;
	//�ж����ò���-----4������Y�ж���ĵڼ�λ
	PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Gropu 1 INT4   //XINT1
    // Enable CPU INT1
	EALLOW;
	GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 6;
	EDIS;
	XIntruptRegs.XINT1CR.bit.POLARITY = 1;      // 1Ϊ�����أ�0��2Ϊ�½��أ�3Ϊ˫����
	//�ж����ò���-----1,����ģ���ж�ʹ�ܣ�
	XIntruptRegs.XINT1CR.bit.ENABLE = 1;         // Enable XINT1
}

//�ⲿ��ͣ��ť GPIO4
interrupt void xint1_isr(void)
{
	if(ISStart()==1){
		//�û����°�ť
	}
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

