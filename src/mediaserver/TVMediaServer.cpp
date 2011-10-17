#include "honesconfig.h"

#include "mediaserver/TVMediaServer.h"
#include "upnp/UPnPHttpServer.h"
#include "util/Utils.h"
using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                        const char *TVMediaServer::DEVICE_TYPE = "urn:schemas-upnp-org:device:MediaServer:1";
                        const char *TVMediaServer::UUID_PATH = ".tvmediaserver.uuid";
                        const char *TVMediaServer::DESCRIPTION =
                                "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<root xmlns=\"urn:schemas-upnp-org:device-1-0\" xmlns:dlna=\"urn:schemas-dlna-org:device-1-0\">"
                                "  <specVersion>"
                                "    <major>1</major>"
                                "    <minor>0</minor>"
                                "  </specVersion>"
                                "  <device>"
                                "    <deviceType>urn:schemas-upnp-org:device:MediaServer:1</deviceType>"
                                "    <friendlyName>TV Lince</friendlyName>"
                                "    <manufacturer>UFSCar/Lince</manufacturer>"
                                "    <manufacturerURL>http://lince.dc.ufscar.br/</manufacturerURL>"
                                "    <modelDescription>Media Server</modelDescription>"
                                "    <modelName>TV Media Server</modelName>"
                                "    <modelNumber>001</modelNumber>"
                                "    <serialNumber>UFSCar-LINCE-TVMS001</serialNumber>"
                                "    <modelURL>http://lince.dc.ufscar.br/</modelURL>"
                                "    <dlna:X_DLNADOC xmlns:dlna=\"urn:schemas-dlna-org:device-1-0\">DMS-1.50</dlna:X_DLNADOC>"
                                "    <dlna:X_DLNADOC xmlns:dlna=\"urn:schemas-dlna-org:device-1-0\">M-DMS-1.50</dlna:X_DLNADOC>"
                                "    <dlna:X_DLNACAP xmlns:dlna=\"urn:schemas-dlna-org:device-1-0\">av-upload,image-upload,audio-upload</dlna:X_DLNACAP>"
                                "    <serviceList>"
                                "      <service>"
                                "        <serviceType>urn:schemas-upnp-org:service:ConnectionManager:1</serviceType>"
                                "        <serviceId>urn:upnp-org:serviceId:ConnectionManager</serviceId>"
                                "        <SCPDURL>/services/TVMSConnectionManager.xml</SCPDURL>"
                                "        <controlURL>/services/TVMSConnectionManager_control</controlURL>"
                                "        <eventSubURL>/services/TVMSConnectionManager_event</eventSubURL>"
                                "      </service>"
                                "      <service>"
                                "        <serviceType>urn:schemas-upnp-org:service:ContentDirectory:1</serviceType>"
                                "        <serviceId>urn:upnp-org:serviceId:ContentDirectory</serviceId>"
                                "        <SCPDURL>/services/TVMSContentDirectory.xml</SCPDURL>"
                                "        <controlURL>/services/TVMSContentDirectory_control</controlURL>"
                                "        <eventSubURL>/services/TVMSContentDirectory_event</eventSubURL>"
                                "      </service>"
                                "      <service>"
                                "        <serviceType>urn:microsoft.com:service:X_MS_MediaReceiverRegistrar:1</serviceType>\n"
                                "        <serviceId>urn:microsoft.com:serviceId:X_MS_MediaReceiverRegistrar</serviceId>\n"
                                "        <SCPDURL>/services/TVMSMediaReceiverRegistrar.xml</SCPDURL>"
                                "        <controlURL>/services/TVMSMediaReceiverRegistrar_control</controlURL>"
                                "        <eventSubURL>/services/TVMSMediaReceiverRegistrar_event</eventSubURL>"
                                "      </service>"
                                "    </serviceList>"
                                "  </device>"
                                "</root>";

                        TVMediaServer::TVMediaServer(){
                            restoreUUID( string(Utils::getHomeDir()).append("/").append(UUID_PATH).c_str() );
                            bool ret = loadDescription(DESCRIPTION);
                            setDescriptionURI("/description.xml");
                            if (ret == false)
                                throw "Invalid description!";

                            conMan = new TVMSConnectionManager(this);
                            conDir = new TVMSContentDirectory(this);
                            regServ = new UPnPRegistrarService(this);
                        }

                        TVMediaServer::~TVMediaServer(){
                            this->stop();
                            delete conMan;
                            delete conDir;
                            delete regServ;
                        }

                        TVMSConnectionManager* TVMediaServer::getConnectionManager() {
                            return conMan;
                        }

                        TVMSContentDirectory* TVMediaServer::getContentDirectory() {
                            return conDir;
                        }

                        UPnPRegistrarService* TVMediaServer::getRegistrarService(){
                            return regServ;
                        }

                        bool TVMediaServer::start() {
                            bool result = UPnPDevice::start();
                            UPnPHttpServer::getInstance()->setServerAddress(Utils::getHostURL(getLocation()));
                            UPnPHttpServer::getInstance()->start();
                            return result;
                        }

                        bool TVMediaServer::stop() {
                            return UPnPDevice::stop();
                        }

                    }
                }
            }
        }
    }
}
