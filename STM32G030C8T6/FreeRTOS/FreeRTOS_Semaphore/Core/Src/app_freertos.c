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
osSemaphoreId osSemaphoreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void semaphore_thread1(void const * argument);
void semaphore_thread2(void const * argument);

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

  /* Create the semaphores(s) */
  /* definition and creation of osSemaphore */
  osSemaphoreDef(osSemaphore);
  osSemaphoreHandle = osSemaphoreCreate(osSemaphore(osSemaphore), 1);

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
  osThreadDef(thread1, semaphore_thread1, osPriorityNormal, 0, 128);
  thread1Handle = osThreadCreate(osThread(thread1), (void *) osSemaphoreHandle);

  /* definition and creation of thread2 */
  osThreadDef(thread2, semaphore_thread2, osPriorityIdle, 0, 128);
  thread2Handle = osThreadCreate(osThread(thread2), (void *) osSemaphoreHandle);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_semaphore_thread1 */
/**
  * @brief  Function implementing the thread1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_semaphore_thread1 */
void semaphore_thread1(void const * argument)
{
  /* USER CODE BEGIN semaphore_thread1 */
	uint32_t osStatus;
	osSemaphoreId semaphore = (osSemaphoreId) argument;
  /* Infinite loop */
  for(;;)
  {
    if (semaphore != NULL)
    {
			/* 尝试获取信号量 */
			osStatus = osSemaphoreWait(semaphore , 100);
      
      if (osStatus == osOK)
      {
        for(int i = 0; i < 10; i++) {
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
						osDelay(50);
				}

        /* 释放信号量 */
        osStatus = osSemaphoreRelease(semaphore);

        /* 挂起本线程去执行另一个线程  */
        osStatus = osThreadSuspend(NULL);
      }
    }
  }
  /* USER CODE END semaphore_thread1 */
}

/* USER CODE BEGIN Header_semaphore_thread2 */
/**
* @brief Function implementing the thread2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_semaphore_thread2 */
void semaphore_thread2(void const * argument)
{
  /* USER CODE BEGIN semaphore_thread2 */
	uint32_t osStatus;
	osSemaphoreId semaphore = (osSemaphoreId) argument;
  /* Infinite loop */
  for(;;)
  {
    if (semaphore != NULL)
    {
      /* 尝试获取信号量 */
      if (osSemaphoreWait(semaphore , 0) == osOK)
      {
        /* 恢复第一个线程运行，虽然第一个线程优先级更高，但由于无法获取信号量而被挂起等待信号量 */
        osStatus = osThreadResume(thread1Handle);

        for(int i = 0; i < 6; i++) {
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
						osDelay(300);
				}

				/* 释放信号量 */
        osStatus = osSemaphoreRelease(semaphore);
      }
    }
  }
  /* USER CODE END semaphore_thread2 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
