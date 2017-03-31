/*
 * timer.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "HK_all_include.h"


timer0 timer0Base={0,0};

__interrupt void cpu_timer0_isr(void);



//---------------------��ʱ��0��ʼ��------------------------
//1ms �ж�
void Timer0_init()
{
	InitCpuTimers();

	//�ж����ò���-----1,����ģ���ж�ʹ�ܣ�λ�� Timer->RegsAddr->TCR.bit.TIE = 1;
	ConfigCpuTimer(&CpuTimer0, 60, 1000);//60MHz CPU Freq, 1 millisecond Period (in uSeconds)
	CpuTimer0Regs.TCR.all = 0x4001;		   // Use write-only instruction to set TSS bit = 0

	//�ж����ò���-----2����ӳ���жϷ�����
	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;
	//�ж����ò���-----3������CPU�ж�Y
	IER |= M_INT1;
	//�ж����ò���-----4������Y�ж���ĵڼ�λ
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}

//---------------------��ʱ��1��ʼ��------------------------
// �ж�
void Timer1_init()
{
//	InitCpuTimers();   //ǰ���Ѿ���ʼ�����

	//�ж����ò���-----1,����ģ���ж�ʹ�ܣ�λ�� Timer->RegsAddr->TCR.bit.TIE = 1;
	ConfigCpuTimer(&CpuTimer1, 60, 5000);//60MHz CPU Freq, 5ms
	CpuTimer1Regs.TCR.all = 0x4001;		   // Use write-only instruction to set TSS bit = 0

	//�ж����ò���-----2����ӳ���жϷ�����
	// Interrupts that are used in this example are re-mapped to
	// ISR functions found within this file.
	EALLOW;
	PieVectTable.TINT1 = &cpu_timer1_isr;
	EDIS;
	//�ж����ò���-----3������CPU�ж�Y
	IER |= M_INT13;
	//�ж����ò���-----4������Y�ж���ĵڼ�λ
//	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}



__interrupt void cpu_timer0_isr(void)
{

   timer0Base.msCounter++;
   timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 1;

   //����ͨѶ����
   if(SCI_Msg.Mark_Para.Status_Bits.rFifoDataflag == 1)
   {
	   SCI_Msg.timerOut++;
   }

   // Acknowledge this interrupt to receive more interrupts from group 1
	EALLOW;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	EDIS;

}



//interrupt void cpu_timer1_isr(void)
//{
//   CpuTimer1.InterruptCount++;
//   // The CPU acknowledges the interrupt.
//   EDIS;
//}
//
//interrupt void cpu_timer2_isr(void)
//{  EALLOW;
//   CpuTimer2.InterruptCount++;
//   // The CPU acknowledges the interrupt.
//   EDIS;
//}

