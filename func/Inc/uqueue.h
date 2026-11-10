#ifndef UQUEUE_H
#define UQUEUE_H

#include "func.h"

/******************************************************************************************/
//UQUEUE任务相关
#define UQUEUE_Priority 3 // UQUEUE任务优先级
#define UQUEUE_StackSize 128 // UQUEUE任务栈大小(单位：字)
extern TaskHandle_t UQUEUE_TaskHandle; // UQUEUE任务句柄
void vTaskUQUEUE(void *pvParameters);

#endif
