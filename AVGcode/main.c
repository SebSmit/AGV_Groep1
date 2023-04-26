/*
 */

#include <avr/io.h>

int main(void)
{

    // Insert code

    while(1)
    {
        DDRB = (1 << PB7);
        while (1){
            PORTB ^=(1<<PB6);
        }

        //Dikke banaan


        //comment (commentaar)
        //dikke sebas 3 bier


    }

    return 0;
}
