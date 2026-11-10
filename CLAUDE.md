# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

STM32F103ZET6（LQFP144）上的 FreeRTOS 学习工程。FreeRTOS Kernel V11.3.1（LTS 202604.01）以源码形式裁剪后手动集成到 `extern/FreeRTOS/`，不依赖 CubeMX 的 FreeRTOS 中间件。CubeMX 只负责生成 HAL 初始化代码（GPIO、USART1、TIM7 时基）。无测试框架，验证方式是编译 + 上板运行。

## 构建与烧录

工具链：`arm-none-eabi-` 必须在 PATH 中（本机位于 `/home/llsjjz/gnu_toolchain/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin`），生成器为 Ninja。

```bash
cmake --preset Debug          # 首次或改动 CMakeLists 后配置
cmake --build --preset Debug  # 增量构建

# 彻底重建
rm -rf build/Debug && cmake --preset Debug && cmake --build --preset Debug
```

产物：`build/Debug/stm32f103zet6_freeRTOS_Dome.elf` 及同名 `.hex`（post-build 自动生成）。

- **Debug / Release 差异是本工程的核心开关**：`cmake/stm32cubemx/CMakeLists.txt` 用生成器表达式在 Debug 配置下定义 `DEBUG` 宏。该宏控制 `FreeRTOSConfig.h` 中的 `configUSE_TRACE_FACILITY`、`configUSE_STATS_FORMATTING_FUNCTIONS`、`INCLUDE_uxTaskGetStackHighWaterMark`、`configCHECK_FOR_STACK_OVERFLOW` 和 `configASSERT`。未定义 `DEBUG` 时 `vTaskDebug` 会立即 `vTaskDelete(NULL)` 自删。
- 烧录：`openocd -f flash.cfg`（ST-Link，`flash.cfg` 中写死了 Debug 版 elf 的绝对路径）。
- 调试：`.vscode/launch.json` 提供 cortex-debug + OpenOCD 的 launch/attach 配置，构建任务为 `cmake --build --preset Debug`。

## 架构

四层结构，自上而下：

1. **`Core/`** — CubeMX 生成代码（`main.c`、`gpio.c`、`usart.c`、`stm32f1xx_it.c` 等）。只能改 `USER CODE BEGIN/END` 区块内的代码，重新生成 `.ioc` 会覆盖区块外内容。
2. **`Drivers/`** — STM32F1 HAL + CMSIS（CubeMX 管理，不要手改）。
3. **`extern/FreeRTOS/`** — 裁剪后的内核：`portable/GCC/ARM_CM3/port.c` + `heap_4.c`，只编译 Cortex-M3 / GCC 所需文件。其 `CMakeLists.txt` 通过链接 `stm32cubemx` 接口库借用 Core/Inc、CMSIS 头文件和 `STM32F103xE` 宏。
4. **`func/`** — 用户代码，编译为静态库 `func_user`。业务入口全部从这里开始，`main()` 中只有 `func_init()` 和 `func_while()` 两个调用。

### 启动流程

`main()` → `HAL_Init()` → `SystemClock_Config()`（HSE 8MHz ×9 = 72MHz）→ `MX_GPIO_Init()` / `MX_USART1_UART_Init()` → `func_init()`。

`func_init()`（`func/Src/func.c`）创建队列 `Q_uqueue`、用 `xTaskCreate` 创建四个任务、调用 `vTaskStartScheduler()` 且不返回。因此 `main()` 里的 `while(1) { func_while(); }` **永远不会执行到**，`func_while()` 目前是空函数，不要往里写逻辑。

### 必须理解的两个机制

**HAL 时基已改为 TIM7，SysTick 归 FreeRTOS。** `stm32f1xx_hal_timebase_tim.c` + `TIM7_IRQHandler` 负责 `HAL_IncTick()`，`HAL_Delay()` / `HAL_GetTick()` 走 TIM7。同时 `FreeRTOSConfig.h` 末尾把 `xPortPendSVHandler` / `vPortSVCHandler` / `xPortSysTickHandler` 重定义为 `PendSV_Handler` / `SVC_Handler` / `SysTick_Handler`，所以 `stm32f1xx_it.c` 里这三个处理函数是**有意留空**的（CubeMX 生成后手工删除了函数体），不要补回，否则与 port.c 重复定义。

**中断优先级约束：`configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY = 5`。** 任何要调用 `...FromISR` 系列 API 的中断，其抢占优先级数值必须 ≥ 5（即优先级低于 5），否则会触发断言或产生不可预期行为。当前 EXTI3 和 USART1 在 `.ioc` 中都配置为 5。新增中断时牢记这一点，优先级在 `.ioc` 里设置而不是手改生成代码。

### 任务模块模式

每个功能模块 = `func/Inc/<x>.h` 声明 + `func/Src/<x>.c` 实现，头文件里定义该任务的 `*_Priority`、`*_StackSize`、`TaskHandle_t` 和任务函数原型，`func_init()` 集中 `xTaskCreate`。现有任务：

| 任务 | 优先级 | 功能 |
|---|---|---|
| vTaskKEY | 3 | 轮询 PE4（KEY0）的按键扫描，50ms 消抖，演示 portDISABLE/ENABLE_INTERRUPTS |
| vTaskUQUEUE | 3 | 阻塞读 `Q_uqueue`，打印 EXTI 按键计数 |
| vTaskLED | 2 | 每 1s 翻转 PB5（LED0） |
| vTaskDebug | 1 | 每 5s 打印剩余堆、`vTaskList()` 任务表、栈历史最小值（仅 Debug 构建） |

**新增模块时必须在 `func/CMakeLists.txt` 的 `target_sources` 里注册源文件**——`func/Inc` 目录已整体 PUBLIC 导出，头文件不需要额外配置。`uexti` 模块没有自己的任务，只在 `uexti.c` 中定义 EXTI 回调，同样需要在列表中注册。

### 数据流：EXTI 按键 → 队列 → 任务

`PE3`（KEY1）为下降沿 EXTI → `EXTI3_IRQHandler` → `HAL_GPIO_EXTI_IRQHandler` → `HAL_GPIO_EXTI_Callback`（定义在 `func/Src/uexti.c`）→ `User_Key_Handler()`：翻转 LED1（PE5），并把递增的计数经 `xQueueSendFromISR` 送入 `Q_uqueue`，`portYIELD_FROM_ISR` 唤醒同优先级以上的任务。这是本工程学习中断与任务通信的主线示例。

## 其他说明

- `printf` 通过 `debug.c` 中的 `_write()` 重定向到 USART1（115200 8N1），使用阻塞式 `HAL_UART_Transmit`；在中断里调用 `printf` 会长时间阻塞。
- FreeRTOS 堆为 36KB（heap_4），任务栈全部动态分配。