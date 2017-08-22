/* Includes ------------------------------------------------------------------*/
#include "rfid.h"
#include "stm32f7xx_hal.h"
#include "lcd_log.h"
#include "cmsis_os.h"

/* SPI handler declaration */
SPI_HandleTypeDef spi_handle;

void spi_init(void)
{
	__HAL_RCC_SPI2_CLK_ENABLE();

	spi_handle.Instance               = SPI2;
	spi_handle.Init.Mode 			  = SPI_MODE_MASTER;
	spi_handle.Init.Direction         = SPI_DIRECTION_2LINES;
	spi_handle.Init.DataSize          = SPI_DATASIZE_8BIT; //?
	spi_handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	spi_handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	spi_handle.Init.NSS               = SPI_NSS_HARD_OUTPUT; //or soft?
	spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	spi_handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	spi_handle.Init.TIMode            = SPI_TIMODE_DISABLE;
	spi_handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	spi_handle.Init.CRCPolynomial     = 7;
	spi_handle.Init.CRCLength 		  = SPI_CRC_LENGTH_DATASIZE;
	spi_handle.Init.NSSPMode 		  = SPI_NSS_PULSE_DISABLE; // or enable /SPI_NSS_SOFT

	HAL_SPI_Init(&spi_handle);
}

void transmit_spi(void)
{
	uint32_t buffer_trans;
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_Receive(&spi_handle, &buffer_trans, 1, 1000);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
}
