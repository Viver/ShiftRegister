#include <avr/io.h>

#include "SPI/SPI.h"
#include "Macro/Interrupt.h"
#include "Macro/BitMask.h"


#define TABLE_NUM	3

#define KEYS_DDR	DDRD
#define KEYS_PORT	PORTD
#define KEY_A		2
#define KEY_B		3

volatile uint8_t	SetTable = 0,
					Cnt = 0;

ISR (TIMER0_OVF_vect)
{
	ShowDataSPI();
}

ISR(INT0_vect) // Выбор таблици
{
	_DELAY_CYCLES(50);	// Задержка для нейнтрализации дребезга кнопки
						// В симуляторе не работает
	if(BitIsSet(KEYS_PORT,KEY_A))
	{
		if(SetTable<(uint8_t)TABLE_NUM)  SetTable++;
		else SetTable = 1;
	}
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		PutInBuf(SetTable);
	}
}

ISR(INT1_vect) // Регулировка скорости обновления регистров
{
	_DELAY_CYCLES(50);
	
	if(CheckBit(KEYS_PORT,KEY_B))
	{
		Cnt++;
		if(Cnt>3) Cnt = 0;
							
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{	
			switch(Cnt)
			{
				case 0: //	clk/8
				TCCR0 =	(0<<CS02)|(1<<CS01)|(0<<CS00);
				break;
				case 1: //	clk/64
				TCCR0 =	(0<<CS02)|(1<<CS01)|(1<<CS00);
				break;
				case 2: //	clk/256
				TCCR0 =	(1<<CS02)|(0<<CS01)|(0<<CS00);
				break;
				case 3: //	clk/1024
				TCCR0 =	(1<<CS02)|(0<<CS01)|(1<<CS00);
				break;
				default: Cnt = 0;
			}
		}
	}
}

int main(void)
{
	InitSPI();
	
	TCCR0 =	(0<<CS02)|(1<<CS01)|(0<<CS00);
	TIMSK = _BV(TOIE0);
	
	KEYS_DDR	&= ~(_BV(KEY_B)|_BV(KEY_A));
	KEYS_PORT	&= ~(_BV(KEY_B)|_BV(KEY_A));
		
	MCUCR = 0x00;
	GICR |= _BV(INT0)|_BV(INT1);
	
	_ENABLE_INTERRUPTS();
	
	SetTable = 1;
	PutInBuf(SetTable);
	
	while(1)
    {
		
    }
}