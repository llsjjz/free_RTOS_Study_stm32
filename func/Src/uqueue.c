#include "uqueue.h"


TaskHandle_t UQUEUE_TaskHandle; // UQUEUE任务句柄
void vTaskUQUEUE(void *pvParameters)
{
    while(1)
    {
        uint8_t key_pressed;
        if (xQueueReceive(Q_uqueue, &key_pressed, portMAX_DELAY) == pdPASS) // 阻塞等待队列中的数据
        {
            // 处理按键按下事件
            printf("Key pressed count: %d\n", key_pressed); // 打印按键按下计数
        }
    }
    vTaskDelete(NULL); // 删除当前任务，永远不会执行到这里
}