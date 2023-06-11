#include "h_bridge_1.h"
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER1_OVF_vect){
	if (OCR1A == 0 && OCR1B == 0){
		port_R1 &= ~(1<<pin_R1);
		port_L1 &= ~(1<<pin_L1);
	}
	else if (OCR1A != 0){
		port_R1 |= (1<<pin_R1);
		port_L1 &= ~(1<<pin_L1);
	}
	else if (OCR1B != 0){
		port_R1 &= ~(1<<pin_R1);
		port_L1 |= (1<<pin_L1);
	}
}

ISR(TIMER1_COMPA_vect)
{
	if (OCR1A != 65535){
		port_R1 &= ~(1<<pin_R1);
	}
}

ISR(TIMER1_COMPB_vect)
{
	if (OCR1B != 65535){
		port_L1 &= ~(1<<pin_L1);
	}
}

void init_motor_1(void)
{
	// Pin outputs
	ddr_R1 |= (1<<pin_R1);
	ddr_L1 |= (1<<pin_L1);

	// zekerheid laag
	port_R1 &= ~(1<<pin_R1);
	port_L1 &= ~(1<<pin_L1);

	// prescale 64
	TCCR1A = 0;
	TCCR1B = (0<<CS12) | (0<<CS11) | (1<<CS10);

	// standaard 0
	OCR1A = 0;
	OCR1B = 0;
	TIMSK1 = (1<<OCIE1B) | (1<<OCIE1A) | (1<<TOIE1);
	Motor_1_set_power(0);
}

void Motor_1_set_power(signed char power)
{
	if (power >= -100 && power <= 100){
		if (power >= 0){
			OCR1B = 0;
			OCR1A = (65535*power)/100;
		}
		else {
			OCR1A = 0;
			OCR1B = (65535*power)/-100;
		}
	}
}
