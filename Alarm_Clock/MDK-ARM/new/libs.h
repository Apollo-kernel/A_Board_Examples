#ifndef _LIBS_H_
#define _LIBS_H_

#include "stdint.h"

extern void Refresh_LED(uint16_t state); // 刷新LED灯（0亮1灭）
extern void Refresh_LED_Neg(uint16_t state); // 刷新LED灯（0灭1亮）
extern void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness); // 刷新数码管
extern void Hundred_Display(uint32_t hundred, uint16_t brightness); // 数码管999
extern void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness);

extern void Clear_Display(); // 数码管清屏

#endif
