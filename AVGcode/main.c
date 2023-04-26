/*
 */

#include <avr/io.h>

int main(void)
{

    // Insert code

    while(1)
    {
        DDRB = (1 << PB7);
    }

    return 0;
}
