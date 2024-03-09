/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
//#include <stdio.h>
#include "CalculateAngle.h"
#include "TrafficControl.h"
#include "LCD1602.h"
#include "usart.h"
#include "iwdg.h"
#include "event_groups.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BIT_Task01_EVENT (EventBits_t)(0x0001 << 0)
#define BIT_Task02_EVENT (EventBits_t)(0x0001 << 1)
#define BIT_TaskALL_EVENT BIT_Task01_EVENT | BIT_Task02_EVENT
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
EventGroupHandle_t MyEvent01Handle;
/* USER CODE END Variables */
osThreadId MPU6050TaskHandle;
osThreadId LcdTaskHandle;
osThreadId SwitchTaskHandle;
osThreadId TrafficTaskHandle;
osThreadId ReceiveTaskHandle;
osThreadId IWDG_MonitorHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartMPU6050Task(void const * argument);
void StartLcdTask(void const * argument);
void StartSwitchTask(void const * argument);
void StartTrafficTask(void const * argument);
void StartRecieveTask(void const * argument);
void StartIWDG_Monitor(void const * argument);

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
	MyEvent01Handle = xEventGroupCreate();
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
  /* definition and creation of MPU6050Task */
  osThreadDef(MPU6050Task, StartMPU6050Task, osPriorityHigh, 0, 128);
  MPU6050TaskHandle = osThreadCreate(osThread(MPU6050Task), NULL);

  /* definition and creation of LcdTask */
  osThreadDef(LcdTask, StartLcdTask, osPriorityIdle, 0, 128);
  LcdTaskHandle = osThreadCreate(osThread(LcdTask), NULL);

  /* definition and creation of SwitchTask */
  osThreadDef(SwitchTask, StartSwitchTask, osPriorityAboveNormal, 0, 128);
  SwitchTaskHandle = osThreadCreate(osThread(SwitchTask), NULL);

  /* definition and creation of TrafficTask */
  osThreadDef(TrafficTask, StartTrafficTask, osPriorityIdle, 0, 128);
  TrafficTaskHandle = osThreadCreate(osThread(TrafficTask), NULL);

  /* definition and creation of ReceiveTask */
  osThreadDef(ReceiveTask, StartRecieveTask, osPriorityNormal, 0, 128);
  ReceiveTaskHandle = osThreadCreate(osThread(ReceiveTask), NULL);

  /* definition and creation of IWDG_Monitor */
  osThreadDef(IWDG_Monitor, StartIWDG_Monitor, osPriorityRealtime, 0, 128);
  IWDG_MonitorHandle = osThreadCreate(osThread(IWDG_Monitor), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartMPU6050Task */
/**
  * @brief  Function implementing the MPU6050Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartMPU6050Task */
void StartMPU6050Task(void const * argument)
{
  /* USER CODE BEGIN StartMPU6050Task */
  /* Infinite loop */
  for(;;)
  {
	  MPU6050_Read_All(&hi2c3, &MPU6050);
	  //Calibration
	  MPU6050_Correction(&MPU6050);
	  //CalculateCompliFilter
	  CalculateCompliFilter(&MPU6050);

//	  char data[100];
//	  gcvt(MPU6050.AngleY, 6, data);
//	  HAL_UART_Transmit(&huart2, (uint8_t*)data, sizeof(data), HAL_MAX_DELAY);

	  osDelay(10);
	  xEventGroupSetBits(MyEvent01Handle, BIT_Task01_EVENT);

//	  uint8_t str1[]="\nMPU6050\n";
//	  HAL_UART_Transmit(&huart2, str1, sizeof(str1), HAL_MAX_DELAY);
  }

  //In case we accidentally exit from task loop
  osThreadTerminate(NULL);

  /* USER CODE END StartMPU6050Task */
}

/* USER CODE BEGIN Header_StartLcdTask */
/**
* @brief Function implementing the LcdTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLcdTask */
void StartLcdTask(void const * argument)
{
  /* USER CODE BEGIN StartLcdTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_UART_Transmit(&huart2, receiveData, sizeof(receiveData), HAL_MAX_DELAY);
	  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	  lcd_put_cur(0, 0);
	  lcd_send_string("CAR:            ");
	  lcd_put_cur(0, 5);
	  lcd_send_string((char*)receiveData);

	  receiveData[0] = '\0';
	  receiveData[1] = '\0';

//	  uint8_t str2[]="LCD\n";
//	  HAL_UART_Transmit(&huart2, str2, sizeof(str2), HAL_MAX_DELAY);

	  osDelay(300);
  }

  //In case we accidentally exit from task loop
  osThreadTerminate(NULL);

  /* USER CODE END StartLcdTask */
}

/* USER CODE BEGIN Header_StartSwitchTask */
/**
* @brief Function implementing the SwitchTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSwitchTask */
void StartSwitchTask(void const * argument)
{
  /* USER CODE BEGIN StartSwitchTask */
  /* Infinite loop */
  for(;;)
  {
	  vTaskDelay(15);
	  if(HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin)){
		  vTaskDelay(2000);
		  if(HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin)){
			  xTaskNotifyGive(TrafficTaskHandle);
		  }
	  }

	  osDelay(50);
	  xEventGroupSetBits(MyEvent01Handle, BIT_Task02_EVENT);

//	  uint8_t str3[]="Switch\n";
//	  HAL_UART_Transmit(&huart2, str3, sizeof(str3), HAL_MAX_DELAY);
  }

  //In case we accidentally exit from task loop
  osThreadTerminate(NULL);

  /* USER CODE END StartSwitchTask */
}

