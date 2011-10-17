#include "honesconfig.h"

#include "mediarenderer/TVRenderer.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediarenderer {
                        
                        const char *TVRenderer::DEVICE_TYPE = "urn:schemas-upnp-org:device:MediaRenderer:1";
                        const char *TVRenderer::UUID_PATH = ".tvrenderer.uuid";
                        const char *TVRenderer::DESCRIPTION =
                                "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                "<root xmlns=\"urn:schemas-upnp-org:device-1-0\" xmlns:dlna=\"urn:schemas-dlna-org:device-1-0\">\n"
                                "   <specVersion>\n"
                                "      <major>1</major>\n"
                                "      <minor>0</minor>\n"
                                "   </specVersion>\n"
                                "   <device>\n"
                                "      <deviceType>urn:schemas-upnp-org:device:MediaRenderer:1</deviceType>\n"
                                "      <friendlyName>TV Lince</friendlyName>\n"
                                "      <manufacturer>UFSCar/Lince</manufacturer>\n"
                                "      <manufacturerURL>http://lince.dc.ufscar.br</manufacturerURL>\n"
                                "      <modelDescription>Media Renderer</modelDescription>\n"
                                "      <modelName>TV Media Renderer</modelName>\n"
                                "      <modelNumber>001</modelNumber>\n"
                                "      <serialNumber>UFSCar-LINCE-TVR001</serialNumber>\n"
                                "      <modelURL>http://lince.dc.ufscar.br</modelURL>\n"
                                "      <dlna:X_DLNADOC xmlns:dlna=\"urn:schemas-dlna-org:device-1-0\">DMR-1.50</dlna:X_DLNADOC>\n"
                                "      <dlna:X_DLNADOC xmlns:dlna=\"urn:schemas-dlna-org:device-1-0\">M-DMR-1.50</dlna:X_DLNADOC>\n"
                                "      <dlna:X_DLNACAP xmlns:dlna=\"urn:schemas-dlna-org:device-1-0\"></dlna:X_DLNACAP>\n"
                                "      <serviceList>\n"
                                "         <service>\n"
                                "            <serviceType>urn:schemas-upnp-org:service:RenderingControl:1</serviceType>\n"
                                "            <serviceId>urn:upnp-org:serviceId:RenderingControl</serviceId>\n"
                                "            <SCPDURL>/service/RenderingControl1.xml</SCPDURL>\n"
                                "            <controlURL>/service/RenderingControl_control</controlURL>\n"
                                "            <eventSubURL>/service/RenderingControl_event</eventSubURL>\n"
                                "         </service>\n"
                                "         <service>\n"
                                "            <serviceType>urn:schemas-upnp-org:service:ConnectionManager:1</serviceType>\n"
                                "            <serviceId>urn:upnp-org:serviceId:ConnectionManager</serviceId>\n"
                                "            <SCPDURL>/service/ConnectionManager1.xml</SCPDURL>\n"
                                "            <controlURL>/service/ConnectionManager_control</controlURL>\n"
                                "            <eventSubURL>/service/ConnectionManager_event</eventSubURL>\n"
                                "         </service>\n"
                                "         <service>\n"
                                "            <serviceType>urn:schemas-upnp-org:service:AVTransport:1</serviceType>\n"
                                "            <serviceId>urn:upnp-org:serviceId:AVTransport</serviceId>\n"
                                "            <SCPDURL>/service/AVTransport1.xml</SCPDURL>\n"
                                "            <controlURL>/service/AVTransport_control</controlURL>\n"
                                "            <eventSubURL>/service/AVTransport_event</eventSubURL>\n"
                                "         </service>\n"
                                "      </serviceList>\n"
                                "   </device>\n"
                                "</root>";


                        TVRenderer::TVRenderer() : UPnPDevice() {
                            restoreUUID( string(Utils::getHomeDir()).append("/").append(UUID_PATH).c_str() );
                            bool ret = loadDescription(DESCRIPTION);
                            if (ret == false)
                                throw "Invalid description!";

                            player = PlayerHandler::PlayerHandlerFactory();
                            conMan = new TVRConnectionManager(this);
                            avTransp = new TVRAVTransport(this);
                            renCtl = new TVRRenderingControl(this);
                        }

                        TVRenderer::~TVRenderer() {
                            delete conMan;
                            delete renCtl;
                            delete avTransp;
                            stop();
                        }

                        TVRConnectionManager* TVRenderer::getConnectionManager() {
                            return conMan;
                        }

                        TVRRenderingControl* TVRenderer::getRenderingControl() {
                            return renCtl;
                        }

                        TVRAVTransport* TVRenderer::getAVTransport() {
                            return avTransp;
                        }

                        PlayerHandler* TVRenderer::getPlayerHandler() {
                            return player;
                        }

                        bool TVRenderer::start() {
                            UPnPDevice::start();
                            getConnectionManager()->initializeStateVariables();
                            getAVTransport()->initializeStateVariables();
                            getRenderingControl()->initializeStateVariables();
                            return true;
                        }

                        bool TVRenderer::stop() {
                            UPnPDevice::stop();
                            return true;
                        }

                    }
                }
            }
        }
    }
}
