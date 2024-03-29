/**************************************************************************
 *
 * Copyright (c) 2000-2003 Intel Corporation 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met: 
 *
 * - Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer. 
 * - Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * - Neither name of Intel Corporation nor the names of its contributors 
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
 **************************************************************************/


#include "config.h"


/*!
 * \file
 *
 * \brief This file implements the functionality and utility functions
 * used by the Miniserver module.
 *
 * The miniserver is a central point for processing all network requests.
 * It is made of:
 *   - The SSDP sockets for discovery.
 *   - The HTTP listeners for description / control / eventing.
 *
 */


#include "miniserver.h"


#include "httpreadwrite.h"
#include "ithread.h"
#include "ssdplib.h"
#include "statcodes.h"
#include "ThreadPool.h"
#include "unixutil.h" /* for socklen_t, EAFNOSUPPORT */
#include "upnpapi.h"
#include "util.h"


#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


#define APPLICATION_LISTENING_PORT 49152


struct mserv_request_t {
	/*! Connection handle. */
	int connfd;
	struct sockaddr_storage foreign_sockaddr;
};


typedef enum {
	MSERV_IDLE,
	MSERV_RUNNING,
	MSERV_STOPPING
} MiniServerState;


unsigned short miniStopSockPort;


////////////////////////////////////////////////////////////////////////////
// module vars
static MiniServerCallback gGetCallback = NULL;
static MiniServerCallback gSoapCallback = NULL;
static MiniServerCallback gGenaCallback = NULL;
static MiniServerState gMServState = MSERV_IDLE;


void SetHTTPGetCallback(MiniServerCallback callback)
{
	gGetCallback = callback;
}


#ifdef INCLUDE_DEVICE_APIS
void SetSoapCallback(MiniServerCallback callback)
{
	gSoapCallback = callback;
}
#endif /* INCLUDE_DEVICE_APIS */

void SetGenaCallback(MiniServerCallback callback)
{
	gGenaCallback = callback;
}


#ifdef INTERNAL_WEB_SERVER
/*!
 * \brief Based on the type pf message, appropriate callback is issued.
 *
 * \return 0 on Success or HTTP_INTERNAL_SERVER_ERROR if Callback is NULL.
 */
static int dispatch_request(
	/*! [in] Socket Information object. */
	IN SOCKINFO *info,
	/*! [in] HTTP parser object. */
	http_parser_t *hparser)
{
	MiniServerCallback callback;

	switch (hparser->msg.method) {
	/* Soap Call */
	case SOAPMETHOD_POST:
	case HTTPMETHOD_MPOST:
		callback = gSoapCallback;
		break;

	/* Gena Call */
	case HTTPMETHOD_NOTIFY:
	case HTTPMETHOD_SUBSCRIBE:
	case HTTPMETHOD_UNSUBSCRIBE:
		UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
			"miniserver %d: got GENA msg\n", info->socket);
			callback = gGenaCallback;
		break;

	/* HTTP server call */
	case HTTPMETHOD_GET:
	case HTTPMETHOD_POST:
	case HTTPMETHOD_HEAD:
	case HTTPMETHOD_SIMPLEGET:
		callback = gGetCallback;
		break;

	default:
		callback = NULL;
	}

	if (callback == NULL) {
		return HTTP_INTERNAL_SERVER_ERROR;
	}

	callback(hparser, &hparser->msg, info);

	return 0;
}


/*!
 * \brief Send Error Message.
 */
static UPNP_INLINE void handle_error(
	/*! [in] Socket Information object. */
	SOCKINFO *info,
	/*! [in] HTTP Error Code. */
	int http_error_code,
	/*! [in] Major Version Number. */
	int major,
	/*! [in] Minor Version Number. */
	int minor)
{
	http_SendStatusResponse(info, http_error_code, major, minor);
}


/*!
 * \brief Free memory assigned for handling request and unitialize socket
 * functionality.
 */
static void free_handle_request_arg(
	/*! [in] Request Message to be freed. */
	void *args)
{
	struct mserv_request_t *request = (struct mserv_request_t *)args;

	shutdown(request->connfd, SD_BOTH);
	UpnpCloseSocket(request->connfd);
	free(request);
}


/*!
 * \brief Receive the request and dispatch it for handling.
 */
