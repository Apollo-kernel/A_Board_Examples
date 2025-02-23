#ifndef __MUSIC_NOTE_H__
#define __MUSIC_NOTE_H__


#include "main.h"

//�����µ�����
#define MUSIC_BEGIN         1
//��ǰ���������ڳ�����
#define MUSIC_CONTINUE      2
//ֹͣ��������
#define MUSIC_FINISH        3

//���������������Ķ�ʱ��ʱ��Ƶ��
#define BEEP_TIM_CLOCK  9142857U	//8000000U
																	//9142857U
//���������б�
enum MusicList
{
  CREATE_MUSIC = 1,
  MISS_MUSIC,
  TWO_TIGERS_MUSIC,
  PEPPA_PIG_MUSIC,
  SUPER_MARIO_MUSIC,
  DOU_DI_ZHU_MUSIC,
  POLICE_MUSIC,
  AMBULANCE_MUSIC,
	
	
	TEST_MUSIC,
	TEST1_MUSIC
	
};

typedef struct
{
  uint16_t tone; //����
  uint16_t time; //ʱ�� Ϊ1��ʾ10ms
}Note_TypeDef;

void enable_music_player(void);
void play_a_note(uint16_t tone,uint8_t volumeLevel);
void play_music_block(uint8_t volume_level);
void music_callback(void);

//�����������C
#define CL1 262
#define CL2 294
#define CL3 330
#define CL4 349
#define CL5 392
#define CL6 440
#define CL7 494

//������������C
#define CM1 523
#define CM2 587
#define CM3 659
#define CM4 698
#define CM5 784
#define CM6 880
#define CM7 988

//�����������C
#define CH1 1047
#define CH2 1175
#define CH3 1319
#define CH4 1397
#define CH5 1568
#define CH6 1760
#define CH7 1976

//�����������D
#define DL1 294
#define DL2 330
#define DL3 370
#define DL4 392
#define DL5 440
#define DL6 494
#define DL7 554

//������������D
#define DM1 587
#define DM2 659
#define DM3 740
#define DM4 784
#define DM5 880
#define DM6 988
#define DM7 1109

//�����������D
#define DH1 1175
#define DH2 1319
#define DH3 1480
#define DH4 1568
#define DH5 1760
#define DH6 1976
#define DH7 2217

//�����������E
#define EL1 330
#define EL2 370
#define EL3 415
#define EL4 440
#define EL5 494
#define EL6 554
#define EL65 587
#define EL7 622

//������������E
#define EM1 659
#define EM2 740
#define EM3 831
#define EM4 880
#define EM5 988
#define EM6 1109
#define EM7 1245

//�����������E
#define EH1 1319
#define EH2 1480
#define EH3 1661
#define EH4 1760
#define EH5 1976

//�����������F (��λ��Hz)
#define FL1 349
#define FL2 392
#define FL3 440
#define FL4 466
#define FL5 523
#define FL6 587
#define FL7 659

//������������F
#define FM1 698
#define FM2 784
#define FM3 880
#define FM4 932
#define FM5 1047
#define FM6 1175
#define FM7 1319

//�����������F
#define FH1 1397
#define FH2 1568
#define FH3 1760
#define FH4 1865


extern const Note_TypeDef testMusic[];
extern const Note_TypeDef test1Music[];


extern const Note_TypeDef MissMusic[];
extern const Note_TypeDef CreatMusic[];
extern const Note_TypeDef AmbulanceMusic[];
extern const Note_TypeDef PoliceMusic[];
extern const Note_TypeDef TwoTigersMusic[];
extern const Note_TypeDef PeppaPigMusic[];
extern const Note_TypeDef SuperMarioMusic[];
extern const Note_TypeDef DouDiZhuMusic[];


#ifdef __cplusplus
}
#endif

#endif
