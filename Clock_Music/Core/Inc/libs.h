#ifndef _LIBS_H_
#define _LIBS_H_

#define MAX_PSC             500			//��Ƶϵ��
#define MAX_BUZZER_PWM      2500		//����ֵ����
#define MIN_BUZZER_PWM      500			//����ֵ����

//����PWM��������������Ķ�ʱ��
#define NOTE_PWM_TIM         htim1
//PWM�����ͨ��
#define NOTE_PWM_CHANNEL     TIM_CHANNEL_1


#include "main.h"
#include "stdint.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

#include "music_note.h"

extern void Refresh_LED(uint16_t state); // ˢ��LED�ƣ�0��1��
extern void Refresh_LED_Neg(uint16_t state); // ˢ��LED�ƣ�0��1����
extern void Refresh_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness); // ˢ�������
extern void Hundred_Display(uint32_t hundred, uint16_t brightness); // �����999
extern void Hundreds_Display(uint8_t code, uint8_t dot, uint8_t id, uint16_t brightness);
extern void buzzer_on(uint16_t psc, uint16_t pwm);
extern void buzzer_off(void);

extern void Clear_Display(); // ���������

extern void BGM_handler(void);
extern void enable_music_player(void);
extern void play_a_note(uint16_t tone,uint8_t volumeLevel);
extern void play_BGM(uint8_t volume_level,Note_TypeDef* BGM);
extern void music_player(uint8_t volume_level , enum MusicList num);

extern uint16_t t_ms,t_led,t_tube,number,led_state;
extern uint8_t alarm,minute,second,Count_Down_Flag;
extern int k;
extern uint8_t display_codes[10];
extern uint16_t psc;
extern uint16_t pwm;

//�������ֲ��ŵ�ȫ�ֱ���
extern uint8_t Music_Volume;     //����
extern uint8_t Music_Flag;       //�л����ֵı�־λ
extern uint8_t Music_Off;     	//����ֹͣ�ı�־λ
extern uint8_t Cycle_Flag;     //����ѭ���ı�־λ
extern uint8_t Music_List;     //�����б����
extern uint16_t BGM_Length;      //BGM�������ֵĳ���
extern Note_TypeDef *BGM_Current;      //ָ��ǰBGM��ָ��
extern unsigned char MoleMusicVolum;   //����,���鷶Χ1~10,1��10С


// ��������ȣ�0~1000��htim3.Instance->ARR��
extern uint16_t display_birghness;

// ���������ʾ������
extern uint16_t num;
extern int iButtonCount_SW1,iButtonCount_SW2,iButtonCount_SW3;//i����int�ͱ�����ButtonCount��ʾ������������
extern int iButtonFlag_SW1,iButtonFlag_SW2,iButtonFlag_SW3;//i����int�ͱ�����ButtonFlag��ʾ�ذ�����־��1�������°�����0Ϊû�����°���
extern int g_iButtonState_SW1,g_iButtonState_SW2,g_iButtonState_SW3;//g��globle����ȫ�ֱ��������������ط����ã�i����int�ͱ�����ButtonState��ʾ������־��1�����£�0�����ɿ�
extern int g_iButtonStates_SW1,g_iButtonStates_SW2,g_iButtonStates_SW3;//g��globle����ȫ�ֱ��������������ط����ã�i����int�ͱ�����ButtonState��ʾ������־��1�����£�0�����ɿ�
extern int g_iButtonStatel_SW1,g_iButtonStatel_SW2,g_iButtonStatel_SW3;//g��globle����ȫ�ֱ��������������ط����ã�i����int�ͱ�����ButtonState��ʾ������־��1�����£�0�����ɿ�
extern int setting_mode;


extern uint8_t Count_Down_Flag;




#endif
