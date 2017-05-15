//============================================================================
// Filename:    A2_Module_GlobalVariableDefAndInit.c                         =
// Author:      xuyq                                                         =
// Built:       20/11/2010                                                   =
// Description: Variable Definitions.                                        =
// Modified:                                                                 =
//============================================================================
// Start here
//============================================================================
#include "GlobalVariableDefAndInits.h"
#include "Module_Project.h"

//============================================================================
//============================================================================
// structure definition
//============================================================================
struct SAMPLE_REG    Sample;
struct MODULECONTROL_REG  ModuleCtlReg;

//===========================================================================
// Variable definition
//===========================================================================

//===========================================================================
// Function definition
//===========================================================================
//===========================================================================
// Function Name: InitB01_50_120V_GlobalVariable()
// Description:   Init the global veriable
//===========================================================================
void Init_GlobalVariable(void)
{
		Uint16 i;

		Sample.JSOutVoltSample  = 0;
		Sample.JSOutVoltSampleOld = 0;
		Sample.JSOutVoltSampleAccSum = 0;
		Sample.JSOutVolt = 0;
		Sample.JSOutVoltReal = 0;

		Sample.JSOutCurrentSample = 0;
		Sample.JSOutCurrentSampleOld = 0;
		Sample.JSOutCurrentSampleAccSum = 0;
		Sample.JSOutCurrent = 0;
		Sample.JSOutCurrentReal = 0;

		Sample.JSACVoltSample = 0;
		Sample.JSACVoltSampleOld = 0;
		Sample.JSACVoltSampleAccSum = 0;
		Sample.JSACVolt = 0;
		Sample.JSACVoltReal = 0;

		Sample.JSACCurrentSample = 0;
		Sample.JSACCurrentSampleOld = 0;
		Sample.JSACCurrentSampleAccSum = 0;
		Sample.JSACCurrent = 0;
		Sample.JSACCurrentReal = 0;

		//定位电压
		for(i=0;i<3;i++){
			Sample.LocateVoltSample[i] = 0;
			Sample.LocateVoltSampleOld[i] = 0;
			Sample.LocateVoltSampleAccSum[i] = 0;
			Sample.LocateVolt[i] = 0;
			Sample.LocateVoltReal[i] = 0;
		}

		//输出过压保护
		Sample.JSOutVoltProtectSample = 0;
		Sample.JSOutVoltProtectSampleOld = 0;
		Sample.JSOutVoltProtectSampleAccSum = 0;
		Sample.JSOutVoltProtect = 0;
		Sample.JSOutVoltProtectReal = 0;

		Sample.DC3P3VoltSample= 0;
		Sample.DC3P3VoltSampleOld= 0;
		Sample.DC3P3VoltSampleAccSum= 0;
		Sample.DC3P3Volt= 0;
		Sample.DC3P3VoltReal= 0;

		Sample.DC5VoltSample= 0;
		Sample.DC5VoltSampleOld= 0;
		Sample.DC5VoltSampleAccSum= 0;
		Sample.DC5Volt= 0;
		Sample.DC5VoltReal= 0;

		Sample.P12VoltSample= 0;
		Sample.P12VoltSampleOld= 0;
		Sample.P12VoltSampleAccSum= 0;
		Sample.P12Volt= 0;
		Sample.P12VoltReal= 0;

		Sample.N12VoltSample= 0;
		Sample.N12VoltSampleOld= 0;
		Sample.N12VoltSampleAccSum= 0;
		Sample.N12Volt= 0;
		Sample.N12VoltReal= 0;

		for(i = 0; i < FASTBUFLENTH; i++)
		{
				Sample.JSACVoltSampleBuffer[i]  = 0;
				Sample.JSACCurrentSampleBuffer[i]  = 0;
				Sample.JSOutVoltSampleBuffer[i]  = 0;
				Sample.JSOutCurrentSampleBuffer[i]  = 0;
				Sample.DC3P3VoltSampleBuffer[i] = 0;
				Sample.DC5VoltSampleBuffer[i] = 0;
				Sample.P12VoltSampleBuffer[i]   = 0;
				Sample.N12VoltSampleBuffer[i] = 0;
				Sample.JSOutVoltProtectSampleBuffer[i] = 0;
				Sample.LocateVoltSampleBuffer[0][i] = 0;
				Sample.LocateVoltSampleBuffer[1][i] = 0;
				Sample.LocateVoltSampleBuffer[2][i] = 0;
		}



		// Module Control
		ModuleCtlReg.BufferOffset                         =0;
		ModuleCtlReg.OutputVoltSet                           = ModultOutputVotInitSet;
		ModuleCtlReg.OutputVoltSetBackup              = ModuleCtlReg.OutputVoltSet;
		ModuleCtlReg.ModuleState                            = MODULE_STATE_OFF;


		ModuleCtlReg.OpenRelaydelayTimeCount				= 0;
		ModuleCtlReg.OpenRelayTimeCount						= 0;

		ModuleCtlReg.OpenPFCTimeCount                       = 0;

		ModuleCtlReg.StateMachineNum                        = 0;
		ModuleCtlReg.SoftStartFanTimeCount                    = 0;
		ModuleCtlReg.ShortRestartTimeCount                  = 0;


		ModuleCtlReg.LEDCtlTimeCount                        = 0;
		ModuleCtlReg.OverloadTimeCount				    = 0;

		// IO Init
		RELAYA_OFF();


}

//===========================================================================
// End of file.
//===========================================================================
