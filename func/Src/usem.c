#include "usem.h"


TaskHandle_t USEM_TaskHandle; // USEM任务句柄
void vTaskUSEM(void *pvParameters)
{
    while(1)
    {
        if (ulTaskNotifyTake( pdFALSE, portMAX_DELAY ) > 0) // 阻塞等待信号量
        {
            // 处理按键按下事件
            printf("Key is pressed\n"); // 打印按键按下计数
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // 延时 10ms，避免任务占用过多 CPU 时间
    }
    vTaskDelete(NULL); // 删除当前任务，永远不会执行到这里
}