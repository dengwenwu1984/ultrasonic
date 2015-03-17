#include <msp430g2231.h>

/* incremented every ms */
unsigned int timer_counter;

void timer_clock_test(void)
{
    /* stop watchdog to prevent time out reset */
    WDTCTL = WDTPW +WDTHOLD;
    /* the following two lines will set clock rate to approximately 7~8MHz */
    BCSCTL1 = 0xCC;
    DCOCTL = 0xFF;
    /* output clock on P1.4 */
    P1SEL = 0x10;
    P1DIR = 0x10;
}

void timer_init(void)
{
    /* the following two lines will set clock rate to approximately 7~8MHz */
    BCSCTL1 = 0xCC;
    DCOCTL = 0xFF;
    /* Timer B is using SMCLK */
    TACTL |= TASSEL_2;
    /* Set timer to give interrupt every ms */
    TACCR0 = 8130000/1000; 
    /* enable global interruption */
    P1DIR = 0x01;
    _BIS_SR(GIE);
}

void timer_start(void)
{
    /* reset counter */
    timer_counter = 0;
    /* enable Timer B interruption, so that interruption function will be called */
    TACCTL0 = CCIE;
    /* start timer, up mode */
    TACTL |= MC_1;
}

void timer_stop(void)
{
    /* stop timer */
    TACTL &= ~(MC_1 | MC_2);
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA (void) {
    P1OUT ^= 0x01;
    timer_counter++;
}

int main(void)
{
    timer_init();
    timer_start();
    while (1) {
        
    }
    return 0;
}