static void handle_request(
	/*! [in] Request Message to be handled. */
	void *args)
{
	SOCKINFO info;
	int http_error_code;
	int ret_code;
	int major = 1;
	int minor = 1;
	http_parser_t parser;
	http_message_t *hmsg = NULL;
	int timeout = HTTP_DEFAULT_TIMEOUT;
	struct mserv_request_t *request = (struct mserv_request_t *)args;
	int connfd = request->connfd;

	UpnpPrintf( UPNP_INFO, MSERV, __FILE__, __LINE__,
		"miniserver %d: READING\n", connfd );
	//parser_request_init( &parser ); ////LEAK_FIX_MK
	hmsg = &parser.msg;

	ret_code = sock_init_with_ip(
		&info, connfd, (struct sockaddr *)&request->foreign_sockaddr);
	if (ret_code != UPNP_E_SUCCESS) {
		free(request);
		httpmsg_destroy(hmsg);
		return;
	}

	// read
	ret_code = http_RecvMessage(
		&info, &parser, HTTPMETHOD_UNKNOWN, &timeout, &http_error_code);
	if (ret_code != 0) {
		goto error_handler;
	}

	UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
		"miniserver %d: PROCESSING...\n", connfd);

	// dispatch
	http_error_code = dispatch_request( &info, &parser );
	if (http_error_code != 0) {
		goto error_handler;
	}

	http_error_code = 0;

error_handler:
	if (http_error_code > 0) {
		if (hmsg) {
			major = hmsg->major_version;
			minor = hmsg->minor_version;
		}
		handle_error(&info, http_error_code, major, minor);
	}

	UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
		"miniserver %d: COMPLETE\n", connfd);
	sock_destroy(&info, SD_BOTH); // should shutdown completely

	httpmsg_destroy(hmsg);
	free(request);
}


/*!
 * \brief Initilize the thread pool to handle a request, sets priority for the
 * job and adds the job to the thread pool.
 */
static UPNP_INLINE void schedule_request_job(
	/*! [in] Socket Descriptor on which connection is accepted. */
	int connfd,
	/*! [in] Clients Address information. */
	struct sockaddr *clientAddr)
{
	struct mserv_request_t *request;
	ThreadPoolJob job;

	request = (struct mserv_request_t *)malloc(sizeof (struct mserv_request_t));
	if (request == NULL) {
		UpnpPrintf( UPNP_INFO, MSERV, __FILE__, __LINE__,
			"mserv %d: out of memory\n", connfd );
		shutdown( connfd, SD_BOTH );
		UpnpCloseSocket( connfd );
		return;
	}

	request->connfd = connfd;
	memcpy(&request->foreign_sockaddr, clientAddr,
		sizeof(request->foreign_sockaddr));

	TPJobInit(&job, (start_routine)handle_request, (void *)request);
	TPJobSetFreeFunction(&job, free_handle_request_arg);
	TPJobSetPriority(&job, MED_PRIORITY);

	if (ThreadPoolAdd(&gMiniServerThreadPool, &job, NULL) != 0) {
		UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
			"mserv %d: cannot schedule request\n", connfd);
		free(request);
		shutdown(connfd, SD_BOTH);
		UpnpCloseSocket(connfd);
		return;
	}
}
#endif


/*!
 * \brief Run the miniserver.
 *
 * The MiniServer accepts a new request and schedules a thread to handle the
 * new request. Checks for socket state and invokes appropriate read and
 * shutdown actions for the Miniserver and SSDP sockets.
 */
