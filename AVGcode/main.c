#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>


#include "Ultra_Sonic_Front.h"
#include "H_bridge_1.h"
#include "H_bridge_2.h"

/* PORT List

Ultra sonic Trig:
            Echo:

Motor 1     iets:
            iets:

Motor 2     iets:
            iets:

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
    int VolgModus = 0;
    int KolommenModus = 0;

    int VolgTarget = 100;
    int Afstand = 0;

    int CurrentKolom = 1;

    while(1)
    {
        while(ConfirmKnop() == 0){
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
        }
        while(VolgModus == 1){
            if(ModeSwitchReset()){
                VolgModus = 0;
                KolommenModus = 0;
            }

            Afstand = GetDistanceFront();

            if(Afstand > VolgTarget){
                /*Motor harder als verder weg als dichterbij zachtjes rijden*/
            }
            else if(Afstand <= VolgTarget){
                Motor_1_set_power(0);
                Motor_2_set_power(0);
            }
        }
        while(KolommenModus == 1){
            while(KolomIR()){
                while(BoomDetectie() == 0){
                    int TargetHeading = 0;
                    int CurrentHeading = 0; //get heading gyro
                    int HeadingDifference = TargetHeading - CurrentHeading;

                }

            }

            //get heading
        }


    }

    return 0;
}


void init (void){
    init_SonicFront();
    initTimer();
    init_motor_1();
    init_motor_2();
    sei();
}

int ConfirmKnop(){
    if (PINF & (1<<PF0)){//A0
        return 1;
    }
    else{
        return 0;
    }
}
int ModeSwitch1(){
    if (PINF & (1<<PF1)){//A1
        return 1;
    }
    else{
        return 0;
    }
}
int ModeSwitch2(){
    if (PINF & (1<<PF2)){//A2
        return 1;
    }
    else{
        return 0;
    }
}
int ModeSwitchReset(){
    if (PINF & (1<<PF2)){//A2
        return 1;
    }
    else{
        return 0;
    }
}
int KolomIR(){
    if (PINF & (1<<PF3)){//A3
        return 1;
    }
    else{
        return 0;
    }
}

int BoomDetectie(){
    if (PINF & (1<<PF4)){//A3
        return 1;
    }
    else if (PINF & (1<<PF5)){
        return 1;
    }
    else{
        return 0;
    }
}

void LightSwap(void){
    PORTE ^= (1<<PE4);
    PORTE ^= (1<<PE5);
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
