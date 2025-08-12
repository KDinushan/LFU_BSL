/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 * hiiiii new change
 * new change 2
 * new 3
 *
 */

/******************************************************************************
 * NOTE 1:  This project provides a simple blinky style project.
 * This file implements the simply blinky style demo.
 *
 * The blinky demo uses FreeRTOS's tickless idle mode to reduce power
 * consumption.
 *
 * NOTE 2:  This file only contains the source code that is specific to the
 * basic demo. Generic functions, such FreeRTOS hook functions, and functions
 * required to configure the hardware, are defined in main.c.
 ******************************************************************************
 *
 * main_blinky() creates one queue, and two tasks.  It then starts the
 * scheduler.
 *
 * The Queue Send Task:
 * The queue send task is implemented by the prvQueueSendTask() function in
 * this file.  prvQueueSendTask() sits in a loop that causes it to repeatedly
 * block for 1 second, before sending the value 100 to the queue that
 * was created within main_blinky().  Once the value is sent, the task loops
 * back around to block for another 1 second.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() sits in a loop where it repeatedly
 * blocks on attempts to read data from the queue that was created within
 * main_blinky().  When data is received, the task checks the value of the
 * data, and if the value equals the expected 100, toggles the LED.  The 'block
 * time' parameter passed to the queue receive function specifies that the
 * task should be held in the Blocked state indefinitely to wait for data to
 * be available on the queue.  The queue receive task will only leave the
 * Blocked state when the queue send task writes to the queue.  As the queue
 * send task writes to the queue every 1 second, the queue receive
 * task leaves the Blocked state every 1 second, and therefore toggles
 * the LED every 1 second.
 */

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "uart.h"
#include "BSL_CI.h"

/* TI includes. */
#include "ti_msp_dl_config.h"


/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define mainQUEUE_SEND_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

#define config_test_STACK_SIZE        ( ( unsigned short ) 64 )
/*
 * The rate at which data is sent to the queue.  The 1s (1000ms) value is
 * converted to ticks using the pdMS_TO_TICKS constant.
 */
#define mainQUEUE_SEND_FREQUENCY_MS (pdMS_TO_TICKS(1000UL))

/*
 * The number of items the queue can hold.  This is 1 as the receive task
 * will remove items as they are added, meaning the send task should always
 * find the queue empty.
 */
#define mainQUEUE_LENGTH (1)

/*
 * Values passed to the two tasks just to check the task parameter
 * functionality.
 */
#define mainQUEUE_SEND_PARAMETER (0x1111UL)
#define mainQUEUE_RECEIVE_PARAMETER (0x22UL)

/*-----------------------------------------------------------*/

/* The tasks as described in the comments at the top of this file. */
static void prvRTOS_LED0_Toggle_Task(void *pvParameters);
 void prvRTOS_LED0_Toggle_Task2(void *pvParameters);

static void prvBootloader_Task(void *pvParameters);


/* Called by main() to create the simply blinky style application */
void main_tasks_create(void);
static uint8_t TI_MSPBoot_AppMgr_AppisValid(void);
void CMD_API_startApplication(void);

//extern void BSL_PI_sendByte(uint8_t data);
/*-----------------------------------------------------------*/


