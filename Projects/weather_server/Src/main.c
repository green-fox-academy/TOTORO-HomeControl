/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-December-2016
  * @brief   This sample code implements a http server application based on 
  *          Netconn API of LwIP stack and FreeRTOS. This application uses 
  *          STM32F7xx the ETH HAL API to transmit and receive data. 
  *          The communication is done with a web browser of a remote PC.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "app_ethernet.h"
#include "lcd_log.h"
#include "socket_server.h"
#include "socket_client.h"
#include "stm32746g_discovery_lcd.h"
#include "httpserver-netconn.h"
#include "projector_server.h"
#include "ac_client.h"
#include "rfid.h"
#include "rc522.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif gnetif; /* network interface structure */

const GUI_POINT tri_up[] = {
{ 417, 60},
{ 433, 60},
{ 425, 44}
};

const GUI_POINT tri_down[] = {
{ 417, 164},
{ 433, 164},
{ 425, 180}
};

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void StartThread(void const * argument);
static void BSP_Config(void);
static void Netif_Config(void);
static void MPU_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void GUIThread(void const * argument);
static void GUI_Startup();
extern SPI_HandleTypeDef hspi1;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  * */

int main(void)
{
	/* Configure the MPU attributes as Device memory for ETH DMA descriptors */
	MPU_Config();

	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	/* STM32F7xx HAL library initialization:
	   - Configure the Flash ART accelerator on ITCM interface
	   - Configure the Systick to generate an interrupt each 1 msec
	   - Set NVIC Group Priority to 4
	   - Global MSP (MCU Support Package) initialization
	*/
	HAL_Init();
  
	/* Configure the system clock to 200 MHz */
	SystemClock_Config();

	/* Initialize LCD */
	BSP_Config();


	spi_init();
	HAL_Delay(500);
	MFRC522_Init();

	while (1) {
		uint8_t str[MFRC522_MAX_LEN];
		memset(str, 0, MFRC522_MAX_LEN);
//		uint8_t asd;
//		uint8_t asd2 = 0x55;
//		cs_reset();
//		HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&asd2, (uint8_t*)&asd, 1, 10);
//		cs_set();
		volatile uint8_t ret = MFRC522_ReadRegister(0xFF);
//		volatile uint8_t ret = MFRC522_Request(PICC_REQIDL, str);
//		ret = MFRC522_Anticoll(str);
		HAL_Delay(1000);
	}

	/* Create GUI task */
	//osThreadDef(GUI_Thread, GUIThread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 20);	//2048
	//osThreadCreate (osThread(GUI_Thread), NULL);

	GUI_Startup();

	/*Init thread */
	osThreadDef(Start, StartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate (osThread(Start), NULL);

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	while(1) {
	}
}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void GUI_Startup()
{
    GUI_Init();
	WM_MULTIBUF_Enable(1);
	GUI_SetLayerVisEx (1, 0);
	GUI_SelectLayer(0);

	//split screen for weather station and projector canvas control
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();
	GUI_SetColor(GUI_DARKBLUE);
	GUI_FillRect(0, 0, 100, 272);		//controls
	GUI_FillRect(105, 0, 275, 170);		//temperature
	GUI_FillRect(280, 0, 365, 85);		//humidity
	GUI_FillRect(280, 90, 365, 170);	//air pressure
	GUI_FillRect(105, 175, 365, 272);	//AC control
	GUI_FillRect(370, 0, 480, 217);		//projector canvas control
	GUI_FillRect(370, 222, 480, 272);	//empty
	GUI_SetColor(GUI_LIGHTGRAY);
	GUI_SetFont(GUI_FONT_16_1);
	GUI_SetBkColor(GUI_DARKBLUE);
	GUI_DispStringAt("HomeControl", 5, 5);
	GUI_SetFont(GUI_FONT_13_1);
	GUI_DispStringAt("Temperature (°C)", 110, 5);
	GUI_DispStringAt("Humidity (%)", 285, 5);
	GUI_DispStringAt("Pressure (hPa)", 285, 95);
	GUI_DispStringAt("Projector", 375, 5);
	GUI_SetColor(GUI_BLUE);
	GUI_FillRect(400, 27, 450, 77);		//up
	GUI_FillRect(400, 87, 450, 137);	//stop
	GUI_FillRect(400, 147, 450, 197);	//down
	GUI_SetColor(GUI_DARKBLUE);
	GUI_FillRect(402, 29, 448, 75);		//up
	GUI_FillRect(402, 89, 448, 135);	//stop
	GUI_FillRect(402, 149, 448, 195);	//down

	GUI_SetColor(GUI_BLUE);
	GUI_FillPolygon(tri_up, 3, 0, 0);	//up
	GUI_FillRect(417, 104, 433, 120);	//stop
	GUI_FillPolygon(tri_down, 3, 0, 0);	//down
}

static void GUIThread(void const * argument)
{
	/* Gui background Task */
	while(1) {
		GUI_Exec(); /* Do the background work ... Update windows etc.) */
		osDelay(20); /* Nothing left to do for the moment ... Idle processing */
	}
}

/**
  * @brief  Start Thread 
  * @param  argument not used
  * @retval None
  */
static void StartThread(void const * argument)
{ 
	/* Initialize LCD */
	//BSP_Config();

	/* Create tcp_ip stack thread */
	tcpip_init(NULL, NULL);

	/* Initialize the LwIP stack */
	Netif_Config();

	/* start httpserver thread */
	http_server_netconn_init();

	/* Notify user about the network interface config */
	User_notification(&gnetif);

	/* Create GUI task */
	//  osThreadDef(GUI_Thread, GUIThread, osPriorityAboveNormal, 0, 2048);
	//  osThreadCreate (osThread(GUI_Thread), NULL);

	/* Start DHCPClient */
	osThreadDef(DHCP, DHCP_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate (osThread(DHCP), &gnetif);
	osDelay(2000);

	//Define and start the server thread
	osThreadDef(SOCKET_SERVER, socket_server_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate (osThread(SOCKET_SERVER), NULL);

	//Define and start the projector thread
	osThreadDef(PROJECTOR_CLIENT, projector_client_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate (osThread(PROJECTOR_CLIENT), NULL);

	//Define and start the projector thread
	osThreadDef(AC_CLIENT, ac_client_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate (osThread(AC_CLIENT), NULL);

	while (1) {
		/* Delete the Init Thread */
		osThreadTerminate(NULL);
	}
}

/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{ 
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	ip_addr_set_zero_ip4(&ipaddr);
	ip_addr_set_zero_ip4(&netmask);
	ip_addr_set_zero_ip4(&gw);

	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

	/*  Registers the default network interface. */
	netif_set_default(&gnetif);
  
	if (netif_is_link_up(&gnetif))
	{
		/* When the netif is fully configured this function must be called.*/
		netif_set_up(&gnetif);
	}
	else
	{
		/* When the netif link is down this function must be called */
		netif_set_down(&gnetif);
	}
}

/**
  * @brief  Initializes the STM327546G-Discovery's LCD  resources.
  * @param  None
  * @retval None
  */
static void BSP_Config(void)
{

	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	/* Initialize the SDRAM */
	BSP_SDRAM_Init();

	/* Initialize the Touch screen */
	//BSP_TS_Init(420, 272);
	BSP_TS_Init(480, 272);

	/* Enable CRC to Unlock GUI */
	__HAL_RCC_CRC_CLK_ENABLE();

	/* Enable Back up SRAM */
	__HAL_RCC_BKPSRAM_CLK_ENABLE();

//	/* Initialize the LCD */
//  BSP_LCD_Init();
//
//  /* Initialize the LCD Layers */
//  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);
//
//  /* Set LCD Foreground Layer  */
//  BSP_LCD_SelectLayer(1);
//
//  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
//
//  /* Initialize TS */
//  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
//
//  /* Initialize LCD Log module */
//  LCD_LOG_Init();
//
//  /* Show Header and Footer texts */
//  LCD_LOG_SetHeader((uint8_t *)"TOTORO socket echo server");
//  LCD_LOG_SetFooter((uint8_t *)"STM32746G-DISCO - GreenFoxAcademy");
//
//  LCD_UsrLog ((char *)"Notification - Ethernet Initialization ...\n");
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 400;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

  /* activate the OverDrive */
	if(HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		Error_Handler();
	}
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
	{
	Error_Handler();
	}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

/**
  * @brief  Configure the MPU attributes as Device for  Ethernet Descriptors in the SRAM1.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Configured Region Size is 256B (size of Rx and Tx ETH descriptors) 
  *       
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU attributes as Device for Ethernet Descriptors in the SRAM */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x20010000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
