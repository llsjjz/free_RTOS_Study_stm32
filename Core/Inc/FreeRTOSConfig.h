#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f1xx.h"   // 根据实际 MCU 型号选择


// Tick 计数位宽：V11 新写法，取代旧的 configUSE_16_BIT_TICKS
// 注意：两者不能同时定义，否则 FreeRTOS.h:65 会报错
#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS     //32 位 TickType_t
#define configENABLE_BACKWARD_COMPATIBILITY     1                           //兼容旧版本写法


// 调度器配置
#define configUSE_PREEMPTION                    1   // 1:抢占式调度
#define configUSE_CO_ROUTINES                   0   // 1:启用协程,与强制抢占式调度冲突,所以这里设为0
#define configUSE_TIME_SLICING                  1   // 1:时间片轮转,允许同优先级任务轮流执行,否则同优先级任务会一直占用CPU,导致其他同优先级任务无法执行
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1  //  1:优化任务选择,stm32有计算前置指令,可以使用优化的任务选择算法,提高效率


// 系统时钟配置
#define configCPU_CLOCK_HZ                      (SystemCoreClock)  // CPU 主频
#define configTICK_RATE_HZ                      ((TickType_t)1000) // 系统节拍 1ms


// 中断优先级配置
#define configPRIO_BITS                         4       // STM32F103 是 Cortex-M3，NVIC 只有 4 位优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15      // stm32最低优先级（数值最大）
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5  // 设置允许FreeRTOS控制的最大中断优先级，由此小于5的中断优先级将不能使用FreeRTOS API函数,否则会导致不可预知的错误
// 下面两个是内核真正读取的、已经移位到 NVIC 寄存器高位的形式,NVIC 寄存器的优先级是 8 位的,但是 STM32F103 只用了高 4 位,所以要左移 4 位.
#define configKERNEL_INTERRUPT_PRIORITY \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )


// 任务相关配置
#define configMAX_PRIORITIES                    (8)               // 最大优先级数,最大为32
#define configMINIMAL_STACK_SIZE                ((unsigned short)128)  // 空闲任务栈大小
#define configIDLE_SHOULD_YIELD                 0                   // 空闲任务是否让出 CPU
#define configMAX_TASK_NAME_LEN                 (16)               // 任务名最大长度


// 内存配置
#define configTOTAL_HEAP_SIZE                    ((size_t)(36*1024))// 堆大小
#define configSUPPORT_DYNAMIC_ALLOCATION         1                 // 支持动态内存申请
#define configSUPPORT_STATIC_ALLOCATION          0                // 静态内存申请


//list
#define configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES  0 // 1:启用列表数据完整性检查,用于调试
#define configUSE_MINI_LIST_ITEM                   1 // 1:启用迷你列表项
#define configUSE_COUNTING_SEMAPHORES              1 // 1:启用计数信号量
#define configUSE_MUTEXES                          1 // 1:启用互斥锁
#define configUSE_RECURSIVE_MUTEXES                1 // 1:启用递归互斥锁

//事件组
#define configUSE_EVENT_GROUPS                     1 // 1:启用事件组

#define configUSE_TIMERS                           1
#define configTIMER_TASK_PRIORITY                  3   // 守护任务优先级, 见 §4 第 5 条
#define configTIMER_QUEUE_LENGTH                   10  // 命令队列长度
#define configTIMER_TASK_STACK_DEPTH               256 // 单位: 字(4 B), 即 1 KB
#define INCLUDE_xTimerPendFunctionCall             1 // 1:启用定时器回调函数    

// 钩子函数配置（暂不启用）
#define configUSE_IDLE_HOOK                      0              // 1:启用空闲钩子函数
#define configUSE_TICK_HOOK                      0              // 1:启用节拍钩子函数
#define configUSE_MALLOC_FAILED_HOOK             0              // 1:启用内存申请失败钩子函数
#ifdef DEBUG
    #define configCHECK_FOR_STACK_OVERFLOW           2          // 开启栈溢出检查->vApplicationStackOverflowHook().
#else
    #define configCHECK_FOR_STACK_OVERFLOW           0
#endif


// 可选功能
#define INCLUDE_vTaskDelay                       1
#define INCLUDE_vTaskDelayUntil                  1
#define INCLUDE_vTaskDelete                      1
#define INCLUDE_vTaskSuspend                     1          //1.portMAX_DELAY为无限延时；0.portMAX_DELAY为0xFFFF
#define INCLUDE_xTaskGetSchedulerState           1


/* ===== 调试辅助 ===== */
#ifdef DEBUG
    // 开启任务追踪（允许使用 vTaskList）
    #define configUSE_TRACE_FACILITY                1
    // 开启格式化函数（允许将任务列表格式化为字符串）
    #define configUSE_STATS_FORMATTING_FUNCTIONS    1
    // 开启查询栈历史最小余量（允许使用 uxTaskGetStackHighWaterMark）
    #define INCLUDE_uxTaskGetStackHighWaterMark     1
    #define configASSERT( x )                                           \
        if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ; ; ) ; }
#else
    #define configASSERT( x )   ( ( void ) 0 )
#endif


//PendSV,SVC,SysTick中断处理函数名称重定义,因为FreeRTOS默认使用的是裸机的中断处理函数名称,而STM32CubeMX生成的代码使用的是HAL库的中断处理函数名称,所以需要重定义
#define xPortPendSVHandler PendSV_Handler
#define vPortSVCHandler SVC_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */


