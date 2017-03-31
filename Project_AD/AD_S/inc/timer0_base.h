/*
 * timer0_base.h
 *
 *  Created on: 2013-10-22
 *      Author: cotto
 */

#ifndef TIMER0_BASE_H_
#define TIMER0_BASE_H_

typedef struct timer0_Type{
	union
	  {
	    Uint16 All;
	    struct{
	            Uint16 OnemsdFlag     :1;  //1ms�жϱ�־
	          }Status_Bits;
	  }Mark_Para;

	  Uint32 msCounter;
}timer0;

extern timer0 timer0Base;

void Timer0_init(void);


#endif /* TIMER0_BASE_H_ */
