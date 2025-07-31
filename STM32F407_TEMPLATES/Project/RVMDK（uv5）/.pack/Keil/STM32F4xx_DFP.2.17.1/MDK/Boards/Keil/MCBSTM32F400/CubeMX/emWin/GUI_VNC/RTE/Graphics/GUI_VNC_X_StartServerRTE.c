/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2021  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.24 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
-------------------------- END-OF-HEADER -----------------------------

File    : GUI_VNC_X_Keil.c
Purpose : Starts the VNC server via TCP/IP.
          This version works with the Keil MDK-Pro TCP/IP stack.

          Enable the BSD socket component.
          Start by calling GUI_VNC_X_StartServer.

          _ServerTask requires 4096 Bytes of private stack.
          Only 1 client is supported.
*/

#include <stdlib.h>
#include <string.h>

#include "RTE_Components.h"             // Component selection

#ifdef RTE_CMSIS_RTOS2
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#else
#include "cmsis_os.h"                   // ::CMSIS:RTOS
#endif

#include "rl_net.h"                     // Keil::Network:CORE
#include "GUI.h"
#include "GUI_VNC.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

//
// Define maximum number of VNC clients that can connect
//
#define VNC_MAX_CLIENTS  1


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static GUI_VNC_CONTEXT    _Context;
static struct sockaddr_in _Addr;
static U8                 _acBuffer[GUI_VNC_BUFFER_SIZE];


/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbVNC_Recv()
*
*  Function description
*    This function is called indirectly by the server; it's address
*    is passed to the actual server code as function pointer. It is
*    needed because the server is independent of the TCP/IP stack
*    implementation, so details for the TCP/IP stack can be placed
*    here.
*
*  Parameters
*    pData      : Where to store the received data.
*    NumBytes   : Maximum amount of data to receive.
*    pConnection: Connection handle (WebSocket context).
*
*  Return value
*    Amount of data received: >  0
*    Connection closed      : == 0
*    Error                  : <  0
*/
static int _cbVNC_Recv(U8* pData, int NumBytes, void* pConnection) {
  return recv((long)pConnection, (char*)pData, NumBytes, 0);
}

/*********************************************************************
*
*       _cbVNC_Send()
*
*  Function description
*    This function is called indirectly by the server; it's address
*    is passed to the actual server code as function pointer. It is
*    needed because the server is independent of the TCP/IP stack
*    implementation, so details for the TCP/IP stack can be placed
*    here.
*
*  Parameters
*    pData      : Data to send.
*    NumBytes   : Amount of data to send.
*    pConnection: Connection handle.
*
*  Return value
*    Amount of data sent: >  0
*    Connection closed  : == 0
*    Error              : <  0
*/
static int _cbVNC_Send(const U8* pData, int NumBytes, void* pConnection) {
  return send((long)pConnection, (const char*)pData, NumBytes, 0);
}

/*********************************************************************
*
*       _ListenAtTCPAddr()
*
*  Function description
*    Creates a non-blocking socket for accepting a VNC connection.
*
*  Parameters
*    Port: Port to use for accepting connections.
*
*  Return value
*    Parent socket handle.
*/
static int _ListenAtTCPAddr(U16 Port) {
         int         hSock;
  struct sockaddr_in InAddr;

  hSock = socket(AF_INET, SOCK_STREAM, 0);
  if (hSock <= 0) {
    while (1);  // Should not happen.
  }
  memset(&InAddr, 0, sizeof(InAddr));
  InAddr.sin_family      = AF_INET;
  InAddr.sin_port        = htons(Port);
  InAddr.sin_addr.s_addr = INADDR_ANY;
  bind(hSock, (struct sockaddr*)&InAddr, sizeof(InAddr));
  listen(hSock, 1);

  return hSock;
}


