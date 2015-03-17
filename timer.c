#include <msp430g2231.h>
#include "timer.h"

unsigned volatile int timer_counter;
unsigned volatile int wdt_counter;

void timer_wdt_start() {
    /* use watchdog as timer that generates interrupts, reset timer */
    WDTCTL = WDTPW + WDTTMSEL + WDTCNTCL;
    /* enable WDT interrupts */
    IE1 |= WDTIE;
}

void timer_wdt_stop() {
    /* disable WDT */
    WDTCTL = WDTPW + WDTHOLD;
    /* disable WDT interrupts */
    IE1 &= ~WDTIE;
}

void timer_init(void)
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

void timer_start(void)
{
    /* reset counter */
    timer_counter = 0;
    /* set P1.0 as input to avoid sending pulses to transmitter on timer overflow */
    P1DIR &= ~0x01;
    /* enable Timer A interruption, so that interruption function will be called */
    TACCTL0 = CCIE;
    /* start timer, up mode */
    TACTL |= MC_1;
}

void timer_stop(void)
{
    /* stop timer */
    TACTL &= ~(MC_1 | MC_2);
}



