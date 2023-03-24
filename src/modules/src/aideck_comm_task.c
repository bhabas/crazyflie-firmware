#include "aideck_comm_task.h"
#include "debug.h"

#define DEBUG_MODULE "AIDECK_COMM_TASK"

// DEFINE AND INITIALIZE ARRAY
static bool isInit = false;
#define ARRAY_LENGTH 3
static uint8_t data_Array[sizeof(float) * ARRAY_LENGTH]; // Byte-String data array
float value_Array[ARRAY_LENGTH]; // Value array to store the received float values

float Theta_x_cam_est = 0.0f;
float Theta_y_cam_est = 0.0f;
float Theta_z_cam_est = 0.0f;

static xQueueHandle taskQueue; // Not entirely sure but is part of task code
STATIC_MEM_QUEUE_ALLOC(taskQueue, 1, sizeof(int));
STATIC_MEM_TASK_ALLOC(AIDeckComm_Task, AIDECK_COMM_STACKSIZE);



void AIDeckComm_TaskInit() {

    // INITIALIZE UART1 CONNECTION
    uart1Init(UART1_BAUDRATE); 

    // INITIALIZE TASK
    taskQueue = STATIC_MEM_QUEUE_CREATE(taskQueue);
    STATIC_MEM_TASK_CREATE(AIDeckComm_Task, AIDeckComm_Task, AIDECK_COMM_TASK_NAME, NULL, AIDECK_COMM_TASK_PRI);

    isInit = true;
}


bool AIDeckComm_TaskTest() {

    return true;
}


void AIDeckComm_Task(void* parameters){
  
    DEBUG_PRINT("--- AIDeckComm_Task is running! ---\n");

    while (true) {

        // RECIEVE BYTE-STRING OVER UART1
        uart1GetBytesWithDefaultTimeout(sizeof(data_Array),data_Array);

        // CONVERT BYTE-STRING TO FLOAT ARRAY
        for (int i = 0; i < ARRAY_LENGTH; i++) {
        memcpy(&value_Array[i], &data_Array[i * sizeof(float)], sizeof(float));
        }

        Theta_x_cam_est = value_Array[0];
        Theta_y_cam_est = value_Array[1];
        Theta_z_cam_est = value_Array[2];

        // PRINT FLOAT ARRAY TO CONSOLE [DEBUG]
        // DEBUG_PRINT("Value1: %f \t Value2: %f \t Value3: %f\n",(double)value_Array[0],(double)value_Array[1],(double)value_Array[2]);
    }
}