/* USER CODE BEGIN Header_StartTrafficTask */
/**
* @brief Function implementing the TrafficTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTrafficTask */
void StartTrafficTask(void const * argument)
{
  /* USER CODE BEGIN StartTrafficTask */
  /* Infinite loop */
  for(;;)
  {
	  //wait for a Task Notification from Button Task
	  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	  ConditionSelect(&MPU6050);

//	  uint8_t str4[]="Traffic\n";
//	  HAL_UART_Transmit(&huart2, str4, sizeof(str4), HAL_MAX_DELAY);

	  osDelay(300);
  }

  //In case we accidentally exit from task loop
  osThreadTerminate(NULL);

  /* USER CODE END StartTrafficTask */
}

/* USER CODE BEGIN Header_StartRecieveTask */
/**
* @brief Function implementing the ReceiveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRecieveTask */
void StartRecieveTask(void const * argument)
{
  /* USER CODE BEGIN StartRecieveTask */
  /* Infinite loop */
  for(;;)
  {
	  if(receiveData[0]>='0'){
		  lcd_put_cur(0, 0);
		  lcd_send_string("CAR:   ");
		  xTaskNotifyGive(LcdTaskHandle);
	  }
	  HAL_UARTEx_ReceiveToIdle_DMA(&huart4, receiveData, sizeof(receiveData));

      osDelay(10);
  }

  //In case we accidentally exit from task loop
  osThreadTerminate(NULL);
  /* USER CODE END StartRecieveTask */
}

/* USER CODE BEGIN Header_StartIWDG_Monitor */
/**
* @brief Function implementing the IWDG_Monitor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartIWDG_Monitor */
void StartIWDG_Monitor(void const * argument)
{
  /* USER CODE BEGIN StartIWDG_Monitor */
  EventBits_t xEvent;
  const TickType_t xTicksToWait = 6000 / portTICK_PERIOD_MS;

  /* Infinite loop */
  for(;;)
  {
	  xEvent = xEventGroupWaitBits(
			  MyEvent01Handle,
			  BIT_TaskALL_EVENT, //uxBitsToWaitFor
			  pdTRUE,	//xClearOnExit
			  pdTRUE,	//xWaitForAllBits
			  xTicksToWait
	  );
	  if((xEvent&(BIT_TaskALL_EVENT)) == (BIT_TaskALL_EVENT)){
//	  	  uint8_t str5[]="Woof Woof\n";
//	  	  HAL_UART_Transmit(&huart2, str5, sizeof(str5), HAL_MAX_DELAY);
		  HAL_IWDG_Refresh(&hiwdg);
	  }

  }

  //In case we accidentally exit from task loop
  osThreadTerminate(NULL);
  /* USER CODE END StartIWDG_Monitor */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
