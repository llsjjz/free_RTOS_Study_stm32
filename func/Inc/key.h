#ifndef KEY_H
#define KEY_H

#include "func.h"

typedef struct {
    GPIO_TypeDef *GPIOx; // GPIO 端口
    uint16_t GPIO_Pin;   // GPIO 引脚
} KEY_TypeDef;

//定义按键状态常量
const uint8_t KEY_ON = 0;
const uint8_t KEY_OFF = 1;

//按键检测
bool isKeyPressed(KEY_TypeDef *key,uint8_t mode);

/******************************************************************************************/
//按键任务相关
#define KEY_Priority 3 // 按键任务优先级
#define KEY_StackSize 128 // 按键任务栈大小(单位：字)
extern TaskHandle_t KEY_TaskHandle; // 按键任务句柄
void vTaskKEY(void *pvParameters);

#endif
