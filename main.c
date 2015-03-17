#include <msp430g2231.h>
#include "timer.h"
#include "pulse.h"
//#include "fsm.h"

unsigned int distance_100um;
unsigned int distance_cm;
int i;

void delay_ms(unsigned int ms)
{
    while (ms) {
        __delay_cycles(8130);
        ms--;
    }
}

void reset_7seg(void);

int main(void)
{
    pulse_init();
    timer_init();

    //curr_state = NORMAL;

    P1DIR |= 0x01;
    P1DIR |= (1<<6);
    P1DIR |= (1<<5);
    
    P1REN &= ~(1<<4);
    P1DIR &= ~(1<<4);

    timer_wdt_start();

    while (1) {
        ////for testing
        wdt_counter = 0;
        distance_100um = 0;
        pulse_send();
        /* listen for echo within 5m */
        while (!(P1IN & (1<<4)) && wdt_counter < 8) {

        }
        timer_stop();
        if (wdt_counter >= 8) {
            distance_100um = 0;
        }
        else {
            distance_100um = timer_counter*18;
        }
        //distance_um = 200*1838UL;
        wdt_counter = 0;
        distance_cm = distance_100um/100;
        //int distance_cm = 40;
        reset_7seg();
        for (i = 0; i < distance_cm; i++) {
            
            P1OUT |= (1<<6);
            delay_ms(1);
            P1OUT &= ~(1<<6);
            
        }
        delay_ms(1000);
        //calculate mean
        //update display
        
    }
    
    return 0;
}

void reset_7seg(void)
{
    P1OUT |= (1<<5);
    delay_ms(1);
    P1OUT &= ~(1<<5);
}

#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA (void) {
    P1OUT ^= 0x01;
    timer_counter++;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void) {
    //check which button
    //handle event
}

#pragma vector=WDT_VECTOR
__interrupt void Watchdog (void) {
    wdt_counter++;
}