static void RunMiniServer(
	/*! [in] Socket Array. */
	MiniServerSockArray *miniSock)
{
	char errorBuffer[ERROR_BUFFER_LEN];
	struct sockaddr_storage clientAddr;
	socklen_t clientLen;
	SOCKET connectHnd;
	SOCKET miniServStopSock =  miniSock->miniServerStopSock;
	SOCKET ssdpSock4 = miniSock->ssdpSock4;
	SOCKET ssdpSock6 = miniSock->ssdpSock6;
#ifdef INTERNAL_WEB_SERVER
	SOCKET miniServSock4 = miniSock->miniServerSock4;
	SOCKET miniServSock6 = miniSock->miniServerSock6;
#endif /* INTERNAL_WEB_SERVER */
#ifdef INCLUDE_CLIENT_APIS
	SOCKET ssdpReqSock4 = miniSock->ssdpReqSock4;
	SOCKET ssdpReqSock6 = miniSock->ssdpReqSock6;
#endif /* INCLUDE_CLIENT_APIS */
	char buf_ntop[64];
	fd_set expSet;
	fd_set rdSet;
	unsigned int maxMiniSock = 0;
	int byteReceived;
	char requestBuf[256];
	int ret = 0;

#ifdef INTERNAL_WEB_SERVER
	if (miniServSock4 != INVALID_SOCKET) {
		maxMiniSock = max(maxMiniSock, miniServSock4);
	}
	if (miniServSock6 != INVALID_SOCKET) {
		maxMiniSock = max(maxMiniSock, miniServSock6);
	}
#endif /* INTERNAL_WEB_SERVER */
	if (ssdpSock4 != INVALID_SOCKET) {
		maxMiniSock = max(maxMiniSock, ssdpSock4);
	}
	if (ssdpSock6 != INVALID_SOCKET) {
		maxMiniSock = max(maxMiniSock, ssdpSock6);
	}
#ifdef INCLUDE_CLIENT_APIS
	if (ssdpReqSock4 != INVALID_SOCKET) {
		maxMiniSock = max(maxMiniSock, ssdpReqSock4);
	}
	if (ssdpReqSock6 != INVALID_SOCKET) {
		maxMiniSock = max(maxMiniSock, ssdpReqSock6);
	}
#endif /* INCLUDE_CLIENT_APIS */
	maxMiniSock = max(maxMiniSock, miniServStopSock);
	++maxMiniSock;

	gMServState = MSERV_RUNNING;
	while(TRUE) {
		FD_ZERO(&rdSet);
		FD_ZERO(&expSet);

		FD_SET(miniServStopSock, &expSet);
		FD_SET(miniServStopSock, &rdSet);
#ifdef INTERNAL_WEB_SERVER
		if(miniServSock4 != INVALID_SOCKET) {
			FD_SET(miniServSock4, &rdSet);
		}
		if(miniServSock6 != INVALID_SOCKET) {
			FD_SET(miniServSock6, &rdSet);
		}
#endif /* INTERNAL_WEB_SERVER */
		if(ssdpSock4 != INVALID_SOCKET) {
			FD_SET(ssdpSock4, &rdSet);
		}
		if(ssdpSock6 != INVALID_SOCKET) {
			FD_SET(ssdpSock6, &rdSet);
		}
#ifdef INCLUDE_CLIENT_APIS
		if(ssdpReqSock4 != INVALID_SOCKET) {
			FD_SET(ssdpReqSock4, &rdSet);
		}
		if(ssdpReqSock6 != INVALID_SOCKET) {
			FD_SET(ssdpReqSock6, &rdSet);
		}
#endif /* INCLUDE_CLIENT_APIS */

		ret = select(maxMiniSock, &rdSet, NULL, &expSet, NULL);
		if (ret == -1) {
			strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
			UpnpPrintf(UPNP_CRITICAL, SSDP, __FILE__, __LINE__,
				"Error in select(): %s\n", errorBuffer);
			/* Avoid 100% CPU in case of repeated error in select() */
			isleep(1);
			continue;
		} else {
#ifdef INTERNAL_WEB_SERVER
			if (miniServSock6 != INVALID_SOCKET &&
			    FD_ISSET(miniServSock6, &rdSet)) {
				clientLen = sizeof( clientAddr );
				connectHnd = accept(miniServSock6,
					(struct sockaddr *)&clientAddr, &clientLen);
				if (connectHnd == -1) {
					strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
					UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
						"miniserver: Error in accept(): %s\n",
						errorBuffer);
					continue;
				}
				schedule_request_job(
					connectHnd, (struct sockaddr *)&clientAddr);
			}
			if (miniServSock4 != INVALID_SOCKET && 
			    FD_ISSET(miniServSock4, &rdSet)) {
				clientLen = sizeof(clientAddr);
				connectHnd = accept(miniServSock4,
					(struct sockaddr *)&clientAddr, &clientLen);
				if (connectHnd == -1) {
					strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
					UpnpPrintf( UPNP_INFO, MSERV, __FILE__, __LINE__,
						"miniserver: Error in accept(): %s\n",
						errorBuffer);
					continue;
				}
				schedule_request_job(
					connectHnd, (struct sockaddr *)&clientAddr);
			}
#endif /* INTERNAL_WEB_SERVER */
#ifdef INCLUDE_CLIENT_APIS
			// ssdp
			if (ssdpReqSock6 != INVALID_SOCKET &&
			    FD_ISSET(ssdpReqSock6, &rdSet)) {
				readFromSSDPSocket(ssdpReqSock6);
			}
			if (ssdpReqSock4 != INVALID_SOCKET &&
			    FD_ISSET( ssdpReqSock4, &rdSet)) {
				readFromSSDPSocket(ssdpReqSock4);
			}
#endif /* INCLUDE_CLIENT_APIS */
			if (ssdpSock6 != INVALID_SOCKET &&
			    FD_ISSET(ssdpSock6, &rdSet)) {
			readFromSSDPSocket(ssdpSock6);
			}
			if (ssdpSock4 != INVALID_SOCKET &&
			    FD_ISSET(ssdpSock4, &rdSet)) {
				readFromSSDPSocket(ssdpSock4);
			}
			if (FD_ISSET( miniServStopSock, &rdSet)) {
				clientLen = sizeof(clientAddr);
				memset((char *)&clientAddr, 0, sizeof(clientAddr));
				byteReceived = recvfrom(miniServStopSock, requestBuf,
					25, 0, (struct sockaddr *)&clientAddr, &clientLen);
				if (byteReceived > 0) {
					requestBuf[byteReceived] = '\0';
					inet_ntop(AF_INET, 
						&((struct sockaddr_in*)&clientAddr)->sin_addr, 
						buf_ntop, sizeof(buf_ntop));
					UpnpPrintf( UPNP_INFO, MSERV, __FILE__, __LINE__,
						"Received response: %s From host %s \n",
						requestBuf, buf_ntop );
					UpnpPrintf( UPNP_PACKET, MSERV, __FILE__, __LINE__,
						"Received multicast packet: \n %s\n",
						requestBuf);
					if (NULL != strstr(requestBuf, "ShutDown")) {
						break;
					}
				}
			}
		}
	}

