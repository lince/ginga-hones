/*******************************************************************************
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
 ******************************************************************************/


/*!
 * \file
 *
 * \brief Contains functions for uri, url parsing utility.
 */


#ifdef __FreeBSD__
	#include <osreldate.h>
	#if __FreeBSD_version < 601103
		#include <lwres/netdb.h>
	#endif
#endif


#include "config.h"


#include "uri.h"


#include "upnpapi.h"


/*!
 * \brief Returns a 1 if a char is a RESERVED char as defined in 
 * http://www.ietf.org/rfc/rfc2396.txt RFC explaining URIs).
 *
 * \return 1 if char is a RESERVED char.
 */
static int is_reserved(
	/*! [in] Char to be matched for RESERVED characters. */
	char in)
{
	if (strchr(RESERVED, in)) {
		return 1;
	} else {
		return 0;
	}
}


/*!
 * \brief Returns a 1 if a char is a MARK char as defined in
 * http://www.ietf.org/rfc/rfc2396.txt (RFC explaining URIs).
 *
 * \return 1 if char is a MARKED char.
 */
int is_mark(
	/*! [in] Char to be matched for MARKED characters. */
	char in)
{
	if (strchr(MARK, in)) {
		return 1;
	} else {
		return 0;
	}
}


/*!
 * \brief Returns a 1 if a char is an UNRESERVED char as defined in
 * http://www.ietf.org/rfc/rfc2396.txt (RFC explaining URIs).
 *
 * \return 1 if char is a UNRESERVED char.
 */
int is_unreserved(
	/*! [in] Char to be matched for UNRESERVED characters. */
	char in)
{
	if (isalnum(in) || is_mark(in)) {
		return 1;
	} else {
		return 0;
	}
}


/*!
 * \brief Returns a 1 if a char[3] sequence is ESCAPED as defined in
 * http://www.ietf.org/rfc/rfc2396.txt (RFC explaining URIs).
 *
 * Size of array is NOT checked (MUST be checked by caller).
 *
 * \return 1 if char is a ESCAPED char.
 */
int is_escaped(
	/*! [in] Char sequence to be matched for ESCAPED characters. */
	const char *in)
{
	if (in[0] == '%' && isxdigit(in[1]) && isxdigit(in[2])) {
		return 1;
	} else {
		return 0;
	}
}


int replace_escaped(char *in, int index, size_t *max)
{
	int tempInt = 0;
	char tempChar = 0;
	int i = 0;
	int j = 0;

	if (in[index] == '%' && isxdigit(in[index + 1]) && isxdigit(in[index + 2])) {
		/* Note the "%2x", makes sure that we convert a maximum of two
		 * characters. */
		if (sscanf(&in[index + 1], "%2x", &tempInt) != 1) {
			return 0;
		}

		tempChar = ( char )tempInt;
		for (i = index + 3, j = index; j < *max; i++, j++) {
			in[j] = tempChar;
			if (i < *max) {
				tempChar = in[i];
			} else {
				tempChar = 0;
			}
		}
		*max -= 2;
		return 1;
	} else {
		return 0;
	}
}


/*!
 * \brief Parses a string of uric characters starting at in[0] as defined in
 * http://www.ietf.org/rfc/rfc2396.txt (RFC explaining URIs).
 *
 * \return 
 */
static int parse_uric(
	/*! [in] String of characters. */
	const char *in,
	/*! [in] Maximum limit. */
	int max,
	/*! [out] Token object where the string of characters is copied. */
	token *out)
{
	int i = 0;

	while (i < max &&
	       (is_unreserved(in[i]) ||
	        is_reserved(in[i])   ||
	        ((i + 2 < max) && is_escaped(&in[i])))) {
		i++;
	}

	out->size = i;
	out->buff = in;
	return i;
}


/*!
 * \brief Tokens are generally pointers into other strings. This copies the
 * offset and size from a token (in) relative to one string (in_base) into
 * a token (out) relative to another string (out_base).
 */
