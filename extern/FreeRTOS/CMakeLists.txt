  # FreeRTOS Kernel V11.3.1 (LTS 202604.01) — 仅 Cortex-M3 / GCC
  # 拷贝自 FreeRTOSv202604.01-LTS，按最小集裁剪

  set(FREERTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

  add_library(freertos_kernel STATIC)

  target_sources(freertos_kernel PRIVATE
      ${FREERTOS_DIR}/croutine.c
      ${FREERTOS_DIR}/event_groups.c
      ${FREERTOS_DIR}/list.c
      ${FREERTOS_DIR}/queue.c
      ${FREERTOS_DIR}/stream_buffer.c
      ${FREERTOS_DIR}/tasks.c
      ${FREERTOS_DIR}/timers.c
      ${FREERTOS_DIR}/portable/GCC/ARM_CM3/port.c
      ${FREERTOS_DIR}/portable/MemMang/heap_4.c
  )

  target_include_directories(freertos_kernel
        PUBLIC
            ${FREERTOS_DIR}/include
            ${FREERTOS_DIR}/portable/GCC/ARM_CM3
    )
  # 借用 stm32cubemx 接口库：它已带上 Core/Inc、CMSIS 器件头文件和 STM32F103xE 宏
  target_link_libraries(freertos_kernel PUBLIC stm32cubemx)

  # 内核源码里用到了 __attribute__((weak)) 等，GCC 需要 C11 或以上
  target_compile_features(freertos_kernel PUBLIC c_std_11)

  # 与官方 CMake 保持一致
  target_compile_definitions(freertos_kernel PRIVATE projCOVERAGE_TEST=0)

  # 内核代码对严格别名敏感（list.c 里大量指针强转），官方移植都带这个选项
  target_compile_options(freertos_kernel PRIVATE -fno-strict-aliasing)