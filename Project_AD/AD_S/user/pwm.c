/*
 * pwm.c
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "HK_all_include.h"

void EPWM1_Config(u16 period) {

	InitEPwm1Gpio();
	EPwm1Regs.TBPRD = period;                        // Set timer period
	EPwm1Regs.TBPHS.half.TBPHS = 0;           // Phase is 0
	EPwm1Regs.TBCTR = 0x0000;                      // Clear counter

	// Setup TBCLK
	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;

	// Setup compare
	EPwm1Regs.CMPA.half.CMPA = 300;
	EPwm1Regs.CMPB = 1500-300;
	// Set actions
	//0------CAU
	EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;
	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
	EPwm1Regs.AQCTLA.bit.PRD = AQ_CLEAR;
	EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
	//PRD----CBD
	EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
	EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;
	EPwm1Regs.AQCTLB.bit.PRD = AQ_SET;
	EPwm1Regs.AQCTLB.bit.CBD = AQ_CLEAR;

//	// Active Low PWMs - Setup Deadband
//	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
//	EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;
//	EPwm1Regs.DBCTL.bit.IN_MODE = DBB_RED_DBA_FED;
//	EPwm1Regs.DBRED = 64;
//	EPwm1Regs.DBFED = 64;

//	   // Interrupt where we will change the Deadband
//	   EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
//	   EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
//	   EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}


void EPWM2_Config(u16 period) {

	InitEPwm2Gpio();
	EPwm2Regs.TBPRD = period;                        // Set timer period
	EPwm2Regs.TBPHS.half.TBPHS = 3000;           // Phase is 0
	EPwm2Regs.TBCTR = 0x0000;                      // Clear counter

	// Setup TBCLK
	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
	EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Disable phase loading
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;

	// Setup compare
	EPwm2Regs.CMPA.half.CMPA = 300;
	EPwm2Regs.CMPB = 300;
	// Set actions
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;             // Set PWM1A on CAU
	EPwm2Regs.AQCTLA.bit.PRD = AQ_SET;			  // Clear PWM1A on CAD
	EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;             // Set PWM1A on CAU
	EPwm2Regs.AQCTLB.bit.PRD = AQ_SET;			  // Clear PWM1A on CAD
//	// Active Low PWMs - Setup Deadband
//	EPWM2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
//	EPWM2Regs.DBCTL.bit.POLSEL = DB_ACTV_HI;
//	EPWM2Regs.DBCTL.bit.IN_MODE = DBB_RED_DBA_FED;
//	EPWM2Regs.DBRED = 64;
//	EPWM2Regs.DBFED = 64;

//	   // Interrupt where we will change the Deadband
//	   EPWM2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
//	   EPWM2Regs.ETSEL.bit.INTEN = 1;                // Enable INT
//	   EPWM2Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event

}
