#include <msp430g2231.h>
#include "timer.h"
#include "fsm.h"

#define NULL 0
#define FACTOR 18

/* default number of measurements (NORMAL mode) */
volatile int measurements = 10;

/* default factor set to measure in cm (NORMAL mode) */
volatile int scale_factor = 100;

transition transition_table[EVENTS][STATES] = {
/*                      NORMAL                          ACCURACY                         LONG_RANGE*/
/* SELECT_BTN */    {{ACCURACY, set_accuracy},      {LONG_RANGE, set_long_range},       {NORMAL, set_normal}},
/* MEASURE_BTN */   {{NORMAL, measure_dist},        {ACCURACY, measure_dist},           {LONG_RANGE, measure_dist}}
};

void delay_ms(unsigned int ms)
{
    while (ms) {
        __delay_cycles(8130);
        ms--;
    }
}

void handle_event(event ev) {
    if (ev <= EVENTS && curr_state <= STATES) {
        transition next_sp = transition_table[ev][curr_state];
        if (next_sp.action != NULL) {
            next_sp.action();
        }
        curr_state = next_sp.next_state;
    }
    else {
        curr_state = NORMAL;
    }
}


void set_normal(void) {
    P1OUT &= ~(1<<3);
    P1OUT &= ~(1<<5);
    measurements = 10;
    scale_factor = 100;
    reset_display();
}

void set_accuracy(void) {
    P1OUT |= (1<<3);
    P1OUT &= ~(1<<5);
    measurements = 20;
    scale_factor = 100;
    reset_display();
}

void set_long_range(void) {
    set_normal();
    P1OUT |= (1<<5);
    measurements = 20;
    scale_factor = 1000;
    reset_display();
}

void measure_dist(void) {
    /* variables */
    unsigned int measurements_100um[20];
    unsigned int distance_100um = 0;
    unsigned int avg_distance_100um = 0;
    unsigned int avg_distance_scaled = 0;
    int valid_measurements = 0;
    int i;

    for (i = 0; i < measurements; i++) {
        wdt_counter = 0;
        distance_100um = 0;
        pulse_send();

        /* listen for echo within 5m */
        while (!(P1IN & (1<<4)) && wdt_counter < 8) {

        }
        timer_stop();

        /* check validity */
        if (wdt_counter < 8) {
            distance_100um = timer_counter*FACTOR;
            /* add to table */
            if (valid_measurements > 0) {
                
                if ((distance_100um >= (measurements_100um[valid_measurements-1]-50)) && (distance_100um <= (measurements_100um[valid_measurements-1]+50))) {
                    measurements_100um[valid_measurements] = distance_100um;
                    valid_measurements++;
                    
                }
            }
            else {
                measurements_100um[valid_measurements] = distance_100um;
                valid_measurements++;
                
            }
            //measurements_100um[valid_measurements] = distance_100um;
            //valid_measurements++;
        }
        delay_ms(10);
    }

    if (valid_measurements > 4) {
        /* calculate mean */
        for (i = 0; i < valid_measurements; i++) {
            avg_distance_100um += measurements_100um[i];
        }
        avg_distance_100um = avg_distance_100um/valid_measurements;
        avg_distance_scaled = avg_distance_100um/scale_factor;
    }
    else {
        avg_distance_scaled = 0;
    }
    
    /* display distance */
    display_dist(avg_distance_scaled);
}

void reset_display(void)
{
    P1OUT |= (1<<7);
    delay_ms(1);
    P1OUT &= ~(1<<7);
}

void display_dist(int value) {
    int i;
    reset_display();

    /* display distance */
    for (i = 0; i < value; i++) {
        P1OUT |= (1<<6);
        delay_ms(1);
        P1OUT &= ~(1<<6);
    }
}

