#include "func.h"

#include "led.h"
#include "debug.h"
#include "key.h"

void func_init(void)
{
  // 创建任务
  xTaskCreate(vTaskLED, "LED", LED_StackSize, NULL, LED_Priority, &LED_TaskHandle); // 优先级设为 2，较高
  xTaskCreate(vTaskDebug, "Debug", DEBUG_StackSize, NULL, DEBUG_Priority, NULL); // 优先级设为 1，较低即可
  xTaskCreate(vTaskKEY, "KEY", KEY_StackSize, NULL, KEY_Priority, &KEY_TaskHandle); // 优先级设为 3，较高

  // 启动FreeRTOS调度器，永远不会返回
  vTaskStartScheduler();
}

void func_while(void)
{
 
}