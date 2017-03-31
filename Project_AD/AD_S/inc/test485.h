/*
 * test485.h
 *
 *  Created on: 2013-11-2
 *      Author: HIKE Tech
 */

#ifndef TEST485_H_
#define TEST485_H_

//SP3485 ’∑¢øÿ÷∆IO
#define TEN485GPIO GPIO44
#define TEN485MUX GpioCtrlRegs.GPBMUX1.bit.TEN485GPIO
#define TEN485DIR GpioCtrlRegs.GPBDIR.bit.TEN485GPIO
#define TEN485ON()  GpioDataRegs.GPBSET.bit.TEN485GPIO =1
#define TEN485OFF() GpioDataRegs.GPBCLEAR.bit.TEN485GPIO =1

void Init485EN(void);
void start485Send(void);
void wait485Send(void);
void handle485RxFIFO(Scan * PtrSegTask);
void handleScanKey485(Scan * PtrSegTask);


#endif /* TEST485_H_ */
