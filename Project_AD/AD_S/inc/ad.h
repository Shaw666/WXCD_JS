/*
 * ad.h
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */

#ifndef _AD_H_
#define _AD_H_

typedef struct WXCDInfo_Type{
	u16 deviceID_State; //8λid 7λ״̬  A5
	u16 regulate_V; //12λad
	u16 regulate_I; //12λad
}WXCDInfo;


extern u16 ADResReg[16];


void ADC_Config(void);

#endif /* AD_H_ */
