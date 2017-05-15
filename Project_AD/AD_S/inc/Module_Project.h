//============================================================================
// Filename:    Module_Project.h                                             =
// Author:      xuyq                                                         =
// Built:       2012/01/12                                                   =
// Description: Macro define and Variable Definitions.                       =
// Modified:                                                                 =
// Cali = calibration                                                        =
// Para = parameter                                                          =
//============================================================================
// Start here
//============================================================================
#ifndef MODULE_PROJECT_H_
#define MODULE_PROJECT_H_

/******************************************************************************
* INCLUDE FILE
******************************************************************************/
#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File

#define SYSCLK 60000000L

#define uint8_t unsigned char
#define uint16_t Uint16
#define uint32_t Uint32

#define u8 unsigned char
#define u16 Uint16
#define u32 Uint32

#include "GlobalVariableDefAndInits.h"
#include "DSP28x_Project.h"
#include "timer.h"
#include "CANcomm.h"
#include "communicationsci.h"
#include "hardiic.h"
#include "ad.h"
#include "pid.h"
#include "DataScope_DP.h"
#include "gpio.h"
#include "Exint.h"
#include "zm5168.h"
#include "sim_spi.h"

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================
// For different project, the next define need to be changed.
//===========================================================================
//===========================================================================
// Module Serial Number
//===========================================================================
#define  SerialNumber                 					0x1111
//===========================================================================
// Signal Sample
// Description in "AD sample calculate.xls"
//===========================================================================
#define  FASTBUFLENTH					8			//

#define  PFCVoltageScale        				    52800//_IQ15(0.161132)*10
#define  PFCCurrentScale                                   1338//_IQ15(0.004084)*10
#define  P15VoltScale 			                       1584//_IQ15(0.0048339)*10
#define  N15VoltScale   		                       1320//_IQ15(0.004028)*10

#define  DC3P3VoltScale    	                             351 // 290//_IQ15(0.000886)*10  js(0.00107179)
#define  DC5VoltScale  				                   528   //528//_IQ15(0.0016113)*10   js(0.0016117)


#define  JSOutVoltageScale					  23237//_IQ15(0.0709158)*10
#define  JSOutCurrentScale					  1320//_IQ15(0.0040293)*10
#define  JSACVoltageScale					  23237//_IQ15(0.0709158)*10
#define  JSACCurrentScale					  1320//_IQ15(0.0040293)*10
#define  LocateVoltScale  			          264//_IQ15(0.00080586)*10
#define  JSOutVoltProtectScale  			  264   //_IQ15()*10
#define  P12VoltScale 			                       1241 //_IQ15(0.0037875)*10
#define  N12VoltScale   		                       1320//_IQ15(0.0040293)*10

#define PFCVoltageOffset        					4000   //U *10

#define OutputCurrentLimit					40//I*10,1600w

//===========================================================================
// Module Output Parameter Set
//===========================================================================
#define ModultOutputVotInitSet        					2200    	// (220V*10)
#define ModultOutputVotSetMin         					2000		//(240V*10)
#define ModultOutputVotSetMax        					2400		//(200V*10)
#define ModuleOutputCurrLimitInitSet  				72    	//(7.2A*10)
#define ModuleOutputCurrLimitSetMax   				80	//(8A*10)


//===========================================================================
// Control
//===========================================================================
#define  MODULE_STATE_ON       		  	       0xffff   // Module ON
#define  MODULE_STATE_OFF            			0x0000   // Module OFF

#define  OpenRelaydelayTime           					200     // 0.4s=1500*2ms
#define  RelayDelayTime    	          					500      //1000ms=200*2ms
#define  PFCDelayTime      	          					600      // 1.2s=1500*2ms
#define  PeotectDelayTime             					500     // 5s=2500*2m
#define  STATEDelayTime    							100
//===========================================================================
// Module Protect Parameter Set
//===========================================================================

//  voltage low
#define  PFCVoltLowThreshold          		800      	// (220V * 0.8 = 176V)*10
#define  PFCVoltLowHysteresis       		       50      	// (5V) * 10
// voltage high
#define  PFCVoltHighThreshold          		2650   	//2600     // (220V * 1.2 = 260V)*10
#define  PFCVoltHighHysteresis        		100     	// (5V) * 10

 // Tempeature
#define  TmpeatureHighProThresholdValue           	1520		//102
#define  TmpeatureHighThresholdValue            		1500     	// 100°Ê = 150°Ê - 50°Ê
#define  TempeatureHighHysteresisValue           		150       	// 15°Ê
#define  TempeatureLowHysteresisValue           			20       	// 2°Ê
#define  TempeatureLimCurrValue           				1350     	// 85°Ê
// Current Short
#define  LLCOutputShortVoltThresholdValue        		400      	// 10V*10
#define  LLCOverLoad                                 		350000	//2500W
#define  LLCOverLoadHysteresisValue                  	10000
//===========================================================================
// AC Sample Channel Define
//===========================================================================

