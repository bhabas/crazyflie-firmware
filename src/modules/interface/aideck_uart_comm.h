#pragma once

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"
#include "uart1.h"
#include "debug.h"
#include "config.h"
#include "queue.h"
#include "static_mem.h"
#include "semphr.h"


void AI_UART_Task_Init();
bool AI_UART_Task_Test();

#define NUM_VALUES 10
extern int32_t data_arr[NUM_VALUES];  // Array to store received values
extern bool isArrUpdated;
extern SemaphoreHandle_t xMutex;