#include <msp430g2231.h>
#define RS 0x20
#define E 0x80 

/* LCD connections
 * RS - P1.5
 * E - P1.7
 * D4 - P1.0
 * D5 - P1.1
 * D6 - P1.2
 * D7 - P1.3
 */

void lcd_reset(void);
void lcd_reset_inv(void);
void lcd_init(void);
void lcd_data (unsigned char dat);
void lcd_cmd (char cmd);


void delay_ms(unsigned int ms)
{
    while (ms) {
        __delay_cycles(8130);
        ms--;
    }
}

int main(void) {

    WDTCTL = WDTPW + WDTHOLD;
    /* the following two lines will set clock rate to approximately 7~8MHz */
    BCSCTL1 = 0xCC;
    DCOCTL = 0xFF;

    P1DIR = 0xFF; 
    /* inverted logic */
    P1OUT = 0xFF; 
    
    lcd_init();

    while (1) {
        lcd_data('A');
        delay_ms(1000);
    }


    return 0;
}

void lcd_reset() {
    P1OUT = ~0xFF;
    delay_ms(50);
    P1OUT = ~(0x03+E);
    P1OUT = ~(0x03);
    delay_ms(10);
    P1OUT = ~(0x03+E);
    P1OUT = ~(0x03);
    delay_ms(2);
    P1OUT = ~(0x03+E);
    P1OUT = ~(0x03);
    delay_ms(2);
    P1OUT = ~(0x02+E);
    P1OUT = ~(0x02);
    delay_ms(2);
}

void lcd_reset_inv() {
    P1OUT = 0x00;
    delay_ms(50);
    P1OUT = 0x7C;
    P1OUT = 0xFC;
    delay_ms(10);
    P1OUT = 0x7C;
    P1OUT = 0xFC;
    delay_ms(2);
    P1OUT = 0x7C;
    P1OUT = 0xFC;
    delay_ms(2);
    P1OUT = 0x7D;
    P1OUT = 0xFD;
    delay_ms(2);
}

void lcd_cmd (char cmd) { 
	P1OUT = ~(((cmd >> 4) & 0x0F) | E);
	P1OUT = ~(((cmd >> 4) & 0x0F));

        delay_ms(2);

	P1OUT = ~((cmd & 0x0F) | E);
	P1OUT = ~((cmd & 0x0F));

	delay_ms(2);
}

void lcd_init () {
	lcd_reset_inv();         // Call LCD reset
	lcd_cmd(0x28);       // 4-bit mode - 2 line - 5x7 font. 
	lcd_cmd(0x0E);       // Display no cursor - no blink.
	lcd_cmd(0x06);       // Automatic Increment - No Display shift.
	lcd_cmd(0x80);       // Address DDRAM with 0 offset 80h.
 }

void lcd_data (unsigned char dat) { 
	P1OUT = ~(((dat >> 4) & 0x0F)|E|RS);
	P1OUT = ~(((dat >> 4) & 0x0F)|RS);

	delay_ms(2);
	
	P1OUT = ~((dat & 0x0F)|E|RS);
	P1OUT = ~((dat & 0x0F)|RS);

	delay_ms(2);
}