/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;
TaskHandle_t RTOS_LED_Toggle_Handler;
TaskHandle_t Bootloader_Handler;
TaskHandle_t App_Handler;
/*-----------------------------------------------------------*/
uint8_t app_area_flag;
void main_tasks_create(void)
{
    /* Create the queue. */
    xQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(uint32_t));

    if (xQueue != NULL) {
        /*
         * Start the two tasks as described in the comments at the top of this
         * file.
         */
        xTaskCreate(
                prvBootloader_Task, /* The function that implements the task. */
                "BSL", /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                configMINIMAL_STACK_SIZE, /* The size of the stack to allocate to the task. */
                NULL, /* The parameter passed to the task - just to check the functionality. */
                mainQUEUE_RECEIVE_TASK_PRIORITY, /* The priority assigned to the task. */
                &Bootloader_Handler); /* The task handle is not required, so NULL is passed. */

        xTaskCreate(
                prvRTOS_LED0_Toggle_Task,
                "LED",
                configMINIMAL_STACK_SIZE,
                (void *) mainQUEUE_SEND_PARAMETER,
                mainQUEUE_SEND_TASK_PRIORITY,
                &RTOS_LED_Toggle_Handler);
//        BSL_PI_sendByte(0xBB);
        app_area_flag = TI_MSPBoot_AppMgr_AppisValid();
//        app_area_flag = 1;
        BSL_PI_sendByte(app_area_flag);
        switch (app_area_flag)
        {
            case 1:
                xTaskCreate(
                        (TaskFunction_t)(*(uint32_t *)(APP_AREA_1_START_ADDR + 4)),
//                        prvRTOS_LED0_Toggle_Task2,
                        "App",
                        config_test_STACK_SIZE,
                        NULL,
                        mainQUEUE_SEND_TASK_PRIORITY,
                        &App_Handler);
                vTaskSuspend(Bootloader_Handler);
 //               SCB->VTOR = APP_AREA_1_START_ADDR;
//                BSL_PI_sendByte(0x11);
                break;
            case 2:
                xTaskCreate(
                        (TaskFunction_t)(*(uint32_t *)(APP_AREA_2_START_ADDR + 4)),
                        "App",
                        config_test_STACK_SIZE,
                        NULL,
                        mainQUEUE_SEND_TASK_PRIORITY,
                        &App_Handler);
                vTaskSuspend(Bootloader_Handler);
//                SCB->VTOR = APP_AREA_2_START_ADDR;
                break;
            default:
                break;
        }
 //       BSL_PI_sendByte(0x22);
        /* Start the tasks. */
        vTaskStartScheduler();
    }

    /*
     * If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle
     * and/or timer tasks to be created.  See the memory management section on
     * the FreeRTOS web site for more details.
     */
    for (;;)
        ;
}
/*-----------------------------------------------------------*/

static void prvRTOS_LED0_Toggle_Task(void *pvParameters)
{
    for (;;) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        vTaskDelay(500);
    }
}

//void Delay_for_tasks_switch(uint32_t delay_ms)
//{
//    ((void (* )(uint32_t xTicksToDelay))0x131)(delay_ms);
//}

//__attribute__((location(APP_AREA_1_START_ADDR + 0x131)))  void prvRTOS_LED0_Toggle_Task2(void *pvParameters)
//{
//    for (;;) {
//        DL_GPIO_togglePins(GPIO_LEDS_PORT2, GPIO_LEDS_USER_LED_2_PIN);
//        Delay_for_tasks_switch(1000);//must be call for tasks switch.
//    }
//}
/*-----------------------------------------------------------*/
extern volatile BSL_RX_states BSL_RX_state;
unsigned char returen_flag;
extern uint8_t BSL_LockedStatus;

static void prvBootloader_Task(void *pvParameters)
{
    TickType_t xNextWakeTime;
    unsigned long ulReceivedValue, ret;
    BSL_RX_state = RX_idle;
    BSL_LockedStatus = BSL_STATUS_LOCKED;
    for (;;) {
        /*
         * Wait until something arrives in the queue - this task will block
         * indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
         * FreeRTOSConfig.h.
         */
        xQueueReceive(xQueue, &ulReceivedValue, portMAX_DELAY);

        returen_flag=0;
//        BSL_RX_state = RX_idle;

        returen_flag = BSL_PI_UART_receive();
        if(returen_flag == 1)
        {
            BSL_CI_interpretCommand();
        }
    }
}

