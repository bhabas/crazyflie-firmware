#include "aideck_uart_comm.h"


#define DEBUG_MODULE "UART"

SemaphoreHandle_t xMutex;
bool isArrUpdated = false;
int32_t value = 0;
int32_t valArr[NUM_VALUES];  // Array to store received values
int32_t valIndex = 0;  // Counter for number of received values

// BYTE PROCESSING
int32_t byteIndex = 0;
uint8_t receivedBytes[6];  
bool lastByteWasEscape = false;
bool startMarkerReceived = false;

uint8_t delimiter = '\1';
uint8_t escape = '\2';
uint8_t startMarker = '\3';
uint8_t endMarker = '\4';

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

    DEBUG_PRINT("\n\nWaiting for activation ...\n");
    uart1Init(115200);
    xMutex = xSemaphoreCreateMutex();
}

bool AI_UART_Task_Test() {
    return isInit;
}

static void AI_UART_Task(void* parameters) {

    DEBUG_PRINT("\n\nExample task main function is running!\n\n");

    // UDPATING ARRAY
    while(1) {

        uint8_t byte;
        uart1GetBytesWithDefaultTimeout(1, &byte);

        // HANDLE START MARKER
        if (byte == startMarker) {
            startMarkerReceived = true;
            byteIndex = 0;  // Reset byte index
            isArrUpdated = false;
            xSemaphoreTake(xMutex,(TickType_t)100);
            continue;
        }

        if (!startMarkerReceived) {
            continue;  // If start marker not received yet, don't process bytes
        }

        // HANDLE END MARKER
        if (byte == endMarker) {
            startMarkerReceived = false;    // Ready to receive a new array
            valIndex = 0;                   // Reset the array index
            continue;
        }

        // HANDLE ESCAPE SEQUENCE
        if (lastByteWasEscape) {
            receivedBytes[byteIndex++] = byte - '\2';
            lastByteWasEscape = false;
        } 
        else if (byte == escape) {
            lastByteWasEscape = true;
        } 
        else if (byte == delimiter) {
            memcpy(&value, receivedBytes, sizeof(int32_t));
            valArr[valIndex++] = value;  // Store received value
            byteIndex = 0;  

            // Reset counter if we received the expected number of values
            if (valIndex >= NUM_VALUES) {
                valIndex = 0;
                isArrUpdated = true;
                xSemaphoreGive(xMutex);
            }
        } 
        else {
            receivedBytes[byteIndex++] = byte;
        }
        
    }
}

