#ifndef BEEP_H
#define BEEP_H

#include <stdbool.h>

void beep_init(void);
void beep_start(int frequency);
void beep_stop(void);
bool beep_is_playing(void);
void beep_shutdown(void);

#endif /* BEEP_H */