#ifdef INTERNAL_WEB_SERVER
	shutdown(miniServSock4, SD_BOTH);
	UpnpCloseSocket(miniServSock4);
	shutdown(miniServSock6, SD_BOTH);
	UpnpCloseSocket(miniServSock6);
#endif /* INTERNAL_WEB_SERVER */
	shutdown(miniServStopSock, SD_BOTH);
	UpnpCloseSocket(miniServStopSock);
	shutdown(ssdpSock4, SD_BOTH);
	UpnpCloseSocket(ssdpSock4);
	shutdown(ssdpSock6, SD_BOTH);
	UpnpCloseSocket(ssdpSock6);
#ifdef INCLUDE_CLIENT_APIS
	shutdown(ssdpReqSock4, SD_BOTH);
	UpnpCloseSocket(ssdpReqSock4);
	shutdown(ssdpReqSock6, SD_BOTH);
	UpnpCloseSocket(ssdpReqSock6);
#endif /* INCLUDE_CLIENT_APIS */

	free(miniSock);
	gMServState = MSERV_IDLE;

	return;
}


/*!
 * \brief Returns port to which socket, sockfd, is bound.
 *
 * \return -1 on error; check errno, otherwise > 0 means port number.
 */
static int get_port(
	/*! [in] Socket descriptor. */
	int sockfd)
{
	struct sockaddr_storage sockinfo;
	socklen_t len;
	int code;
	int port = 0;

	len = sizeof(sockinfo);
	code = getsockname(sockfd, (struct sockaddr *)&sockinfo, &len);
	if (code == -1) {
		return -1;
	}

	if (sockinfo.ss_family == AF_INET) {
		port = ntohs(((struct sockaddr_in*)&sockinfo)->sin_port);
	} else if(sockinfo.ss_family == AF_INET6) {
		port = ntohs(((struct sockaddr_in6*)&sockinfo)->sin6_port);
	}
	UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
		"sockfd = %d, .... port = %d\n", sockfd, port);

	return port;
}


#ifdef INTERNAL_WEB_SERVER
/*!
 * \brief Creates a STREAM socket, binds to INADDR_ANY and listens for
 * incoming connecttions. Returns the actual port which the sockets
 * sub-system returned. 
 *
 * Also creates a DGRAM socket, binds to the loop back address and 
 * returns the port allocated by the socket sub-system.
 *
 * \return
 *	\li UPNP_E_OUTOF_SOCKET: Failed to create a socket.
 *	\li UPNP_E_SOCKET_BIND: Bind() failed.
 *	\li UPNP_E_LISTEN: Listen() failed.	
 *	\li UPNP_E_INTERNAL_ERROR: Port returned by the socket layer is < 0.
 *	\li UPNP_E_SUCCESS: Success.
 */
