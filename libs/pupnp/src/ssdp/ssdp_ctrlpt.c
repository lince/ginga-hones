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


#include "util.h"


#ifdef INCLUDE_CLIENT_APIS
#if EXCLUDE_SSDP == 0


#include "httpparser.h"
#include "httpreadwrite.h"
#include "ssdp_ResultData.h"
#include "ssdplib.h"
#include "statcodes.h"
#include "unixutil.h"
#include "upnpapi.h"
#include "UpnpInet.h"
#include "ThreadPool.h"


#include <stdio.h>


#ifdef WIN32
	#include <string.h>
#endif /* WIN32 */


/************************************************************************
 * Function: send_search_result
 *
 * Parameters:
 *	IN void *data: Search reply from the device
 *
 * Description:
 *	This function sends a callback to the control point application with 
 *	a SEARCH result
 *
 * Returns: void
 *
 ***************************************************************************/
void send_search_result(IN void *data)
{
	SSDPResultData *temp = (SSDPResultData *)data;

	SSDPResultData_Callback(temp);
	SSDPResultData_delete(temp);
}

/************************************************************************
 * Function: ssdp_handle_ctrlpt_msg
 *
 * Parameters:
 *	IN http_message_t *hmsg:
 *		SSDP message from the device
 *	IN struct sockaddr *dest_addr:
 *		Address of the device
 *	IN xboolean timeout:
 *		timeout kept by the control point while
 *		sending search message
 *	IN void* cookie:
 *		Cookie stored by the control point application. 
 *		This cookie will be returned to the control point
 *		in the callback 
 *
 * Description:
 *	This function handles the ssdp messages from the devices. These 
 *	messages includes the search replies, advertisement of device coming 
 *	alive and bye byes.
 *
 * Returns: void
 *
 ***************************************************************************/
