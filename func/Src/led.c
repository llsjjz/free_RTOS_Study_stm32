#include "led.h"
#include "func.h"

//定义LED对象
LED_TypeDef led0 = {LED0_GPIO_Port, LED0_Pin};
LED_TypeDef led1 = {LED1_GPIO_Port, LED1_Pin};

/******************************************************************************************/
//LED任务相关
TaskHandle_t LED_TaskHandle; // LED任务句柄
void vTaskLED(void *pvParameters) 
{
  ( void ) pvParameters;
  for (;;) {
    xEventGroupWaitBits(xEventGroup, 0x03, pdFALSE, pdTRUE, portMAX_DELAY); // 等待事件组标志位被设置
    
    LED0_Toggle(); // 切换LED状态  
    vTaskDelay(pdMS_TO_TICKS( 1000 )); // 延迟1000个时钟节拍，即1000ms
  }
  vTaskDelete(NULL); // 删除当前任务，永远不会执行到这里
}


void LED0_Toggle(void)
{
  if( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
  {
    HAL_GPIO_TogglePin(led0.GPIOx, led0.GPIO_Pin); // 切换LED状态
      xSemaphoreGive( xMutex );
  }
}
