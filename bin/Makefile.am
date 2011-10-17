METASOURCES = AUTO

INCLUDES = \
    -I/usr/local/include \
    -I$(top_srcdir)/include \
    $(all_includes) \
    -I/usr/local/include/ginga \
    -I../include/upnp \
    -I../include/idservice \
    -I../include/mediaserver \
    -I../include/mediarenderer \
    -I../include/mediacenter \
    -I../include/util \
    -I../include/ui \
    -I../include/capture \
    -I../include/recorder \
    -I../include/tvcontrol

########################################
# upnp
########################################

upnp_header = \
	../include/upnp/UPnPAction.h \
	../include/upnp/UPnPActionListener.h \
	../include/upnp/UPnPActionList.h \
	../include/upnp/UPnPActionRequest.h \
	../include/upnp/UPnPActionResponse.h \
	../include/upnp/UPnPAllowedValue.h \
	../include/upnp/UPnPAllowedValueList.h \
	../include/upnp/UPnPAllowedValueRange.h \
	../include/upnp/UPnPArgument.h \
	../include/upnp/UPnPArgumentList.h \
	../include/upnp/UPnPDevice.h \
	../include/upnp/UPnPDeviceController.h \
	../include/upnp/UPnPDeviceList.h \
	../include/upnp/UPnPDeviceListener.h \
	../include/upnp/UPnPEventListener.h \
	../include/upnp/UPnPHttpListener.h \
	../include/upnp/UPnPHttpServer.h \
	../include/upnp/UPnPIcon.h \
	../include/upnp/UPnPIconList.h \
	../include/upnp/UPnPQueryListener.h \
	../include/upnp/UPnPQueryRequest.h \
	../include/upnp/UPnPQueryResponse.h \
	../include/upnp/UPnPRegistrarService.h \
	../include/upnp/UPnPService.h \
	../include/upnp/UPnPServiceList.h \
	../include/upnp/UPnPServiceStateTable.h \
	../include/upnp/UPnPStateVariable.h \
	../include/upnp/UPnPStatus.h

upnp_source = \
	../src/upnp/UPnPAction.cpp \
	../src/upnp/UPnPActionList.cpp \
	../src/upnp/UPnPActionRequest.cpp \
	../src/upnp/UPnPActionResponse.cpp \
	../src/upnp/UPnPAllowedValue.cpp \
	../src/upnp/UPnPAllowedValueList.cpp \
	../src/upnp/UPnPAllowedValueRange.cpp \
	../src/upnp/UPnPArgument.cpp \
	../src/upnp/UPnPArgumentList.cpp \
	../src/upnp/UPnPDevice.cpp \
	../src/upnp/UPnPDeviceController.cpp \
	../src/upnp/UPnPDeviceList.cpp \
	../src/upnp/UPnPHttpServer.cpp \
	../src/upnp/UPnPIcon.cpp \
	../src/upnp/UPnPIconList.cpp \
	../src/upnp/UPnPQueryRequest.cpp \
	../src/upnp/UPnPQueryResponse.cpp \
	../src/upnp/UPnPRegistrarService.cpp \
	../src/upnp/UPnPService.cpp \
	../src/upnp/UPnPServiceList.cpp \
	../src/upnp/UPnPServiceStateTable.cpp \
	../src/upnp/UPnPStateVariable.cpp \
	../src/upnp/UPnPStatus.cpp

########################################
# LibUPnP >= 1.8.0 not found
########################################

if COMPILE_UPNP180
EXTRA_DIST = \
	../libs/pupnp/LICENSE \
	../libs/pupnp/src/win_dll.c

INCLUDES += \
	-I../libs/pupnp \
	-I../libs/pupnp/upnp \
	-I../libs/pupnp/src/inc \
	-I../libs/pupnp/src/ssdp \
	-I../libs/pupnp/build

LT_VERSION_IXML         = 2:4:0
LT_VERSION_THREADUTIL   = 4:3:2
LT_VERSION_UPNP         = 4:0:0

libupnpincludedir	= /usr/include/upnp

lib_LTLIBRARIES		= libixml.la libthreadutil.la libupnp.la

# ixml
libixml_la_LDFLAGS	= \
	-version-info $(LT_VERSION_IXML) \
	-export-symbols-regex '^ixml.*'
