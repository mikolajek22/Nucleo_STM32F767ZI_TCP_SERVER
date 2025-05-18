#ifndef __TCP_APP_H
#define __TCP_APP_H

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Global Threads Ids */
extern osThreadId_t TcpTaskHandle;

/* Global Threads Attributes */
extern const osThreadAttr_t TcpTask_attributes;

/* Task Prototypes */
void StartTcpTask(void *argument);
#endif