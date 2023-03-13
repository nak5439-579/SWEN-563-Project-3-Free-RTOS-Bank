/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "manager.h"
#include "customer.h"
#include "teller.h"
#include "queue.h"
#include "semaphores.h"

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
TaskHandle_t customer_task_handle;
TaskHandle_t teller_1_task_handle;
TaskHandle_t teller_2_task_handle;
TaskHandle_t teller_3_task_handle;
TaskHandle_t manager_task_handle;

SemaphoreHandle_t xUSART;
SemaphoreHandle_t xQueue;
SemaphoreHandle_t xStats;
SemaphoreHandle_t xRNG;
SemaphoreHandle_t xBuffer;
QueueHandle_t customer_queue;
char end_flag;
char customer_finished;
char teller_1_finished;
char teller_2_finished;
char teller_3_finished;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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
	xUSART = xSemaphoreCreateMutex();
	configASSERT(NULL != xUSART);
	xQueue = xSemaphoreCreateMutex();
	configASSERT(NULL != xQueue);
	xStats = xSemaphoreCreateMutex();
	configASSERT(NULL != xStats);
	xRNG = xSemaphoreCreateMutex();
	configASSERT(NULL != xRNG);
	xBuffer = xSemaphoreCreateMutex();
	configASSERT(NULL != xBuffer);
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	customer_queue = xQueueCreate(25, sizeof(customer_t));

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  end_flag = 1;
  teller_1_finished = 1;
  teller_2_finished = 1;
  teller_3_finished = 1;
  configASSERT(pdPASS == xTaskCreate(manager_task, "Manager Task", 256, NULL, 1, &manager_task_handle));
  configASSERT(pdPASS == xTaskCreate(customer_task, "Customer Task", 256, NULL, 1, &customer_task_handle));
  configASSERT(pdPASS == xTaskCreate(teller_task, "Teller 1 Task\r\n", 256, NULL, 1, &teller_1_task_handle));
  configASSERT(pdPASS == xTaskCreate(teller_task, "Teller 2 Task\r\n", 256, NULL, 1, &teller_2_task_handle));
  configASSERT(pdPASS == xTaskCreate(teller_task, "Teller 3 Task\r\n", 256, NULL, 1, &teller_3_task_handle));

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