static void copy_token(
	/*! [in] Source token. */
	const token *in,
	/*! [in] . */
	const char *in_base,
	/*! [out] Destination token. */
	token *out,
	/*! [in] . */
	char *out_base)
{
	out->size = in->size;
	out->buff = out_base + (in->buff - in_base);
}


int copy_URL_list(URL_list *in, URL_list *out)
{
    int len = strlen( in->URLs ) + 1;
    int i = 0;

    out->URLs = NULL;
    out->parsedURLs = NULL;
    out->size = 0;

    out->URLs = ( char * )malloc( len );
    out->parsedURLs =
        ( uri_type * ) malloc( sizeof( uri_type ) * in->size );

    if( ( out->URLs == NULL ) || ( out->parsedURLs == NULL ) )
        return UPNP_E_OUTOF_MEMORY;

    memcpy( out->URLs, in->URLs, len );

    for( i = 0; i < in->size; i++ ) {
        //copy the parsed uri
        out->parsedURLs[i].type = in->parsedURLs[i].type;
        copy_token( &in->parsedURLs[i].scheme, in->URLs,
                    &out->parsedURLs[i].scheme, out->URLs );

        out->parsedURLs[i].path_type = in->parsedURLs[i].path_type;
        copy_token( &in->parsedURLs[i].pathquery, in->URLs,
                    &out->parsedURLs[i].pathquery, out->URLs );
        copy_token( &in->parsedURLs[i].fragment, in->URLs,
                    &out->parsedURLs[i].fragment, out->URLs );
        copy_token( &in->parsedURLs[i].hostport.text,
                    in->URLs, &out->parsedURLs[i].hostport.text,
                    out->URLs );

        memcpy( &out->parsedURLs[i].hostport.IPaddress,
            &in->parsedURLs[i].hostport.IPaddress, 
            sizeof(struct sockaddr_storage) );
    }
    out->size = in->size;

    return HTTP_SUCCESS;
}


void free_URL_list(URL_list *list)
{
	if (list->URLs) {
		free(list->URLs);
	}
	if (list->parsedURLs) {
		free(list->parsedURLs);
	}
	list->size = 0;
}


#ifdef DEBUG
void print_uri(uri_type *in)
{
	print_token(&in->scheme);
	print_token(&in->hostport.text);
	print_token(&in->pathquery);
	print_token(&in->fragment);
}
#endif /* DEBUG */


#ifdef DEBUG
void print_token(token * in)
{
    int i = 0;
    printf( "Token Size : %"PRIzu"\n\'", in->size );
    for( i = 0; i < in->size; i++ ) {
        putchar( in->buff[i] );
    }
    putchar( '\'' );
    putchar( '\n' );
}
#endif /* DEBUG */


int token_string_casecmp(token *in1, char *in2)
{
	int in2_length = strlen(in2);

	if (in1->size != in2_length) {
		return 1;
	} else {
		return strncasecmp(in1->buff, in2, in1->size);
	}
}


int token_string_cmp(token * in1, char *in2)
{
	int in2_length = strlen(in2);

	if (in1->size != in2_length) {
		return 1;
	} else {
		return strncmp(in1->buff, in2, in1->size);
	}
}


int token_cmp(token *in1, token *in2)
{
	if (in1->size != in2->size) {
		return 1;
	} else {
		return memcmp(in1->buff, in2->buff, in1->size);
	}
}


