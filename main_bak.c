#include <msp430g2231.h>
#include "timer.h"
#include "pulse.h"
#include "fsm.h"

unsigned int distance_100um;
unsigned int distance_cm;



int main(void)
{
    pulse_init();
    timer_init();

    curr_state = NORMAL;

    /* outputs */
    P1DIR |= (1<<0);
    P1DIR |= (1<<3);
    P1DIR |= (1<<5);
    P1DIR |= (1<<6);

    P1OUT &= ~(1<<0);
    P1OUT &= ~(1<<3);
    P1OUT &= ~(1<<5);
    P1OUT &= ~(1<<6);

    
    /* inputs */
    P1DIR &= ~(1<<1);
    P1DIR &= ~(1<<2);
    P1DIR &= ~(1<<4);

    P1REN &= ~(1<<1);
    P1REN &= ~(1<<2);

    P1IE |= (1<<1);
    P1IE |= (1<<2);

    timer_wdt_start();

    while (1) {
        //measure_dist();
        //delay_ms(2000);
        
    }
    
    return 0;
}


#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA (void) {
    P1OUT ^= 0x01;
    timer_counter++;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void) {
    //check which button
    if (P1IFG & (1<<1)) {
        P1IFG &= ~(1<<1);
        handle_event(MEASURE_BTN);
    }
    if (P1IFG & (1<<2)) {
        P1IFG &= ~(1<<2);
        handle_event(SELECT_BTN);
        
    }
}

#pragma vector=WDT_VECTOR
__interrupt void Watchdog (void) {
    wdt_counter++;
}