void ssdp_handle_ctrlpt_msg(
	IN http_message_t *hmsg,
	IN struct sockaddr *dest_addr,
	IN xboolean timeout, // only in search reply
	IN void *cookie)    // only in search reply
{
    int handle;
    struct Handle_Info *ctrlpt_info = NULL;
    memptr hdr_value;
    xboolean is_byebye;         // byebye or alive
    UpnpDiscovery *param = UpnpDiscovery_new();
    int expires;
    int ret;
    SsdpEvent event;
    xboolean nt_found;
    xboolean usn_found;
    xboolean st_found;
    char save_char;
    Upnp_EventType event_type;
    Upnp_FunPtr ctrlpt_callback;
    void *ctrlpt_cookie;
    ListNode *node = NULL;
    SsdpSearchArg *searchArg = NULL;
    int matched = 0;
    SSDPResultData *threadData = NULL;
    ThreadPoolJob job;

    // we are assuming that there can be only one client supported at a time

    HandleReadLock();

    if ( GetClientHandleInfo( &handle, &ctrlpt_info ) != HND_CLIENT ) {
        HandleUnlock();
	goto end_ssdp_handle_ctrlpt_msg;
    }
    // copy
    ctrlpt_callback = ctrlpt_info->Callback;
    ctrlpt_cookie = ctrlpt_info->Cookie;
    HandleUnlock();

    // search timeout
    if ( timeout ) {
        ctrlpt_callback( UPNP_DISCOVERY_SEARCH_TIMEOUT, NULL, cookie );
	goto end_ssdp_handle_ctrlpt_msg;
    }

    UpnpDiscovery_set_ErrCode(param, UPNP_E_SUCCESS);

    // MAX-AGE
    // assume error
    expires = -1;
    UpnpDiscovery_set_Expires(param, expires);
    if ( httpmsg_find_hdr( hmsg, HDR_CACHE_CONTROL, &hdr_value ) != NULL ) {
        ret = matchstr( hdr_value.buf, hdr_value.length, "%imax-age = %d%0", &expires );
        UpnpDiscovery_set_Expires(param, expires);
        if( ret != PARSE_OK ) {
            goto end_ssdp_handle_ctrlpt_msg;
	}
    }

    // DATE
    if ( httpmsg_find_hdr( hmsg, HDR_DATE, &hdr_value ) != NULL ) {
        UpnpDiscovery_strcpy_Date(param, hdr_value.buf);
    }

    // dest addr
    UpnpDiscovery_set_DestAddr(param, dest_addr);

    // EXT
    if ( httpmsg_find_hdr( hmsg, HDR_EXT, &hdr_value ) != NULL ) {
    	UpnpDiscovery_strncpy_Ext(param, hdr_value.buf, hdr_value.length);
    }
    // LOCATION
    if ( httpmsg_find_hdr( hmsg, HDR_LOCATION, &hdr_value ) != NULL ) {
        UpnpDiscovery_strncpy_Location(param, hdr_value.buf, hdr_value.length);
    }
    // SERVER / USER-AGENT
    if ( httpmsg_find_hdr( hmsg, HDR_SERVER, &hdr_value ) != NULL ||
        httpmsg_find_hdr( hmsg, HDR_USER_AGENT, &hdr_value ) != NULL ) {
        UpnpDiscovery_strncpy_Os(param, hdr_value.buf, hdr_value.length);
    }
    // clear everything
    event.UDN[0] = '\0';
    event.DeviceType[0] = '\0';
    event.ServiceType[0] = '\0';

    nt_found = FALSE;

    if ( httpmsg_find_hdr( hmsg, HDR_NT, &hdr_value ) != NULL ) {
        save_char = hdr_value.buf[hdr_value.length];
        hdr_value.buf[hdr_value.length] = '\0';
        nt_found = ( ssdp_request_type( hdr_value.buf, &event ) == 0 );
        hdr_value.buf[hdr_value.length] = save_char;
    }

    usn_found = FALSE;
    if ( httpmsg_find_hdr( hmsg, HDR_USN, &hdr_value ) != NULL ) {
        save_char = hdr_value.buf[hdr_value.length];
        hdr_value.buf[hdr_value.length] = '\0';
        usn_found = ( unique_service_name( hdr_value.buf, &event ) == 0 );
        hdr_value.buf[hdr_value.length] = save_char;
    }

    if ( nt_found || usn_found ) {
        UpnpDiscovery_strcpy_DeviceID(   param, event.UDN);
        UpnpDiscovery_strcpy_DeviceType( param, event.DeviceType);
        UpnpDiscovery_strcpy_ServiceType(param, event.ServiceType);
    }

    // ADVERT. OR BYEBYE
    if( hmsg->is_request ) {
        // use NTS hdr to determine advert., or byebye
        if ( httpmsg_find_hdr( hmsg, HDR_NTS, &hdr_value ) == NULL ) {
            // error; NTS header not found
            goto end_ssdp_handle_ctrlpt_msg;
        }
        if ( memptr_cmp( &hdr_value, "ssdp:alive" ) == 0 ) {
            is_byebye = FALSE;
        } else if( memptr_cmp( &hdr_value, "ssdp:byebye" ) == 0 ) {
            is_byebye = TRUE;
        } else {
            // bad value
            goto end_ssdp_handle_ctrlpt_msg;
        }

        if ( is_byebye ) {
            // check device byebye
            if( !nt_found || !usn_found ) {
                // bad byebye
                goto end_ssdp_handle_ctrlpt_msg;
            }
            event_type = UPNP_DISCOVERY_ADVERTISEMENT_BYEBYE;
        } else {
            // check advertisement      
            // .Expires is valid if positive. This is for testing
            //  only. Expires should be greater than 1800 (30 mins)
            if( !nt_found ||
                !usn_found ||
                UpnpString_get_Length(UpnpDiscovery_get_Location(param)) == 0 ||
		UpnpDiscovery_get_Expires(param) <= 0 ) {
                // bad advertisement
                goto end_ssdp_handle_ctrlpt_msg;
            }
            event_type = UPNP_DISCOVERY_ADVERTISEMENT_ALIVE;
        }

        // call callback
        ctrlpt_callback( event_type, param, ctrlpt_cookie );
    } else {
        // reply (to a SEARCH)
	//
        // only checking to see if there is a valid ST header
        st_found = FALSE;
        if( httpmsg_find_hdr( hmsg, HDR_ST, &hdr_value ) != NULL ) {
            save_char = hdr_value.buf[hdr_value.length];
            hdr_value.buf[hdr_value.length] = '\0';
            st_found = ssdp_request_type( hdr_value.buf, &event ) == 0;
            hdr_value.buf[hdr_value.length] = save_char;
        }
        if( hmsg->status_code != HTTP_OK ||
            UpnpDiscovery_get_Expires(param) <= 0 ||
            UpnpString_get_Length(UpnpDiscovery_get_Location(param)) == 0 ||
	    !usn_found ||
	    !st_found ) {
            // bad reply
            goto end_ssdp_handle_ctrlpt_msg;
        }
        // check each current search
        HandleLock();
        if( GetClientHandleInfo( &handle, &ctrlpt_info ) != HND_CLIENT ) {
            HandleUnlock();
            goto end_ssdp_handle_ctrlpt_msg;
        }
        node = ListHead( &ctrlpt_info->SsdpSearchList );

        // temporary add null termination
        //save_char = hdr_value.buf[ hdr_value.length ];
        //hdr_value.buf[ hdr_value.length ] = '\0';

        while( node != NULL ) {
            searchArg = node->item;
            matched = 0;
            // check for match of ST header and search target
            switch ( searchArg->requestType ) {
            case SSDP_ALL:
                        matched = 1;
                        break;
            case SSDP_ROOTDEVICE:
                        matched = ( event.RequestType == SSDP_ROOTDEVICE );
                        break;
            case SSDP_DEVICEUDN:
                        matched = !( strncmp( searchArg->searchTarget,
                                              hdr_value.buf,
                                              hdr_value.length ) );
                        break;
            case SSDP_DEVICETYPE: {
                        int m = min( hdr_value.length,
                                     strlen( searchArg->searchTarget ) );
                        matched = !( strncmp( searchArg->searchTarget,
                                              hdr_value.buf, m ) );
                        break;
            }
            case SSDP_SERVICE: {
                        int m = min( hdr_value.length,
                                     strlen( searchArg->searchTarget ) );

                        matched = !( strncmp( searchArg->searchTarget,
                                              hdr_value.buf, m ) );
                        break;
            }
            default:
                        matched = 0;
                        break;
            }

            if (matched) {
                // schedule call back
		threadData = SSDPResultData_new();
                if (threadData != NULL) {
                    SSDPResultData_set_Param(threadData, param);
                    SSDPResultData_set_Cookie(threadData, searchArg->cookie);
                    SSDPResultData_set_CtrlptCallback(threadData, ctrlpt_callback);
                    TPJobInit(&job, (start_routine)send_search_result, threadData);
                    TPJobSetPriority(&job, MED_PRIORITY);
                    TPJobSetFreeFunction(&job, (free_routine)SSDPResultData_delete);
                    ThreadPoolAdd(&gRecvThreadPool, &job, NULL);
                }
            }
            node = ListNext( &ctrlpt_info->SsdpSearchList, node );
        }

        HandleUnlock();
        //ctrlpt_callback( UPNP_DISCOVERY_SEARCH_RESULT, param, cookie );
    }
end_ssdp_handle_ctrlpt_msg:
    UpnpDiscovery_delete(param);
}


