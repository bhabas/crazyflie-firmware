#include "aideck_uart_comm.h"


#define DEBUG_MODULE "UART"

SemaphoreHandle_t xMutex;

// DEFINE AND INIT STATE MACHINE
#define STATE_WAIT_START 0
#define STATE_RECEIVE_DATA 1
#define STATE_WAIT_END 2
int state = STATE_WAIT_START;

// DATA CAP MARKERS
uint8_t START_MARKER[] = {0xAA, 0xBB, 0xCC, 0xCD};
uint8_t END_MARKER[] = {0xFF, 0xEE, 0xDD, 0xDC};


// INIT DATA ARRAY
int32_t data_arr[UART_ARR_SIZE];
int data_counter = 0;
uint8_t buffer[4];
bool isArrUpdated = false;


// RTOS TASK STUFF
static xQueueHandle inputQueue;
STATIC_MEM_QUEUE_ALLOC(inputQueue, 1, sizeof(int));

static void AI_UART_Task(void*);
STATIC_MEM_TASK_ALLOC(AI_UART_Task, AI_UART_TASK_STACKSIZE);

static bool isInit = false;

void AI_UART_Task_Init() {
    inputQueue = STATIC_MEM_QUEUE_CREATE(inputQueue);
    STATIC_MEM_TASK_CREATE(AI_UART_Task, AI_UART_Task, AI_UART_TASK_NAME, NULL, AI_UART_TASK_PRI);
    isInit = true;

    DEBUG_PRINT("\n\n[UART TASK] Waiting for activation ...\n");
    uart1Init(115200);
    xMutex = xSemaphoreCreateMutex();
}

bool AI_UART_Task_Test() {
    return isInit;
}

// ASSUME 4-BYTE CHUNK
int32_t bytes_to_int32(uint8_t* bytes) {
    int32_t result = 0;
    for (int i = 0; i < 4; i++) {
        result |= ((int32_t) bytes[i]) << (i*8);
    }
    return result;
}

static void AI_UART_Task(void* parameters) {

    DEBUG_PRINT("\n\nExample task main function is running!\n\n");

    

    // UDPATING ARRAY
    while(1) {

        // COLLECT NEW 4-BYTE BUFFER
        uart1GetBytesWithDefaultTimeout(4,buffer);

        // DEBUG
        // for (int i = 0; i < 4; i++)
        // {
        //     consolePrintf("%02X ",buffer[i]);
        // }
        // consolePrintf("\n");

        switch (state)
        {
            case STATE_WAIT_START:

                // CHECK IF BUFFER MATCHES START SEQUENCE
                if (memcmp(buffer, START_MARKER, sizeof(START_MARKER)) == 0) {

                    // TAKE MUTEXT
                    xSemaphoreTake(xMutex,(TickType_t)100);

                    // TRANSISTION TO NEXT STATE
                    state = STATE_RECEIVE_DATA;
                    data_counter = 0;

                    // DEBUG
                    // consolePrintf("1\n");
                }

                break;

            case STATE_RECEIVE_DATA:

                // CONVERT BUFFER TO INT32_T DATA
                data_arr[data_counter] = bytes_to_int32(buffer);
                data_counter++;

                // DEBUG
                // consolePrintf("Val: %d \t %ld\n",data_counter,bytes_to_int32(buffer));
         
                // TRANSITION TO NEXT STATE
                if(data_counter == UART_ARR_SIZE)
                {
                    state = STATE_WAIT_END;

                    // DEBUG
                    // consolePrintf("2\n");
                }
                break;

            case STATE_WAIT_END:

                // CHECK IF BUFFER MATCHES END SEQUENCE
                if (memcmp(buffer, END_MARKER, sizeof(END_MARKER)) == 0) {

                    // RESET STATE TO WAIT FOR NEXT TRANSMISSION
                    state = STATE_WAIT_START;
                    isArrUpdated = true;

                    // GIVE MUTEX 
                    xSemaphoreGive(xMutex);

                    // DEBUG
                    // Print the received data
                    // for (int i = 0; i < UART_ARR_SIZE; i++) {
                    //     consolePrintf("%ld ", data_arr[i]);
                    // }
                    // consolePrintf("\n");
                    // consolePrintf("\n3\n");
                }

                break;

        }

        
        
    }
}

