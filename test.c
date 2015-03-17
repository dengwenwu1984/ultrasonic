#include <msp430g2452.h>

volatile unsigned int timer_counter = 0;

void delay_ms(unsigned int ms)
{
    while (ms) {
        __delay_cycles(6944);
        ms--;
    }
}

void timer_init(void)
{
    /* the following two lines will set clock rate to approximately 7~8MHz */
    BCSCTL1 = 0xCC;
    DCOCTL = 0xFF;
    /* Timer A is using SMCLK */
    TACTL |= TASSEL_2;
    /* Set timer to give interrupt every ms */
    TACCR0 = 6944000/1000; 
    /* enable global interruption */
    _BIS_SR(GIE);
}

void timer_start(void)
{
    /* reset counter */
    timer_counter = 0;
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

void send_reply(void) {
    delay_ms(2);
    P1OUT |= (1<<6);
    delay_ms(1);
    P1OUT &= ~(1<<6);
}

int main(void) {
    /* stop watchdog to prevent time out reset */
    WDTCTL = WDTPW +WDTHOLD;

    /* P1.6 as output */
    P1DIR |= (1<<6);
    P1OUT &= ~(1<<6);

    /* P1.4 as input */
    P1DIR &= ~(1<<4);
    /* enable interrupt for pin P1.0 */
    P1IE |= (1<<4);
    /* interrupt on rising edge */
    P1IES &= ~(1<<4);

    /* enable global interruption */
    _BIS_SR(GIE);

    timer_init();

    while (1) {
        
    }

    return 0;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA (void) {
    timer_counter++;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1 (void) {
    send_reply();

    /* clear interrupt flag */
    P1IFG &= ~(1<<4);
}