/************************************************************************
* Function : CreateClientRequestPacket
*
* Parameters:
*	IN char * RqstBuf:Output string in HTTP format.
*	IN char *SearchTarget:Search Target
*	IN int Mx dest_addr: Number of seconds to wait to 
*		collect all the responses
*	IN int AddressFamily: search address family
*
* Description:
*	This function creates a HTTP search request packet 
* 	depending on the input parameter.
*
* Returns: void
*
***************************************************************************/
static void
CreateClientRequestPacket( IN char *RqstBuf,
                           IN int Mx,
                           IN char *SearchTarget,
                           IN int AddressFamily )
{
    char TempBuf[COMMAND_LEN];

    strcpy( RqstBuf, "M-SEARCH * HTTP/1.1\r\n" );

    if (AddressFamily == AF_INET) {
        sprintf( TempBuf, "HOST: %s:%d\r\n", SSDP_IP, SSDP_PORT );
    } else if (AddressFamily == AF_INET6) {
        sprintf( TempBuf, "HOST: [%s]:%d\r\n", SSDP_IPV6_LINKLOCAL, SSDP_PORT );
    }
    strcat( RqstBuf, TempBuf );
    strcat( RqstBuf, "MAN: \"ssdp:discover\"\r\n" );

    if( Mx > 0 ) {
        sprintf( TempBuf, "MX: %d\r\n", Mx );
        strcat( RqstBuf, TempBuf );
    }

    if( SearchTarget != NULL ) {
        sprintf( TempBuf, "ST: %s\r\n", SearchTarget );
        strcat( RqstBuf, TempBuf );
    }
    strcat( RqstBuf, "\r\n" );
}