int parse_hostport(
	const char *in,
	int max,
	hostport_type *out)
{
    char workbuf[256];
    char* c;
    struct sockaddr_in* sai4 = (struct sockaddr_in*)&out->IPaddress;
    struct sockaddr_in6* sai6 = (struct sockaddr_in6*)&out->IPaddress;
    char *srvname = NULL;
    char *srvport = NULL;
    char *last_dot = NULL;
    unsigned short int port;
    int af = AF_UNSPEC;
    int hostport_size;
    int has_port = 0;
    int ret;

    memset( out, 0, sizeof(hostport_type) );

    // Work on a copy of the input string.
    strncpy( workbuf, in, sizeof(workbuf) );

    c = workbuf;
    if( *c == '[' ) {
        // IPv6 addresses are enclosed in square brackets.
        srvname = ++c;
        while( *c != '\0'  &&  *c != ']' ) {
            c++;
        }
        if( *c == '\0' ) {
            // did not find closing bracket.
            return UPNP_E_INVALID_URL;
        }
        // NULL terminate the srvname and then increment c.
        *c++ = '\0';    // overwrite the ']'
        if( *c == ':' ) {
            has_port = 1;
            c++;
        }
        af = AF_INET6;
    }
    else {
        // IPv4 address -OR- host name.
        srvname = c;
        while( (*c != ':') && (*c != '/') && ( (isalnum(*c)) || (*c == '.') || (*c == '-') ) ) {
            if( *c == '.' )
                last_dot = c;
            c++;
        }
        has_port = (*c == ':') ? 1 : 0;
        // NULL terminate the srvname
        *c = '\0';
        if( has_port == 1 )
            c++;

        if( last_dot != NULL  &&  isdigit(*(last_dot+1)) ) {
            // Must be an IPv4 address.
            af = AF_INET;
        }
        else {
            // Must be a host name.
            struct addrinfo hints, *res, *res0;

            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;

            ret = getaddrinfo(srvname, NULL, &hints, &res0);
            if( ret == 0 ) {
                for (res = res0; res; res = res->ai_next) {
                    if( res->ai_family == AF_INET || 
                        res->ai_family == AF_INET6 ) {
                        // Found a valid IPv4 or IPv6 address.
                        memcpy( &out->IPaddress, res->ai_addr, 
                            res->ai_addrlen );
                        break;
                    }
                }
                freeaddrinfo(res0);

                if( res == NULL ) {
                    // Didn't find an AF_INET or AF_INET6 address.
                    return UPNP_E_INVALID_URL;
                }
            }
            else {
                // getaddrinfo failed.
                return UPNP_E_INVALID_URL;
            }
        }
    }

    // Check if a port is specified.
    if( has_port == 1 ) {
        // Port is specified.
        srvport = c;
        while( *c != '\0'  &&  isdigit(*c) ) {
            c++;
        }
        port = (unsigned short int)atoi(srvport);
        if( port == 0 ) {
            // Bad port number.
            return UPNP_E_INVALID_URL;
        }
    }
    else {
        // Port was not specified, use default port.
        port = 80;
    }

    // The length of the host and port string can be calculated by
    // subtracting pointers.
    hostport_size = (int)(c - workbuf);

    // Fill in the 'out' information.
    if( af == AF_INET ) {
        sai4->sin_family = AF_INET;
        sai4->sin_port = htons(port);
        ret = inet_pton(AF_INET, srvname, &sai4->sin_addr);
    }
    else if( af == AF_INET6 ) {
        sai6->sin6_family = AF_INET6;
        sai6->sin6_port = htons(port);
        sai6->sin6_scope_id = gIF_INDEX;
        ret = inet_pton(AF_INET6, srvname, &sai6->sin6_addr);
    } else {
        // IP address was set by the hostname (getaddrinfo).
        // Override port:
        if( out->IPaddress.ss_family == AF_INET )
            sai4->sin_port = htons(port);
        else
            sai6->sin6_port = htons(port);
        ret = 1;
    }

    // Check if address was converted successfully.
    if( ret <= 0 ) {
        return UPNP_E_INVALID_URL;
    }

    out->text.size = hostport_size;
    out->text.buff = in;
    return hostport_size;
}

/*!
 * \brief parses a uri scheme starting at in[0] as defined in 
 * http://www.ietf.org/rfc/rfc2396.txt (RFC explaining URIs).
 *
 * (e.g. "http:" -> scheme= "http").
 *
 * \note String MUST include ':' within the max charcters.
 *
 * \return 
 */
