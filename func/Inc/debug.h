#ifndef DEBUG_H
#define DEBUG_H

#include "func.h"

//debug任务相关
#define DEBUG_Priority 1 // 调试任务优先级
#define DEBUG_StackSize 256 // 调试任务栈大小(单位：字)
void vTaskDebug(void *argument);

#endif