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

float CurrentHeading = 360.0;

int main(void)
{
    init();
    init_display_meneer();
    int VolgModus = 0;
    int KolommenModus =1;
    int TestModus =0;

    int VolgTarget = 80;

    int TargetHeading = 360;
    int Afstand = 0;
    int Difference =0;
    int HeadingDifference = 0;
    int FrontDifference = 0;
    int Obstructie = 0;

    int CurrentKolom = 1;
    int InKolom = 1;

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
        while(TestModus == 1){

            int AfstandLinks = GetDistanceLeft();
            int AfstandRechts = GetDistanceRight();
            int Afstand = GetDistanceFront();
            CurrentHeading = UpdateHeading(CurrentHeading);
            printgetal = (int)CurrentHeading;
            display_getal(AfstandRechts);
        }
        while(VolgModus == 1){
            /*if(RedSwitch()){
                VolgModus = 0;
                KolommenModus = 0;
            }*/

            Afstand = GetDistanceFront();
            FrontDifference = Afstand - VolgTarget;
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

            Afstand = GetDistanceFront();
            FrontDifference = Afstand - VolgTarget;

            int AfstandLinks = GetDistanceLeft();
            int AfstandRechts = GetDistanceRight();
            CurrentHeading = UpdateHeading(CurrentHeading);
            printgetal = (int)CurrentHeading;
            display_getal(AfstandLinks);
            Difference = TargetHeading - CurrentHeading;

            if(FrontDifference < 60){
                Obstructie = 1;
                if (FrontDifference > 20 && FrontDifference < 60){
                Motor_1_set_power(15);
                Motor_2_set_power(15);
                }
                if (FrontDifference > 0 && FrontDifference < 20){
                    Motor_1_set_power(12);
                    Motor_2_set_power(12);
                }
                if ( -5 < FrontDifference && FrontDifference < 5){
                    Motor_1_set_power(0);
                    Motor_2_set_power(0);
                    PORTE &= ~(1<<PE5);
                    wait_ms(20);
                    PORTE |= (1<<PE5);
                    wait_ms(20);
                }
            }
            if (FrontDifference > 90){
                Obstructie = 0;
            }

            if(Obstructie ==0 && InKolom != 0){
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
                    Motor_1_set_power(50);
                    Motor_2_set_power(50);
                    wait_ms(180);
                }
                if(KolomIR()==1){
                    InKolom = 2;
                   display_getal(5555);
                }
                if(KolomIR()==0){
                    if(InKolom == 2){
                        InKolom =0;
                    }
                    display_getal(0000);
                }
            }
            if(InKolom == 0){
                display_getal(0000);
                BochtMaken();
                TargetHeading = 540;
                InKolom = 1;
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
        return 0;
    }
    else{
        return 1;
    }
}

