#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void pwm_on (void);
void pwm_off (void);

volatile unsigned char a=20, st=0, b ;
volatile uint16_t duti;

void(*pwm_com)(void);

// PWM channels
#define AH_Fet	PB3
#define BH_Fet	PB2
#define CH_Fet	PB1
#define PORT_HFet	PORTB

#define AL_Fet	PD3
#define BL_Fet	PD4
#define CL_Fet	PD5
#define PORT_LFet	PORTD

#define AH_ON	PORT_HFet |= (1<<AH_Fet)
#define AH_OFF	PORT_HFet &=~(1<<AH_Fet)

#define BH_ON	PORT_HFet |= (1<<BH_Fet)
#define BH_OFF	PORT_HFet &=~(1<<BH_Fet)

#define CH_ON	PORT_HFet |= (1<<CH_Fet)
#define CH_OFF	PORT_HFet &=~(1<<CH_Fet)


#define AL_ON	PORT_LFet |= (1<<AL_Fet)
#define AL_OFF	PORT_LFet &=~(1<<AL_Fet)

#define BL_ON	PORT_LFet |= (1<<BL_Fet)
#define BL_OFF	PORT_LFet &=~(1<<BL_Fet)

#define CL_ON	PORT_LFet |= (1<<CL_Fet)
#define CL_OFF	PORT_LFet &=~(1<<CL_Fet)

volatile int  rotor=0;

//#define AH_ON   TCCR2  |= (1<<COM21);
//#define AH_OFF  TCCR2  &=~(1<<COM21);

//#define AL_ON   PORTD  |= (1<<PD3);
//#define AL_OFF  PORTD  &=~(1<<PD3);


//#define BH_ON   TCCR1A |= (1<<COM1B1);
//#define BH_OFF  TCCR1A &=~(1<<COM1B1);

//#define BL_ON   PORTD  |= (1<<PD4);
//#define BL_OFF  PORTD  &=~(1<<PD4);


//#define CH_ON	TCCR1A |= (1<<COM1A1);
//#define CH_OFF	TCCR1A &=~(1<<COM1A1);

//#define CL_ON	PORTD  |= (1<<PD5);
//#define CL_OFF	PORTD  &=~(1<<PD5);

#define PHASE_ALL_OFF AH_OFF;AL_OFF;BH_OFF;BL_OFF;CH_OFF;CL_OFF;

#define N_A ADMUX = 0x00;
#define N_B ADMUX = 0x01;
#define N_C ADMUX = 0x02;
#define OUT_COMP   (ACSR & (1<<ACO))

#define COMP_ON   SFIOR  |= (1<<ACME);
#define COMP_OFF  SFIOR  &=~(1<<ACME);

/*
void comut(unsigned char start)

{
	
	switch (st)
		{
			case 0:
			if ((start!=0) || OUT_COMP)
			{		
					CL_OFF;
					_delay_us(10);
					AL_ON;
					N_C;
					st=1;
					TCNT0=0;
					start=0x00;
			}							
			break;

			case 1:
			if ((start!=0) || !OUT_COMP)
			{
					BH_OFF;
					_delay_us(10);
					CH_ON;
					N_B;
					st=2;
					TCNT0=0;
					start=0x00;
			}		
			break;

			case 2:
			if ((start!=0) || OUT_COMP)
			{
					AL_OFF;
					_delay_us(10);
					BL_ON;
					N_A;
					st=3;
					TCNT0=0;
					start=0x00;
			}			
			break;
			
			case 3:
			if ((start!=0) || !OUT_COMP)
			{
					CH_OFF;
					_delay_us(10);
					AH_ON;
					N_C;
					st=4;
					TCNT0=0;
					start=0x00;
			}		
			break;

			case 4:
			if ((start!=0) || OUT_COMP)
			{
					BL_OFF;
					_delay_us(10);
					CL_ON;
					N_B;
					st=5;
					TCNT0=0;
					start=0x00;
			}		
			break;

			case 5:
			if ((start!=0) || !OUT_COMP)
			{		
					AH_OFF;
					_delay_us(10);
					BH_ON;
					N_A;
					st=0;
					TCNT0=0;
					start=0x00;
			}		
			break;
	
		
		}


}



ISR (TIMER0_OVF_vect)
{
	
		rotor=0;
		comut(1);
		OCR1A  = 10;
		OCR1B  = 10;
		OCR2   = 10;
	
}

ISR (ANA_COMP_vect)
{
	rotor++;
	if (rotor>200) rotor=200;
	if (rotor==200)
	{
		comut(0);
		//TIMSK  &=~(1<<TOIE0);
				
	}
	
}*/

ISR (TIMER2_OVF_vect)
{
//	static uint8_t tcnt2h;
	
//	if (tcnt2h > 0) {
//		tcnt2h--;
//	} else {
//
//	}

//	PORT_HFet ^= (1<<AH_Fet);


	(*pwm_com)();
}

void pwm_on (void)
{
	AH_OFF;
	pwm_com = &pwm_off;
}

void pwm_off (void)
{
	AH_ON;
	pwm_com = &pwm_on;
}


int main (void)

{
	DDRB		 = (1<<AH_Fet)|(1<<BH_Fet)|(1<<CH_Fet);	
	PORT_HFet	&=~((1<<AH_Fet)|(1<<BH_Fet)|(1<<CH_Fet));

	DDRC   = 0x00;
	PORTC  = 0x00;

	DDRD		 = (1<<AL_Fet)|(1<<BL_Fet)|(1<<CL_Fet);
	PORT_LFet  	&=~((1<<AL_Fet)|(1<<BL_Fet)|(1<<CL_Fet));

	ADCSRA &=~(1<<ADEN);
	SFIOR  |= (1<<ACME);
//	ACSR   |= (1<<ACIE);


//	TCCR0  |= (1<<CS02)|(1<<CS00);
//	TIMSK  |= (1<<TOIE0);		

//	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM10);
//	TCCR1B |= (1<<WGM12)|(1<<CS10);
//	OCR1A  = a;
//	OCR1B  = a;

	TCCR2  = (1<<CS20);
	TIMSK	|= (1<<TOIE2);
		
	PHASE_ALL_OFF;
	
	duti = 500;

	pwm_com = &pwm_on;

	sei();


	while (1)

	{

		int a, b;
		a++;
		b++;
		PORT_HFet ^= (1<<BH_Fet);

	}

}
