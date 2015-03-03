#include <msp430g2211.h>

#define EVENTS 3
#define STATES 4

void set_normal(void);
void set_accuracy(void);
void set_extra(void);
void measure_dist(void);
void display_dist(void);

typedef enum {
    NORMAL,
    ACCURACY,
    EXTRA,
    MEASURE
} state;

state curr_state;

typedef enum {
    SELECT,
    MEASURE,
    COMPLETE
} event;

typedef struct {
    int next_state;
    int (*action)(void);
} transition;

transition transistions_table[EVENTS][STATES] = {
/*              NORMAL                          ACCURACY                        EXTRA                   MEASURE*/
/* SELECT */    {{ACCURACY, set_accuracy},      {EXTRA, set_extra},             {NORMAL, set_normal},   {MEASURE, NULL}},
/* MEASURE */   {{NORMAL, measure_dist},        {ACCURACY, measure_dist},       {EXTRA, measure_dist},  {MEASURE, NULL}}
};


void handle_event(event ev) {
    if (ev <= EVENTS && curr_state <= STATES) {
        transition next_sp = transition_table[event][curr_state];
        if (next_sp.action != NULL) {
            next_sp.action();
        }
        curr_state = next_sp.next_state;
    }
    else {
        curr_state = NORMAL;

    }
}

int main(void)
{
    init_display();
    init_pulse();
    init_io();

    curr_state = NORMAL;

    while (1) {
        
    }
    
    return 0;
}


void set_normal(void) {
}
void set_accuracy(void) {
}
void set_extra(void) {
}
void measure_dist(void) {
    //send and receive pulse dependent on curr_state
    //calculate mean
    //update display

}
void display_dist(void) {
}

interrupt_routine(select_pin) {
    handle_event(SELECT)
}

interrupt_routine(measure_pin) {
    handle_event(MEASURE)
}

