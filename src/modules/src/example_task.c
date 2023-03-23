#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"
#include "uart1.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define DEBUG_MODULE "CUSTOM_TASK"
#define ARRAY_LENGTH 3


uint8_t tempArray[sizeof(float) * ARRAY_LENGTH]; // temperary array of uint8_t to store the received data
float myArray[ARRAY_LENGTH] = {0.0f,0.0f,0.0f}; // array to store the 3 float values

static xQueueHandle inputQueue;
STATIC_MEM_QUEUE_ALLOC(inputQueue, 1, sizeof(int));

static void exampleTask(void*);
STATIC_MEM_TASK_ALLOC(exampleTask, EXAMPLE_TASK_STACKSIZE);

static bool isInit = false;

void exampleTaskInit() {
  inputQueue = STATIC_MEM_QUEUE_CREATE(inputQueue);
  uart1Init(UART1_BAUDRATE);
  // TODO
  STATIC_MEM_TASK_CREATE(exampleTask, exampleTask, EXAMPLE_TASK_NAME, NULL, EXAMPLE_TASK_PRI);
  isInit = true;
}


bool exampleTaskTest() {
  return isInit;
}

static void exampleTask(void* parameters){
    DEBUG_PRINT("--- Example task main function is running! ---\n");
    while (true) {

      uart1GetBytesWithDefaultTimeout(sizeof(tempArray),tempArray);

      for (int i = 0; i < ARRAY_LENGTH; i++) {
      memcpy(&myArray[i], &tempArray[i * sizeof(float)], sizeof(float));
      }
      DEBUG_PRINT("Value1: %f \t Value2: %f \t Value3: %f\n",(double)myArray[0],(double)myArray[1],(double)myArray[2]);
    }
}

void exampleTaskEnqueueInput(int value) {
    xQueueOverwrite(inputQueue, &value);
}