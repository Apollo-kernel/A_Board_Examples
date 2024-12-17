#include "buzzer.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;
void buzzer_on(uint16_t psc, uint16_t pwm)
{
	//设置分频
	__HAL_TIM_PRESCALER(&htim1, psc);
	//设置 PWM 的占空比
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, pwm);
}
void buzzer_off(void)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
}