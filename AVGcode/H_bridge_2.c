#include "h_bridge_2.h"
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER3_OVF_vect){
	if (OCR3A == 0 && OCR3B == 0){
		port_R2 &= ~(1<<pin_R2);
		port_L2 &= ~(1<<pin_L2);
	}
	else if (OCR3A != 0){
		port_R2 |= (1<<pin_R2);
		port_L2 &= ~(1<<pin_L2);
	}
	else if (OCR3B != 0){
		port_R2 &= ~(1<<pin_R2);
		port_L2 |= (1<<pin_L2);
	}
}

ISR(TIMER3_COMPA_vect)
{
	if (OCR3A != 65535){
		port_R2 &= ~(1<<pin_R2);
	}
}

ISR(TIMER3_COMPB_vect)
{
	if (OCR3B != 65535){
		port_L2 &= ~(1<<pin_L2);
	}
}

void init_motor_2(void)
{
	// Pin outputs
	ddr_R2 |= (1<<pin_R2);
	ddr_L2 |= (1<<pin_L2);

	// zekerheid laag
	port_R2 &= ~(1<<pin_R2);
	port_L2 &= ~(1<<pin_L2);

	// prescale 64
	TCCR3A = 0;
	TCCR3B = (0<<CS32) | (0<<CS31) | (1<<CS30);

	// standaard 0
	OCR3A = 0;
	OCR3B = 0;
	TIMSK3 = (1<<OCIE3B) | (1<<OCIE3A) | (1<<TOIE3);
	Motor_2_set_power(0);
}

void Motor_2_set_power(signed char power)
{
	if (power >= -100 && power <= 100){
		if (power >= 0){
			OCR3B = 0;
			OCR3A = (65535*power)/100;
		}
		else {
			OCR3A = 0;
			OCR3B = (65535*power)/-100;
		}
	}
}
