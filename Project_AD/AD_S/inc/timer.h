/*
 * timer.h
 *
 *  Created on: 2017-3-20
 *      Author: Shaw
 */

#ifndef _TIMER_H_
#define _TIMER_H_

typedef struct timer0_Type{
	union
	  {
	    Uint16 All;
	    struct{
	            Uint16 OnemsdFlag     :1;  //1ms÷–∂œ±Í÷æ
	          }Status_Bits;
	  }Mark_Para;

	  Uint32 msCounter;
}timer0;

extern timer0 timer0Base;

void Timer0_init(void);
void Timer1_init(void);


#endif /* TIMER0_BASE_H_ */
