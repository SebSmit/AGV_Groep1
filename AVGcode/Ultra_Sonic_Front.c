#define F_CPU 16000000UL
#include <util/delay.h>
#include "Ultra_Sonic_Front.h"
#include <avr/io.h>
#include <avr/interrupt.h>
volatile int count = 0;

void init_SonicSensors(void){
	ddr_ech_F &= ~(1<<pin_ech_F);
	ddr_tri_F |= (1<<pin_tri_F);
    port_tri_F &= ~(1<<pin_tri_F);

	ddr_ech_R &= ~(1<<pin_ech_R);
	ddr_tri_R |= (1<<pin_tri_R);
    port_tri_R &= ~(1<<pin_tri_R);

	ddr_ech_L &= ~(1<<pin_ech_L);
	ddr_tri_L |= (1<<pin_tri_L);
    port_tri_L &= ~(1<<pin_tri_L);

    TCCR2A = 0;
    TCCR2B |= (1<<CS20) ;
    //16000000 / 256 = 62,500
    //1 / 62500 = 0.000016s
    //62500x/sec
    TIMSK2 = (1<<TOIE2);
}

int GetDistanceFront(void){
    int Distance;
    TriggerPulseFront();
    while ((port_ech_F & (1<<pin_ech_F)) == 0){
    }
    TCNT2 = 0;
    TIFR2 = 1<<TOV2;
    count = 0;
    while ((port_ech_F & (1<<pin_ech_F)) != 0){

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
void TriggerPulseFront (void){
    port_tri_F |= (1<<pin_tri_F);
    _delay_us(10);
    port_tri_F &= (~(1<<pin_tri_F));
}

int GetDistanceLeft(void){
    int Distance;
    TriggerPulseLeft();
    while ((port_ech_L & (1<<pin_ech_L)) == 0){
    }
    TCNT2 = 0;
    TIFR2 = 1<<TOV2;
    count = 0;
    while ((port_ech_L & (1<<pin_ech_L)) != 0){

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
void TriggerPulseLeft (void){
    port_tri_L |= (1<<pin_tri_L);
    _delay_us(10);
    port_tri_L &= (~(1<<pin_tri_L));
}

int GetDistanceRight(void){
    int Distance;
    TriggerPulseRight();
    while ((port_ech_R & (1<<pin_ech_R)) == 0){
    }
    TCNT2 = 0;
    TIFR2 = 1<<TOV2;
    count = 0;
    while ((port_ech_R & (1<<pin_ech_R)) != 0){

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
void TriggerPulseRight (void){
    port_tri_R |= (1<<pin_tri_R);
    _delay_us(10);
    port_tri_R &= (~(1<<pin_tri_R));
}

ISR(TIMER2_OVF_vect){
    count++;
}
