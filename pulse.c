#include <msp430g2231.h>
#include "pulse.h"
#include "timer.h"

void pulse_init(void)
{
    /* stop watchdog to prevent time out reset */
    WDTCTL = WDTPW +WDTHOLD;
    /* the following two lines will set clock rate to approximately 7~8MHz */
    BCSCTL1 = 0xCC;
    DCOCTL = 0xFF;
    /* Timer A is using SMCLK */
    TACTL |= TASSEL_2;
    /* NOTE you need to change ‘***’ to the frequency have measured in step 1 */
    TACCR0 = 8130000/80000;
    /* enable global interruption */
    _BIS_SR(GIE);
}

void pulse_send(void) {
    /* reset counter */
    timer_counter = 0;
    /* set P1.0 back to output */
    P1DIR |= 0x01;
    /* enable Timer A interruption, so that interruption function will be called */
    TACCTL0 |= CCIE;
    /* start timer, up mode */
    TACTL |= MC_1;
    /* wait until 80/2=40 waves are sent => 1ms */
    while (timer_counter < 80) {
        
    }
    /* set pin low */
    P1OUT &= ~0x01;
    /* set P1.0 as input to avoid sending pulses to transmitter on timer overflow */
    P1DIR &= ~0x01;
    /* stop timer */
    //TACTL &= ~(MC_1 | MC_2);
}


