#include <msp430g2231.h>
#include "timer.h"
#include "fsm.h"

#define NULL 0
#define FACTOR 1838UL

/* default number of measurements (NORMAL mode) */
volatile int measurements = 5;

transition transition_table[EVENTS][STATES] = {
/*              NORMAL                          ACCURACY                        EXTRA                   MEASURE*/
/* SELECT */    {{ACCURACY, set_accuracy},      {EXTRA, set_extra},             {NORMAL, set_normal},   {MEASURE, NULL}},
/* MEASURE */   {{NORMAL, measure_dist},        {ACCURACY, measure_dist},       {EXTRA, measure_dist},  {MEASURE, NULL}}
};


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
    measurements = 5;
}

void set_accuracy(void) {
    measurements = 10;
}

void set_extra(void) {
}

void measure_dist(void) {
    /* variables */
    unsigned long measurements_um[10];
    unsigned long distance_um = 0;
    unsigned long avg_distance_um = 0;
    int valid_measurements = 0;
    int i;

    /* take measurements */
    for (i = 0; i < measurements; i++) {
        timer_start();
        timer_wdt_start();
        pulse_send();
        /* while no echo and no timeout */
        while (!(P1IN & (1<<4)) && wdt_counter < 16) {

        }
        timer_stop();
        timer_wdt_stop();
        if (wdt_counter < 16) {
            distance_um = (unsigned long)timer_counter*FACTOR;
        }
        measurements_um[i] = distance_um;
        
    }

    /* calculate mean */
    for (i = 0; i < measurements; i++) {
       avg_distance_um += measurements_um[i];
       if (measurements_um[i]) {
           valid_measurements++;
       }
    }
    avg_distance_um = avg_distance_um / valid_measurements; 

    /* display distance */

}

void display_dist(void) {
}


