#include "debug.h"

// 调试任务函数
void vTaskDebug(void *argument)
{
    // vTaskList 需要传入一个足够大的字符数组来存储表格数据
    // 任务多的话，这个数组要相应加大，512字节大约能存 6-8 个任务
    static char task_list_buffer[512]; 
    size_t free_heap;
    UBaseType_t stack_left;

    while (1)
    {
        printf("\r\n================ FreeRTOS DEBUG ================\r\n");

        // 1. 查看剩余堆内存
        free_heap = xPortGetFreeHeapSize();
        printf("[Heap] Free Heap Size: %u Bytes\r\n", (unsigned int)free_heap);

        // 2. 打印任务表
        memset(task_list_buffer, 0, sizeof(task_list_buffer));
        vTaskList(task_list_buffer); // 获取任务状态
        printf("[Task List]\r\n");
        printf("Name          State           Prio           Stack           Num\r\n");
        printf("%s", task_list_buffer);

        // 3. 查看当前任务的栈余量
        // 传入 NULL 表示查询当前正在运行的任务（即 Debug_Task 自己）
        // 注意：返回值的单位是 "字(Word)"。Cortex-M3 是 32 位，所以 1 Word = 4 Bytes。
        stack_left = uxTaskGetStackHighWaterMark(NULL);
        printf("[Stack] Debug_Task High Water Mark: %u Words (%u Bytes)\r\n", 
               (unsigned int)stack_left, (unsigned int)(stack_left * 4));

        printf("===================================================\r\n\r\n");

        // 每 5 秒打印一次
        vTaskDelay(pdMS_TO_TICKS(5000)); 
    }
}

// 重定向 printf 到 UART1
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}
