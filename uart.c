#include "uart.h"
#include "portmacro.h"



/* BSL PI Return codes */
#define BSL_PI_FAIL                     ((uint8_t)0x0)
#define BSL_PI_SUCCESS                  ((uint8_t)0x1)


unsigned char data;

volatile BSL_RX_states BSL_RX_state;
volatile uint32_t BSL_PI_dataPointer;
volatile uint32_t BSL_PI_checksum;
volatile uint16_t BSL_PI_RxDataCnt;
uint8_t BSL_receive_buf[200];
uint8_t BSL_send_buf[40];
uint16_t BSL_RXBufferSize;


uint32_t jj;
extern uint8_t app_area_flag;
uint8_t BSL_PI_UART_receive(void)
{
    /*
     * If the last received command packet is processed, and BSL core is ready
     * to accept a new data packet, the state will move from BLOCKED to IDLE.
     */
    if (BSL_RX_state == RX_blocked) {
        BSL_RX_state = RX_idle;
    }
    /*
     * Once the packet is started to arrive, the RX state will be changed to
     * RECEIVING and the same state is retained until complete packet is
     * received.
     */
    while (BSL_RX_state == RX_receiving) {
        if (BSL_PI_dataPointer == (uint32_t) 0) {
            if (BSL_PI_RxDataCnt > (uint16_t) 0) {
                /* Check if first byte in the packet matches the Header byte */
                if (BSL_receive_buf[0] != BSL_PACKET_HEADER_BYTE) {
                    BSL_PI_sendByte(BSL_ERROR_HEADER_INCORRECT);
                    if(BSL_receive_buf[0] == 0x55)
                        BSL_PI_sendByte(app_area_flag);
                    BSL_RX_state = RX_idle;
                } else {
                    BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
                }
            }
        }
        /* Next two bytes are buffer length */
        else if (BSL_PI_dataPointer == (uint32_t) 1) {
            if (BSL_PI_RxDataCnt > (uint16_t) 1) {
                BSL_RXBufferSize   = (uint16_t) BSL_receive_buf[1];
                BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
            }
        } else if (BSL_PI_dataPointer == (uint32_t) 2) {
            if (BSL_PI_RxDataCnt > (uint16_t) 2) {
                uint16_t temp = (uint16_t) BSL_receive_buf[2] << (uint16_t) 8;
                BSL_RXBufferSize |= temp;

                /*
                 * Check if the packet size received is valid
                 * else return the error
                 */
                if (BSL_RXBufferSize == (uint16_t) 0) {
                    BSL_PI_sendByte(BSL_ERROR_PACKET_SIZE_ZERO);
                    BSL_RX_state = RX_idle;
                }
                if ((BSL_RXBufferSize + BSL_PI_WRAPPER_SIZE) >
                    BSL_maxBufferSize) {
                    BSL_PI_sendByte(BSL_ERROR_PACKET_SIZE_TOO_BIG);
                    BSL_RX_state = RX_idle;
                }
                BSL_PI_dataPointer =
                    (uint32_t) BSL_RXBufferSize + (uint32_t) 3;
            }
        }
        /* Skip over the buffer data and go to the 32-bit checksum at the end */
        /* Start with the low checksum byte */
        else if (BSL_PI_dataPointer ==
                 ((uint32_t) BSL_RXBufferSize + (uint32_t) 3)) {
            if (BSL_PI_RxDataCnt > (BSL_RXBufferSize + (uint16_t) 3)) {
                BSL_PI_checksum =
                    (uint32_t) BSL_receive_buf[BSL_RXBufferSize + (uint16_t) 3];
                BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
            }
        } else if (BSL_PI_dataPointer ==
                   ((uint32_t) BSL_RXBufferSize + (uint32_t) 4)) {
            if (BSL_PI_RxDataCnt > (BSL_RXBufferSize + (uint16_t) 4)) {
                BSL_PI_checksum =
                    BSL_PI_checksum |
                    (BSL_receive_buf[BSL_RXBufferSize + (uint16_t) 4] << 8);
                BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
            }
        } else if (BSL_PI_dataPointer ==
                   ((uint32_t) BSL_RXBufferSize + (uint32_t) 5)) {
            if (BSL_PI_RxDataCnt > (BSL_RXBufferSize + (uint16_t) 5)) {
                BSL_PI_checksum =
                    BSL_PI_checksum |
                    (BSL_receive_buf[BSL_RXBufferSize + (uint16_t) 5] << 16);
                BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;
            }
        } else if (BSL_PI_dataPointer ==
                   ((uint32_t) BSL_RXBufferSize + (uint32_t) 6)) {
            if (BSL_PI_RxDataCnt > (BSL_RXBufferSize + (uint16_t) 6)) {
                BSL_PI_checksum =
                    BSL_PI_checksum |
                    (BSL_receive_buf[BSL_RXBufferSize + (uint16_t) 6] << 24);
                BSL_PI_dataPointer = BSL_PI_dataPointer + (uint32_t) 1;

                BSL_RX_state = RX_blocked;

                uint8_t* BSL_core_data_start = &BSL_receive_buf[3];
                jj=BSL_calculateCRC(BSL_core_data_start, BSL_RXBufferSize);
                /* Verify checksum */
                if (BSL_calculateCRC(BSL_core_data_start, BSL_RXBufferSize) ==
                    BSL_PI_checksum) {
                          /*
                     * Return the packet address to the BSL core for processing
                     */
                    BSL_PI_sendByte(BSL_ACK);
                    return 1;
                } else /* Checksum failed */
                {
 //                   __BKPT(0);
                    BSL_PI_sendByte(BSL_ERROR_CHECKSUM_INCORRECT);
                }
                BSL_RX_state = RX_idle;

            }
        } else {
            /* Do nothing */
        }

    }
    return 0;
}

