METASOURCES = AUTO

INCLUDES = \
    -I/usr/local/include \
    -I$(top_srcdir)/include \
    $(all_includes) \
    -I../include/upnp \
    -I../include/idservice \
    -I../include/mediaserver \
    -I../include/mediarenderer \
    -I../include/util \
    -I../include/ui

noinst_PROGRAMS = \
	TVMediaServerTest \
	DeviceControllerTest \
	BtDeviceControllerTest \
	TVRendererTest \
	HelloWorld \
	HelloWorldClient \
	UITest

TVMediaServerTest_SOURCES = \
	../test/TVMediaServerTest.cpp

TVMediaServerTest_LDADD = -lgingahones -ldl

TVMediaServerTest_LDFLAGS = -L../bin/ -L../bin/.libs/ -L/lib/ -L/usr/lib/ -L/usr/local/lib/

DeviceControllerTest_SOURCES = \
	../test/DeviceControllerTest.cpp

DeviceControllerTest_LDADD = -lgingahones -ldl

DeviceControllerTest_LDFLAGS = -L../bin/ -L../bin/.libs/ -L/lib/ -L/usr/lib/ -L/usr/local/lib/

TVRendererTest_SOURCES = \
	../test/TVRendererTest.cpp

TVRendererTest_LDADD = -lgingahones -ldl

TVRendererTest_LDFLAGS = -L../bin/ -L../bin/.libs/ -L/lib/ -L/usr/lib/ -L/usr/local/lib/

BtDeviceControllerTest_SOURCES = \
	../test/BtDeviceControllerTest.cpp

BtDeviceControllerTest_LDADD = -lgingahones -ldl

BtDeviceControllerTest_LDFLAGS = -L../bin/ -L../bin/.libs/ -L/lib/ -L/usr/lib/ -L/usr/local/lib/

UITest_SOURCES = \
	../test/UITest.cpp

UITest_LDADD = -lgingahones -ldl

UITest_LDFLAGS = -L../bin/ -L../bin/.libs/ -L/lib/ -L/usr/lib/ -L/usr/local/lib/

HelloWorld_SOURCES = \
	../test/HelloWorld.h \
	../test/HelloWorld.cpp \
	../test/HelloWorldService.h \
	../test/HelloWorldService.cpp

HelloWorld_LDADD = -lgingahones -ldl

HelloWorld_LDFLAGS = -L../bin/ -L../bin/.libs/ -L/lib/ -L/usr/lib/ -L/usr/local/lib/

HelloWorldClient_SOURCES = \
	../test/HelloWorldClient.cpp

HelloWorldClient_LDADD = -lgingahones -ldl

HelloWorldClient_LDFLAGS = -L../bin/ -L../bin/.libs/ -L/lib/ -L/usr/lib/ -L/usr/local/lib/
