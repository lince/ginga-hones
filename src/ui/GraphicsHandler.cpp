#include "honesconfig.h"

#include <directfb/directfb.h>

#ifdef HAVE_GINGACCCM
#include <ginga/cm/IComponentManager.h>
#include <ginga/system/io/IDeviceManager.h>
using namespace br::pucrio::telemidia::ginga::core::cm;
using namespace br::pucrio::telemidia::ginga::core::system::io;
#endif
#include <unistd.h>
#include "ui/GraphicsHandler.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace ui {

                        HLoggerPtr GraphicsHandler::logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.util.graphicshandler");
                        int GraphicsHandler::screenWidth = 0;
                        int GraphicsHandler::screenHeight = 0;

                        void GraphicsHandler::initialize() {
                            IDirectFB* dfb = (IDirectFB*) getGraphicsRoot();
#ifndef HAVE_GINGACCCM
                            DFBDisplayLayerConfig displayLayerConf;
                            IDirectFBDisplayLayer* displayLayer;

                            dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &displayLayer);
                            displayLayer->GetConfiguration(displayLayer, &displayLayerConf);

                            screenWidth = displayLayerConf.width;
                            screenHeight = displayLayerConf.height;
#endif
                        }

                        void GraphicsHandler::release() {
#ifdef HAVE_GINGACCCM
                            IComponentManager* cm = IComponentManager::getCMInstance();
                            IDeviceManager* dm = NULL;
                            if (cm != NULL)
                                dm = ((DeviceManagerCreator*) (cm->getObject("DeviceManager")))();
                            if (dm != NULL)
                                dm->release();
#else
                            IDirectFB* dfb = (IDirectFB*) getGraphicsRoot();
                            dfb->Release(dfb);
#endif
                        }

                        void* GraphicsHandler::getGraphicsRoot() {
                            static IDirectFB* dfb = NULL;
#ifdef HAVE_GINGACCCM
                            if (dfb == NULL) {
                                IComponentManager* cm = IComponentManager::getCMInstance();
                                IDeviceManager* dm = NULL;
                                if (cm != NULL)
                                    dm = ((DeviceManagerCreator*) (cm->getObject("DeviceManager")))();
                                if (dm != NULL) {
                                    if (dm->getDeviceWidth() <= 0)
                                        dm->createDevice();

                                    screenWidth = dm->getDeviceWidth();
                                    screenHeight = dm->getDeviceHeight();

                                    dfb = (IDirectFB*) dm->getGfxRoot();
                                }
                            }
#else
                            static DFBDisplayLayerConfig layer_config;

                            if (dfb == NULL) {
                                IDirectFBDisplayLayer* gfxLayer;
                                DFBDisplayLayerConfigFlags failed_flags;

                                DirectFBInit(NULL, NULL);
                                DirectFBCreate(&dfb);
                                dfb->SetCooperativeLevel(dfb, DFSCL_NORMAL);
                                dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &gfxLayer);
                                gfxLayer->GetConfiguration(gfxLayer, &layer_config);
                                layer_config.flags = (DFBDisplayLayerConfigFlags) (DLCONF_BUFFERMODE | DLCONF_OPTIONS);
                                layer_config.buffermode = DLBM_BACKVIDEO;
                                layer_config.options = DLOP_ALPHACHANNEL;

                                if (gfxLayer->TestConfiguration(gfxLayer, &layer_config, &failed_flags) == DFB_OK) {
                                    gfxLayer->SetConfiguration(gfxLayer, &layer_config);
                                }
                            }
#endif
                            return dfb;
                        }

                        int GraphicsHandler::getScreenWidth() {
                            return screenWidth;
                        }

                        int GraphicsHandler::getScreenHeight() {
                            return screenHeight;
                        }

                        void* GraphicsHandler::createAVProvider(char* uri) {
                            IDirectFBVideoProvider* provider = NULL;

                            DFBResult result = ((IDirectFB*) getGraphicsRoot())->CreateVideoProvider((IDirectFB*) getGraphicsRoot(), uri, &provider);

                            LoggerUtil_debug(logger, "GraphicsHandler::createAVProvider(uri): result " << ((int) result));

                            if (result == DFB_OK)
                                return provider;
                            else
                                return NULL;
                        }

                        void* GraphicsHandler::createImageProvider(char* uri) {
                            IDirectFBImageProvider* provider = NULL;

                            DFBResult result = ((IDirectFB*) getGraphicsRoot())->CreateImageProvider((IDirectFB*) getGraphicsRoot(), uri, &provider);

                            LoggerUtil_debug(logger, "GraphicsHandler::createImageProvider(uri): result " << ((int) result));

                            if (result == DFB_OK)
                                return provider;
                            else
                                return NULL;
                        }

                        void* GraphicsHandler::createFrame(void* dsc) {
                            IDirectFBWindow* window = NULL;
                            IDirectFBDisplayLayer* displayLayer;

                            ((IDirectFB*) getGraphicsRoot())->GetDisplayLayer((IDirectFB*) getGraphicsRoot(), DLID_PRIMARY, &displayLayer);

                            if (displayLayer->CreateWindow(displayLayer, (const DFBWindowDescription*) dsc, &window) == DFB_OK)
                                return window;
                            else
                                return NULL;
                        }

                    }
                }
            }
        }
    }
}