//#define  DC3P3VoltADCResult     			AdcResult.ADCRESULT1
//#define  DC5VoltADCResult     				AdcResult.ADCRESULT2
//#define  P15Volt_ADCResult     				AdcResult.ADCRESULT4
//#define  N15Volt_ADCResult     				AdcResult.ADCRESULT3
//#define  PFCVolt_ADCResult      			AdcResult.ADCRESULT8
//#define  PFCCurr_ADCResult     			    AdcResult.ADCRESULT9
//#define  Temperature_ADCResult    		    AdcResult.ADCRESULT6

#define  DC3P3VoltADCResult     			AdcResult.ADCRESULT12
#define  DC5VoltADCResult     				AdcResult.ADCRESULT11
#define  P12Volt_ADCResult     				AdcResult.ADCRESULT7
#define  N12Volt_ADCResult     				AdcResult.ADCRESULT10

#define  JSOutVolt_ADCResult				AdcResult.ADCRESULT8
#define  JSOutCurrent_ADCResult				AdcResult.ADCRESULT9
#define  JSACVolt_ADCResult					AdcResult.ADCRESULT1
#define  JSACCurrent_ADCResult				AdcResult.ADCRESULT2
#define  LocateVolt_ADCResult1				AdcResult.ADCRESULT3
#define  LocateVolt_ADCResult2				AdcResult.ADCRESULT4
#define  LocateVolt_ADCResult3				AdcResult.ADCRESULT6

#define  Multiplier_ADCResult  				AdcResult.ADCRESULT14	//‘›≤ª¥¶¿Ì
#define  JSOutVoltProtect_ADCResult				AdcResult.ADCRESULT15


//===========================================================================
// LED State define //LM
//===========================================================================
// GREEN----GPIO33
#define  LED_1_FLICKER()     		       GpioDataRegs.GPBTOGGLE.bit.GPIO33 	= 1
#define  LED_1_ON()           			GpioDataRegs.GPBSET.bit.GPIO33  		= 1
#define  LED_1_OFF()          			GpioDataRegs.GPBCLEAR.bit.GPIO33  	= 1
// RED------GPIO32
#define  LED_2_FLICKER()        		GpioDataRegs.GPBTOGGLE.bit.GPIO32 	= 1
#define  LED_2_ON()             			GpioDataRegs.GPBSET.bit.GPIO32    		= 1
#define  LED_2_OFF()            			GpioDataRegs.GPBCLEAR.bit.GPIO32  	= 1
//===========================================================================
// Fan_Control
//===========================================================================
// Fan_EN---GPIO11
#define  FAN_ON()		  	  		      GpioDataRegs.GPASET.bit.GPIO5    		= 1
#define  FAN_OFF()		  	  			GpioDataRegs.GPACLEAR.bit.GPIO5  	= 1

//===========================================================================
// Relay Control
//===========================================================================
// RLY_EN---GPIO11
#define  RELAYA_ON()		  	  				GpioDataRegs.GPASET.bit.GPIO11    		= 1
#define  RELAYA_OFF()		  	  			GpioDataRegs.GPACLEAR.bit.GPIO11  	= 1
//===========================================================================
// GPIO17---DRModul_Fualt
//===========================================================================
#define  DRModul_Fualt  						GpioDataRegs.GPBDAT.bit.GPIO17

// GPIO10---PFC_CLS
#define  PFC_Enable()       	  				GpioDataRegs.GPASET.bit.GPIO10    		= 1
#define  PFC_Disable()	      				       GpioDataRegs.GPACLEAR.bit.GPIO10  	       = 1
//===========================================================================
// emergence stop
//===========================================================================
#define  EMERG_STOP()	 				GpioDataRegs.GPBDAT.bit.GPIO4

#define  STOP_PWM 								EPwm1Regs.AQCSFRC.bit.CSFA = 1;\
										             EPwm1Regs.AQCSFRC.bit.CSFB = 1;
#define  START_PWM 							EPwm1Regs.AQCSFRC.bit.CSFA = 3;\
														EPwm1Regs.AQCSFRC.bit.CSFB = 3

//===========================================================================
// State judge bit define
//===========================================================================
#define ON                        					1
#define OFF                       					0

//#define DIS                       0
//#define EN                        1

//===========================================================================
#ifdef __cplusplus
}
#endif

#endif /*MODULE_PROJECT_H_*/