/*********************************************************************
*
*       _ServerTask
*
*  Function description
*    This routine is the actual server task.
*    It executes some one-time init code, then runs in an ednless loop.
*    It therefore does not terminate.
*    In the endless loop it
*      - Waits for a conection from a client
*      - Runs the server code
*      - Closes the connection
*/
#ifdef RTE_CMSIS_RTOS2
__NO_RETURN static void _ServerTask(void *arguments) {
#else
__NO_RETURN static void _ServerTask(void const *arguments) {
#endif
  int s, Sock, AddrLen;
  U16 Port;

  (void)arguments;

  //
  // Prepare socket (one time setup)
  //
  Port = 5900 + _Context.ServerIndex; // Default port for VNC is is 590x, where x is the 0-based layer index
  //
  // Loop until we get a socket into listening state
  //
  do {
    s = _ListenAtTCPAddr(Port);
    if (s != -1) {
      break;
    }
    osDelay(100); // Try again
  } while (1);
  //
  // Loop once per client and create a thread for the actual server
  //
  while (1) {
    //
    // Wait for an incoming connection
    //
    AddrLen = sizeof(_Addr);
    if ((Sock = accept(s, (struct sockaddr*)&_Addr, &AddrLen)) < 0) {
      continue; // Error
    }
    //
    // Run the actual server
    //
    GUI_VNC_Process(&_Context, _cbVNC_Send, _cbVNC_Recv, (void *)Sock);
    //
    // Close the connection
    //
    closesocket(Sock);
    memset(&_Addr, 0, sizeof(struct sockaddr_in));
  }
}

#ifdef RTE_CMSIS_RTOS2
static uint64_t _ServerTask_stk[4096 / 8];
static const osThreadAttr_t _ServerTask_attr = {
  .stack_mem  = &_ServerTask_stk[0],
  .stack_size = sizeof(_ServerTask_stk)
};
#else
static osThreadDef(_ServerTask, osPriorityNormal, 0, 4096);
#endif

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_VNC_X_StartServer()
*
*  Function description
*    This routine starts a separate task for the VNC server. It
*    requires that the OS and TCP/IP stack are already initialized.
*
*  Parameters
*    LayerIndex : Index of the GUI layer that is shown via VNC.
*    ServerIndex: Zero-based server index.
*
*  Return value
*    O.K.: 0
*
*  Additional information
*    This sample is intended to be used for starting one VNC server.
*    In case multiple layers shall be accessed by different server
*    instances (ServerIndex) this sample needs to be modified.
*/
int GUI_VNC_X_StartServer(int LayerIndex, int ServerIndex) {
  //
  // Check if maximum number of VNC clients is not exceeded.
  //
  if (ServerIndex >= VNC_MAX_CLIENTS) {
    return -1;
  }
  //
  // Init VNC context and attach to layer (so context is
  // updated if the display-layer-contents change).
  //
  GUI_VNC_AttachToLayer(&_Context, LayerIndex);
  _Context.ServerIndex = (uint16_t)ServerIndex;
  //
  // Set up pointer to RFB buffer.
  //
  _Context.pBuffer      = _acBuffer;
  _Context.SizeOfBuffer = GUI_VNC_BUFFER_SIZE;
  //
  // Create task for VNC Server
  //
#ifdef RTE_CMSIS_RTOS2
  osThreadNew(_ServerTask, NULL, &_ServerTask_attr);
#else
  osThreadCreate(osThread(_ServerTask),NULL);
#endif
  //
  // O.k., server has been started
  //

  return 0;
}

/*********************************************************************
*
*       GUI_VNC_X_getpeername()
*
*  Function description
*    Retrieves the IP addr. of the currently connected VNC client.
*
*  Parameters
*    ServerIndex: Index of server instance.
*    pIPAddr:[OUT] VNC client connected: U32 IP addr. in network endianess.
*                  No client connected : 0
*/
void GUI_VNC_X_getpeername(int ServerIndex, U32 * pIPAddr) {
  struct sockaddr_in Client;
  int                Len;
  void             * pConnection;

  if ((ServerIndex >= VNC_MAX_CLIENTS) || (ServerIndex < 0)) {
    return;
  }
  if (pIPAddr) {
    *pIPAddr = _Addr.sin_addr.s_addr;
  }
}

/*************************** End of file ****************************/
