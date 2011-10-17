#include "honesconfig.h"

#include "tvcontrol/TVControlHandler.h"
#include "ui/GraphicsHandler.h"
#include "util/Utils.h"
#include <directfb/directfb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifdef HAVE_GINGACCCM
#include <ginga/cm/IComponentManager.h>
#include <ginga/tuner/ITuner.h>
using namespace br::pucrio::telemidia::ginga::core::cm;
using namespace br::pucrio::telemidia::ginga::core::tuning;
#endif

using namespace br::ufscar::lince::ginga::hones::ui;
using namespace br::ufscar::lince::ginga::hones::util;
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace tvcontrol {

                        TVControlHandler* TVControlHandler::instance = NULL;

                        TVControlHandler::TVControlHandler() {
                        }

                        TVControlHandler::~TVControlHandler() {
                        }

                        TVControlHandler* TVControlHandler::TVControlHandlerFactory() {
                            if(instance == NULL){
                                instance = new TVControlHandler();
                            }
                            return instance;
                        }

                        void TVControlHandler::TVControlHandlerDestructor() {
                            if (instance != NULL) {
                                delete instance;
                                instance = NULL;
                            }
                        }

                        bool TVControlHandler::channelUp() {
#ifdef HAVE_GINGACCCM
                            IComponentManager* cm = IComponentManager::getCMInstance();
                            ITuner* tuner = NULL;
                            if (cm != NULL)
                                tuner = ((TunerCreator*) (cm->getObject("Tuner")))();
                            if (tuner != NULL)
                                tuner->channelUp();
                            return true;
#else
                            return false;
#endif
                        }

                        bool TVControlHandler::channelDown() {
#ifdef HAVE_GINGACCCM
                            IComponentManager* cm = IComponentManager::getCMInstance();
                            ITuner* tuner = NULL;
                            if (cm != NULL)
                                tuner = ((TunerCreator*) (cm->getObject("Tuner")))();
                            if (tuner != NULL)
                                tuner->channelDown();
                            return true;
#else
                            return false;
#endif
                        }

                        bool TVControlHandler::setChannel(int channel) {
#ifdef HAVE_GINGACCCM
                            IComponentManager* cm = IComponentManager::getCMInstance();
                            ITuner* tuner = NULL;
                            if (cm != NULL)
                                tuner = ((TunerCreator*) (cm->getObject("Tuner")))();
                            //if (tuner != NULL)
                            //    tuner->changeChannel(channel);
                            return true;
#else
                            return false;
#endif
                        }

                        int TVControlHandler::getChannel() {
#ifdef HAVE_GINGACCCM
                            IComponentManager* cm = IComponentManager::getCMInstance();
                            ITuner* tuner = NULL;
                            if (cm != NULL)
                                tuner = ((TunerCreator*) (cm->getObject("Tuner")))();
                            //if (tuner != NULL){
                            //    INetworkInterface* interface = tuner->getCurrentInterface();
                            //    IChannel* channel = interface->getCurrentChannel();
                            //    return channel->getId();
                            //}
#endif
                            return 0;
                        }

                        bool TVControlHandler::setVolume(int v) {
                            string volume = "amixer set Master ";
                            volume.append(Utils::integerToString((int)v/100*63));
                            system(volume.c_str());
                            return true;
                        }

                        bool TVControlHandler::setBrightness(int v) {
                            IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                            IDirectFBDisplayLayer* gfxLayer;
                            dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &gfxLayer);
                            DFBColorAdjustment colorAdj;
                            gfxLayer->GetColorAdjustment(gfxLayer, &colorAdj);
                            colorAdj.flags = (DFBColorAdjustmentFlags) (DCAF_ALL);
                            colorAdj.brightness = (v/100) * 0xFFFF;
                            gfxLayer->SetColorAdjustment(gfxLayer, &colorAdj);
                            return true;
                        }

                        int TVControlHandler::getBrightness() {
                            IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                            IDirectFBDisplayLayer* gfxLayer;
                            dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &gfxLayer);
                            DFBColorAdjustment colorAdj;
                            gfxLayer->GetColorAdjustment(gfxLayer, &colorAdj);

                            return colorAdj.brightness/0xFFFF * 100;
                        }

                        bool TVControlHandler::setContrast(int v) {
                            IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                            IDirectFBDisplayLayer* gfxLayer;
                            dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &gfxLayer);
                            DFBColorAdjustment colorAdj;
                            gfxLayer->GetColorAdjustment(gfxLayer, &colorAdj);
                            colorAdj.flags = (DFBColorAdjustmentFlags) (DCAF_ALL);
                            colorAdj.contrast = (v/100) * 0xFFFF;
                            gfxLayer->SetColorAdjustment(gfxLayer, &colorAdj);
                            return true;
                        }

                        int TVControlHandler::getContrast() {
                            IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                            IDirectFBDisplayLayer* gfxLayer;
                            dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &gfxLayer);
                            DFBColorAdjustment colorAdj;
                            gfxLayer->GetColorAdjustment(gfxLayer, &colorAdj);

                            return colorAdj.contrast/0xFFFF * 100;
                        }

                        bool TVControlHandler::setSaturation(int v) {
                            IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                            IDirectFBDisplayLayer* gfxLayer;
                            dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &gfxLayer);
                            DFBColorAdjustment colorAdj;
                            gfxLayer->GetColorAdjustment(gfxLayer, &colorAdj);
                            colorAdj.flags = (DFBColorAdjustmentFlags) (DCAF_ALL);
                            colorAdj.saturation = (v/100) * 0xFFFF;
                            gfxLayer->SetColorAdjustment(gfxLayer, &colorAdj);
                            return true;
                        }

                        int TVControlHandler::getSaturation() {
                            IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                            IDirectFBDisplayLayer* gfxLayer;
                            dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &gfxLayer);
                            DFBColorAdjustment colorAdj;
                            gfxLayer->GetColorAdjustment(gfxLayer, &colorAdj);

                            return colorAdj.saturation/0xFFFF * 100;
                        }

                        bool TVControlHandler::setHue(int v) {
                            IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                            IDirectFBDisplayLayer* gfxLayer;
                            dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &gfxLayer);
                            DFBColorAdjustment colorAdj;
                            gfxLayer->GetColorAdjustment(gfxLayer, &colorAdj);
                            colorAdj.flags = (DFBColorAdjustmentFlags) (DCAF_ALL);
                            colorAdj.hue = (v/100) * 0xFFFF;
                            gfxLayer->SetColorAdjustment(gfxLayer, &colorAdj);
                            return true;
                        }

                        int TVControlHandler::getHue() {
                            IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                            IDirectFBDisplayLayer* gfxLayer;
                            dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &gfxLayer);
                            DFBColorAdjustment colorAdj;
                            gfxLayer->GetColorAdjustment(gfxLayer, &colorAdj);

                            return colorAdj.hue/0xFFFF * 100;
                        }

                    }
                }
            }
        }
    }
}

