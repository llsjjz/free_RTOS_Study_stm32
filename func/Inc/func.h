#ifndef FUNC_H
#define FUNC_H


#include "main.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" // 如果需要使用队列等功能

#include <stdio.h> // 使用 printf
#include <string.h>
#include <stdbool.h> // 使用 bool 类型

void func_init(void);
void func_while(void);

#endif