#include <msp430g2231.h>

volatile int pulse_counter;

void delay_ms(unsigned int ms)
{
    while (ms) {
        __delay_cycles(8130);
        ms--;
    }
}

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
    /* use P1.0 as output */
    P1DIR |= 0x01;
    /* enable global interruption */
    _BIS_SR(GIE);
}

void pulse_send(void) {
    /* reset counter */
    pulse_counter = 0;
    /* enable Timer A interruption, so that interruption function will be called */
    TACCTL0 = CCIE;
    /* start timer, up mode */
    TACTL |= MC_1;
    /* wait until 80/2=40 waves are sent => 1ms */
    while (pulse_counter < 80) {
        
    }
    P1OUT &= ~0x01;
    /* stop timer */
    TACTL &= ~(MC_1 | MC_2);
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA (void) {
    /* flipp P1.0 */
    P1OUT ^= 0x01;
    pulse_counter++;
}

int main(void) {
    pulse_init();
    while (1) {
        pulse_send();
        delay_ms(5000);
        
    }

    
    return 0;
}

