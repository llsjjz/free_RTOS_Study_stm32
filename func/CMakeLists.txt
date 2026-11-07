add_library(func_user STATIC)

target_sources(func_user PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/func.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/led.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/debug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/key.c
)

target_include_directories(func_user
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc 
)

target_link_libraries(func_user PUBLIC stm32cubemx)
target_link_libraries(func_user PUBLIC freertos_kernel)

