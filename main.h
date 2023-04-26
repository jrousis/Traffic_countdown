#ifndef MAIN_H
#define MAIN_H

#define F_CPU 8000000UL

#define	Ser_in	0		
#define	Sclk	1
#define	Enable	2
#define	Rclk	3

#define SDA_PIN			0	        /* The SDA port pin */
#define SCL_PIN			1	        /* The SCL port pin */
#define ENABLE_PIN		2
#define OE_PIN			2
#define RCL_PIN			3

#define DISPLAY_ON() 	  PORTA &= ~(1<<ENABLE_PIN)
#define DISPLAY_OFF() 	  PORTA |= (1<<ENABLE_PIN)
#define ENABLE_ON() 	  PORTA &= ~(1<<ENABLE_PIN)
#define ENABLE_OFF() 	  PORTA |= (1<<ENABLE_PIN)

#define RCL_0() 	  PORTA &= ~(1<<RCL_PIN)
#define RCL_1() 	  PORTA |= (1<<RCL_PIN)
#define SCL_0() 	  PORTA &= ~(1<<SCL_PIN)
#define SCL_1() 	  PORTA |= (1<<SCL_PIN)
#define SDA_0() 	  PORTA &= ~(1<<SDA_PIN)
#define SDA_1() 	  PORTA |= (1<<SDA_PIN)
#define OE_0()	 	  PORTA &= ~(1<<OE_PIN)
#define OE_1()	 	  PORTA |= (1<<OE_PIN)

#endif