void BSL_PI_sendByte(uint8_t data)
{
    DL_UART_Main_clearInterruptStatus(
            UART_0_INST, DL_UART_MAIN_INTERRUPT_EOT_DONE);

    /* Transmit the data and wait until it is transmitted completely */
    DL_UART_Main_transmitData(UART_0_INST, data);
    while (DL_UART_Main_getRawInterruptStatus(
            UART_0_INST, DL_UART_MAIN_INTERRUPT_EOT_DONE) == false) {
    }
}

uint32_t BSL_calculateCRC(uint8_t* data, uint16_t dataSize)
{
    uint16_t bufferIndex;

    /*
     * @note: CRC module would have been configured by the ROM BSL with
     * required specification.
     */

    /* Set the Seed value to reset the calculation */
    DL_CRC_setSeed32(BSL_CRC, BSL_CRC_SEED);

    /* Feed the data to CRC module */
    for (bufferIndex = (uint16_t) 0; bufferIndex < dataSize; bufferIndex++) {
        DL_CRC_feedData8(BSL_CRC, data[bufferIndex]);
    }
    /* Return the 32 bit result */
    return DL_CRC_getResult32(BSL_CRC);
}



void BSL_PI_UART_send(uint8_t* data, uint16_t len)
{
    uint32_t  checksum = (uint32_t)0;
    uint16_t i;
    uint8_t* BSL_core_data_start = &data[3];
    uint16_t dataSize = (uint16_t)data[1];
    dataSize = dataSize | (data[2] << 8U);

    /* Perform CRC on BSL Core Command and data */
    checksum = BSL_calculateCRC(BSL_core_data_start, dataSize);

    /* Save 32-bit CRC result into buffer */
    data[len] = (uint8_t)checksum & (uint8_t)0xFF;
    uint8_t temp = checksum >> 8U;
    data[len + (uint16_t)1] = temp & (uint8_t)0xFF;
    temp = checksum >> 16U;
    data[len + (uint16_t)2] = temp & (uint8_t)0xFF;
    data[len + (uint16_t)3] = (checksum >> 24U);

    for (i = (uint16_t)0; i < (BSL_PI_CRC_SIZE + len); i++)
    {
        DL_UART_Main_clearInterruptStatus(UART_0_INST, DL_UART_MAIN_INTERRUPT_EOT_DONE);
        while (DL_UART_isTXFIFOFull(UART_0_INST))
        {
        }
        DL_UART_transmitData(UART_0_INST, data[i]);
    }
    while (DL_UART_Main_getRawInterruptStatus(UART_0_INST, DL_UART_MAIN_INTERRUPT_EOT_DONE) !=
            DL_UART_MAIN_INTERRUPT_EOT_DONE)
    {
    }
}


