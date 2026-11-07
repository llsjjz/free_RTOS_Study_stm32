#ifndef LED_H
#define LED_H

#include "func.h"

typedef struct {
    GPIO_TypeDef *GPIOx; // GPIO 端口
    uint16_t GPIO_Pin;   // GPIO 引脚
} LED_TypeDef;

/******************************************************************************************/
//LED任务相关
#define LED_Priority 2 // LED任务优先级
#define LED_StackSize 128 // LED任务栈大小(单位：字)
extern TaskHandle_t LED_TaskHandle; // LED任务句柄
void vTaskLED(void *pvParameters);

#endif