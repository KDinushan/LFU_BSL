/*
 * uart.h
 *
 *  Created on: Nov 7, 2022
 *      Author: a0223946
 */

#ifndef UART_H_
#define UART_H_

#include "stdint.h"
#include "ti_msp_dl_config.h"
//#include "task.h"

#define BSL_maxBufferSize 256


/* BSL Packet definitions */
#define BSL_PACKET_HEADER_BYTE ((uint8_t) 0x80)

/* Wrapper size includes 1 Header + 2 length + 4 crc bytes */
#define BSL_PI_WRAPPER_SIZE ((uint16_t) 0x7)

/* 4 CRC bytes for CRC32 */
#define BSL_PI_CRC_SIZE ((uint16_t) 0x4)

/* BSL Acknowledgment responses */
#define BSL_ERROR_HEADER_INCORRECT ((uint8_t) 0x51)
#define BSL_ERROR_CHECKSUM_INCORRECT ((uint8_t) 0x52)
#define BSL_ERROR_PACKET_SIZE_ZERO ((uint8_t) 0x53)
#define BSL_ERROR_PACKET_SIZE_TOO_BIG ((uint8_t) 0x54)
#define BSL_ERROR_UNKNOWN_ERROR ((uint8_t) 0x55)
#define BSL_ACK ((uint8_t) 0x0)

/* Definitions for CRC */
#define BSL_CRC (CRC)
#define BSL_CRC_SEED ((uint32_t) 0xFFFFFFFFU)



/*
 *
 * Enum that lists the various states involved in packet reception
 */
typedef enum {
    /*
     * IDLE state indicates that the BSL is ready to accept
     * the new data packet from the Host.
     */
    RX_idle = 0x0,
    /*
     * When the Interface receives first byte of the packet, the receive state
     * machine will be moved from IDLE to REVEIVING. At this state it reads
     * the data from Host
     */
    RX_receiving = 0x1,
    /*
     * BLOCKED is a state when complete packet is received and is yet to be
     * processed. At this state any data from the Host will be ignored.
     */
    RX_blocked = 0x2

} BSL_RX_states;



uint8_t BSL_PI_UART_receive(void);
void BSL_PI_sendByte(uint8_t data);
uint32_t BSL_calculateCRC(uint8_t* data, uint16_t dataSize);
void BSL_PI_UART_send(uint8_t* data, uint16_t len);

#endif /* UART_H_ */
