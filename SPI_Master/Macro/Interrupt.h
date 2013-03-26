#pragma onec
#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include <avr/interrupt.h>
#include <util/atomic.h>

#define _ENABLE_INTERRUPTS()	sei()
#define _DISABLE_INTERRUPTS()	cli()
#define _SAVE_INTERRUPTS()		SREG
#define _RESTORE_INTERRUPTS(s)	SREG = (s)
#define _NO_OPERATION()			asm volatile("nop\n\t"::)
#define _DELAY_CYCLES(cycles)	{uint8_t cnt = (uint8_t)cycles;\
								do { _NO_OPERATION(); cnt--;}\
								while(cnt);}

#endif //__INTERRUPT_H_
