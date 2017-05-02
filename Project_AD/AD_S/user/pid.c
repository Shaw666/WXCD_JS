/*
 * pid.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "HK_all_include.h"

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
	int Error,Last1Error,Last2Error;  //ǰһ�κ�ǰ�������
	float kp,ki,kd;
}PID_type;

PID_type PID_V;
PID_type PID_F;

u16 F_OUT;  //���
u16 V_OUT;  //���

//PID������ʼ��
void PID_Init()
{
	/*��ѹ����ʼ��*/	
    PID_V.Input = 0;
	PID_V.Output = 0;
	PID_V.MyOutput = 0;   //��ǰPWM���
	PID_V.Error = 0,PID_V.Last1Error = 0,PID_V.Last2Error = 0;  //ǰһ�κ�ǰ�������
	PID_V.kp = 0.1,PID_V.ki = 0.2,PID_V.kd = 0;

//	PID_F.Input = 0;
//	PID_F.Output = 0;
//	PID_F.MyOutput = 0;   //��ǰPWM���
//	PID_F.Error = 0,PID_F.Last1Error = 0,PID_F.Last2Error = 0;  //ǰһ�κ�ǰ�������
//	PID_F.kp = 0.1,PID_F.ki = 0.2,PID_F.kd = 0;

}

u8 xfeq;
void PID_Control_F(void)     //Ƶ�ʻ�
{
	PID_F.MySetPoint = 50;
	PID_F.Input = xfeq;  //  ����Ϊ�����ѹ
	PID_F.Error = PID_F.MySetPoint-PID_F.Input; 
  	if((PID_F.MySetPoint-PID_F.Input)<10&&(PID_F.MySetPoint-PID_F.Input)>0){}
//		TIM_SetCompare1(TIM1,V_OUT);
	else 
	{
	PID_F.Output = PID_F.MyOutput + PID_F.kp*PVAL_F + PID_F.ki*IVAL_F+PID_F.kd*DVAL_F;//outputΪ��ѹֵ	
	PID_F.MyOutput = PID_F.Output; 
	PID_F.Last2Error = PID_F.Last1Error;     
	PID_F.Last1Error = PID_F.Error;
	if(PID_F.MyOutput>647){PID_F.MyOutput=647;}
	if(PID_F.MyOutput<72) {PID_F.MyOutput=72; }
    V_OUT = PID_F.MyOutput;
//	gai(F_OUT);
	}	
}

void PID_Control_V(void)     //��ѹ��
{

	PID_V.MySetPoint =2.8*4095/3.3;
		//36*97.54;                 //12*4095/24/3.3;    620.4
	PID_V.Input = ADResReg[15];  //  ����Ϊ�����ѹ
	
	PID_V.Error = PID_V.MySetPoint-PID_V.Input; 
	
	PID_V.Output = PID_V.MyOutput + PID_V.kp*PVAL_V + PID_V.ki*IVAL_V+PID_V.kd*DVAL_V;//outputΪ��ѹֵ	
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
	   D402TOGGLE();
//	   PID_Control_V();
//	   Upper_Uart();
   }
  // Acknowledge this interrupt to receive more interrupts from group 1
   EDIS;
}


unsigned char Send_Count; //������Ҫ���͵����ݸ���
int j=0;     //���ڷ������ݼ���
void Upper_Uart(void)//��λ�����ͳ���
{
	DataScope_Get_Channel_Data(ADResReg[15],1);   //����ѹ��PID���� д��ͨ�� 1
	DataScope_Get_Channel_Data(ADResReg[14]*3.3/4095.0,2);   //����ѹ��PID���� д��ͨ�� 1
//	DataScope_Get_Channel_Data(ADResReg[13]*3.3/4095.0,3);   //����ѹ��PID���� д��ͨ�� 1
//	DataScope_Get_Channel_Data(ADResReg[12]*3.3/4095.0,4);   //����ѹ��PID���� д��ͨ�� 1

	/*******************
	���ڷ������ݸ���λ��
	********************/
	Send_Count = DataScope_Data_Generate(2); //����4��ͨ���� ��ʽ��֡���ݣ�����֡���ݳ���
    for( j = 0;j<Send_Count;j++)  //ѭ������,ֱ���������
	{
//    	while (SciaRegs.SCIFFTX.bit.TXFFST != 0);
//		SciaRegs.SCITXBUF=DataScope_OutPut_Buffer[j];

	while(LinaRegs.SCIFLR.bit.TXRDY == 0);
	//Begin transmission
	LinaRegs.SCITD = DataScope_OutPut_Buffer[j];
	}
}
