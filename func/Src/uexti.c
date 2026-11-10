#include "uexti.h"

void uexti_init(void)
{

}

void User_Key_Handler(void)
{
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); // 切换LED状态

    // 计数器，加 static 保证累加
    static uint8_t key_cnt = 0; 
    key_cnt++;
    
    // 定义一个变量，用于记录是否唤醒了高优先级任务
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; 

    // 传入变量的地址
    xQueueSendFromISR(Q_uqueue, (void *)&key_cnt, &xHigherPriorityTaskWoken); 
    
    // 如果唤醒了高优先级任务，中断结束时立刻进行任务切换
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_3)
    {
        User_Key_Handler(); // 调用你的处理函数
    }
}