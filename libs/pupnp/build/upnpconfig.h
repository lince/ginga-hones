/* upnp/inc/upnpconfig.h.  Generated from upnpconfig.h.in by configure.  */
/* -*- C -*- */
/*******************************************************************************
 *
 * Copyright (c) 2006 R�mi Turboult <r3mi@users.sourceforge.net>
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
 * * Neither name of Intel Corporation nor the names of its contributors 
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

#ifndef UPNP_CONFIG_H
#define UPNP_CONFIG_H 


/***************************************************************************
 * Library version 
 ***************************************************************************/ 

/** The library version (string) e.g. "1.3.0" */
#define UPNP_VERSION_STRING "1.8.0"

/** Major version of the library */
#define UPNP_VERSION_MAJOR 1

/** Minor version of the library */
#define UPNP_VERSION_MINOR 8

/** Patch version of the library */
#define UPNP_VERSION_PATCH 0

/** The library version (numeric) e.g. 10300 means version 1.3.0 */
#define UPNP_VERSION	\
  ((UPNP_VERSION_MAJOR*100 + UPNP_VERSION_MINOR)*100 + UPNP_VERSION_PATCH)



/***************************************************************************
 * Library optional features
 ***************************************************************************/ 

/*
 * The following defines can be tested in order to know which 
 * optional features have been included in the installed library.
 */


/** Defined to 1 if the library has been compiled with DEBUG enabled 
 *  (i.e. configure --enable-debug) : <upnp/upnpdebug.h> file is available */
#undef UPNP_HAVE_DEBUG


/** Defined to 1 if the library has been compiled with client API enabled 
 *  (i.e. configure --enable-client) */
#define UPNP_HAVE_CLIENT 1


/** Defined to 1 if the library has been compiled with device API enabled 
 *  (i.e. configure --enable-device) */
#define UPNP_HAVE_DEVICE 1


/** Defined to 1 if the library has been compiled with integrated web server
 *  (i.e. configure --enable-webserver --enable-device) */
#define UPNP_HAVE_WEBSERVER 1


/** Defined to 1 if the library has been compiled with helper API
 *  (i.e. configure --enable-tools) : <upnp/upnptools.h> file is available */
#undef UPNP_HAVE_TOOLS


#endif /* UPNP_CONFIG_H */

