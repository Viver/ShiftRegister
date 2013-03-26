#include "SPI.h"

#include <avr/io.h>
#include "../Macro/BitMask.h"
#include "Tables.h"

#define SPI_PORT	PORTB
#define SPI_DDR		DDRB

#define SPI_SCK		5   //В режиме "Мастер" - выход
#define SPI_MISO	4	//В режиме "Мастер" - вход
#define SPI_MOSI	3	//В режиме "Мастер" - выход
#define SPI_SS		2	//В режиме "Мастер" - выход

uint16_t SizeTable;
uint8_t BufSPI[MAX_TABLE_SIZE],
		DataUpd;

/*
------------== 74HC595  ==--------------

fmax maximum frequency SHCP or STCP;
VCC = 2 V		9	30 - 4.8 - 4 - MHz
VCC = 4.5 V		30	91 - 24 - 20 - MHz
VCC = 6 V		35	108 - 28 - 24 - MHz

Symbol	Pin		Description
Q1		1		parallel data output 1
Q2		2		parallel data output 2
Q3		3		parallel data output 3
Q4		4		parallel data output 4
Q5		5		parallel data output 5
Q6		6		parallel data output 6
Q7		7		parallel data output 7
GND		8		ground (0 V)
Q7S		9		serial data output
MR		10		master reset (active LOW)
SHCP	11		shift register clock input
STCP	12		storage register clock input
OE		13		output enable input (active LOW)
DS		14		serial data input
Q0		15		parallel data output 0
VCC		16		supply voltage
*/
void InitSPI(void)
{
	SPCR = /*_BV(SPIE)|*/_BV(SPE)|_BV(MSTR)|_BV(SPR1)/*|_BV(SPR0)*/;
	// SPCR - регистр управления
	// SPIE - разрешение прерывания от SPI
	// SPE - включение/выключение SPI
	// DORD - порядок передачи данныхw2
	//		DORD = 1 - первым передается старший бит
	//		DORD = 0 - первым передается младший бит
	// MSTR - выбор режима роботы Master\Slave
	//		MSTR = 1 - Master
	// CPOL - полярность тактового сигнала
	//		CPOL = 1 - генерируются импульсы 
	//		положительной полярности, при отсутствии импульсов
	//		на выводе присутствует низкий уровень
	// CPHA - фаза тактового сигнала
	//		CPHA = 1 - обработка производится по заднему
	//		фронту импульсов сигнала SCK
	// SPR1:SPR0 - скорость передачи
	// SPSR - регистр состояния
	// SPSR |= _BV(SPI2X);
	// SPI2X = 1 - удвоение скорости
	// SPIF - флаг прерывания от SPI. 1 - по окончанию передачи байта
	// WCOL - флаг конфликта записи
	// SPDR - регистр данных
	
	SPI_PORT = _BV(SPI_MISO);
	SPI_DDR  = _BV(SPI_SCK)|_BV(SPI_MOSI)|_BV(SPI_SS);
		
	volatile uint8_t IOReg;
	IOReg   = SPSR;
	SPDR = 0;
	for (IOReg=0;IOReg<254;IOReg++)	{}; // Так сказать синхронизация
};

void TxSPI(uint8_t Data)
{
	while(BitIsClear(SPSR,SPIF));
	SPDR = Data;
}

void SetSSSPI(uint8_t State)
{
	if(State>0)
	{
		SetBit(SPI_PORT,SPI_SS);
	}
	else
	{
		ClearBit(SPI_PORT, SPI_SS);
	}
}

void ClearBufISP(void)
{
	uint16_t i;
	
	for(i=0;i<MAX_TABLE_SIZE;i++)
	{
		BufSPI[i] = 0;
	}
}

void PutInBuf(uint8_t TabNum)
{
	uint16_t i;
	
	DataUpd = 1;
	
	switch(TabNum)
	{
		case 1:
		{
			ClearBufISP();
			SizeTable = sizeof(Table1);
			for(i=0;i<SizeTable;i++)
			{
				BufSPI[i] = pgm_read_byte(Table1+i);
			}
		};break;
		case 2:
		{
			ClearBufISP();
			SizeTable = sizeof(Table2);
			for(i=0;i<SizeTable;i++)
			{
				BufSPI[i] = pgm_read_byte(Table2+i);
			}
		};break;
		case 3:
		{
			ClearBufISP();
			SizeTable = sizeof(Table3);
			for(i=0;i<SizeTable;i++)
			{
				BufSPI[i] = pgm_read_byte(Table3+i);
			}			
		};break;
		default: DataUpd = 0;
	}
}

void ShowDataSPI(void)
{	
	static volatile uint16_t Cnt = 0;
	if(DataUpd)
	{
		DataUpd = 0;
		Cnt = 0;
	}
	if(BitIsSet(SPSR,SPIF))
	{
		if(Cnt>SizeTable) Cnt = 0;
		
		SetSSSPI(1);
		TxSPI(BufSPI[Cnt++]);
		while(BitIsClear(SPSR,SPIF));
		TxSPI(BufSPI[Cnt++]);;
		while(BitIsClear(SPSR,SPIF));
		TxSPI(BufSPI[Cnt++]);;
		while(BitIsClear(SPSR,SPIF));
		SetSSSPI(0);
	}
}

//ISR(SPI_STC_vect) //Serial Transfer - Complete
//{
//}