#define F_CPU 16000000UL
#include <util/delay.h>
#include "Ultra_Sonic_Front.h"
#include <avr/io.h>
#include <avr/interrupt.h>
volatile int count = 0;

void init_SonicFront(void){
	ddr_ech &= ~(1<<pin_ech);
	ddr_tri |= (1<<pin_tri);
    port_tri &= ~(1<<pin_tri);
    TCCR2A = 0;
    TCCR2B |= (1<<CS20) ;
    //16000000 / 256 = 62,500
    //1 / 62500 = 0.000016s
    //62500x/sec
    TIMSK2 = (1<<TOIE2);
}

int GetDistanceFront(void){

    int Distance;

    TriggerPulse();

    while ((port_ech & (1<<pin_ech)) == 0){

    }
    TCNT2 = 0;
    TIFR2 = 1<<TOV2;
    count = 0;

    while ((port_ech & (1<<pin_ech)) != 0){

    }
    Distance = count;

    Distance = Distance * 0.008 * 343;

    //340  m/s
    //34 cm / ms
    //340   mm / ms
    //0.34  mm / us
    //5.44 * count geeft distance in mm

    return Distance;
}
void TriggerPulse (void){
    port_tri |= (1<<pin_tri);
    _delay_us(10);
    port_tri &= (~(1<<pin_tri));
}
ISR(TIMER2_OVF_vect){
    count++;
}