libixml_la_SOURCES	= \
	../libs/pupnp/src/attr.c \
	../libs/pupnp/src/document.c \
	../libs/pupnp/src/element.c \
	../libs/pupnp/src/ixml.c \
	../libs/pupnp/src/ixmldebug.c \
	../libs/pupnp/src/ixmlparser.c \
	../libs/pupnp/src/ixmlmembuf.c \
	../libs/pupnp/src/namedNodeMap.c \
	../libs/pupnp/src/node.c \
	../libs/pupnp/src/nodeList.c
libixml_la_CPPFLAGS	= \
	-DNO_DEBUG -DNDEBUG

libupnpinclude_HEADERS	= \
	../libs/pupnp/upnp/ixml.h \
	../libs/pupnp/upnp/ixmldebug.h


# threadutil
libthreadutil_la_LDFLAGS = -version-info $(LT_VERSION_THREADUTIL)
libthreadutil_la_SOURCES = \
	../libs/pupnp/src/FreeList.c \
	../libs/pupnp/src/LinkedList.c \
	../libs/pupnp/src/ThreadPool.c \
	../libs/pupnp/src/TimerThread.c
libthreadutil_la_CPPFLAGS	= \
	-DNO_DEBUG -DNDEBUG

libupnpinclude_HEADERS	+= \
        ../libs/pupnp/upnp/FreeList.h \
        ../libs/pupnp/upnp/LinkedList.h \
        ../libs/pupnp/upnp/ThreadPool.h \
        ../libs/pupnp/upnp/TimerThread.h \
        ../libs/pupnp/upnp/ithread.h

# upnp
libupnp_la_LDFLAGS = \
	-version-info $(LT_VERSION_UPNP) \
	-export-symbols-regex '^Upnp.*' \
	$(top_builddir)/bin/libthreadutil.la \
	$(top_builddir)/bin/libixml.la
# ssdp
libupnp_la_SOURCES = \
	../libs/pupnp/src/ssdp/ssdp_ResultData.c \
	../libs/pupnp/src/ssdp/ssdp_device.c \
	../libs/pupnp/src/ssdp/ssdp_ctrlpt.c \
	../libs/pupnp/src/ssdp/ssdp_server.c
# soap
libupnp_la_SOURCES += \
	../libs/pupnp/src/soap/soap_device.c \
	../libs/pupnp/src/soap/soap_ctrlpt.c \
	../libs/pupnp/src/soap/soap_common.c
# genlib
libupnp_la_SOURCES += \
	../libs/pupnp/src/genlib/miniserver/miniserver.c \
	../libs/pupnp/src/genlib/service_table/service_table.c \
	../libs/pupnp/src/genlib/util/membuffer.c \
	../libs/pupnp/src/genlib/util/strintmap.c \
	../libs/pupnp/src/genlib/util/upnp_timeout.c \
	../libs/pupnp/src/genlib/util/util.c \
	../libs/pupnp/src/genlib/client_table/client_table.c \
	../libs/pupnp/src/genlib/net/sock.c \
	../libs/pupnp/src/genlib/net/http/httpparser.c \
	../libs/pupnp/src/genlib/net/http/httpreadwrite.c \
	../libs/pupnp/src/genlib/net/http/statcodes.c \
	../libs/pupnp/src/genlib/net/http/webserver.c \
	../libs/pupnp/src/genlib/net/http/parsetools.c \
	../libs/pupnp/src/genlib/net/uri/uri.c
# gena
libupnp_la_SOURCES += \
	../libs/pupnp/src/gena/gena_device.c \
	../libs/pupnp/src/gena/gena_ctrlpt.c \
	../libs/pupnp/src/gena/gena_callback2.c
# api
libupnp_la_SOURCES += \
	../libs/pupnp/src/api/ActionComplete.c \
	../libs/pupnp/src/api/ActionRequest.c \
	../libs/pupnp/src/api/Discovery.c \
	../libs/pupnp/src/api/Event.c \
	../libs/pupnp/src/api/EventSubscribe.c \
	../libs/pupnp/src/api/FileInfo.c \
	../libs/pupnp/src/api/StateVarComplete.c \
	../libs/pupnp/src/api/StateVarRequest.c \
	../libs/pupnp/src/api/SubscriptionRequest.c \
	../libs/pupnp/src/api/UpnpString.c \
	../libs/pupnp/src/api/upnpapi.c
# uuid
libupnp_la_SOURCES += \
	../libs/pupnp/src/uuid/md5.c \
	../libs/pupnp/src/uuid/sysdep.c \
	../libs/pupnp/src/uuid/uuid.c