static int parse_scheme(
	/*! [in] String of characters representing a scheme. */
	const char *in,
	/*! [in] Maximum number of characters. */
	int max,
	/*! [out] Output parameter whose buffer is filled in with the scheme. */
	token *out)
{
    int i = 0;

    out->size = 0;
    out->buff = NULL;

    if( ( max == 0 ) || ( !isalpha( in[0] ) ) )
        return FALSE;

    i++;
    while( ( i < max ) && ( in[i] != ':' ) ) {

        if( !( isalnum( in[i] ) || ( in[i] == '+' ) || ( in[i] == '-' )
               || ( in[i] == '.' ) ) )
            return FALSE;

        i++;
    }
    if( i < max ) {
        out->size = i;
        out->buff = &in[0];
        return i;
    }

    return FALSE;

}


int remove_escaped_chars(INOUT char *in, INOUT size_t *size )
{
    int i = 0;

    for( i = 0; i < *size; i++ ) {
        replace_escaped( in, i, size );
    }
    return UPNP_E_SUCCESS;
}


int remove_dots(char *in, size_t size)
{
    char *copyTo = in;
    char *copyFrom = in;
    char *max = in + size;
    char **Segments = NULL;
    int lastSegment = -1;

    Segments = malloc( sizeof( char * ) * size );

    if( Segments == NULL )
        return UPNP_E_OUTOF_MEMORY;

    Segments[0] = NULL;
    UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
        "REMOVE_DOTS: before: %s\n", in );
    while( ( copyFrom < max ) && ( *copyFrom != '?' )
           && ( *copyFrom != '#' ) ) {

        if( ( ( *copyFrom ) == '.' )
            && ( ( copyFrom == in ) || ( *( copyFrom - 1 ) == '/' ) ) ) {
            if( ( copyFrom + 1 == max )
                || ( *( copyFrom + 1 ) == '/' ) ) {

                copyFrom += 2;
                continue;
            } else if( ( *( copyFrom + 1 ) == '.' )
                       && ( ( copyFrom + 2 == max )
                            || ( *( copyFrom + 2 ) == '/' ) ) ) {
                copyFrom += 3;

                if( lastSegment > 0 ) {
                    copyTo = Segments[--lastSegment];
                } else {
                    free( Segments );
                    //TRACE("ERROR RESOLVING URL, ../ at ROOT");
                    return UPNP_E_INVALID_URL;
                }
                continue;
            }
        }

        if( ( *copyFrom ) == '/' ) {

            lastSegment++;
            Segments[lastSegment] = copyTo + 1;
        }
        ( *copyTo ) = ( *copyFrom );
        copyTo++;
        copyFrom++;
    }
    if( copyFrom < max ) {
        while( copyFrom < max ) {
            ( *copyTo ) = ( *copyFrom );
            copyTo++;
            copyFrom++;
        }
    }
    ( *copyTo ) = 0;
    free( Segments );
    UpnpPrintf( UPNP_ALL, API, __FILE__, __LINE__,
        "REMOVE_DOTS: after: %s\n", in );
    return UPNP_E_SUCCESS;
}


