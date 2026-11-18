#include "func.h"

#include "led.h"
#include "debug.h"
#include "key.h"
#include "uqueue.h"
#include "uexti.h"
#include "usem.h"

//******************************************************************************************/
QueueHandle_t Q_uqueue;
SemaphoreHandle_t xSem;
SemaphoreHandle_t xMutex;
EventGroupHandle_t xEventGroup; // 事件组句柄
//******************************************************************************************/

void func_init(void)
{
  Q_uqueue = xQueueCreate(5, sizeof(uint8_t)); // 创建队列，长度为 5，每个元素大小为 uint8_t
  xSem = xSemaphoreCreateBinary(); // 创建二值信号量
  xMutex = xSemaphoreCreateMutex(); // 创建互斥锁
  xEventGroup = xEventGroupCreate(); // 创建事件组

  // 初始化按键、LED、调试和队列
  uexti_init(); // 初始化外部中断

  // 创建任务
  xTaskCreate(vTaskLED, "LED", LED_StackSize, NULL, LED_Priority, &LED_TaskHandle); // 优先级设为 2，较高
  xTaskCreate(vTaskDebug, "Debug", DEBUG_StackSize, NULL, DEBUG_Priority, NULL); // 优先级设为 1，较低即可
  xTaskCreate(vTaskKEY, "KEY", KEY_StackSize, NULL, KEY_Priority, &KEY_TaskHandle); // 优先级设为 2，较高
  xTaskCreate(vTaskUQUEUE, "UQUEUE", UQUEUE_StackSize, NULL, UQUEUE_Priority, &UQUEUE_TaskHandle); // 优先级设为 3，较高
  xTaskCreate(vTaskUSEM, "USEM", USEM_StackSize, NULL, USEM_Priority, &USEM_TaskHandle); // 优先级设为 3，较高
  // 启动FreeRTOS调度器，永远不会返回
  vTaskStartScheduler();
}

void func_while(void)
{
  
}