static int get_miniserver_sockets(
	/*! [in] Socket Array. */
	MiniServerSockArray *out,
	/*! [in] port on which the server is listening for incoming IPv4 connections. */
	unsigned short listen_port4,
	/*! [in] port on which the server is listening for incoming IPv6 connections. */
	unsigned short listen_port6)
{
	char errorBuffer[ERROR_BUFFER_LEN];
	struct sockaddr_storage __ss_v4;
	struct sockaddr_storage __ss_v6;
	struct sockaddr_in* serverAddr4 = (struct sockaddr_in*)&__ss_v4;
	struct sockaddr_in6* serverAddr6 = (struct sockaddr_in6*)&__ss_v6;
	SOCKET listenfd4, listenfd6;
	int ret_code;
	unsigned short actual_port4, actual_port6;
	int reuseaddr_on = 0;
	int sockError = UPNP_E_SUCCESS;
	int errCode = 0;

	// Create listen socket for IPv4/IPv6. An error here may indicate
	// that we don't have an IPv4/IPv6 stack.
	listenfd4 = socket(AF_INET, SOCK_STREAM, 0);
	listenfd6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listenfd4 == INVALID_SOCKET && listenfd6 == INVALID_SOCKET) {
		return UPNP_E_OUTOF_SOCKET;
	}

	// As per the IANA specifications for the use of ports by applications
	// override the listen port passed in with the first available 
	if (listen_port4 < APPLICATION_LISTENING_PORT) {
		listen_port4 = APPLICATION_LISTENING_PORT;
	}
	if (listen_port6 < APPLICATION_LISTENING_PORT) {
		listen_port6 = APPLICATION_LISTENING_PORT;
	}

	memset(&__ss_v4, 0, sizeof (__ss_v4));
	serverAddr4->sin_family = AF_INET;
	serverAddr4->sin_addr.s_addr = htonl(INADDR_ANY);

	memset(&__ss_v6, 0, sizeof (__ss_v6));
	serverAddr6->sin6_family = AF_INET6;
	serverAddr6->sin6_addr = in6addr_any;

	// Getting away with implementation of re-using address:port and instead 
	// choosing to increment port numbers.
	// Keeping the re-use address code as an optional behaviour that can be 
	// turned on if necessary. 
	// TURN ON the reuseaddr_on option to use the option.
	if (reuseaddr_on) {
		// THIS IS ALLOWS US TO BIND AGAIN IMMEDIATELY
		// AFTER OUR SERVER HAS BEEN CLOSED
		// THIS MAY CAUSE TCP TO BECOME LESS RELIABLE
		// HOWEVER IT HAS BEEN SUGESTED FOR TCP SERVERS
		UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
			"get_miniserver_sockets: resuseaddr set\n");

		if (listenfd4 != INVALID_SOCKET) {
			sockError = setsockopt(listenfd4, SOL_SOCKET, SO_REUSEADDR,
				(const char *)&reuseaddr_on, sizeof (int));
			if (sockError == -1) {
				shutdown(listenfd4, SD_BOTH);
				UpnpCloseSocket(listenfd4);
				shutdown(listenfd6, SD_BOTH);
				UpnpCloseSocket(listenfd6);
				return UPNP_E_SOCKET_BIND;
			}

			sockError = bind(listenfd4, (struct sockaddr *)&__ss_v4, 
				sizeof (__ss_v4));
			if (sockError == -1) {
				strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
				UpnpPrintf( UPNP_INFO, MSERV, __FILE__, __LINE__,
					"get_miniserver_sockets: Error in IPv4 bind(): %s\n", 
					errorBuffer);
				shutdown(listenfd4, SD_BOTH);
				UpnpCloseSocket(listenfd4);
				shutdown(listenfd6, SD_BOTH);
				UpnpCloseSocket(listenfd6);
				/* Bind failed */
				return UPNP_E_SOCKET_BIND;
			}
		}

		if(listenfd6 != INVALID_SOCKET) {
			sockError = setsockopt(listenfd6, SOL_SOCKET, SO_REUSEADDR,
			(const char *)&reuseaddr_on, sizeof (int));
			if (sockError == -1) {
				shutdown(listenfd4, SD_BOTH);
				UpnpCloseSocket(listenfd4);
				shutdown(listenfd6, SD_BOTH);
				UpnpCloseSocket(listenfd6);
				return UPNP_E_SOCKET_BIND;
			}

			sockError = bind(listenfd6, (struct sockaddr *)&__ss_v6, sizeof (__ss_v6));
			if (sockError == -1) {
				strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
				UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
					"get_miniserver_sockets: Error in IPv6 bind(): %s\n", 
					errorBuffer);
				shutdown(listenfd4, SD_BOTH);
				UpnpCloseSocket(listenfd4);
				shutdown(listenfd6, SD_BOTH);
				UpnpCloseSocket(listenfd6);
				/* Bind failed */
				return UPNP_E_SOCKET_BIND;
			}
		}
	} else {
		if (listenfd4 != INVALID_SOCKET) {
			do {
				serverAddr4->sin_port = htons(listen_port4++);
				sockError = bind(listenfd4, (struct sockaddr *)&__ss_v4, sizeof(__ss_v4));
				if (sockError == -1) {
#ifdef WIN32
					errCode = WSAGetLastError();
#else
					errCode = errno;
#endif
					if (errno == EADDRINUSE) {
						errCode = 1;
					}
				} else {
					errCode = 0;
				}
			} while ( errCode != 0 );

			if (sockError == -1) {
				strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
				UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
					"get_miniserver_sockets: Error in IPv4 bind(): %s\n",
					errorBuffer);
				shutdown(listenfd4, SD_BOTH);
				UpnpCloseSocket(listenfd4);
				shutdown(listenfd6, SD_BOTH);
				UpnpCloseSocket(listenfd6);
				return UPNP_E_SOCKET_BIND;  // bind failed
			}
		}

		if (listenfd6 != INVALID_SOCKET) {
			do {
				serverAddr6->sin6_port = htons(listen_port6++);
				sockError = bind(listenfd6, (struct sockaddr *)&__ss_v6, sizeof(__ss_v6));
				if (sockError == -1) {
#ifdef WIN32
					errCode = WSAGetLastError();
#else
					errCode = errno; 
#endif
					if (errno == EADDRINUSE) {
						errCode = 1;
					}
				} else {
					errCode = 0;
				}
			} while (errCode != 0);

			if (sockError == -1) {
				strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
				UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
					"get_miniserver_sockets: Error in IPv6 bind(): %s\n",
					errorBuffer);
				shutdown(listenfd4, SD_BOTH);
				UpnpCloseSocket(listenfd4);
				shutdown(listenfd6, SD_BOTH);
				UpnpCloseSocket(listenfd6);
				/* Bind failied */
				return UPNP_E_SOCKET_BIND;
			}
		}
	}

	UpnpPrintf( UPNP_INFO, MSERV, __FILE__, __LINE__,
		"get_miniserver_sockets: bind successful\n" );

	if (listenfd4 != INVALID_SOCKET) {
		ret_code = listen(listenfd4, SOMAXCONN);
		if (ret_code == -1) {
			strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
			UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
				"mserv start: Error in IPv4 listen(): %s\n",
				errorBuffer);
			shutdown(listenfd4, SD_BOTH);
			UpnpCloseSocket(listenfd4);
			shutdown(listenfd6, SD_BOTH);
			UpnpCloseSocket(listenfd6);
			return UPNP_E_LISTEN;
		}

		actual_port4 = get_port(listenfd4);
		if (actual_port4 <= 0) {
			shutdown(listenfd4, SD_BOTH);
			UpnpCloseSocket(listenfd4);
			shutdown(listenfd6, SD_BOTH);
			UpnpCloseSocket(listenfd6);
			return UPNP_E_INTERNAL_ERROR;
		}

		out->miniServerPort4 = actual_port4;
	}

	if (listenfd6 != INVALID_SOCKET) {
		ret_code = listen(listenfd6, SOMAXCONN);
		if (ret_code == -1) {
			strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
			UpnpPrintf(UPNP_INFO, MSERV, __FILE__, __LINE__,
				"mserv start: Error in IPv6 listen(): %s\n",
				errorBuffer);
			shutdown(listenfd4, SD_BOTH);
			UpnpCloseSocket(listenfd4);
			shutdown(listenfd6, SD_BOTH);
			UpnpCloseSocket(listenfd6);
			return UPNP_E_LISTEN;
		}

		actual_port6 = get_port(listenfd6);
		if (actual_port6 <= 0) {
			shutdown(listenfd4, SD_BOTH);
			UpnpCloseSocket(listenfd4);
			shutdown(listenfd6, SD_BOTH);
			UpnpCloseSocket(listenfd6);
			return UPNP_E_INTERNAL_ERROR;
		}

		out->miniServerPort6 = actual_port6;
	}

	out->miniServerSock4 = listenfd4;
	out->miniServerSock6 = listenfd6;

	return UPNP_E_SUCCESS;
}
#endif /* INTERNAL_WEB_SERVER */


