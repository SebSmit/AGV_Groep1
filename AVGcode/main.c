#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Ultra_Sonic_Front.h"
#include "H_bridge_1.h"
#include "H_bridge_2.h"
#include "Gyro.h"

/* PORT List

Ultra sonic Trig:
            Echo: 11 10 9

Motor 1     iets: A15
            iets: A14

Motor 2     iets: A13
            iets: A12

    TIMER List
8    0-
16   1- Motor 1
8    2- ultra sonic
16   3- Motor 2
16   4- Wacht functie
16   5-
*/



int main(void)
{
    init();
    init_display_meneer();
    int VolgModus = 0;
    int KolommenModus = 1;

    int VolgTarget = 100;
    float CurrentHeading = 360.0;
    int TargetHeading = 360;
    int Afstand = 0;
    int Difference =0;
    int HeadingDifference = 0;

    int CurrentKolom = 1;

    int printgetal;
    DDRB |= ((1<<PB7) | (1<<PB6));
    PORTB |= ((1<<PB7) | (1<<PB6));
    DDRE |= (1<<PE5);
    PORTE |= (1<<PE5);
    while(1)
    {

        /*while(RedSwitch() == 0){
            Motor_1_set_power(0);
            Motor_2_set_power(0);
            CurrentHeading = UpdateHeading(CurrentHeading);

            if(ModeSwitch1()){
                wait_ms(300);
                LightSwap();
                VolgModus = 1;
                KolommenModus = 0;
            }
            if(ModeSwitch2()){
                wait_ms(300);
                LightSwap();
                VolgModus = 0;
                KolommenModus = 1;
            }
        }*/
        while(VolgModus == 1){
            /*if(RedSwitch()){
                VolgModus = 0;
                KolommenModus = 0;
            }*/

            Afstand = GetDistanceFront();
            Difference = Afstand - VolgTarget;
            display_getal(Afstand);

            CurrentHeading = UpdateHeading(CurrentHeading);
            HeadingDifference = TargetHeading - CurrentHeading;

            if (HeadingDifference > 3){
                Motor_1_set_power(35);
                Motor_2_set_power(25);
            }
            if (HeadingDifference < -3){
                Motor_1_set_power(25);
                Motor_2_set_power(35);
            }
            if (Difference > 200){
                Motor_1_set_power(35);
                Motor_2_set_power(35);
            }
            if (Difference > 120 && Difference < 200){
                Motor_1_set_power(22);
                Motor_2_set_power(22);
            }
            if (Difference > 60 && Difference < 120){
                Motor_1_set_power(18);
                Motor_2_set_power(18);
            }
            if (Difference > 20 && Difference < 60){
                Motor_1_set_power(15);
                Motor_2_set_power(15);
            }
            if (Difference > 0 && Difference < 20){
                Motor_1_set_power(12);
                Motor_2_set_power(12);
            }
            if (Difference < -5){
                Motor_1_set_power(-18);
                Motor_2_set_power(-18);
            }
            if ( -5 < Difference && Difference < 5){
                Motor_1_set_power(0);
                Motor_2_set_power(0);
            }

        }
        while(KolommenModus == 1){
            /*if(RedSwitch()){
                VolgModus = 0;
                KolommenModus = 0;
            }*/

            int AfstandLinks = GetDistanceLeft();
            int AfstandRechts = GetDistanceRight();
            CurrentHeading = UpdateHeading(CurrentHeading);
            printgetal = (int)CurrentHeading;
            display_getal(printgetal);
            Difference = TargetHeading - CurrentHeading;

            if(Difference > 1){
                //stuur naar links
                Motor_1_set_power(50);
                Motor_2_set_power(45);
            }
            if(Difference > 3){
                //stuur naar links
                Motor_1_set_power(50);
                Motor_2_set_power(40);
            }
            if(Difference > 8){
                //stuur naar links
                Motor_1_set_power(50);
                Motor_2_set_power(20);
            }
            if(Difference < -1){
                //stuur naar rechts
                Motor_1_set_power(45);
                Motor_2_set_power(50);
            }
            if(Difference < -3){
                //stuur naar rechts
                Motor_1_set_power(40);
                Motor_2_set_power(50);
            }
            if(Difference < -8){
                //stuur naar rechts
                Motor_1_set_power(20);
                Motor_2_set_power(50);
            }
            if ( -1 < Difference && Difference < 1){
                Motor_1_set_power(50);
                Motor_2_set_power(50);
            }
            if(BoomDetectie() == 1){
                Motor_1_set_power(0);
                Motor_2_set_power(0);
                GeluidSignaal();
                wait_ms(2000);
            }
        }



    }

    return 0;
}


void init (void){
    init_SonicSensors();
    initTimer();
    init_motor_1();
    init_motor_2();
    init_Gyro();
    PORTK &= ~(1<<PK0);
    sei();
}


