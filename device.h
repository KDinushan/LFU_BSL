/*
 * device.h
 *
 *  Created on: Nov 17, 2022
 *      Author: a0223946
 */

#ifndef DEVICE_H_
#define DEVICE_H_


#define APP_AREA_1_START_ADDR       0x4000
#define APP_AREA_2_START_ADDR       0x12000

#define MSPM0_MAIN_FLASH_START_ADDRESS   ((uint32_t)0x00004000) //This depends on the bootloader's project firmware size, current size <0x4000
#define MSPM0_MAIN_FLASH_END_ADDRESS     ((uint32_t)0x00020000) // For 128k flash device


#endif /* DEVICE_H_ */
