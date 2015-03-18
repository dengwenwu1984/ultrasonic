
#define EVENTS 2
#define STATES 3

typedef enum {
    NORMAL,
    ACCURACY,
    LONG_RANGE,
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
void set_long_range(void);
void measure_dist(void);
void reset_display(void);
void display_dist(int value);