// 48 46 44
int ModeSwitch1(){
    if (PINL & (1<<PL1)){//D48
        return 1;
    }
    else{
        return 0;
    }
}
int ModeSwitch2(){
    if (PINL & (1<<PL3)){//D46
        return 1;
    }
    else{
        return 0;
    }
}
int RedSwitch(){
    if (PINL & (1<<PL5)){//D44
        return 1;
    }
    else{
        return 0;
    }
}
int KolomIR(){
    if (PINK & (1<<PK0)){//A8
        return 1;
    }
    else{
        return 0;
    }
}

int BoomDetectie(){
    int afstand_Links = GetDistanceLeft();
    int afstand_Rechts = GetDistanceRight();
    int LinksDetectie = 0;
    int RechtsDetectie = 0;
    if (afstand_Links <= 90 & LinksDetectie == 0){
        LinksDetectie = 1;
        return 1;
    }
    if(afstand_Links > 120){
        LinksDetectie = 0;
    }
    if (afstand_Rechts <= 90 & RechtsDetectie == 0){
        RechtsDetectie = 1;
        return 1;
    }
    if(afstand_Rechts > 120){
        RechtsDetectie = 0;
    }
    else{
        return 0;
    }

}

void LightSwap(void){
    PORTB ^= (1<<PB7);
    PORTB ^= (1<<PB6);
}
void GeluidSignaal(void){
    PORTE &= ~(1<<PE5);
    wait_ms(40);
    PORTE |= (1<<PE5);
    wait_ms(60);
    PORTE &= ~(1<<PE5);
    wait_ms(40);
    PORTE |= (1<<PE5);
    wait_ms(150);
    PORTE &= ~(1<<PE5);
    wait_ms(70);
    PORTE |= (1<<PE5);
}


void TurnTo(int TargetHeading){
    //Get heading




}

void initTimer(void)
{
    TCCR4A = 0;
    TCCR4B |=  (1<<CS41);
}

void wait_ms(int ms){
    int WachtTijd = ms/33;
    int teller =0;
    while(teller < WachtTijd)
    {
        if (TIFR4 & (1<<TOV4))
        {
            TIFR4 = (1<<TOV4);
            teller++;
        }
    }
}


#define SDI_BIT		PH5
#define DDR_SDI		DDRH
#define PORT_SDI	PORTH

#define SFTCLK_BIT	PH4
#define DDR_SFTCLK	DDRH
#define PORT_SFTCLK	PORTH

#define LCHCLK_BIT	PG5
#define DDR_LCHCLK	DDRG
#define PORT_LCHCLK	PORTG

static unsigned int segmentcodes[] = {
	~0xFC, ~0x60, ~0xDA, ~0xF2,
	~0x66, ~0xB6, ~0xBE, ~0xE0,
	~0xFE, ~0xF6, ~0xEE, ~0x3E,
	~0x9C, ~0x7A, ~0x9E, ~0x8E };

void init_display_meneer (void)
{
	// Initialiseer de pinnen voor datain, shiftclk en latchclk als output
	DDR_SDI    |= (1 << SDI_BIT);
	DDR_SFTCLK |= (1 << SFTCLK_BIT);
	DDR_LCHCLK |= (1 << LCHCLK_BIT);

	// Maak shiftclk en latchclk laag
	PORT_SFTCLK &= ~(1 << SFTCLK_BIT);
	PORT_LCHCLK &= ~(1 << LCHCLK_BIT);
}

void send_data(char data)
{
	for (unsigned i = 0; i < 8; i++)
	// Herhaal voor alle bits in een char
	{
		// Bepaal de waarde van de bit die je naar het schuifregister
		// wil sturen
		int bit = data & 1;
		data >>= 1;

		// Maak de juiste pin hoog of laag op basis van de bepaalde waarde
		// van het bit
		if (bit)
		{
			PORT_SDI |= (1 << SDI_BIT);
		}
		else
		{
			PORT_SDI &= ~(1 << SDI_BIT);
		}

		// Toggle shiftclk (hoeveel tijd moest het signaal minimaal hoog zijn?)
		// Puls moet minimaal 13 ns hoog zijn. Een clk cycle op de Arduino duurt
		// 62 ns, dus signaal kan hoog en de volgende cycle weer omlaag
		PORT_SFTCLK |= (1 << SFTCLK_BIT);
		PORT_SFTCLK &= ~(1 << SFTCLK_BIT);
	}
}

void send_enable(int display_nummer)
{
	send_data(0x10 << display_nummer);
}

void display(char data, int display_nummer)
{
	send_data(data);
	send_enable(display_nummer);

	// Toggle latchclk (hoeveel tijd moest het signaal minimaal hoog zijn?)
	// Puls moet minimaal 13 ns hoog zijn. Een clk cycle op de Arduino duurt
	// 62 ns, dus signaal kan hoog en de volgende cycle weer omlaag
	PORT_LCHCLK |= (1 << LCHCLK_BIT);
	PORT_LCHCLK &= ~(1 << LCHCLK_BIT);
}

void display_getal(unsigned int getal)
{
	for (int i = 0; i < 4; i++)
	{
		display(segmentcodes[getal%10], i);
		getal /= 10;
		_delay_ms(1);   // 1 kHz
	}
}
