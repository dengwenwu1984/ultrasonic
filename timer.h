/* incremented every ms */
extern volatile unsigned int timer_counter;
extern volatile unsigned int wdt_counter;

void timer_init(void);

void timer_start(void);

void timer_stop(void);