# urlconfig
libupnp_la_SOURCES += \
    ../libs/pupnp/src/urlconfig/urlconfig.c
# inet_pton (needed on Win32, compiles to nothing elsewhere)
libupnp_la_SOURCES += \
	../libs/pupnp/src/inet_pton.c

libupnp_la_CPPFLAGS	= \
	-DNO_DEBUG -DNDEBUG

libupnpinclude_HEADERS	+= \
        ../libs/pupnp/upnp/upnp.h \
        ../libs/pupnp/upnp/upnpdebug.h \
        ../libs/pupnp/upnp/ActionComplete.h \
        ../libs/pupnp/upnp/ActionRequest.h \
        ../libs/pupnp/upnp/Discovery.h \
        ../libs/pupnp/upnp/Event.h \
        ../libs/pupnp/upnp/EventSubscribe.h \
        ../libs/pupnp/upnp/FileInfo.h \
        ../libs/pupnp/upnp/StateVarComplete.h \
        ../libs/pupnp/upnp/StateVarRequest.h \
        ../libs/pupnp/upnp/SubscriptionRequest.h \
        ../libs/pupnp/upnp/UpnpGlobal.h \
        ../libs/pupnp/upnp/UpnpInet.h \
        ../libs/pupnp/upnp/UpnpString.h

nodist_libupnpinclude_HEADERS = ../libs/pupnp/build/upnpconfig.h

endif

########################################
# LibDLNA >= 0.2.3 not found
########################################

if COMPILE_DLNA023
INCLUDES += \
	-I../libs/libdlna/src
upnp_source += \
    ../libs/libdlna/src/dlna.h \
	../libs/libdlna/src/dlna_internals.h \
	../libs/libdlna/src/containers.h \
	../libs/libdlna/src/profiles.h \
	../libs/libdlna/src/profiles.c \
	../libs/libdlna/src/containers.c \
	../libs/libdlna/src/image_jpeg.c \
	../libs/libdlna/src/image_png.c \
	../libs/libdlna/src/audio_aac.c \
	../libs/libdlna/src/audio_ac3.c \
	../libs/libdlna/src/audio_amr.c \
	../libs/libdlna/src/audio_atrac3.c \
	../libs/libdlna/src/audio_g726.c \
	../libs/libdlna/src/audio_lpcm.c \
	../libs/libdlna/src/audio_mp2.c \
	../libs/libdlna/src/audio_mp3.c \
	../libs/libdlna/src/audio_wma.c \
	../libs/libdlna/src/av_mpeg1.c \
	../libs/libdlna/src/av_mpeg2.c \
	../libs/libdlna/src/av_mpeg4_part2.c \
	../libs/libdlna/src/av_mpeg4_part10.c \
	../libs/libdlna/src/av_wmv9.c
endif

########################################
# Util
########################################

util_header = \
	../include/util/Vector.h \
	../include/util/Utils.h

util_source = \
	../src/util/Vector.cpp \
	../src/util/Utils.cpp

########################################
# UI
########################################

ui_header = \
	../include/ui/PlayerHandler.h \
	../include/ui/GraphicsHandler.h \
	../include/ui/Container.h \
	../include/ui/Frame.h \
	../include/ui/TextArea.h \
	../include/ui/SelectableTextArea.h

ui_source = \
	../src/ui/PlayerHandler.cpp \
	../src/ui/GraphicsHandler.cpp \
	../src/ui/Container.cpp \
	../src/ui/Frame.cpp \
	../src/ui/TextArea.cpp \
	../src/ui/SelectableTextArea.cpp

########################################
# IdService
########################################

idservice_header = \
	../include/idservice/BtDevice.h \
	../include/idservice/BtDeviceList.h \
	../include/idservice/BtDeviceListener.h \
	../include/idservice/BtDeviceController.h \
	../include/idservice/TVDeviceId.h

idservice_source = \
	../src/idservice/BtDevice.cpp \
	../src/idservice/BtDeviceList.cpp \
	../src/idservice/BtDeviceController.cpp \
	../src/idservice/TVDeviceId.cpp

########################################
# TV Media Server
########################################

tvmediaserver_header = \
	../include/mediaserver/File.h \
	../include/mediaserver/FileList.h \
	../include/mediaserver/Directory.h \
	../include/mediaserver/DirectoryList.h \
	../include/mediaserver/TVMSContentDirectory.h \
	../include/mediaserver/TVMSConnectionManager.h \
	../include/mediaserver/TVMediaServer.h

