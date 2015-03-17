#include <msp430g2231.h>


int main(void)
{
    /* Initialize Timer A for 9600 baud */
    TA0CCR0 |= UART_TBIT;
    return 0;
}