/************************************************************************
* Function : searchExpired
*
* Parameters:
*		IN void * arg:
*
* Description:
*	This function 
*
* Returns: void
*
***************************************************************************/
void
searchExpired( void *arg )
{

    int *id = ( int * )arg;
    int handle = -1;
    struct Handle_Info *ctrlpt_info = NULL;

    //remove search Target from list and call client back
    ListNode *node = NULL;
    SsdpSearchArg *item;
    Upnp_FunPtr ctrlpt_callback;
    void *cookie = NULL;
    int found = 0;

    HandleLock();

    //remove search target from search list

    if( GetClientHandleInfo( &handle, &ctrlpt_info ) != HND_CLIENT ) {
        free( id );
        HandleUnlock();
        return;
    }

    ctrlpt_callback = ctrlpt_info->Callback;

    node = ListHead( &ctrlpt_info->SsdpSearchList );

    while( node != NULL ) {
        item = ( SsdpSearchArg * ) node->item;
        if( item->timeoutEventId == ( *id ) ) {
            free( item->searchTarget );
            cookie = item->cookie;
            found = 1;
            item->searchTarget = NULL;
            free( item );
            ListDelNode( &ctrlpt_info->SsdpSearchList, node, 0 );
            break;
        }
        node = ListNext( &ctrlpt_info->SsdpSearchList, node );
    }
    HandleUnlock();

    if( found ) {
        ctrlpt_callback( UPNP_DISCOVERY_SEARCH_TIMEOUT, NULL, cookie );
    }

    free( id );
}

