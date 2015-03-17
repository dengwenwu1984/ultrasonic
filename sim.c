#include <msp430g2452.h>

int main(void) {
    /* stop watchdog to prevent time out reset */
    WDTCTL = WDTPW +WDTHOLD;
    /* the following two lines will set clock rate to approximately 7~8MHz */
    BCSCTL1 = 0xCC;
    DCOCTL = 0xFF;

    /* Timer A using SMCLK */
    TACTL |= TASSEL_2;

    /* P1.0 as input */
    P1DIR &= ~(1<<0);

    /* P1.4 as output */
    P1DIR |= (1<<4);

    while (1) {
        
    }

    return 0;
}