tvmediaserver_source = \
	../src/mediaserver/File.cpp \
	../src/mediaserver/FileList.cpp \
	../src/mediaserver/Directory.cpp \
	../src/mediaserver/DirectoryList.cpp \
	../src/mediaserver/TVMSContentDirectory.cpp \
	../src/mediaserver/TVMSConnectionManager.cpp \
	../src/mediaserver/TVMediaServer.cpp

########################################
# TV Media Renderer
########################################

tvmediarenderer_header = \
	../include/mediarenderer/TVRAVTransport.h \
	../include/mediarenderer/TVRConnectionManager.h \
	../include/mediarenderer/TVRRenderingControl.h \
	../include/mediarenderer/TVRenderer.h

tvmediarenderer_source = \
	../src/mediarenderer/TVRAVTransport.cpp \
	../src/mediarenderer/TVRConnectionManager.cpp \
	../src/mediarenderer/TVRRenderingControl.cpp \
	../src/mediarenderer/TVRenderer.cpp

########################################
# Capture
########################################

capture_header = \
	../include/capture/Exceptions.h \
	../include/capture/OutputStream.h \
	../include/capture/AudioCapture.h \
	../include/capture/ImageCapture.h \
	../include/capture/VideoCapture.h \
	../include/capture/TVCapture.h

capture_source = \
	../src/capture/Exceptions.cpp \
	../src/capture/AudioCapture.cpp \
	../src/capture/ImageCapture.cpp \
	../src/capture/VideoCapture.cpp \
	../src/capture/TVCapture.cpp

########################################
# Recorder
########################################

recorder_header = \
	../include/recorder/ScheduledItem.h \
	../include/recorder/VideoRecorder.h \
	../include/recorder/TVRecorder.h

recorder_source = \
	../src/recorder/ScheduledItem.cpp \
	../src/recorder/VideoRecorder.cpp \
	../src/recorder/TVRecorder.cpp

########################################
# TV Control
########################################

tvcontrol_header = \
	../include/tvcontrol/TVControlHandler.h \
	../include/tvcontrol/TVControl.h

tvcontrol_source = \
	../src/tvcontrol/TVControlHandler.cpp \
	../src/tvcontrol/TVControl.cpp

########################################
# TV Media Center
########################################

tvmediacenter_header = \
	../include/mediacenter/TVMediaCenter.h

tvmediacenter_source = \
	../src/mediacenter/TVMediaCenter.cpp

########################################
# Install
########################################

alsafiledir = $(HOME)
alsafile_DATA = ../files/.asoundrc

imgdir = /usr/local/etc/ginga/files/img/hones/
img_DATA = ../files/Client.png \
	../files/Movies.png \
	../files/Music.png \
	../files/Pictures.png \
	../files/Movies1.png \
	../files/Movies2.png \
	../files/Music1.png \
	../files/Music2.png \
	../files/Pictures1.png \
	../files/Pictures2.png \
	../files/Server.png \
	../files/Exit1.png \
	../files/Exit2.png \
	../files/Back1.png \
	../files/Back2.png \
	../files/Up1.png \
	../files/Up2.png \
	../files/Down1.png \
	../files/Down2.png \
	../files/playpause.png \
	../files/backward.png \
	../files/forward.png

localIncludedir = /usr/local/include/ginga/hones

localInclude_HEADERS = \
	$(util_header) \
	$(ui_header) \
	$(upnp_header) \
	$(idservice_header) \
	$(tvmediaserver_header) \
	$(tvmediarenderer_header) \
	$(tvmediacenter_header) \
	$(capture_header) \
	$(recorder_header) \
	$(tvcontrol_header)

localLibdir = /usr/local/lib/ginga

localLib_LTLIBRARIES = libgingahones.la

libgingahones_la_SOURCES = \
	$(util_source) \
	$(ui_source) \
	$(upnp_source) \
	$(idservice_source) \
	$(tvmediaserver_source) \
	$(tvmediarenderer_source) \
	$(tvmediacenter_source) \
	$(capture_source) \
	$(recorder_source) \
	$(tvcontrol_source)

libgingahones_la_LDFLAGS = -L/lib/ -L/usr/lib/ -L/usr/local/lib/ -L/usr/local/lib/ginga/ -L/usr/local/lib/ginga/iocontents -version-info 0:0:0 -shared -Wl,-soname -Wl,libgingahones.so $(HONES_LIBS) -lupnp -ldl