/*!
 * \brief Creates the miniserver STOP socket. This socket is created and 
 *  listened on to know when it is time to stop the Miniserver.
 *
 * \return 
 *	\li UPNP_E_OUTOF_SOCKET: Failed to create a socket.
 *	\li UPNP_E_SOCKET_BIND: Bind() failed.
 *	\li UPNP_E_INTERNAL_ERROR: Port returned by the socket layer is < 0.
 *	\li UPNP_E_SUCCESS: Success.
 */
static int get_miniserver_stopsock(
	/*! [in] Miniserver Socket Array. */
	MiniServerSockArray *out)
{
	char errorBuffer[ERROR_BUFFER_LEN];
	struct sockaddr_in stop_sockaddr;
	int miniServerStopSock = 0;
	int ret = 0;

	miniServerStopSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (miniServerStopSock == -1) {
		strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
		UpnpPrintf(UPNP_CRITICAL, MSERV, __FILE__, __LINE__,
			"Error in socket(): %s\n", errorBuffer);
		return UPNP_E_OUTOF_SOCKET;
	}

	// bind to local socket
	memset(&stop_sockaddr, 0, sizeof (stop_sockaddr));
	stop_sockaddr.sin_family = AF_INET;
	stop_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = bind(miniServerStopSock, (struct sockaddr *)&stop_sockaddr,
		sizeof(stop_sockaddr));
	if (ret == -1) {
		UpnpPrintf(UPNP_CRITICAL,
		MSERV, __FILE__, __LINE__,
			"Error in binding localhost!!!\n");
		shutdown(miniServerStopSock, SD_BOTH);
		UpnpCloseSocket(miniServerStopSock);
		return UPNP_E_SOCKET_BIND;
	}

	miniStopSockPort = get_port( miniServerStopSock );
	if (miniStopSockPort <= 0) {
		shutdown(miniServerStopSock, SD_BOTH);
		UpnpCloseSocket(miniServerStopSock);
		return UPNP_E_INTERNAL_ERROR;
	}

	out->miniServerStopSock = miniServerStopSock;
	out->stopPort = miniStopSockPort;

	return UPNP_E_SUCCESS;
}


