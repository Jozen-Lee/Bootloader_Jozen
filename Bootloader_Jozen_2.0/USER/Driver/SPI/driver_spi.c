#include "driver_spi.h"

//SPI¶ÁÐ´º¯Êý
uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
	uint8_t RxData;
	HAL_SPI_TransmitReceive(&hspi5, &TxData, &RxData, 1, 1000);
	return RxData;
}