volatile int LinksDetectie = 0;
volatile int RechtsDetectie = 0;
int BoomDetectie(){
    int afstand_Links = GetDistanceLeft();
    int afstand_Rechts = GetDistanceRight();

    if (afstand_Links <= 80)
        {
            if (LinksDetectie == 0) // knop is niet al eerder ingedrukt
            {
                wait_ms(20);
                LinksDetectie = 1;
				return 1;
            }
        }
    if (afstand_Links > 90)
        {
            if (LinksDetectie != 0) // knop is zojuist losgelaten
            {
                _delay_ms(20);
                LinksDetectie = 0;
            }
        }
    if (afstand_Rechts <= 80)
        {
            if (RechtsDetectie == 0) // knop is niet al eerder ingedrukt
            {
                wait_ms(20);
                RechtsDetectie = 1;
				return 1;
            }
        }
    if (afstand_Rechts > 90)
        {
            if (RechtsDetectie != 0) // knop is zojuist losgelaten
            {
                _delay_ms(20);
                RechtsDetectie = 0;
            }
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
    wait_ms(80);
    PORTE &= ~(1<<PE5);
    wait_ms(40);
    PORTE |= (1<<PE5);
    wait_ms(150);
    PORTE &= ~(1<<PE5);
    wait_ms(70);
    PORTE |= (1<<PE5);
}


void TurnTo(int TargetHeading){
    CurrentHeading = UpdateHeading(CurrentHeading);
    int Difference = TargetHeading - CurrentHeading;
    int turning = 1;
    int printgetal;
    while (turning ==1){
        CurrentHeading = UpdateHeading(CurrentHeading);
        Difference = TargetHeading - CurrentHeading;
        printgetal = (int)CurrentHeading;
        display_getal(printgetal);
        if(Difference > 0){
            Motor_1_set_power(25);
            Motor_2_set_power(-25);
        }
        if(Difference > 10){
            Motor_1_set_power(35);
            Motor_2_set_power(-35);
        }
        if(Difference > 20){
            Motor_1_set_power(45);
            Motor_2_set_power(-45);
        }
        if(Difference < 0){
            Motor_1_set_power(-25);
            Motor_2_set_power(25);
        }
        if(Difference < -10){
            Motor_1_set_power(-35);
            Motor_2_set_power(35);
        }
        if(Difference < -20){
            Motor_1_set_power(-45);
            Motor_2_set_power(45);
        }
        if ( -1 < Difference && Difference < 1){
            Motor_1_set_power(0);
            Motor_2_set_power(0);
            turning = 0;
        }
    }

}
void Drive(int RijAfstand,int TargetHeading){
    int Difference;
    for(int i = 0; i<RijAfstand; i++){
        CurrentHeading = UpdateHeading(CurrentHeading);
        Difference = TargetHeading - CurrentHeading;
        display_getal(i);
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
    }


}
void BochtMaken(void){
    wait_ms(500);
    Drive(100, 360);
    TurnTo(450);
    Drive(180, 450);
    TurnTo(540);
    Drive(70, 540);
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



void dummyben(void)
{

   DDRB = 0b11000000;
    int led1 = 1;
    int led2 = 0;
    int tellen1 = 0;
    int tellen2 = 0;

    begin:
    while(led1 <= 5  )
    {
        PORTB = 0b11000000;
        _delay_ms (500);
        PORTB = 0b01000000;
        _delay_ms (500);
        led1 = led1 + 1;
    }


    while(led1 >5)
    {
        PORTB = 0b11000000;
        _delay_ms (500);
        PORTB = 0b10000000;
        _delay_ms (500);
        led1 = led1 + 1;

     if (led1 == 11)
     {
        led1 = 0;
        goto begin;

     }

    }
}

void yourisdummy()
{
    {
	DDRB |= (1<<PB7) | (1<<PB5);
	PORTB &= ~(1<<PB7);



	while (1)
	{
		if ((PINF & (1<<PF1)) == 0)
		{
			PORTB &= ~(1<<PB5);
		}
		else
		{
			PORTB |= (1<<PB5);
		}
	}
}
}

void RobinDum(void)
{
    void init_led(void)
    {
        DDRB |= (1<<PB7);
        PORTB |= (1<<PB7);
    }

    void init_timer1(void)
    {
        TCCR1A = 0;
        TCCR1B = (1<<CS12) | (0<<CS11) | (0<<CS10);
        TCNT1 = 34286;
    }

    void init(void)
    {
        init_led();
        init_timer1();
    }

    void led_D1_on(void)
    {
        PORTB &= ~(1<<PB7);
    }

    void led_D1_off(void)
    {
        PORTB |= (1<<PB7);
    }

    void led_D1_toggle(void)
    {
        PORTB ^= (1<<PB7);
    }

    int main(void)
    {
        init();

        while (1)
        {
            if (TIFR1 & (1<<TOV1))
            {
                // Re-init tcnt
                TCNT1 = 34286;
                // Clear flag
                TIFR1 = (1<<TOV1);
                // Toggle led D1
                led_D1_toggle();
            }
        }
    }
}







