cmake_minimum_required(VERSION 3.12)
project(freertos)

add_library(freertos
    src/croutine.c
    src/event_groups.c 
    src/heap_2.c
    src/list.c 
    src/port.c 
    src/queue.c 
    src/stream_buffer.c 
    src/tasks.c 
    src/timers.c 
)
target_include_directories(freertos PUBLIC
    inc     
)
