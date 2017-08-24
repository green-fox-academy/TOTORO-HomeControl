/**
 *
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/httpser-netconn.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    30-December-2016
  * @brief   Basic http server implementation using LwIP netconn API  
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
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "string.h"
#include <stdio.h>
#include "httpserver-netconn.h"
#include "cmsis_os.h"
#include "stm32746g_discovery.h"
#include "socket_server.h"
#include <ff.h>
#include "main.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WEBSERVER_THREAD_PRIO    osPriorityAboveNormal
#define THREAD_STACKSIZE	( configMINIMAL_STACK_SIZE * 24 )
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

FIL dynamic_header;

unsigned char d_content[512];



/* Format of dynamic web page: the page header */
unsigned char PAGE_START[512];



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief serve tcp connection  
  * @param conn: pointer on connection structure 
  * @retval None
  */
static void http_server_serve(struct netconn *conn) 
{
  struct netbuf *inbuf;
  err_t recv_err;
  char* buf;
  u16_t buflen;
  //struct fs_file file;
  
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
  recv_err = netconn_recv(conn, &inbuf);
  
  if (recv_err == ERR_OK)
  {
    if (netconn_err(conn) == ERR_OK) 
    {
      netbuf_data(inbuf, (void**)&buf, &buflen);
    
      /* Is this an HTTP GET command? (only check the first 5 chars, since
      there are other formats for GET, and we're keeping it very simple )*/
      if ((buflen >=5) && (strncmp(buf, "GET /", 5) == 0))
      {

           /* Load dynamic page */
           DynWebPage(conn);
           //netconn_write(conn, &weather_data, sizeof(char), NETCONN_NOCOPY);
      }
      }
    }

  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);
  
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
}


/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void http_server_netconn_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
    /* Bind to port 80 (HTTP) with default IP address */
    err = netconn_bind(conn, NULL, 80);
    
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        /* accept any incoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
          /* serve connection */
          http_server_serve(newconn);

          /* delete connection */
          netconn_delete(newconn);
        }
      }
    }
  }
}

/**
  * @brief  Initialize the HTTP server (start its thread) 
  * @param  none
  * @retval None
  */
void http_server_netconn_init()
{
	volatile osThreadId id = sys_thread_new("HTTP", http_server_netconn_thread, NULL, THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
	id += 1;
}

/**
  * @brief  Create and send a dynamic Web Page. This page contains the list of 
  *         running tasks and the number of page hits. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void DynWebPage(struct netconn *conn)
{
    portCHAR PAGE_BODY[512];
    portFLOAT weather_data [3];
    portCHAR buf[128];
    UINT byteCount;


    memset(PAGE_BODY, 0,512);
    memcpy(buf, &weather_data, sizeof(float));

    strcat((char *)PAGE_BODY, "<w_data><pre><br>Temperature (�C):		Humidity (%):		Pressure (Pa):" );
    strcat((char *)PAGE_BODY, "<br>--------------------------------------------------------------------<br>");
    sprintf(buf, "%.2f 	 			%.1f 			%.2f", received_weather_data[0], received_weather_data[1], received_weather_data[2]);
    strcat(PAGE_BODY, buf);
    strcat((char *)PAGE_BODY, "<br>---------------------------------------------------------------------<br></w_data>");
    strcat((char *)PAGE_BODY, "<style>w_data {text-align: center; color: #D3D3D3; font-family: Arial;} </style>");
    /* Send the dynamically generated page */

    f_open(&dynamic_header, "STARTPAGE.HTML", FA_OPEN_EXISTING | FA_READ);

    while(!f_eof(&dynamic_header))
    {
	f_read(&dynamic_header, d_content, 512, &byteCount);

	netconn_write(conn, d_content, byteCount, NETCONN_COPY);
    }
    f_close(&dynamic_header);
    netconn_write(conn, PAGE_BODY, strlen(PAGE_BODY), NETCONN_COPY);

}
