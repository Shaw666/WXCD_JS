//============================================================================
// Filename:    Module_GlobalVariableDefAndInits.h                           =
// Author:      xuyq                                                         =
// Built:       2012/01/12                                                   =
// Description: Macro define and Variable Definitions.                       =
// Modified:                                                                 =
// Cali = calibration                                                        =
// Para = parameter                                                          =
//============================================================================
// Start here
//============================================================================
#ifndef  MODULE_GLOBALVARIABLEDEFANDINITS_H
#define  MODULE_GLOBALVARIABLEDEFANDINITS_H

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include "Module_Project.h"

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================
// STRUCT DEFINE
//===========================================================================
// SAMPLE FOR PHASE
struct SAMPLE_REG {
	// Signal sample
//接收端输出电压
	Uint16 JSOutVoltSample;
	Uint16 JSOutVoltSampleOld;
	Uint16 JSOutVoltSampleBuffer[64];
	Uint32 JSOutVoltSampleAccSum;
	Uint16 JSOutVolt;
	Uint16 JSOutVoltReal;
//接收端输出电流
	Uint16 JSOutCurrentSample;
	Uint16 JSOutCurrentSampleOld;
	Uint16 JSOutCurrentSampleBuffer[64];
	Uint32 JSOutCurrentSampleAccSum;
	Uint16 JSOutCurrent;
	Uint16 JSOutCurrentReal;
//接收端输入交流电压
	Uint16 JSACVoltSample;
	Uint16 JSACVoltSampleOld;
	Uint16 JSACVoltSampleBuffer[64];
	Uint32 JSACVoltSampleAccSum;
	Uint16 JSACVolt;
	Uint16 JSACVoltReal;
//接收端输入交流电流
	Uint16 JSACCurrentSample;
	Uint16 JSACCurrentSampleOld;
	Uint16 JSACCurrentSampleBuffer[64];
	Uint32 JSACCurrentSampleAccSum;
	Uint16 JSACCurrent;
	Uint16 JSACCurrentReal;
//定位电压
	Uint16 LocateVoltSample[3];
	Uint16 LocateVoltSampleOld[3];
	Uint16 LocateVoltSampleBuffer[3][64];
	Uint32 LocateVoltSampleAccSum[3];
	Uint16 LocateVolt[3];
	Uint16 LocateVoltReal[3];
//输出过压保护
	Uint16 JSOutVoltProtectSample;
	Uint16 JSOutVoltProtectSampleOld;
	Uint16 JSOutVoltProtectSampleBuffer[64];
	Uint32 JSOutVoltProtectSampleAccSum;
	Uint16 JSOutVoltProtect;
	Uint16 JSOutVoltProtectReal;

	Uint16 DC3P3VoltSample;
	Uint16 DC3P3VoltSampleOld;
	Uint16 DC3P3VoltSampleBuffer[64];
	Uint32 DC3P3VoltSampleAccSum;
	Uint16 DC3P3Volt;
	Uint16 DC3P3VoltReal;

	Uint16 DC5VoltSample;
	Uint16 DC5VoltSampleOld;
	Uint16 DC5VoltSampleBuffer[64];
	Uint32 DC5VoltSampleAccSum;
	Uint16 DC5Volt;
	Uint16 DC5VoltReal;

	Uint16 P12VoltSample;
	Uint16 P12VoltSampleOld;
	Uint16 P12VoltSampleBuffer[64];
	Uint32 P12VoltSampleAccSum;
	Uint16 P12Volt;
	Uint16 P12VoltReal;

	Uint16 N12VoltSample;
	Uint16 N12VoltSampleOld;
	Uint16 N12VoltSampleBuffer[64];
	Uint32 N12VoltSampleAccSum;
	Uint16 N12Volt;
	Uint16 N12VoltReal;

};

// CONTROL FOR MODULE
struct MODULECONTROL_REG {

	// control
	Uint16 BufferOffset;
	Uint16 OutputVoltSet;
	Uint16 OutputVoltSetBackup;
	Uint16 ModuleState;
	Uint16 StateMachineNum;
	Uint16 WorkState;
	//Uint16  WorkCmd;
	// relay
	Uint16 OpenRelaydelayTimeCount;
	Uint16 OpenRelayTimeCount;
	// PFC
	Uint16 OpenPFCTimeCount;
	Uint16 CheckPFCTimeCount;
	// FAN
	Uint16 SoftStartFanTimeCount;
	// Protect
	Uint16 PeotectDelayTimeCount;
	Uint16 ShortRestartTimeCount;

	// LED
	Uint16 LEDCtlTimeCount;
	Uint16 OverloadTimeCount;
};

//===========================================================================
// Control and fault and state bit field definition
//===========================================================================

// Module PFC fault msg
struct Module_Fault_BITS {                   // bit   Init description
//发射端
	Uint16 PFCVoltTooLow :1;     		       //    0    0-- ok    .1-  error
	Uint16 PFCVoltTooHigh :1;     		      //      0    0-- ok    .1-  error
	Uint16 IGBTTemperatureFault :1;				//       0-- ok    .1-  error
	Uint16 FSAuxiliaryPower :1;				// 0--ok 1- error
	Uint16 FANFault :1;						//暂无     0-- ok    .1-  error

//接收端
	Uint16 JSACVoltHighFault :1;		//      0    0-- ok    .1-  error
	Uint16 JSACCurrentHighFault :1;   //      0    0-- ok    .1-  error
	Uint16 JSOutputVoltHighFault:1;
	Uint16 JSOutputCurrentHighFault:1;
	Uint16 BuckVoltHighFault :1;		//0 0--ok  .1-- error
//共用
	Uint16 CommunicationFault :1;			//      0-- ok    .1-  error
	Uint16 SCICommFault :1;
	Uint16 rsvd :4;                         			//  reserved
};
union Module_Fault_REG {
	Uint16 all;
	struct Module_Fault_BITS bit;
};

// Module state msg
struct Module_State_BITS {                   	 // bit  Init description

	Uint16 FSRELAY_STATE :1;                  //     0----OFF  .1---ON
	Uint16 SCRAMBUTTON_STATE :1;			 //    0--OFF  1---ON
	Uint16 FAN_STATE :1;
	Uint16 PFC_STATE :1;
	Uint16 PWM_STATE :1;
	Uint16 PWMDRIVER_STATE :1;

	Uint16 JSRELAY_STATE :1;                  //     0----OFF  .1---ON
	Uint16 START_STATE :1;
	Uint16 rsvd :8;                          //      reserved
};
union Module_State_REG {
	Uint16 all;
	struct Module_State_BITS bit;
};

//============================================================================
// External References & Function Declarations:
//============================================================================
// struct
extern struct SAMPLE_REG Sample;
extern union Module_State_REG ModuleStateReg;
extern struct MODULECONTROL_REG ModuleCtlReg;
extern union Module_Fault_REG ModuleFault;

/******************************************************************************
 * EXTERN FUNCTION PROTOTYPE
 ******************************************************************************/
extern void Init_GlobalVariable(void);

//===========================================================================
#ifdef __cplusplus
}
#endif

#endif  // end of A2_MODULE_GLOBALVARIABLEDEFANDINITS_H definition
//===========================================================================
// End of file.
//===========================================================================
