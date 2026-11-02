#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f1xx.h"   // 根据实际 MCU 型号选择

// Tick 计数位宽：V11 新写法，取代旧的 configUSE_16_BIT_TICKS
// 注意：两者不能同时定义，否则 FreeRTOS.h:65 会报错
#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS

// 调度器配置
#define configUSE_PREEMPTION                    1   // 1:抢占式调度
#define configUSE_TIME_SLICING                  1   // 1:时间片轮转
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1  // 1:优化任务选择

// 系统时钟配置
#define configCPU_CLOCK_HZ                      (SystemCoreClock)  // CPU 主频
#define configTICK_RATE_HZ                      ((TickType_t)1000) // 系统节拍 1ms

// STM32F103 是 Cortex-M3，NVIC 只有 4 位优先级
#define configPRIO_BITS                         4

// 最低优先级（数值最大）
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15

// 允许调用 FreeRTOS FromISR 类 API 的最高优先级（数值最小）
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

// 下面两个是内核真正读取的、已经移位到 NVIC 寄存器高位的形式
#define configKERNEL_INTERRUPT_PRIORITY \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

// 任务相关配置
#define configMAX_PRIORITIES                    (32)               // 最大优先级数
#define configMINIMAL_STACK_SIZE                ((unsigned short)128)  // 空闲任务栈大小
#define configMAX_TASK_NAME_LEN                 (16)               // 任务名最大长度

// 内存配置
#define configTOTAL_HEAP_SIZE                    ((size_t)(36*1024))// 堆大小
#define configSUPPORT_DYNAMIC_ALLOCATION         1                 // 支持动态内存申请
#define configSUPPORT_STATIC_ALLOCATION          0                // 静态内存申请

// 钩子函数配置（暂不启用）
#define configUSE_IDLE_HOOK                      0
#define configUSE_TICK_HOOK                      0
#define configUSE_MALLOC_FAILED_HOOK             0
#define configCHECK_FOR_STACK_OVERFLOW           0

// 可选功能
#define INCLUDE_vTaskDelay                       1
#define INCLUDE_vTaskDelayUntil                  1
#define INCLUDE_vTaskDelete                      1
#define INCLUDE_vTaskSuspend                     1

#ifdef DEBUG
    #define configASSERT( x )                                           \
        if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ; ; ) ; }
#else
    #define configASSERT( x )   ( ( void ) 0 )
#endif

#define xPortPendSVHandler PendSV_Handler
#define vPortSVCHandler SVC_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */


