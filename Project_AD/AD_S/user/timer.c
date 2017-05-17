/*
 * timer.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Module_Project.h"


timer0 timer0Base={0,0};

__interrupt void cpu_timer0_isr(void);



//---------------------定时器0初始化------------------------
//1ms 中断
void Timer0_init()
{
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 60, 1000);//60MHz CPU Freq, 1 millisecond Period (in uSeconds)
	CpuTimer0Regs.TCR.all = 0x4001;		   // Use write-only instruction to set TSS bit = 0
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}

//---------------------定时器1初始化------------------------
// 中断
void Timer1_init()
{
//	//中断配置步骤-----1,开启模块中断使能，位于 Timer->RegsAddr->TCR.bit.TIE = 1;
//	ConfigCpuTimer(&CpuTimer1, 60, 5000);//60MHz CPU Freq, 5ms
//	CpuTimer1Regs.TCR.all = 0x4001;		   // Use write-only instruction to set TSS bit = 0
//
//	//中断配置步骤-----2，重映射中断服务函数
//	// Interrupts that are used in this example are re-mapped to
//	// ISR functions found within this file.
//	EALLOW;
//	PieVectTable.TINT1 = &cpu_timer1_isr;
//	EDIS;
//	//中断配置步骤-----3，连接CPU中断Y
//	IER |= M_INT13;
//	//中断配置步骤-----4，连接Y中断里的第几位
////	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}



__interrupt void cpu_timer0_isr(void)
{
   timer0Base.msCounter++;
   timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 1;
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

