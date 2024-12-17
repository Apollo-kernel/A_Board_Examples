#ifndef __BUZZER_H__
#define __BUZZER_H__
#include "stdint.h"

extern void buzzer_on(uint16_t psc, uint16_t pwm);
extern void buzzer_off(void);

#endif