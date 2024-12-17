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

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId thread1Handle;
osThreadId thread2Handle;
osMessageQId osQueueHandle;

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

  /* Create the queue(s) */
  /* definition and creation of osQueue */
  osMessageQDef(osQueue, 1, uint16_t);
  osQueueHandle = osMessageCreate(osMessageQ(osQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of thread1 */
  osThreadDef(thread1, producer_thread, osPriorityNormal, 0, 128);
  thread1Handle = osThreadCreate(osThread(thread1), NULL);

  /* definition and creation of thread2 */
  osThreadDef(thread2, consumer_thread, osPriorityBelowNormal, 0, 128);
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
	uint32_t value = 0;
  /* Infinite loop */
  for(;;)
  {
    osDelay(300);
		if (osMessagePut(osQueueHandle, value, 100) == osOK)
    {
				value++;
    }
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
	(void) argument;
	uint32_t value = 0;
	osEvent event;
  /* Infinite loop */
  for(;;)
  {
    event = osMessageGet(osQueueHandle, 100);

    if (event.status == osEventMessage)
    {
      if (event.value.v == value)
      {
					value++;
					HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
      }
		}
  }
  /* USER CODE END consumer_thread */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
