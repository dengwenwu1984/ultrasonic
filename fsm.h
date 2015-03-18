
#define EVENTS 2
#define STATES 2

typedef enum {
    NORMAL,
    ACCURACY,
} state;

state curr_state;

typedef enum {
    SELECT_BTN,
    MEASURE_BTN,
} event;

typedef struct {
    int next_state;
    int (*action)(void);
} transition;

void delay_ms(unsigned int ms);
void handle_event(event ev);
void set_normal(void);
void set_accuracy(void);
void measure_dist(void);
void reset_7seg(void);
void display_dist(void);

