#include "msp430g2231.h"
// Hardware-related definitions
#define UART_TXD 0x02                                  // TXD on P1.1 (Timer0_A.OUT0)

#define UART_TBIT (1000000 / 9600)                     // 9600 Baud, SMCLK = 1MHz
// Globals for transmit UART communication
unsigned int txData;                                   // UART internal variable for TX

void TimerA_UART_tx(unsigned char byte);               // Function prototypes
void TimerA_UART_print(char *string);

int main(void){

    WDTCTL = WDTPW + WDTHOLD;                            // Stop watchdog timer
    DCOCTL = 0x00;                                       // Set DCOCLK to 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1OUT = UART_TXD;                                    // Initialize P1.1
    P1SEL = UART_TXD;                                    // Timer function for TXD pin
    P1DIR = UART_TXD;                                    // Set TXD pin to output

    // Timer_A for transmit UART operation
    TACCTL0 = OUT;                                      // Set TXD Idle as Mark = '1'
    TACCTL1 = SCS + CM1 + CAP;                          // Sync, Neg Edge, Capture
    TACTL = TASSEL_2 + MC_2;                            // SMCLK, start in continuous mode

    _BIS_SR(GIE);                                        // Enable CPU interrupts

    TimerA_UART_print("G2553 TimerA UART\r\n");          // Send test message
    TimerA_UART_print("READY.\r\n");
    return 0;
}

void TimerA_UART_tx(unsigned char byte) {              // Outputs one byte using the Timer_A UART

    while (TACCTL0 & CCIE);                              // Ensure last char got TX'd

    TACCR0 = TAR;                                       // Current state of TA counter

    TACCR0 += UART_TBIT;                                // One bit time till first bit

    txData = byte;                                       // Load transmit data, e.g. 'A'=01000001

    txData |= 0x100;                                     // Add mark stop bit, e.g. 101000001

    txData <<= 1;                                        // Add space start bit, e.g. 1010000010

    TACCTL0 = OUTMOD0 + CCIE;                           // Set TXD on, enable counter interrupt
}

void TimerA_UART_print(char *string) {                 // Prints a string using the Timer_A UART

    while (*string)
        TimerA_UART_tx(*string++);
}

#pragma vector = TIMER0_A0_VECTOR                      // Timer_A UART - Transmit ISR

__interrupt void Timer_A0_ISR(void) {

    static unsigned char txBitCnt = 10;

    TACCR0 += UART_TBIT;                                // Add Offset to CCRx

    if (txBitCnt == 0) {                                 // All bits TXed?

        TACCTL0 &= ~CCIE;                                 // All bits TXed, disable interrupt

        txBitCnt = 10;                                     // Re-load bit counter
    }
    else {
        if (txData & 0x01)
            TACCTL0 &= ~OUTMOD2;                            // TX Mark '1'
        else
            TACCTL0 |= OUTMOD2;                             // TX Space '0'
    }
    txData >>= 1;                                        // Shift right 1 bit (low bits TX'ed first)
    txBitCnt--;
}
