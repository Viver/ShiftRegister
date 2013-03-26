#pragma onec
#ifndef __BITMASK_H__
#define __BITMASK_H__

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

/** Set y bit in the byte x (установка бита y в байте x)
 */
#define SetBit(reg,bit)		((reg)|=(1<<(bit)))
#define SETBIT(reg,bit)		(SetBit(reg,bit))

/** Set bit into 1 or 0 in the reg
 ** Example: SetBitVal(PORTB, 3, 1), SetBitVal(PORTB, 2, 0)
 */
#define SetBitVal(reg, bit, val) \
	do {\
		if ((val&1)==0) reg &= (~(1<<(bit)));\
        else reg |= (1<<(bit));\
		} while(0)
#define SETBITVAL(reg, bit, val) (SetBitVal(reg, bit, val))

/** Clear y bit in the byte x (сброс бита y в байте x)
 */
#define ClearBit(reg,bit)	(reg&=(~(1<<(bit))))
#define CLEARBIT(reg,bit)	(ClearBit(reg,bit))

/** Check y bit in the byte x (проверка бита y в байте x) 
 */
#define CheckBit(reg,bit)	((reg) & (1<<(bit)))
#define CHECKBIT(reg,bit)	(CheckBit(reg,bit))

/** Example: if (BitIsClear(PORTB,1)) {...}
 */
#define BitIsClear(reg, bit)    ((reg&(1<<(bit)))==0)
#define BITISCLEAR(reg, bit)	(BitIsClear(reg, bit))

/** Example: if(BitIsSet(PORTB,2)) {...}
 */
#define BitIsSet(reg, bit)  ((reg&(1<<(bit)))!=0)
#define BITISSET(reg, bit)  (BitIsSet(reg, bit))  

/** Example: InvBit(PORTB, 1)
 */
#define InvBit(reg, bit)	  ((reg)^=(1<<(bit)))

#ifdef LITTLE_ENDIAN_DATA_FORMAT //little-endian data store format (Intel)
	#define GetByte(src,rel) 	*(((unsigned char*)&(src)+(rel)))					/**< Access Nth byte in variable */
	#define SetByte(des,rel) 	*(((unsigned char*)&(des)+(rel)))
#else                           //big-endian data store format (Motorola)
	#define GetByte(src,rel) 	*(((unsigned char*)&(src)+sizeof((src))-1-(rel))) 	/**< Access Nth byte in variable */
	#define SetByte(des,rel) 	*(((unsigned char*)&(des)+sizeof((des))-1-(rel)))
#endif
#ifndef _AB
	#define _AB(src,rel) 		(GetByte(src,rel))
#endif
#define GETBYTE(src,rel) 	(GetByte(src,rel))
#define SETBYTE(des,rel)	(SetByte(des,rel))	
	
/** Converts a bit number into a byte value. 
 */
#ifndef _BV
 #define _BV(bit) (1 << (bit))
#endif

/** Helpful macro used for generation of bit mask
 */
#ifndef _CBV
	#define _CBV(v,b) (((uint32_t)v) << b)
#endif

#endif //__BITMASK_H__