extern uint8_t BSL_receive_buf[256];
extern volatile uint32_t BSL_PI_dataPointer;
extern volatile uint16_t BSL_PI_RxDataCnt;
void UART_0_INST_IRQHandler(void)
{
    uint8_t receivedData;
    uint8_t send_buff;
    BaseType_t YieldRequired;
    const unsigned long ulValueToSend = 100UL;

    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            receivedData = DL_UART_Main_receiveData(UART_0_INST);

            /*
             * If a byte is received when RX is in IDLE state, it is treated as
             * a new command packet and buffer parameters are initialized
             * to receive the packet.
             */
            if( (eTaskGetState(Bootloader_Handler)==eRunning) || (eTaskGetState(Bootloader_Handler)==eReady)\
                    ||(eTaskGetState(Bootloader_Handler)==eBlocked))
            {
                if (BSL_RX_state == RX_idle) {
                    BSL_PI_RxDataCnt   = (uint16_t) 1;
                    BSL_PI_dataPointer = (uint32_t) 0;
                    BSL_receive_buf[0]    = receivedData;
                    BSL_RX_state       = RX_receiving;
                }
                /*
                 * Whenever a new byte is received, after a packet reception is
                 * started, add it to the data buffer
                 */
                else if (BSL_RX_state == RX_receiving) {
                    if (BSL_PI_RxDataCnt < (BSL_maxBufferSize + (uint16_t) 4)) {
                        BSL_receive_buf[BSL_PI_RxDataCnt] = receivedData;
                        BSL_PI_RxDataCnt = BSL_PI_RxDataCnt + (uint16_t) 1;
                    }
                }
                /* When RX state machine is in any other state (BLOCKED),
                 * packet is dropped */
                else {
                }
            }else
            {
                if(receivedData==0x55)
                {
                    BSL_PI_sendByte(0x51);
                    BSL_PI_sendByte(app_area_flag);

                    send_buff = (uint8_t)(APP_AREA_1_START_ADDR>>16);
                    BSL_PI_sendByte(send_buff);
                    send_buff = (uint8_t)(APP_AREA_1_START_ADDR>>8);
                    BSL_PI_sendByte(send_buff);
                    send_buff = (uint8_t)APP_AREA_1_START_ADDR;
                    BSL_PI_sendByte(send_buff);

                    send_buff = (uint8_t)(APP_AREA_2_START_ADDR>>16);
                    BSL_PI_sendByte(send_buff);
                    send_buff = (uint8_t)(APP_AREA_2_START_ADDR>>8);
                    BSL_PI_sendByte(send_buff);
                    send_buff = (uint8_t)APP_AREA_2_START_ADDR;
                    BSL_PI_sendByte(send_buff);
                }
                if(receivedData==0xAA)
                {
                    YieldRequired = xTaskResumeFromISR(Bootloader_Handler);
                    BSL_PI_sendByte(0xBB);
                }
                if(receivedData==0x11)
                {
                    BSL_PI_sendByte(0x22);
                }

            }

            break;

        default:
            break;
    }
    xQueueSendFromISR(xQueue, &ulValueToSend, 0U);
    if(YieldRequired == pdTRUE)
    {
        portYIELD_FROM_ISR(YieldRequired);
    }
}

static uint8_t TI_MSPBoot_AppMgr_AppisValid(void)
{
    // Check if Application Reset vector exists
    if ((*(uint32_t *)(APP_AREA_1_START_ADDR) != 0xFFFFFFFF) &&(*(uint32_t *)(APP_AREA_1_START_ADDR+4)!= 0xFFFFFFFF))
    {
        return 1;
    }
    else
    {
        if ((*(uint32_t *)(APP_AREA_2_START_ADDR) != 0xFFFFFFFF) &&(*(uint32_t *)(APP_AREA_2_START_ADDR+4)!= 0xFFFFFFFF))
        {
        return 2;
        }

        else
            return 0;
    }
}

void CMD_API_startApplication(void)
{
    uint8_t erase_status = (uint8_t)1;
    switch (app_area_flag)
    {
        case 1:
            DL_FlashCTL_unprotectSector(FLASHCTL, APP_AREA_1_START_ADDR, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_eraseMemory(FLASHCTL, APP_AREA_1_START_ADDR, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
            erase_status = erase_status & (uint8_t)DL_FlashCTL_waitForCmdDone(FLASHCTL);
            break;
        case 2:
            DL_FlashCTL_unprotectSector(FLASHCTL, APP_AREA_2_START_ADDR, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_eraseMemory(FLASHCTL, APP_AREA_2_START_ADDR, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
            erase_status = erase_status & (uint8_t)DL_FlashCTL_waitForCmdDone(FLASHCTL);
            break;

        default:
            break;
    }
    if(erase_status == (uint8_t)0)
    {
        BSL_PI_sendByte(0x12);
    }
    else
        BSL_PI_sendByte(0x13);
    DL_SYSCTL_resetDevice(DL_SYSCTL_RESET_POR);
}




/*-----------------------------------------------------------*/
