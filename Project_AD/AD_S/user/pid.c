/*
 * pid.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "Module_Project.h"

#define PVAL_V (PID_V.Error-PID_V.Last1Error)
#define IVAL_V (PID_V.Error)
#define DVAL_V (PID_V.Error-2*PID_V.Last1Error+PID_V.Last2Error)

#define PVAL_F (PID_F.Error-PID_F.Last1Error)
#define IVAL_F (PID_F.Error)
#define DVAL_F (PID_F.Error-2*PID_F.Last1Error+PID_F.Last2Error)

typedef struct _pid
{
	int MySetPoint; 
	int Input;
	int Output;
	int MyOutput;   
	int Error,Last1Error,Last2Error;  //前一次和前两次误差
	float kp,ki,kd;
}PID_type;

PID_type PID_V;
PID_type PID_F;

u16 F_OUT;  //输出
u16 V_OUT;  //输出

//PID参数初始化
void PID_Init()
{
	/*电压环初始化*/	
    PID_V.Input = 0;
	PID_V.Output = 0;
	PID_V.MyOutput = 0;   //当前PWM输出
	PID_V.Error = 0,PID_V.Last1Error = 0,PID_V.Last2Error = 0;  //前一次和前两次误差
	PID_V.kp = 0.1,PID_V.ki = 0.2,PID_V.kd = 0;

//	PID_F.Input = 0;
//	PID_F.Output = 0;
//	PID_F.MyOutput = 0;   //当前PWM输出
//	PID_F.Error = 0,PID_F.Last1Error = 0,PID_F.Last2Error = 0;  //前一次和前两次误差
//	PID_F.kp = 0.1,PID_F.ki = 0.2,PID_F.kd = 0;

}

u8 xfeq;
void PID_Control_F(void)     //频率环
{
	PID_F.MySetPoint = 50;
	PID_F.Input = xfeq;  //  输入为输出电压
	PID_F.Error = PID_F.MySetPoint-PID_F.Input; 
  	if((PID_F.MySetPoint-PID_F.Input)<10&&(PID_F.MySetPoint-PID_F.Input)>0){}
//		TIM_SetCompare1(TIM1,V_OUT);
	else 
	{
	PID_F.Output = PID_F.MyOutput + PID_F.kp*PVAL_F + PID_F.ki*IVAL_F+PID_F.kd*DVAL_F;//output为电压值	
	PID_F.MyOutput = PID_F.Output; 
	PID_F.Last2Error = PID_F.Last1Error;     
	PID_F.Last1Error = PID_F.Error;
	if(PID_F.MyOutput>647){PID_F.MyOutput=647;}
	if(PID_F.MyOutput<72) {PID_F.MyOutput=72; }
    V_OUT = PID_F.MyOutput;
//	gai(F_OUT);
	}	
}

void PID_Control_V(void)     //电压环
{

	PID_V.MySetPoint =2.8*4095/3.3;
		//36*97.54;                 //12*4095/24/3.3;    620.4
	PID_V.Input = 1;  //  输入为输出电压
	
	PID_V.Error = PID_V.MySetPoint-PID_V.Input; 
	
	PID_V.Output = PID_V.MyOutput + PID_V.kp*PVAL_V + PID_V.ki*IVAL_V+PID_V.kd*DVAL_V;//output为电压值	
	PID_V.MyOutput = PID_V.Output; 
	PID_V.Last2Error = PID_V.Last1Error;     
	PID_V.Last1Error = PID_V.Error;
	if(PID_V.MyOutput>1500-32) {PID_V.MyOutput=1500-32; }
	if(PID_V.MyOutput<15) {PID_V.MyOutput=15; }
	V_OUT = PID_V.MyOutput;
	EPwm1Regs.CMPA.half.CMPA = V_OUT;
	EPwm1Regs.CMPB = 1500-V_OUT;
}

__interrupt void cpu_timer1_isr(void)
{
   CpuTimer1.InterruptCount++;
   if(CpuTimer1.InterruptCount%20==0)
   {
	   CpuTimer1.InterruptCount=0;
//	   D402TOGGLE();
//	   PID_Control_V();
	   Upper_Uart();
	   AdcRegs.ADCSOCFRC1.all = 0xdfde; //软件触发AD 的 SOC0--SOC3采样
   }
  // Acknowledge this interrupt to receive more interrupts from group 1
   EDIS;
}


unsigned char Send_Count; //串口需要发送的数据个数
int j=0;     //串口发送数据计数
void Upper_Uart(void)//上位机发送程序
{
	DataScope_Get_Channel_Data(1,1);   //将电压环PID输入 写入通道 1

	/*******************
	串口发送数据给上位机
	********************/
	Send_Count = DataScope_Data_Generate(10); //生成4个通道的 格式化帧数据，返回帧数据长度
    for( j = 0;j<Send_Count;j++)  //循环发送,直到发送完毕
	{
//    	while (SciaRegs.SCIFFTX.bit.TXFFST != 0);
//		SciaRegs.SCITXBUF=DataScope_OutPut_Buffer[j];

	while(LinaRegs.SCIFLR.bit.TXRDY == 0);
	//Begin transmission
	LinaRegs.SCITD = DataScope_OutPut_Buffer[j];
	}
}
