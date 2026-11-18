#ifndef USEM_H
#define USEM_H

#include "func.h"

/******************************************************************************************/
//USEM任务相关
#define USEM_Priority 3 // USEM任务优先级
#define USEM_StackSize 128 // USEM任务栈大小(单位：字)
extern TaskHandle_t USEM_TaskHandle; // USEM任务句柄
void vTaskUSEM(void *pvParameters);

#endif
