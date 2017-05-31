/*
 * ad.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "Module_Project.h"

interrupt void Module_ADCalibration_ISR(void);

//-------------------------------------------------------------------------
//AD��ʼ��
//��ģ��ֵ���뵽A1ͨ��
//�������P10���루6�룬λ�ڰ������Ͻǣ���RG��Ϊ�ɵ������������ɽ�RGֱ�ӽ��뵽ģ�������A1��������

//##########################################################################
void ADC_Config(void) {

	InitAdc();  // �ȵ��ùٷ�ADĬ�ϵ����ú�����ʼ�������룩
	InitAdcAio();

	EALLOW;
	PieVectTable.ADCINT3 = &Module_ADCalibration_ISR;  // AD�жϺ���ӳ��
	EDIS;

	PieCtrlRegs.PIEIER10.bit.INTx3 = 1;	// Enable INT 10.3 in the PIE
	IER |= M_INT10; 						// Enable CPU Interrupt 10

// Configure ADC
	EALLOW;
//	AdcRegs.ADCCTL2.bit.CLKDIV2EN = 0; //ADCclock = CPUclock
	AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;    //REF Int
	AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;//ADCINT1 trips after AdcResults latch //--> P39 HIKE,EOCx��������Դѡ��
	AdcRegs.INTSEL3N4.bit.INT3E = 1;	//Enabled ADCINT3
	AdcRegs.INTSEL3N4.bit.INT3CONT = 0;	//Disable ADCINT3 Continuous mode��single conversion mode
	//HIKE,ADCINTx EOC Source Select, ���ĸ� EOCx �����ж�
	AdcRegs.INTSEL3N4.bit.INT3SEL = 3;//setup EOC3 to trigger ADCINT1 to fire , EOC3 is trigger for ADCINTx

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADC�������ָ����SOCͨ����һ�����������ݲ���ȷ���ӵڶ�����ʼΪ��ȷ�����ݣ�������SOC0 SOC1���ݲ�������

//	//P17 P35 ͬ�������Ļ�������SOC��CHSEL����һ��
//	//Simultaneous sampling enable for SOC2/SOC3
//	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN0=1;	//SOC0/1 Simultaneous Sampling Enable	,SOC0�SOC1��Ϲ���
//	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 1;	//SOC0 (ADCINA1/ADCINB1 pair)������ģ��ͨ������
//	AdcRegs.ADCSOC1CTL.bit.CHSEL 	= 1;	//SOC1 (ADCINA1/ADCINB1 pair)������ģ��ͨ������
//
//	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN2=1;	//SOC2/3 Simultaneous Sampling Enable	,SOC2�SOC3��Ϲ���
//	AdcRegs.ADCSOC2CTL.bit.CHSEL 	= 1;	//SOC2 (ADCINA1/ADCINB1 pair)������ģ��ͨ������
//	AdcRegs.ADCSOC3CTL.bit.CHSEL 	= 1;	//SOC3 (ADCINA1/ADCINB1 pair)������ģ��ͨ������

//SOC ������������ֵ��ͨ����������
//0-7ΪA0��A7
//8-16ΪB0��B7
//	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 0;	//set SOC0 channel select to ADCINA0
	AdcRegs.ADCSOC1CTL.bit.CHSEL = 1;	//set SOC1 channel select to ADCINB0
	AdcRegs.ADCSOC2CTL.bit.CHSEL = 2;	//set SOC2 channel select to ADCINA1
	AdcRegs.ADCSOC3CTL.bit.CHSEL = 3;	//set SOC3 channel select to ADCINB1
	AdcRegs.ADCSOC4CTL.bit.CHSEL = 4;	//set SOC0 channel select to ADCINA0
//	AdcRegs.ADCSOC5CTL.bit.CHSEL 	= 5;	//set SOC1 channel select to ADCINB0
	AdcRegs.ADCSOC6CTL.bit.CHSEL = 6;	//set SOC2 channel select to ADCINA1
	AdcRegs.ADCSOC7CTL.bit.CHSEL = 7;	//set SOC3 channel select to ADCINB1
	AdcRegs.ADCSOC8CTL.bit.CHSEL = 8;	//set SOC0 channel select to ADCINA0
	AdcRegs.ADCSOC9CTL.bit.CHSEL = 9;	//set SOC1 channel select to ADCINB0
	AdcRegs.ADCSOC10CTL.bit.CHSEL = 10;	//set SOC2 channel select to ADCINA1
	AdcRegs.ADCSOC11CTL.bit.CHSEL = 11;	//set SOC3 channel select to ADCINB1
	AdcRegs.ADCSOC12CTL.bit.CHSEL = 12;	//set SOC0 channel select to ADCINA0
//	AdcRegs.ADCSOC13CTL.bit.CHSEL 	= 13;	//set SOC1 channel select to ADCINB0
//	AdcRegs.ADCSOC14CTL.bit.CHSEL = 14;	//set SOC2 channel select to ADCINA1
	AdcRegs.ADCSOC15CTL.bit.CHSEL = 15;	//set SOC3 channel select to ADCINB1

//-->HIKE ,ADѡ�񴥷�Դ,������� P34
//	AdcRegs.ADCSOC0CTL.bit.TRIGSEL 	= 0x00;
	AdcRegs.ADCSOC1CTL.bit.TRIGSEL = 0x00; //ADCTRIG0- Software only
	AdcRegs.ADCSOC2CTL.bit.TRIGSEL = 0x00;
	AdcRegs.ADCSOC3CTL.bit.TRIGSEL = 0x00;
	AdcRegs.ADCSOC4CTL.bit.TRIGSEL = 0x00;
//	AdcRegs.ADCSOC5CTL.bit.TRIGSEL 	= 0x00; //ADCTRIG0- Software only
	AdcRegs.ADCSOC6CTL.bit.TRIGSEL = 0x00;
	AdcRegs.ADCSOC7CTL.bit.TRIGSEL = 0x00;
	AdcRegs.ADCSOC8CTL.bit.TRIGSEL = 0x00;
	AdcRegs.ADCSOC9CTL.bit.TRIGSEL = 0x00; //ADCTRIG0- Software only
	AdcRegs.ADCSOC10CTL.bit.TRIGSEL = 0x00;
	AdcRegs.ADCSOC11CTL.bit.TRIGSEL = 0x00;
	AdcRegs.ADCSOC12CTL.bit.TRIGSEL = 0x00;
//	AdcRegs.ADCSOC13CTL.bit.TRIGSEL 	= 0x00; //ADCTRIG0- Software only
	AdcRegs.ADCSOC14CTL.bit.TRIGSEL = 0x00;
	AdcRegs.ADCSOC15CTL.bit.TRIGSEL = 0x00;
// 6+13ADCʱ������
//	AdcRegs.ADCSOC0CTL.bit.ACQPS 	= 6;
	AdcRegs.ADCSOC1CTL.bit.ACQPS = 6;//set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC2CTL.bit.ACQPS = 6;
	AdcRegs.ADCSOC3CTL.bit.ACQPS = 6;
	AdcRegs.ADCSOC4CTL.bit.ACQPS = 6;
//	AdcRegs.ADCSOC5CTL.bit.ACQPS 	= 6;	//set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC6CTL.bit.ACQPS = 6;
	AdcRegs.ADCSOC7CTL.bit.ACQPS = 6;
	AdcRegs.ADCSOC8CTL.bit.ACQPS = 6;
	AdcRegs.ADCSOC9CTL.bit.ACQPS = 6;//set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC10CTL.bit.ACQPS = 6;
	AdcRegs.ADCSOC11CTL.bit.ACQPS = 6;
	AdcRegs.ADCSOC12CTL.bit.ACQPS = 6;
//	AdcRegs.ADCSOC13CTL.bit.ACQPS 	= 6;	//set SOC0 S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
	AdcRegs.ADCSOC14CTL.bit.ACQPS = 6;
	AdcRegs.ADCSOC15CTL.bit.ACQPS = 6;

	EDIS;
}

void Module_ADCalibration_ISR(void) {
//	Uint16 LocateVCount =0;
	EALLOW;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;   // Acknowledge interrupt to PIE
	EDIS;
	AdcRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //Clear ADCINT1 flag reinitialize for next SOC

//	GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1;
//---------------------------�˲�-----------------------------------------
//	Sample.JSOutVoltSampleBuffer[ModuleCtlReg.BufferOffset] =
//			((JSOutVolt_ADCResult + Sample.JSOutVoltSampleOld * 3) >> 2);
//	Sample.JSOutVoltSampleOld =
//			Sample.JSOutVoltSampleBuffer[ModuleCtlReg.BufferOffset];
//
//	Sample.JSOutCurrentSampleBuffer[ModuleCtlReg.BufferOffset] =
////			((
//					JSOutCurrent_ADCResult ;
//					+ Sample.JSOutCurrentSampleOld * 1) >> 1);
//	Sample.JSOutCurrentSampleOld =
//			Sample.JSOutCurrentSampleBuffer[ModuleCtlReg.BufferOffset];

//	Sample.JSACVoltSampleBuffer[ModuleCtlReg.BufferOffset] =
//			((JSACVolt_ADCResult + Sample.JSACVoltSampleOld * 3) >> 2);
//	Sample.JSACVoltSampleOld =
//			Sample.JSACVoltSampleBuffer[ModuleCtlReg.BufferOffset];
//
//	Sample.JSACCurrentSampleBuffer[ModuleCtlReg.BufferOffset] =
//			((JSACCurrent_ADCResult + Sample.JSACCurrentSampleOld * 3) >> 2);
//	Sample.JSACCurrentSampleOld =
//			Sample.JSACCurrentSampleBuffer[ModuleCtlReg.BufferOffset];

//	Sample.LocateVoltSampleBuffer[0][ModuleCtlReg.BufferOffset] =
//			((LocateVolt_ADCResult1 + Sample.LocateVoltSampleOld[0] * 3) >> 2);
//	Sample.LocateVoltSampleOld[0] =
//			Sample.LocateVoltSampleBuffer[0][ModuleCtlReg.BufferOffset];
//	Sample.LocateVoltSampleBuffer[1][ModuleCtlReg.BufferOffset] =
//			((LocateVolt_ADCResult2 + Sample.LocateVoltSampleOld[1] * 3) >> 2);
//	Sample.LocateVoltSampleOld[1] =
//			Sample.LocateVoltSampleBuffer[1][ModuleCtlReg.BufferOffset];
//	Sample.LocateVoltSampleBuffer[2][ModuleCtlReg.BufferOffset] =
//			((LocateVolt_ADCResult3 + Sample.LocateVoltSampleOld[2] * 3) >> 2);
//	Sample.LocateVoltSampleOld[2] =
//			Sample.LocateVoltSampleBuffer[2][ModuleCtlReg.BufferOffset];


//	Sample.DC3P3VoltSampleBuffer[ModuleCtlReg.BufferOffset] =
//			((DC3P3VoltADCResult + Sample.DC3P3VoltSampleOld * 3) >> 2);
//	Sample.DC3P3VoltSampleOld =
//			Sample.DC3P3VoltSampleBuffer[ModuleCtlReg.BufferOffset];
//
//	Sample.DC5VoltSampleBuffer[ModuleCtlReg.BufferOffset] = ((DC5VoltADCResult
//			+ Sample.DC5VoltSampleOld * 3) >> 2);
//	Sample.DC5VoltSampleOld =
//			Sample.DC5VoltSampleBuffer[ModuleCtlReg.BufferOffset];
//
//	Sample.P12VoltSampleBuffer[ModuleCtlReg.BufferOffset] = ((P12Volt_ADCResult
//			+ Sample.P12VoltSampleOld * 3) >> 2);
//	Sample.P12VoltSampleOld =
//			Sample.P12VoltSampleBuffer[ModuleCtlReg.BufferOffset];
//
//	Sample.N12VoltSampleBuffer[ModuleCtlReg.BufferOffset] = ((N12Volt_ADCResult
//			+ Sample.N12VoltSampleOld * 3) >> 2);
//	Sample.N12VoltSampleOld =
//			Sample.N12VoltSampleBuffer[ModuleCtlReg.BufferOffset];
//
	Sample.JSOutVoltProtectSampleBuffer[ModuleCtlReg.BufferOffset] =
//			((
			JSOutVoltProtect_ADCResult;
//			+ Sample.JSOutVoltProtectSampleOld * 1) >> 1);
//	Sample.JSOutVoltProtectSampleOld = Sample.JSOutVoltProtectSampleBuffer[ModuleCtlReg.BufferOffset];
//
////------------------------------------�ۼ�--------------------------------------
//	Sample.JSOutVoltSampleAccSum +=
//			Sample.JSOutVoltSampleBuffer[ModuleCtlReg.BufferOffset];
//	Sample.JSOutCurrentSampleAccSum +=
//			Sample.JSOutCurrentSampleBuffer[ModuleCtlReg.BufferOffset];
//	Sample.JSACVoltSampleAccSum +=
//			Sample.JSACVoltSampleBuffer[ModuleCtlReg.BufferOffset];
//	Sample.JSACCurrentSampleAccSum +=
//			Sample.JSACCurrentSampleBuffer[ModuleCtlReg.BufferOffset];
//	Sample.LocateVoltSampleAccSum[0] +=
//			Sample.LocateVoltSampleBuffer[0][ModuleCtlReg.BufferOffset];
//	Sample.LocateVoltSampleAccSum[1] +=
//			Sample.LocateVoltSampleBuffer[1][ModuleCtlReg.BufferOffset];
//	Sample.LocateVoltSampleAccSum[2] +=
//			Sample.LocateVoltSampleBuffer[2][ModuleCtlReg.BufferOffset];
//	Sample.DC3P3VoltSampleAccSum +=
//			Sample.DC3P3VoltSampleBuffer[ModuleCtlReg.BufferOffset];
//	Sample.DC5VoltSampleAccSum +=
//			Sample.DC5VoltSampleBuffer[ModuleCtlReg.BufferOffset];
//	Sample.P12VoltSampleAccSum +=
//			Sample.P12VoltSampleBuffer[ModuleCtlReg.BufferOffset];
//	Sample.N12VoltSampleAccSum +=
//			Sample.N12VoltSampleBuffer[ModuleCtlReg.BufferOffset];
	Sample.JSOutVoltProtectSampleAccSum +=
			Sample.JSOutVoltProtectSampleBuffer[ModuleCtlReg.BufferOffset];
//----------------------------------ƽ��---------------------------------
	if (FASTBUFLENTH <= ModuleCtlReg.BufferOffset) {
		ModuleCtlReg.BufferOffset = 0;
//		// output current
//		Sample.JSOutVolt = (Uint16) (Sample.JSOutVoltSampleAccSum >> 1);
//		Sample.JSOutVoltSampleAccSum = 0;
//		Sample.JSOutVoltReal = (Uint16) ((Uint32) Sample.JSOutVolt
//				* (Uint32) JSOutVoltageScale >> 15);
//
//		Sample.JSOutCurrent = (Uint16) (Sample.JSOutCurrentSampleAccSum >> 0);
//		Sample.JSOutCurrentSampleAccSum = 0;
//		Sample.JSOutCurrentReal = (int16) ((Uint32) Sample.JSOutCurrent
//				* (Uint32) JSOutCurrentScale >> 15);
//
//		Sample.JSACVolt = (Uint16) (Sample.JSACVoltSampleAccSum >> 3);
//		Sample.JSACVoltSampleAccSum = 0;
//		Sample.JSACVoltReal = (Uint16) ((Uint32) Sample.JSACVolt
//				* (Uint32) JSACVoltageScale >> 15);
//
//		Sample.JSACCurrent = (Uint16) (Sample.JSACCurrentSampleAccSum >> 3);
//		Sample.JSACCurrentSampleAccSum = 0;
//		Sample.JSACCurrentReal = (int16) ((Uint32) Sample.JSACCurrent
//				* (Uint32) JSACCurrentScale >> 15);

		Sample.JSOutVoltProtect = (Uint16) (Sample.JSOutVoltProtectSampleAccSum >>6 );
		Sample.JSOutVoltProtectSampleAccSum = 0;
		Sample.JSOutVoltProtectReal = (Uint16) ((Uint32)	Sample.JSOutVoltProtect
				* (Uint32) JSOutVoltProtectScale >> 15);
//		for(LocateVCount=0;LocateVCount<3;LocateVCount++){
//			Sample.LocateVolt[LocateVCount] = (Uint16)(Sample.LocateVoltSampleAccSum[LocateVCount] >>3);
//			Sample.LocateVoltSampleAccSum[LocateVCount] = 0;
//			Sample.LocateVoltReal[LocateVCount] = (int16) ((Uint32) Sample.LocateVolt[LocateVCount]
//						* (Uint32) LocateVoltScale >> 15);
//		}

//		Sample.DC3P3Volt = (Uint16) (Sample.DC3P3VoltSampleAccSum >> 3);
//		Sample.DC3P3VoltSampleAccSum = 0;
//		Sample.DC3P3VoltReal = (int16) ((Uint32) Sample.DC3P3Volt
//				* (Uint32) DC3P3VoltScale >> 15);
//
//		Sample.DC5Volt = (Uint16) (Sample.DC5VoltSampleAccSum >> 3);
//		Sample.DC5VoltSampleAccSum = 0;
//		Sample.DC5VoltReal = (int16) ((Uint32) Sample.DC5Volt
//				* (Uint32) DC5VoltScale >> 15);
//
//		Sample.P12Volt = (Uint16) (Sample.P12VoltSampleAccSum >> 3);
//		Sample.P12VoltSampleAccSum = 0;
//		Sample.P12VoltReal = (int16) ((Uint32) Sample.P12Volt
//				* (Uint32) P12VoltScale >> 15);
//
//		Sample.N12Volt = (Uint16) (Sample.N12VoltSampleAccSum >> 3);
//		Sample.N12VoltSampleAccSum = 0;
//		Sample.N12VoltReal = (int16) ((Uint32) Sample.N12Volt
//				* (Uint32) N12VoltScale >> 15);
//��ʱ������˷���
		// Calibration use y=ax+b

	}
	ModuleCtlReg.BufferOffset++;
	AdcRegs.ADCSOCFRC1.all = 0X9FDE;
//	SendRequestSCI(0xA2);//���������ѹ

}

void ArbitramentProtect(void) {

}

