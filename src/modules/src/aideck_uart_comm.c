#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"
#include "aideck_uart_comm.h"

#define DEBUG_MODULE "UART"

static xQueueHandle inputQueue;
STATIC_MEM_QUEUE_ALLOC(inputQueue, 1, sizeof(int));

static void AI_UART_Task(void*);
STATIC_MEM_TASK_ALLOC(AI_UART_Task, AI_UART_TASK_STACKSIZE);

static bool isInit = false;

void AI_UART_Task_Init() {
  inputQueue = STATIC_MEM_QUEUE_CREATE(inputQueue);
  // TODO
  STATIC_MEM_TASK_CREATE(AI_UART_Task, AI_UART_Task, AI_UART_TASK_NAME, NULL, AI_UART_TASK_PRI);
  isInit = true;
}

bool AI_UART_Task_Test() {
  return isInit;
}

static void AI_UART_Task(void* parameters) {
  DEBUG_PRINT("\n\nExample task main function is running!\n\n");
  while (true) {
    int input;
    if (pdTRUE == xQueueReceive(inputQueue, &input, portMAX_DELAY)) {
      // Respond to input here!
    }
  }
}

void exampleTaskEnqueueInput(int value) {
  xQueueOverwrite(inputQueue, &value);
}