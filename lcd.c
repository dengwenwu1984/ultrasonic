#include <msp430g2211.h>
#define LED BIT0
#define RS 0x20
#define E 0x80 
#define DB4 BIT4
#define DB5 BIT5
#define DB6 BIT6
#define DB7 BIT7

/* LCD connections
 * RS - P1.5
 * E - P1.7
 * D4 - P1.0
 * D5 - P1.1
 * D6 - P1.2
 * D7 - P1.3
 */

void lcd_reset(void);
void lcd_init(void);
void lcd_data (unsigned char dat);
void lcd_cmd (char cmd);


void delayMS(unsigned int ms )
{
    unsigned int i;
    for (i = 0; i<= ms; i++)
        __delay_cycles(2000);
}

int main(void) {

    WDTCTL = WDTPW + WDTHOLD;
    P1DIR |= 0xFF; 
    
    lcd_init();

    while (1) {
        
    }


    return 0;
}

void lcd_reset() {
    P1OUT = 0xFF;
    delayMS(50);
    P1OUT = 0x03|E;
    P1OUT = 0x03;
    delayMS(2);
    P1OUT = 0x03|E;
    P1OUT = 0x03;
    delayMS(2);
    P1OUT = 0x03|E;
    P1OUT = 0x03;
    delayMS(2);
    P1OUT = 0x02|E;
    P1OUT = 0x02;
    delayMS(2);
}

void lcd_cmd (char cmd) { 
	P1OUT = ((cmd >> 4) & 0x0F) | E;
	P1OUT = ((cmd >> 4) & 0x0F);

	delayMS(2);

	P1OUT = (cmd & 0x0F) | E;
	P1OUT = (cmd & 0x0F);

	delayMS(2);
}

void lcd_init () {
	lcd_reset();         // Call LCD reset
	lcd_cmd(0x28);       // 4-bit mode - 2 line - 5x7 font. 
	lcd_cmd(0x0E);       // Display no cursor - no blink.
	lcd_cmd(0x06);       // Automatic Increment - No Display shift.
	lcd_cmd(0x80);       // Address DDRAM with 0 offset 80h.
 }

void lcd_data (unsigned char dat) { 
	P1OUT = (((dat >> 4) & 0x0F)|E|RS);
	P1OUT = (((dat >> 4) & 0x0F)|RS);

	delayMS(2);
	
	P1OUT = ((dat & 0x0F)|E|RS);
	P1OUT = ((dat & 0x0F)|RS);

	delayMS(2);
}
