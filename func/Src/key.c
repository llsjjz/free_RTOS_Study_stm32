#include "key.h"
#include "usem.h" 
#include "led.h"

//定义按键对象
KEY_TypeDef key0 = {KEY0_GPIO_Port, KEY0_Pin};

/******************************************************************************************/
//按键任务相关
TaskHandle_t KEY_TaskHandle; // 按键任务句柄
void vTaskKEY(void *pvParameters)
{
    while (1)
    {
        static uint8_t key_pressed = 0; // 按键按下计数器
        if(isKeyPressed(&key0, 0)) // 检测按键是否被按下
        {
            if(key_pressed == 0)
            {
                key_pressed = 1; // 按键按下，计数器加 1
                xEventGroupSetBits(xEventGroup, 0x01); // 设置事件组标志位，通知其他任务按键事件发生
            }
            else
            {
                key_pressed = 0; // 按键释放，计数器清零
                xEventGroupClearBits(xEventGroup, 0x01); // 清除事件组标志位，通知其他任务按键事件结束
            }

            LED0_Toggle(); // 切换LED状态
            xTaskNotifyGive(USEM_TaskHandle); // 通知 USEM 任务按键事件发生
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // 延时 10ms，避免任务占用过多 CPU 时间
    }
    vTaskDelete(NULL); // 删除当前任务，永远不会执行到这里
}

/******************************************************************************************/
//按键检测函数
bool isKeyPressed(KEY_TypeDef *key,uint8_t mode)
{
    static uint32_t key_time;       //记录按键按下的时间
    static bool key_num = false;    //按键按下计数器，false表示按键未按下，true表示按键已按下

    if(mode)key_num = true;// 如果 mode 为 1，则每次调用函数都会返回按键按下事件

    if (HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin) == KEY_OFF)
    {
        key_time = HAL_GetTick(); // 记录按键释放时的时间
        key_num = true; // 按键未按下
    }
    else if (HAL_GPIO_ReadPin(key->GPIOx, key->GPIO_Pin) == KEY_ON && (HAL_GetTick() - key_time) > 20) // 按键按下且持续时间超过20ms
    {
        if(key_num == true) // 按键之前未按下
        {
            key_num = false; // 标记按键已按下
            return true; // 返回按键按下事件
        }
    }
    return false;
}