#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"
#include "uart1.h"
#include "aideck_comm_task.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define DEBUG_MODULE "CUSTOM_TASK"

// DEFINE AND INITIALIZE ARRAYS
#define ARRAY_LENGTH 3
static bool isInit = false;
static uint8_t data_Array[sizeof(float) * ARRAY_LENGTH]; // Byte-String data array
float value_Array[ARRAY_LENGTH]; // Value array to store the received float values

float value_3 = 0.0;

static xQueueHandle taskQueue; // Not entirely sure but is part of task code
STATIC_MEM_QUEUE_ALLOC(taskQueue, 1, sizeof(int));
STATIC_MEM_TASK_ALLOC(AIDeckComm_Task, AIDECK_COMM_STACKSIZE);



void AIDeckComm_TaskInit() {

  uart1Init(UART1_BAUDRATE); // Initialize UART1 connection

  // INITIALIZE TASK
  taskQueue = STATIC_MEM_QUEUE_CREATE(taskQueue);
  STATIC_MEM_TASK_CREATE(AIDeckComm_Task, AIDeckComm_Task, AIDECK_COMM_TASK_NAME, NULL, AIDECK_COMM_TASK_PRI);

  isInit = true;
}


bool AIDeckComm_TaskTest() {
  return isInit;
}


void AIDeckComm_Task(void* parameters){
  
    DEBUG_PRINT("--- AI-DECK COMM task main function is running! ---\n");

    while (true) {

      // RECIEVE BYTE-STRING OVER UART1
      uart1GetBytesWithDefaultTimeout(sizeof(data_Array),data_Array);

      // CONVERT BYTE-STRING TO FLOAT ARRAY
      for (int i = 0; i < ARRAY_LENGTH; i++) {
        memcpy(&value_Array[i], &data_Array[i * sizeof(float)], sizeof(float));
      }

      value_3 = value_Array[0];

      // PRINT FLOAT ARRAY TO CONSOLE [DEBUG]
      DEBUG_PRINT("Value1: %f \t Value2: %f \t Value3: %f\n",(double)value_Array[0],(double)value_Array[1],(double)value_Array[2]);
    }
}
