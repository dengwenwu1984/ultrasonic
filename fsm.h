
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
    SELECT_BTN,
    MEASURE_BTN,
} event;

typedef struct {
    int next_state;
    int (*action)(void);
} transition;