int StartMiniServer(
	/*! [in,out] Port on which the server listens for incoming IPv4 connections. */
	unsigned short *listen_port4, 
	/*! [in,out] Port on which the server listens for incoming IPv6 connections. */
	unsigned short *listen_port6)
{
	int ret_code;
	int count;
	int max_count = 10000;

	MiniServerSockArray *miniSocket;
	ThreadPoolJob job;

	if (gMServState != MSERV_IDLE) {
		return UPNP_E_INTERNAL_ERROR;   // miniserver running
	}

	miniSocket = (MiniServerSockArray *)malloc(sizeof (MiniServerSockArray));
	if (miniSocket == NULL) {
		return UPNP_E_OUTOF_MEMORY;
	}
	memset(miniSocket, 0, sizeof (*miniSocket));

#ifdef INTERNAL_WEB_SERVER
	// V4 and V6 http listeners.
	ret_code = get_miniserver_sockets(miniSocket, *listen_port4, *listen_port6);
	if (ret_code != UPNP_E_SUCCESS) {
		free(miniSocket);
		return ret_code;
	}
#endif

	// Stop socket (To end miniserver processing).
	ret_code = get_miniserver_stopsock(miniSocket);
	if (ret_code != UPNP_E_SUCCESS) {
#ifdef INTERNAL_WEB_SERVER
		shutdown(miniSocket->miniServerSock4, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerSock4);
		shutdown(miniSocket->miniServerSock6, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerSock6);
#endif
		free(miniSocket);
		return ret_code;
	}

	// SSDP socket for discovery / advertising.
	ret_code = get_ssdp_sockets(miniSocket);
	if (ret_code != UPNP_E_SUCCESS) {
#ifdef INTERNAL_WEB_SERVER
		shutdown(miniSocket->miniServerSock4, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerSock4);
		shutdown(miniSocket->miniServerSock6, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerSock6);
#endif
		shutdown(miniSocket->miniServerStopSock, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerStopSock);
		free(miniSocket);
		return ret_code;
	}

	TPJobInit(&job, (start_routine)RunMiniServer, (void *)miniSocket);
	TPJobSetPriority(&job, MED_PRIORITY);
	TPJobSetFreeFunction(&job, (free_routine)free);
	ret_code = ThreadPoolAddPersistent(&gMiniServerThreadPool, &job, NULL);
	if (ret_code < 0) {
#ifdef INTERNAL_WEB_SERVER
		shutdown(miniSocket->miniServerSock4, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerSock4);
		shutdown(miniSocket->miniServerSock6, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerSock6);
#endif
		shutdown(miniSocket->miniServerStopSock, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerStopSock);
		shutdown(miniSocket->ssdpSock4, SD_BOTH);
		UpnpCloseSocket(miniSocket->ssdpSock4);
		shutdown(miniSocket->ssdpSock6, SD_BOTH);
		UpnpCloseSocket(miniSocket->ssdpSock6);
#ifdef INCLUDE_CLIENT_APIS
		shutdown(miniSocket->ssdpReqSock4, SD_BOTH );
		UpnpCloseSocket(miniSocket->ssdpReqSock4 );
		shutdown(miniSocket->ssdpReqSock6, SD_BOTH );
		UpnpCloseSocket(miniSocket->ssdpReqSock6 );
#endif
		return UPNP_E_OUTOF_MEMORY;
	}
	// wait for miniserver to start
	count = 0;
	while (gMServState != MSERV_RUNNING && count < max_count) {
		usleep(50 * 1000);    // 0.05s
		count++;
	}

	// taking too long to start that thread
	if (count >= max_count) {
#ifdef INTERNAL_WEB_SERVER
		shutdown(miniSocket->miniServerSock4, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerSock4);
		shutdown(miniSocket->miniServerSock6, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerSock6);
#endif
		shutdown(miniSocket->miniServerStopSock, SD_BOTH);
		UpnpCloseSocket(miniSocket->miniServerStopSock);
		shutdown(miniSocket->ssdpSock4, SD_BOTH);
		UpnpCloseSocket(miniSocket->ssdpSock4);
		shutdown(miniSocket->ssdpSock6, SD_BOTH);
		UpnpCloseSocket(miniSocket->ssdpSock6);
#ifdef INCLUDE_CLIENT_APIS
		shutdown(miniSocket->ssdpReqSock4, SD_BOTH);
		UpnpCloseSocket(miniSocket->ssdpReqSock4);
		shutdown(miniSocket->ssdpReqSock6, SD_BOTH);
		UpnpCloseSocket(miniSocket->ssdpReqSock6);
#endif
		return UPNP_E_INTERNAL_ERROR;
	}
#ifdef INTERNAL_WEB_SERVER
	*listen_port4 = miniSocket->miniServerPort4;
	*listen_port6 = miniSocket->miniServerPort6;
#endif

	return UPNP_E_SUCCESS;
}


int StopMiniServer()
{
	char errorBuffer[ERROR_BUFFER_LEN];
	int socklen = sizeof (struct sockaddr_in);
	SOCKET sock;
	struct sockaddr_in ssdpAddr;
	char buf[256] = "ShutDown";
	int bufLen = strlen(buf);

	if(gMServState == MSERV_RUNNING) {
		gMServState = MSERV_STOPPING;
	} else {
		return 0;
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
		UpnpPrintf(UPNP_INFO, SSDP, __FILE__, __LINE__,
			"SSDP_SERVER: StopSSDPServer: Error in socket() %s\n",
			errorBuffer);
		return 0;
	}

	while(gMServState != MSERV_IDLE) {
		ssdpAddr.sin_family = AF_INET;
		ssdpAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		ssdpAddr.sin_port = htons(miniStopSockPort);
		sendto(sock, buf, bufLen, 0, (struct sockaddr *)&ssdpAddr,
			socklen);
		usleep(1000);
		if (gMServState == MSERV_IDLE) {
			break;
		}
		isleep(1);
	}
	shutdown(sock, SD_BOTH);
	UpnpCloseSocket(sock);

	return 0;
}

