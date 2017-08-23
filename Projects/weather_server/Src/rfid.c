/* Includes ------------------------------------------------------------------*/
#include "rfid.h"
#include "stm32f7xx_hal.h"
#include "lcd_log.h"
#include "cmsis_os.h"

/* SPI handler declaration */
SPI_HandleTypeDef hspi1;

void spi_init(void)
{
	__HAL_RCC_SPI2_CLK_ENABLE();

	hspi1.Instance               = SPI2;
	hspi1.Init.Mode 			 = SPI_MODE_MASTER;
	hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize          = SPI_DATASIZE_8BIT; //?
	hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;
	hspi1.Init.NSS               = SPI_NSS_SOFT; //or soft?
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial     = 10;
	hspi1.Init.CRCLength 		  = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode 		  = SPI_NSS_PULSE_DISABLE; // or enable /SPI_NSS_SOFT

	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		while(1);
	}
}

//void transmit_spi(void)
//{
//	uint32_t buffer_trans;
//    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET);
//	HAL_SPI_Receive(&hspi1, &buffer_trans, 1, 1000);
//	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
//}
