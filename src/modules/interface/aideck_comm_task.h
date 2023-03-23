#pragma once

#include <stdbool.h>


/**
 * Initialization of AI-Deck Communication task
 */
void AIDeckComm_TaskInit();

/**
 * Performs unit test on task to validate success
 *
 * @return true if unit test passes 
 */
bool AIDeckComm_TaskTest();

/**
 * System task that continously waits for data to be received over the UART1
 * connection from the AI-Deck. This task is used to receive output data from
 * the camera sensor.
 *
 * @param[in] void  Parameters that will be input
 * 
 */
void AIDeckComm_Task(void*);
