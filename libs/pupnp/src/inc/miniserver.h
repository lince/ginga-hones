/************************************************************************
 * 
 * Copyright (c) 2000-2003 Intel Corporation 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met: 
 *
 * * Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer. 
 * * Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * Neither name of Intel Corporation nor the names of its contributors 
 * may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************/

#ifndef MINISERVER_H
#define MINISERVER_H


#include "sock.h"
#include "httpparser.h"


extern SOCKET gMiniServerStopSock;


typedef struct MServerSockArray {
	/* socket for listening for miniserver requests */
	SOCKET miniServerSock4;
	SOCKET miniServerSock6;
	/* socket for stopping miniserver */
	SOCKET miniServerStopSock;
	/* socket for incoming advertisments and search requests */
	SOCKET ssdpSock4;
	SOCKET ssdpSock6;

	SOCKET stopPort;
	SOCKET miniServerPort4;
	SOCKET miniServerPort6;

	/* socket for sending search requests and receiving search replies */
	CLIENTONLY(SOCKET ssdpReqSock4;)
	CLIENTONLY(SOCKET ssdpReqSock6;)
} MiniServerSockArray;


typedef void (*MiniServerCallback)(
	IN http_parser_t *parser,
	IN http_message_t* request, 
	IN SOCKINFO *info );


#ifdef __cplusplus
extern "C" {
#endif


/*!
 * \brief Set HTTP Get Callback.
 */
void SetHTTPGetCallback(
	/*! [in] HTTP Callback to be invoked . */
	MiniServerCallback callback);


/*!
 * \brief Set SOAP Callback.
 */
#ifdef INCLUDE_DEVICE_APIS
void SetSoapCallback(
	/*! [in] SOAP Callback to be invoked . */
	MiniServerCallback callback);
#else  /* INCLUDE_DEVICE_APIS */
static inline void SetSoapCallback(MiniServerCallback callback) {}
#endif /* INCLUDE_DEVICE_APIS */


/*!
 * \brief Set GENA Callback.
 */
void SetGenaCallback(
	/*! [in] GENA Callback to be invoked. */
	MiniServerCallback callback);


/*!
 * \brief Initialize the sockets functionality for the Miniserver.
 *
 * Initialize a thread pool job to run the MiniServer and the job to the
 * thread pool.
 *
 * If listen port is 0, port is dynamically picked.
 *
 * Use timer mechanism to start the MiniServer, failure to meet the 
 * allowed delay aborts the attempt to launch the MiniServer.
 *
 * \return
 *	\li On success: UPNP_E_SUCCESS.
 *	\li On error: UPNP_E_XXX.
 */
int StartMiniServer(
	/*! [in,out] Port on which the server listens for incoming IPv4 connections. */
	unsigned short *listen_port4, 
	/*! [in,out] Port on which the server listens for incoming IPv6 connections. */
	unsigned short *listen_port6);


/*!
 * \brief Stop and Shutdown the MiniServer and free socket resources.
 *
 * \return Always returns 0.
 */
int StopMiniServer();


#ifdef __cplusplus
}   /* extern C */
#endif


#endif /* MINISERVER_H */