char *resolve_rel_url(char *base_url, char *rel_url)
{
    uri_type base;
    uri_type rel;
    char temp_path = '/';

    int i = 0;
    char *finger = NULL;

    char *last_slash = NULL;

    char *out = NULL;
    char *out_finger = NULL;

    if( base_url && rel_url ) {
        out =
            ( char * )malloc( strlen( base_url ) + strlen( rel_url ) + 2 );
        out_finger = out;
    } else {
        if( rel_url )
            return strdup( rel_url );
        else
            return NULL;
    }

    if( out == NULL ) {
        return NULL;
    }

    if( ( parse_uri( rel_url, strlen( rel_url ), &rel ) ) == HTTP_SUCCESS ) {

        if( rel.type == ABSOLUTE ) {

            strcpy( out, rel_url );
        } else {

            if( ( parse_uri( base_url, strlen( base_url ), &base ) ==
                  HTTP_SUCCESS )
                && ( base.type == ABSOLUTE ) ) {

                if( strlen( rel_url ) == 0 ) {
                    strcpy( out, base_url );
                } else {
                    memcpy( out, base.scheme.buff, base.scheme.size );
                    out_finger += base.scheme.size;
                    ( *out_finger ) = ':';
                    out_finger++;

                    if( rel.hostport.text.size > 0 ) {
                        sprintf( out_finger, "%s", rel_url );
                    } else {
                        if( base.hostport.text.size > 0 ) {
                            memcpy( out_finger, "//", 2 );
                            out_finger += 2;
                            memcpy( out_finger, base.hostport.text.buff,
                                    base.hostport.text.size );
                            out_finger += base.hostport.text.size;
                        }

                        if( rel.path_type == ABS_PATH ) {
                            strcpy( out_finger, rel_url );

                        } else {

                            if( base.pathquery.size == 0 ) {
                                base.pathquery.size = 1;
                                base.pathquery.buff = &temp_path;
                            }

                            finger = out_finger;
                            last_slash = finger;
                            i = 0;

                            while( ( i < base.pathquery.size ) &&
                                   ( base.pathquery.buff[i] != '?' ) ) {
                                ( *finger ) = base.pathquery.buff[i];
                                if( base.pathquery.buff[i] == '/' )
                                    last_slash = finger + 1;
                                i++;
                                finger++;

                            }
                            i = 0;
                            strcpy( last_slash, rel_url );
                            if( remove_dots( out_finger,
                                             strlen( out_finger ) ) !=
                                UPNP_E_SUCCESS ) {
                                free(out);
                                /* free(rel_url); */
                                return NULL;
                            }
                        }

                    }
                }
            } else {
                free(out);
                /* free(rel_url); */
                return NULL;
            }
        }
    } else {
        free(out);
        /* free(rel_url); */          
        return NULL;
    }

    /* free(rel_url); */
    return out;
}


int parse_uri(const char *in, int max, uri_type *out)
{
    int begin_path = 0;
    int begin_hostport = 0;
    int begin_fragment = 0;

    if( ( begin_hostport = parse_scheme( in, max, &out->scheme ) ) ) {
        out->type = ABSOLUTE;
        out->path_type = OPAQUE_PART;
        begin_hostport++;
    } else {
        out->type = RELATIVE;
        out->path_type = REL_PATH;
    }

    if( ( ( begin_hostport + 1 ) < max ) && ( in[begin_hostport] == '/' )
        && ( in[begin_hostport + 1] == '/' ) ) {
        begin_hostport += 2;

        if( ( begin_path = parse_hostport( &in[begin_hostport],
                                           max - begin_hostport,
                                           &out->hostport ) ) >= 0 ) {
            begin_path += begin_hostport;
        } else
            return begin_path;

    } else {
        memset( &out->hostport, 0, sizeof(out->hostport) );
        begin_path = begin_hostport;
    }

    begin_fragment =
        parse_uric( &in[begin_path], max - begin_path,
                    &out->pathquery ) + begin_path;

    if( ( out->pathquery.size ) && ( out->pathquery.buff[0] == '/' ) ) {
        out->path_type = ABS_PATH;
    }

    if( ( begin_fragment < max ) && ( in[begin_fragment] == '#' ) ) {
        begin_fragment++;
        parse_uric( &in[begin_fragment], max - begin_fragment,
                    &out->fragment );
    } else {
        out->fragment.buff = NULL;
        out->fragment.size = 0;
    }
    return HTTP_SUCCESS;
}


int parse_uri_and_unescape(char *in, int max, uri_type *out)
{
	int ret = parse_uri(in, max, out);

	if (ret != HTTP_SUCCESS) {
		return ret;
	}

	if (out->pathquery.size > 0) {
		remove_escaped_chars((char *)out->pathquery.buff, &out->pathquery.size);
	}
	if (out->fragment.size > 0) {
		remove_escaped_chars((char *)out->fragment.buff, &out->fragment.size);
	}

	return HTTP_SUCCESS;
}

