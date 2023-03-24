#pragma once

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"

#include "uart1.h"


extern float Theta_x_cam_est;
extern float Theta_y_cam_est;
extern float Theta_z_cam_est;

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
