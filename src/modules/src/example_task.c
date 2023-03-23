#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"

#define DEBUG_MODULE "CUSTOM_TASK"


static xQueueHandle inputQueue;
STATIC_MEM_QUEUE_ALLOC(inputQueue, 1, sizeof(int));

static void exampleTask(void*);
STATIC_MEM_TASK_ALLOC(exampleTask, EXAMPLE_TASK_STACKSIZE);

static bool isInit = false;

void exampleTaskInit() {
  inputQueue = STATIC_MEM_QUEUE_CREATE(inputQueue);
  // TODO
  STATIC_MEM_TASK_CREATE(exampleTask, exampleTask, EXAMPLE_TASK_NAME, NULL, EXAMPLE_TASK_PRI);
  isInit = true;
}


bool exampleTaskTest() {
  return isInit;
}

static void exampleTask(void* parameters){
    DEBUG_PRINT("Example task main function is running!\n");
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