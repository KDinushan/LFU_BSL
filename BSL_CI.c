/*
 * BSL_CI.c
 *
 *  Created on: Nov 12, 2022
 *      Author: a0223946
 */

#include "BSL_CI.h"
#include <ti/driverlib/dl_flashctl.h>
#include "uart.h"


static uint32_t BSL_CI_getFlashEndAddress(void);


uint8_t BSL_LockedStatus;
extern uint8_t BSL_receive_buf[200];
extern uint8_t BSL_send_buf[40];

extern void CMD_API_startApplication(void);
static bool DL_FlashCTL_programMemoryBlocking2(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t *data, uint32_t dataSize,
    DL_FLASHCTL_REGION_SELECT regionSelect);

void BSL_CI_interpretCommand(void)
{
    uint8_t error_buff;
    uint8_t command = BSL_receive_buf[CMD_INDEX];
    uint32_t addr = *(uint32_t *)&BSL_receive_buf[START_ADDRESS_INDEX];

    switch (command)
    {
        case CMD_UNLOCK_BSL:
        {
            CMD_API_Unlock_BSL();
            break;
        }
        case CMD_FLASH_RANGE_ERASE:
        {
            BSL_CI_sendMessage(CMD_API_Flash_Range_Erase(addr));
            break;
        }
        case CMD_PROGRAM_DATA:
        {
            BSL_CI_sendMessage(CMD_API_Program_Data(addr));
            break;
        }
        case CMD_START_APPLICATION:
        {
            CMD_API_startApplication();
            break;
        }
        default:
        {
            BSL_CI_sendMessage(BSL_UNKNOWN_COMMAND);
            break;
        }
    }
}



/**
 * Compares the data buffer against the BSL password and sets lock state accordingly.
 *
 * \paran data Pointer to the password in the buffer
 *
 * \return Status of operation
 */
void CMD_API_Unlock_BSL(void)
{
    uint8_t ret = BSL_PASSWORD_ERROR;

    uint32_t passwordComparison = (uint32_t)0;
    uint16_t i;
    uint8_t index;
    uint16_t length;
    uint32_t *password, *password2;
    uint32_t expPassword;

    length = (uint16_t)BSL_receive_buf[LEN_BYTE_L_INDEX] | ((uint16_t)BSL_receive_buf[LEN_BYTE_H_INDEX] << 8U);
    password = (uint32_t *)&BSL_receive_buf[PASSWORD_INDEX];
    password2 = (uint32_t *)(0x41C00110u);

    if(length == UNLOCK_BSL_CMD_LEN)
    {
        for(index = (uint8_t)0; index < BSL_PASSWORD_LEN; index++)
        {
            passwordComparison |= password2[index] ^ password[index];
        }

        if(passwordComparison == (uint32_t)0)
        {
            BSL_LockedStatus = BSL_STATUS_UNLOCKED;
            BSL_CI_sendMessage(BSL_SUCCESSFUL_OPERATION);
            ret = BSL_SUCCESSFUL_OPERATION;
        }
    }

    if(ret != BSL_SUCCESSFUL_OPERATION)
    {
        BSL_LockedStatus = BSL_STATUS_LOCKED;
        BSL_CI_sendMessage(BSL_PASSWORD_ERROR);
    }

}


/**
 * Sends a reply message with attached information.
 */
void BSL_CI_sendMessage(uint8_t message)
{

    BSL_send_buf[HEADER_INDEX]     = PACKET_HEADER_RESPONSE;
    BSL_send_buf[LEN_BYTE_L_INDEX] = MESSAGE_RSP_LEN;
    BSL_send_buf[LEN_BYTE_H_INDEX] = (uint8_t)0x00;
    BSL_send_buf[CMD_INDEX]        = BSL_MESSAGE_REPLY;
    BSL_send_buf[MESSAGE_INDEX]    = message;

    BSL_PI_UART_send(BSL_send_buf, (MESSAGE_RSP_LEN + PACKET_HEADER_LEN));

}



/**
 * Erase sectors included the range in main flash.
 *
 * \paran start address
 *
 * \return Status of operation
 */
uint8_t CMD_API_Flash_Range_Erase(uint32_t addr_start)
{
    uint8_t ret = BSL_SUCCESSFUL_OPERATION;
    uint32_t main_flash_start_address = MSPM0_MAIN_FLASH_START_ADDRESS;
    uint32_t main_flash_end_address = MSPM0_MAIN_FLASH_END_ADDRESS;
    uint32_t addr_end;
    uint32_t data_pointer;
    volatile DL_FLASHCTL_COMMAND_STATUS gCmdStatus;

    addr_end = *((uint32_t *)&BSL_receive_buf[END_ADDRESS_INDEX]);
    data_pointer = addr_start;

    // Check if BSL is unlocked
    if(BSL_LockedStatus == BSL_STATUS_UNLOCKED)
    {
        if((addr_start  >= main_flash_start_address) &&
           (addr_end    <= main_flash_end_address)   &&
           (addr_start  <= addr_end))
        {
            while(data_pointer <= addr_end)
            {
                DL_FlashCTL_unprotectSector(FLASHCTL, data_pointer, DL_FLASHCTL_REGION_SELECT_MAIN);
                /* Erase sector in main memory */
                gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
                    FLASHCTL, data_pointer, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
                if (gCmdStatus == DL_FLASHCTL_COMMAND_STATUS_FAILED) {
                    /* If command was not successful, set a breakpoint */
                    ret = BSL_FLASH_ERASE_FAILED;
                }
//                DL_FlashCTL_eraseMemory(FLASHCTL, data_pointer, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
//                erase_status = erase_status & (uint8_t)DL_FlashCTL_waitForCmdDone(FLASHCTL);
                data_pointer = data_pointer + MAIN_SECTOR_SIZE;
            }
//            BSL_PI_UART_send(&BSL_receive_buf[START_ADDRESS_INDEX],4);
//            DL_FlashCTL_unprotectSector(FLASHCTL, addr_end, DL_FLASHCTL_REGION_SELECT_MAIN);
//            DL_FlashCTL_eraseMemory(FLASHCTL, addr_end, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
//            erase_status = erase_status & (uint8_t)DL_FlashCTL_waitForCmdDone(FLASHCTL);

        }
        else
        {
            ret = BSL_INVALID_MEMORY_RANGE;
        }
    }
    else
    {
        ret = BSL_LOCKED;
    }
    return ret;
}

