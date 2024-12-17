/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{ /* Mail object structure */
  uint32_t var1; 	/* var1 is a uint32_t */
  uint32_t var2; 	/* var2 is a uint32_t */
  uint8_t var3; 	/* var3 is a uint8_t */
} Amail_TypeDef;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define blckqSTACK_SIZE   configMINIMAL_STACK_SIZE
#define MAIL_SIZE        (uint32_t) 1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osMailQId mailId;

uint32_t ProducerValue1 = 0, ProducerValue2 = 0;
uint8_t ProducerValue3 = 0;
__IO uint32_t ProducerErrors = 0;
uint32_t ConsumerValue1 = 0, ConsumerValue2 = 0;
uint8_t ConsumerValue3 = 0;

/* USER CODE END Variables */
osThreadId thread1Handle;
osThreadId thread2Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void producer_thread(void const * argument);
void consumer_thread(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  osMailQDef(mail, MAIL_SIZE, Amail_TypeDef); /* Define mail queue */
  mailId = osMailCreate(osMailQ(mail), NULL); /* create mail queue */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of thread1 */
  osThreadDef(thread1, producer_thread, osPriorityNormal, 0, 128);
  thread1Handle = osThreadCreate(osThread(thread1), NULL);

  /* definition and creation of thread2 */
  osThreadDef(thread2, consumer_thread, osPriorityNormal, 0, 128);
  thread2Handle = osThreadCreate(osThread(thread2), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_producer_thread */
/**
  * @brief  Function implementing the thread1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_producer_thread */
void producer_thread(void const * argument)
{
  /* USER CODE BEGIN producer_thread */
	(void) argument;
	Amail_TypeDef *pTMail;
  /* Infinite loop */
  for(;;)
  {
		/* 分配消息邮箱内存并设置内容 */
    pTMail = osMailAlloc(mailId, osWaitForever);
    pTMail->var1 = ProducerValue1;
    pTMail->var2 = ProducerValue2;
    pTMail->var3 = ProducerValue3;

		++ProducerValue1;
    ProducerValue2 += 2;
    ProducerValue3 += 3;
		
    osMailPut(mailId, pTMail);
		
    osDelay(300);
  }
  /* USER CODE END producer_thread */
}

/* USER CODE BEGIN Header_consumer_thread */
/**
* @brief Function implementing the thread2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_consumer_thread */
void consumer_thread(void const * argument)
{
  /* USER CODE BEGIN consumer_thread */
  /* Infinite loop */
	(void) argument;
	osEvent event;
  Amail_TypeDef *pRMail;
  /* Infinite loop */
  for(;;)
  {
    event = osMailGet(mailId, osWaitForever); 

    if(event.status == osEventMail)
    {
      pRMail = event.value.p;

      if((pRMail->var1 == ConsumerValue1) && (pRMail->var2 == ConsumerValue2) && (pRMail->var3 == ConsumerValue3))
      {
        ++ConsumerValue1;
        ConsumerValue2 += 2;
        ConsumerValue3 += 3;
				
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      }
		}

    osMailFree(mailId, pRMail);
  }
  /* USER CODE END consumer_thread */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