/************************************************************************
* Function : SearchByTarget
*
* Parameters:
*	IN int Mx:Number of seconds to wait, to collect all the	responses.
*	IN char *St: Search target.
*	IN void *Cookie: cookie provided by control point application.
*		This cokie will be returned to application in the callback.
*
* Description:
*   This function implements the search request of the discovery phase.
*   A M-SEARCH request is sent on the SSDP channel for both IPv4 and
*   IPv6 addresses. The search target(ST) is required and must be one of
*   the following:
*       - "ssdp:all" : Search for all devices and services.
*       - "ssdp:rootdevice" : Search for root devices only.
*       - "uuid:<device-uuid>" : Search for a particular device.
*       - "urn:schemas-upnp-org:device:<deviceType:v>"
*       - "urn:schemas-upnp-org:service:<serviceType:v>"
*       - "urn:<domain-name>:device:<deviceType:v>"
*       - "urn:<domain-name>:service:<serviceType:v>"
*
* Returns: int
*	1 if successful else appropriate error
***************************************************************************/
int
SearchByTarget( IN int Mx,
                IN char *St,
                IN void *Cookie )
{
    char errorBuffer[ERROR_BUFFER_LEN];
    int socklen = sizeof( struct sockaddr_storage );
    int *id = NULL;
    int ret = 0;
    char ReqBufv4[BUFSIZE];
    char ReqBufv6[BUFSIZE];
    struct sockaddr_storage __ss_v4;
    struct sockaddr_storage __ss_v6;
    struct sockaddr_in* destAddr4 = (struct sockaddr_in*)&__ss_v4;
    struct sockaddr_in6* destAddr6 = (struct sockaddr_in6*)&__ss_v6;
    fd_set wrSet;
    SsdpSearchArg *newArg = NULL;
    int timeTillRead = 0;
    int handle;
    struct Handle_Info *ctrlpt_info = NULL;
    enum SsdpSearchType requestType;
    unsigned long addrv4 = inet_addr( gIF_IPV4 );
    int max_fd = 0;

    //ThreadData *ThData;
    ThreadPoolJob job;

    requestType = ssdp_request_type1( St );
    if( requestType == SSDP_SERROR ) {
        return UPNP_E_INVALID_PARAM;
    }

    UpnpPrintf(UPNP_INFO, SSDP, __FILE__, __LINE__, "Inside SearchByTarget\n");

    timeTillRead = Mx;

    if( timeTillRead < MIN_SEARCH_TIME ) {
        timeTillRead = MIN_SEARCH_TIME;
    } else if( timeTillRead > MAX_SEARCH_TIME ) {
        timeTillRead = MAX_SEARCH_TIME;
    }

    CreateClientRequestPacket( ReqBufv4, timeTillRead, St, AF_INET );
    CreateClientRequestPacket( ReqBufv6, timeTillRead, St, AF_INET6 );

    memset( &__ss_v4, 0, sizeof( __ss_v4 ) );
    destAddr4->sin_family = AF_INET;
    inet_pton( AF_INET, SSDP_IP, &destAddr4->sin_addr );
    destAddr4->sin_port = htons( SSDP_PORT );

    memset( &__ss_v6, 0, sizeof( __ss_v6 ) );
    destAddr6->sin6_family = AF_INET6;
    inet_pton( AF_INET6, SSDP_IPV6_LINKLOCAL, &destAddr6->sin6_addr );
    destAddr6->sin6_port = htons( SSDP_PORT );
    destAddr6->sin6_scope_id = gIF_INDEX;

    // add search criteria to list
    HandleLock();
    if( GetClientHandleInfo( &handle, &ctrlpt_info ) != HND_CLIENT ) {
        HandleUnlock();
        return UPNP_E_INTERNAL_ERROR;
    }

    newArg = ( SsdpSearchArg * ) malloc( sizeof( SsdpSearchArg ) );
    newArg->searchTarget = strdup( St );
    newArg->cookie = Cookie;
    newArg->requestType = requestType;

    id = ( int * )malloc( sizeof( int ) );
    TPJobInit( &job, ( start_routine ) searchExpired, id );
    TPJobSetPriority( &job, MED_PRIORITY );
    TPJobSetFreeFunction( &job, ( free_routine ) free );

    // Schedule a timeout event to remove search Arg
    TimerThreadSchedule( &gTimerThread, timeTillRead,
                         REL_SEC, &job, SHORT_TERM, id );
    newArg->timeoutEventId = ( *id );

    ListAddTail( &ctrlpt_info->SsdpSearchList, newArg );
    HandleUnlock();

    FD_ZERO( &wrSet );
    if( gSsdpReqSocket4 != INVALID_SOCKET ) {
        setsockopt( gSsdpReqSocket4, IPPROTO_IP, IP_MULTICAST_IF,
            (char *)&addrv4, sizeof (addrv4) );
        FD_SET( gSsdpReqSocket4, &wrSet );
        max_fd = max(max_fd, gSsdpReqSocket4);
    }
    if( gSsdpReqSocket6 != INVALID_SOCKET ) {
        setsockopt( gSsdpReqSocket6, IPPROTO_IPV6, IPV6_MULTICAST_IF,
            (char *)&gIF_INDEX, sizeof(gIF_INDEX) );
        FD_SET( gSsdpReqSocket6, &wrSet );
        max_fd = max(max_fd, gSsdpReqSocket6);
    }

    ret = select( max_fd + 1, NULL, &wrSet, NULL, NULL );
    if( ret == -1 ) {
        strerror_r(errno, errorBuffer, ERROR_BUFFER_LEN);
        UpnpPrintf( UPNP_INFO, SSDP, __FILE__, __LINE__,
            "SSDP_LIB: Error in select(): %s\n",
            errorBuffer );
	    shutdown( gSsdpReqSocket4, SD_BOTH );
        UpnpCloseSocket( gSsdpReqSocket4 );
	    shutdown( gSsdpReqSocket6, SD_BOTH );
        UpnpCloseSocket( gSsdpReqSocket6 );

        return UPNP_E_INTERNAL_ERROR;
    } 
    if( gSsdpReqSocket6 != INVALID_SOCKET && 
        FD_ISSET( gSsdpReqSocket6, &wrSet ) ) {
        int NumCopy = 0;
        while( NumCopy < NUM_SSDP_COPY ) {
            sendto( gSsdpReqSocket6, ReqBufv6, strlen( ReqBufv6 ), 0,
                (struct sockaddr *)&__ss_v6, socklen );
            NumCopy++;
            imillisleep( SSDP_PAUSE );
        }
    }
    if( gSsdpReqSocket4 != INVALID_SOCKET && 
        FD_ISSET( gSsdpReqSocket4, &wrSet ) ) {
        int NumCopy = 0;
        while( NumCopy < NUM_SSDP_COPY ) {
            sendto( gSsdpReqSocket4, ReqBufv4, strlen( ReqBufv4 ), 0,
                (struct sockaddr *)&__ss_v4, socklen );
            NumCopy++;
            imillisleep( SSDP_PAUSE );
        }
    } 

    return 1;
}

#endif // EXCLUDE_SSDP
#endif // INCLUDE_CLIENT_APIS