/**
 * Program data at specific address.
 *
 * \paran start address
 *
 * \return Status of operation
 */
uint8_t CMD_API_Program_Data(uint32_t addr_start)
{
    uint8_t ret = BSL_SUCCESSFUL_OPERATION;

    uint32_t main_flash_start_address = MSPM0_MAIN_FLASH_START_ADDRESS;
    uint32_t main_flash_end_address = MSPM0_MAIN_FLASH_END_ADDRESS;
    uint32_t info_flash_start_address = MSPM0_INFO_FLASH_START_ADDRESS;
    uint32_t info_flash_end_address = MSPM0_INFO_FLASH_END_ADDRESS;

    DL_FLASHCTL_REGION_SELECT flash_region;
    uint16_t length = (uint16_t)0U;

    /* Initialize address */

    length = (uint16_t)BSL_receive_buf[1];
    length += BSL_receive_buf[2] << 8U;
    length -= (uint16_t)5U; // Command byte (1) + Address (4)

    if(BSL_LockedStatus == BSL_STATUS_UNLOCKED)
    {
        if((BSL_RAM_bufStartAddress <= addr_start) && ((addr_start + (uint32_t)length) <= BSL_RAM_bufEndAddress))
        {
            // Copy data to RAM
            uint8_t *ram_pointer;
            uint8_t *data = &BSL_receive_buf[8];
            ram_pointer = (uint8_t*)addr_start;
            uint16_t i;

            for(i=(uint16_t)0; i<length; i++)
            {
                ram_pointer[i] = data[i];
            }
        }
        else
        {
            /*
             * Since Flash allows programming for only 64 bits aligned address,
             * This condition checks if given memory address and length are 8 bytes aligned
             */
            if(((addr_start % (uint32_t)8U) == (uint32_t)0U) &&
               (((uint32_t)length % (uint32_t)8U) == (uint32_t)0U))
            {
                /*
                 * Checks if the memory address given is in the valid memory range.
                 * Also checks if FACTORY reset mode is not disabled for programming Non-main Flash region
                 */
                if(( (main_flash_start_address <= addr_start) && ((addr_start + (uint32_t)length) <= main_flash_end_address) ) ||
                  ( (info_flash_start_address <= addr_start) && ((addr_start + (uint32_t)length) <= info_flash_end_address) ))
                {
                    flash_region = DL_FLASHCTL_REGION_SELECT_MAIN;
                    if(info_flash_start_address <= addr_start)
                    {
                        flash_region = DL_FLASHCTL_REGION_SELECT_NONMAIN;
                    }
                    if(DL_FlashCTL_programMemoryBlocking2(FLASHCTL, addr_start, (uint32_t *)&BSL_receive_buf[8], (uint32_t)length/(uint32_t)4, flash_region) == false)
                    {
                        ret = BSL_FLASH_PROGRAM_FAILED;
                    }
                }
                else
                {
                    ret = BSL_INVALID_MEMORY_RANGE;
                }
            }
            else
            {
                ret = BSL_INVALID_ADDR_LEN_ALIGNMENT;
            }
        }
    }
    else
    {
        ret = BSL_LOCKED;
    }
    return ret;
}







static bool DL_FlashCTL_programMemoryBlocking2(FLASHCTL_Regs *flashctl,
    uint32_t address, uint32_t *data, uint32_t dataSize,
    DL_FLASHCTL_REGION_SELECT regionSelect)
{
    bool status = true;

    /* Check for valid data size */
    if (dataSize == (uint32_t) 0) {
        status = false;
    }

    while ((dataSize != (uint32_t) 0) && status) {
        /* Unprotect sector before every write */
        DL_FlashCTL_unprotectSector(flashctl, address, regionSelect);

        /* 32-bit case */
        if (dataSize == (uint32_t) 1) {
            DL_FlashCTL_programMemory32WithECCGenerated(flashctl, address, data);

            dataSize = dataSize - (uint32_t) 1;
            data     = data + 1;
            address  = address + (uint32_t) 4;
        } else {
            /* 64-bit case */
            DL_FlashCTL_programMemory64WithECCGenerated(flashctl, address, data);
            dataSize = dataSize - (uint32_t) 2;
            data     = data + 2;
            address  = address + (uint32_t) 8;
        }

        status = DL_FlashCTL_waitForCmdDone(flashctl);
    }

    return (status);
}




