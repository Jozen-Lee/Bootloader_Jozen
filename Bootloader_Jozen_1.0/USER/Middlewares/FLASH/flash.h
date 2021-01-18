#ifndef __FLASH_H
#define __FLASH_H			    
#include "main.h"
#include "driver_spi.h"

/*---------------------用户自行定义板载FLASH的CS引脚连接的芯片引脚-----------------*/
#define FLASH_CS_GPIO GPIOF
#define FLASH_CS_PIN GPIO_PIN_6
/*------------------------------------------------------------------------------*/

/*-------------------------------内部定义函数---------------------------------*/
#define FLASH_CS_LOW()  HAL_GPIO_WritePin(FLASH_CS_GPIO, FLASH_CS_PIN, GPIO_PIN_RESET);
#define FLASH_CS_HIGH()  HAL_GPIO_WritePin(FLASH_CS_GPIO, FLASH_CS_PIN, GPIO_PIN_SET);
/*---------------------------------------------------------------------------*/	   

/*-------------------------------指令表---------------------------------------*/
//W25X系列/Q系列芯片列表	   
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

//命令集
#define W25X_WriteEnable			0x06 
#define W25X_WriteDisable			0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData					0x03 
#define W25X_FastReadData			0x0B 
#define W25X_FastReadDual			0x3B 
#define W25X_PageProgram			0x02 
#define W25X_BlockErase				0xD8 
#define W25X_SectorErase			0x20 
#define W25X_ChipErase				0xC7 
#define W25X_PowerDown				0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID					0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
/*----------------------------------------------------------------------------*/

/*------------------------------存储地址---------------------------------------*/
//每个程序占179K,程序名占1K,即整个程序占180K
#define FLASH_PROGRESS_1 		0
#define FLASH_NAME_1				(PRO_MAX_SIZE-1)*1024
#define FLASH_PROGRESS_2 		 PRO_MAX_SIZE*1024
#define FLASH_NAME_2				(PRO_MAX_SIZE*2-1)*1024
#define FLASH_PROGRESS_3 		 PRO_MAX_SIZE*2*1024
#define FLASH_NAME_3				(PRO_MAX_SIZE*3-1)*1024
#define FLASH_PROGRESS_4 		 PRO_MAX_SIZE*3*1024
#define FLASH_NAME_4				(PRO_MAX_SIZE*4-1)*1024
#define FLASH_PROGRESS_5 		 PRO_MAX_SIZE*4*1024
#define FLASH_NAME_5				(PRO_MAX_SIZE*5-1)*1024
#define FLASH_PROGRESS_6 		 PRO_MAX_SIZE*5*1024
#define FLASH_NAME_6				(PRO_MAX_SIZE*6-1)*1024
#define FLASH_PROGRESS_7 		 PRO_MAX_SIZE*6*1024
#define FLASH_NAME_7				(PRO_MAX_SIZE*7-1)*1024
#define FLASH_PROGRESS_8 		 PRO_MAX_SIZE*7*1024
#define FLASH_NAME_8				(PRO_MAX_SIZE*8-1)*1024
#define FLASH_PROGRESS_9 		 PRO_MAX_SIZE*8*1024
#define FLASH_NAME_9				(PRO_MAX_SIZE*9-1)*1024
#define FLASH_PROGRESS_10 	 PRO_MAX_SIZE*9*1024
#define FLASH_NAME_10				(PRO_MAX_SIZE*10-1)*1024

extern uint16_t SPI_FLASH_TYPE;		//定义我们使用的flash芯片型号	
/*---------------------------------------API函数----------------------------------------------*/
void SPI_Flash_Init(void);
uint16_t  SPI_Flash_ReadID(void);  	    //读取FLASH ID
uint8_t	 SPI_Flash_ReadSR(void);        //读取状态寄存器 
void SPI_FLASH_Write_SR(uint8_t sr);  	//写状态寄存器
void SPI_FLASH_Write_Enable(void);  		//写使能 
void SPI_FLASH_Write_Disable(void);			//写保护
void SPI_Flash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_Flash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //读取flash
void SPI_Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//写入flash
void SPI_Flash_Erase_Chip(void);    	  																					//整片擦除
void SPI_Flash_Erase_Sector(uint32_t Dst_Addr);																		//扇区擦除
void SPI_Flash_Wait_Busy(void);           																				//等待空闲
void SPI_Flash_PowerDown(void);          																					//进入掉电模式
void SPI_Flash_WAKEUP(void);			  																							//唤醒
void SPI_Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_Flash_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void SPI_Flash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
#endif
















