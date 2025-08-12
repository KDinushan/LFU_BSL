/*
 * BSL_CI.h
 *
 *  Created on: Nov 12, 2022
 *      Author: a0223946
 */




#ifndef BSL_CI_H_
#define BSL_CI_H_

#include "stdint.h"
#include "device.h"

#define MAIN_SECTOR_SIZE                    (uint32_t)1024
#define INFO_SECTOR_SIZE                    1024



#define BSL_RAM_bufStartAddress   ((uint32_t)0x20200000)
#define BSL_RAM_bufEndAddress     ((uint32_t)0x20208000)

#define MSPM0_INFO_FLASH_START_ADDRESS   ((uint32_t)0x41C00000)
#define MSPM0_INFO_FLASH_END_ADDRESS     ((uint32_t)0x41C00400)
/* BSL Commands */
#define CMD_UNLOCK_BSL                   ((uint8_t)0x21)
#define CMD_FLASH_RANGE_ERASE            ((uint8_t)0x23)
#define CMD_PROGRAM_DATA                 ((uint8_t)0x20)
#define CMD_START_APPLICATION            ((uint8_t)0x40)

#define BSL_STATUS_LOCKED                ((uint8_t)0x01)
#define BSL_STATUS_UNLOCKED              ((uint8_t)0xA5)

/* BSL Responses */
#define BSL_MEMORY_READ_BACK                ((uint8_t)0x30)
#define GET_IDENTITY                        ((uint8_t)0X31)
#define STANDALONE_VERIFICATION             ((uint8_t)0X32)
#define BSL_MESSAGE_REPLY                   ((uint8_t)0x3B)

/* BSL Error Codes */
/* Command Interpreter error codes */
#define BSL_SUCCESSFUL_OPERATION                ((uint8_t)0x00)
#define BSL_FLASH_PROGRAM_FAILED                ((uint8_t)0x01)
#define BSL_MASS_ERASE_FAIL                     ((uint8_t)0x02)
#define BSL_LOCKED                              ((uint8_t)0x04)
#define BSL_PASSWORD_ERROR                      ((uint8_t)0x05)
#define BSL_MULTIPLE_PASSWORD_ERROR             ((uint8_t)0x06)
#define BSL_UNKNOWN_COMMAND                     ((uint8_t)0x07)
#define BSL_INVALID_MEMORY_RANGE                ((uint8_t)0x08)
#define BSL_FACTORY_RESET_DISABLED              ((uint8_t)0x0B)
#define BSL_FACTORY_RESET_PASSWORD_ERROR        ((uint8_t)0x0C)
#define BSL_READOUT_DISABLED                    ((uint8_t)0x0D)
#define BSL_FLASH_ERASE_FAILED                  ((uint8_t)0x0E)
#define BSL_FACTORY_RESET_FAILED                ((uint8_t)0x0F)
#define BSL_INVALID_ADDR_LEN_ALIGNMENT          ((uint8_t)0x10)
#define BSL_STANDALONE_VERIFICATION_INVALID_LEN ((uint8_t)0x11)


#define BSL_ONE_SECOND_TIMEOUT          ((uint16_t)533U)
#define BSL_TWO_SECOND_TIMEOUT          ((uint16_t)2U  * (BSL_ONE_SECOND_TIMEOUT))
#define BSL_TEN_SECOND_TIMEOUT          ((uint16_t)10U * (BSL_ONE_SECOND_TIMEOUT))
#define BSL_TIMER_PRE_SCALE             ((uint8_t)60U)


#define PASS                            ((uint8_t)0x01)
#define FAIL                            ((uint8_t)0x00)

#define PLUGIN_ACTIVE_VALID              (uint32_t)0x12000180

#define PLUGIN_INDEX_UART               (uint8_t)0U
#define PLUGIN_INDEX_I2C                (uint8_t)1U
#define PLUGIN_INDEX_OTHER              (uint8_t)2U

#define MAX_PLUGINS                     (3U)
#define ROM_PLUGINS                     (2U)

#define MSPM0_BSL_CRC_SEED              (uint32_t)0xFFFFFFFFU

/* TIMER_MODE */
#define NO_PLUGIN_DETECTED              (uint8_t)0x00
#define NO_PACKET_RECEIVE               (uint8_t)0X01
#define PASSWORD_WRONG                  (uint8_t)0X02

#define PACKET_HEADER_UART_I2C_SPI      ((uint8_t)0x80)
#define PACKET_HEADER_RESPONSE          ((uint8_t)0x08)
#define PACKET_HEADER_CAN_USB           ((uint8_t)0x3F)

#define HEADER_INDEX                    ((uint16_t)0U)
#define LEN_BYTE_L_INDEX                ((uint16_t)1U)
#define LEN_BYTE_H_INDEX                ((uint16_t)2U)
#define CMD_INDEX                       ((uint16_t)3U)
#define START_ADDRESS_INDEX             ((uint16_t)4U)
#define END_ADDRESS_INDEX               ((uint16_t)8U)
#define DATA_LENGTH_INDEX               ((uint16_t)8U)
#define PASSWORD_INDEX                  ((uint16_t)4U)

#define MESSAGE_INDEX                   ((uint16_t)4U)
#define VERIFICATION_CRC_INDEX          ((uint16_t)4U)

#define PACKET_HEADER_LEN               ((uint16_t)3U)

#define UNLOCK_BSL_CMD_LEN              ((uint16_t)0x21U)
#define FACTORY_RESET_CMD_LEN           ((uint16_t)0x11U)

#define GET_IDENTITY_RSP_LEN            ((uint8_t)0x0D)
#define MESSAGE_RSP_LEN                 ((uint8_t)0x02)
#define STANDALONE_VERIFY_RSP_LEN       ((uint8_t)0x05)

#define PASSWORD_ERROR_LIMIT            ((uint8_t)3U)
#define BSL_PASSWORD_LEN                ((uint8_t)8U)

void BSL_CI_interpretCommand(void);
void CMD_API_Unlock_BSL(void);
void BSL_CI_sendMessage(uint8_t message);
uint8_t CMD_API_Flash_Range_Erase(uint32_t addr_start);
uint8_t CMD_API_Program_Data(uint32_t addr_start);


#endif /* BSL_CI_H_ */
