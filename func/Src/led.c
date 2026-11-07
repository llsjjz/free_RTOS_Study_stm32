#include "led.h"

//定义LED对象
LED_TypeDef led0 = {LED0_GPIO_Port, LED0_Pin};

/******************************************************************************************/
//LED任务相关
TaskHandle_t LED_TaskHandle; // LED任务句柄
void vTaskLED(void *pvParameters) {
  ( void ) pvParameters;
  for (;;) {
    HAL_GPIO_TogglePin(led0.GPIOx,led0.GPIO_Pin);
    vTaskDelay(pdMS_TO_TICKS( 1000 )); // 延迟1000个时钟节拍，即1000ms
  }
}