#include "honesconfig.h"

#include <sstream>
#include "capture/Exceptions.h"
#include "capture/VideoCapture.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace capture {

                VideoCapture* VideoCapture::instance = NULL;
                GingaFFMpeg VideoCapture::ffmpegCapture;

                VideoCapture::VideoCapture() {
                    running = false;
                    frameRate = 25;
                    sampleRate = 44100;
                    videoBitrate = 200000;
                    audioBitrate = 64000;
                    audioChannels = 1;
                    width = 1024;
                    height = 768;
                    videoFormat = "avi";
                    videoCodec = "mpeg2video";
                    audioCodec = "mp2";
                }

                VideoCapture::~VideoCapture() {
                    stop();
                }

                VideoCapture* VideoCapture::VideoCaptureFactory() {
                    if (instance == NULL) {
                        instance = new VideoCapture();
                    }
                    return instance;
                }

                void VideoCapture::VideoCaptureDestructor() {
                    if (instance != NULL) {
                        delete instance;
                        instance = NULL;
                    }
                }

                void VideoCapture::setFrameRate(int frameRate) {
                    if (frameRate <= 0) {
                        instance->frameRate = 25;
                    } else {
                        instance->frameRate = frameRate;
                    }
                }

                int VideoCapture::getFrameRate() {
                    return instance->frameRate;
                }

                void VideoCapture::setSampleRate(int sampleRate) {
                    if (sampleRate <= 0) {
                        instance->sampleRate = 44100;
                    } else {
                        instance->sampleRate = sampleRate;
                    }
                }

                int VideoCapture::getSampleRate() {
                    return instance->sampleRate;
                }

                void VideoCapture::setVideoBitrate(int videoBitrate) {
                    if (videoBitrate <= 0) {
                        instance->videoBitrate = 200000;
                    } else {
                        instance->videoBitrate = videoBitrate;
                    }
                }

                int VideoCapture::getVideoBitrate() {
                    return instance->videoBitrate;
                }

                void VideoCapture::setAudioBitrate(int audioBitrate) {
                    if (audioBitrate <= 0) {
                        instance->audioBitrate = 64000;
                    } else {
                        instance->audioBitrate = audioBitrate;
                    }
                }

                int VideoCapture::getAudioBitrate() {
                    return instance->audioBitrate;
                }

                void VideoCapture::setAudioChannels(int audioChannels) {
                    if (audioChannels <= 0) {
                        instance->audioChannels = 1;
                    } else {
                        instance->audioChannels = audioChannels;
                    }
                }

                int VideoCapture::getAudioChannels() {
                    return instance->audioChannels;
                }

                void VideoCapture::setWidth(int width) {
                    if (width > 0) {
                        instance->width = width;
                    }
                }

                int VideoCapture::getWidth() {
                    return instance->width;
                }

                void VideoCapture::setHeight(int height) {
                    if (height > 0) {
                        instance->height = height;
                    }
                }

                int VideoCapture::getHeight() {
                    return instance->height;
                }

                void VideoCapture::setVideoFormat(const char* videoFormat) {
                    if (videoFormat != NULL) {
                        instance->videoFormat = videoFormat;
                    }
                    else{
                        instance->videoFormat = "avi";
                    }
                }

                const char* VideoCapture::getVideoFormat() {
                    return instance->videoFormat.c_str();
                }

                void VideoCapture::setVideoCodec(const char* videoCodec) {
                    if (videoCodec != NULL) {
                        instance->videoCodec = videoCodec;
                    } else {
                        instance->videoCodec = "mpeg2video";
                    }
                }

                const char* VideoCapture::getVideoCodec() {
                    return instance->videoCodec.c_str();
                }

                void VideoCapture::setAudioCodec(const char* audioCodec) {
                    if (audioCodec != NULL) {
                        instance->audioCodec = audioCodec;
                    } else {
                        instance->audioCodec = "mp2";
                    }
                }

                const char* VideoCapture::getAudioCodec() {
                    return instance->audioCodec.c_str();
                }

                void VideoCapture::addDestination(const char* destination) throw () {//TODO: adicionar exceções; quebrar destination em protocolo, etc.
                    string* s = new string(destination);
                    instance->destinations.push_back(s);
                    stop();
                    start();
                }

                void VideoCapture::removeDestination(const char* destination) throw () {
                    for (int i = 0; i < instance->destinations.size(); i++){
                        if (!strcmp(instance->destinations[i]->c_str(), destination)){
                            string* s = instance->destinations[i];
                            instance->destinations.erase(instance->destinations.begin() + i--);
                            delete s;
                            stop();
                            start();
                        }
                    }
                }

                void VideoCapture::addDestination(vector<const char*> destination) throw () {
                    for (int i = 0; i < destination.size(); i++){
                        try{
                            string* s = new string(destination[i]);
                            instance->destinations.push_back(s);
                        } catch(...){
                            throw;
                        }
                    }
                    stop();
                    start();
                }

                void VideoCapture::removeDestination(vector<const char*> destination) throw () {
                    for (int i = 0; i < destination.size(); i++){
                        try{
                            for (int j = 0; j < instance->destinations.size(); j++) {
                                if (!strcmp(instance->destinations[j]->c_str(), destination[i])) {
                                    string* s = instance->destinations[j];
                                    instance->destinations.erase(instance->destinations.begin() + j--);
                                    delete s;
                                }
                            }
                        } catch(...){
                            throw;
                        }
                    }
                    stop();
                    start();
                }

                vector<const char*> VideoCapture::getDestinations() {
                    vector<const char*> dest;

                    for (int i = 0; i < instance->destinations.size(); i++){
                        dest.push_back(instance->destinations[i]->c_str());
                    }

                    return dest;
                }

                bool VideoCapture::addOutputStream(OutputStream* stream) {
                    return false;
                }

                bool VideoCapture::removeOutputStream(OutputStream* stream) {
                    return false;
                }

                bool VideoCapture::isRunning(){
                    return running;
                }

                bool VideoCapture::start() {
                    if (isRunning() == false) {
                        if (pthread_create(&thread, NULL, createVideoCaptureThread, this) != 0)
                            return false;

                        pthread_detach(thread);

                        running = true;

                        return true;
                    } else {
                        return false;
                    }
                }

                bool VideoCapture::stop() {
                    if (isRunning() == true) {
                        ffmpegCapture.stop_recording();

                        pthread_cancel(thread);
                        pthread_detach(thread);

                        running = false;

                        return true;
                    } else {
                        return false;
                    }
                }

                void* VideoCapture::createVideoCaptureThread(void* param) {
                    pthread_testcancel();

                    int i;
                    vector<char*> arguments;
                    char** argv;
                    stringstream sR, aC, aB, sW, sH, fR, vB;
                    string sSR, sAC, sAB, sFR, sVB, resolution;

                    sW << instance->width;
                    sH << instance->height;
                    resolution = sW.str() + "x" + sH.str();

                    fR << instance->frameRate;
                    sFR = fR.str();

                    vB << instance->videoBitrate;
                    sVB = vB.str();

                    sR << instance->sampleRate;
                    sSR = sR.str();

                    aC << instance->audioChannels;
                    sAC = aC.str();

                    aB << instance->audioBitrate;
                    sAB = aB.str();

                    arguments.push_back("ffmpeg");
                    arguments.push_back("-fflags");
                    arguments.push_back("+genpts");
                    arguments.push_back("-re");
                    arguments.push_back("-f");
                    arguments.push_back("fbgrab");
                    arguments.push_back("-r");
                    arguments.push_back((char*) sFR.c_str());
                    arguments.push_back("-i");
                    arguments.push_back(":");
                    arguments.push_back("-f");
                    arguments.push_back("mpeg2video");

                    if (instance->videoFormat == "rtp"){
                        arguments.push_back("-an");
                    }

                    arguments.push_back("-vcodec");
                    arguments.push_back((char*) instance->videoCodec.c_str());
                    arguments.push_back("-sameq");

                    if (instance->videoFormat == "rtp"){
                        arguments.push_back("-f");
                        arguments.push_back((char*) instance->videoFormat.c_str());
                        arguments.push_back((char*) instance->destinations[0]->c_str());
                    }

                    arguments.push_back("-f");
#if HAVE_ALSA_ASOUNDLIB_H
                    arguments.push_back("alsa");
#else
                    arguments.push_back("oss");
#endif
                    arguments.push_back("-ar");
                    arguments.push_back((char*) sSR.c_str());
                    arguments.push_back("-ac");
                    arguments.push_back((char*) sAC.c_str());
                    arguments.push_back("-ab");
                    arguments.push_back((char*) sAB.c_str());
                    arguments.push_back("-i");
#if HAVE_ALSA_ASOUNDLIB_H
                    arguments.push_back("default");
#else
                    arguments.push_back("/dev/dsp");
#endif
                    arguments.push_back("-acodec");
                    arguments.push_back("pcm_s16le");

                    if (instance->videoFormat == "rtp"){
                        arguments.push_back("-vn");
                    }

                    arguments.push_back("-acodec");
                    arguments.push_back((char*) instance->audioCodec.c_str());
                    arguments.push_back("-sameq");
                    arguments.push_back("-f");
                    arguments.push_back((char*) instance->videoFormat.c_str());

                    if (instance->videoFormat == "rtp"){
                        arguments.push_back((char*) instance->destinations[1]->c_str());
                        arguments.push_back("-newaudio");
                        i = 39;
                    } else{
                        arguments.push_back((char*) instance->destinations[0]->c_str());
                        i = 33;
                    }

                    argv = &arguments[0];

                    ffmpegCapture.set_recording(i, argv);
                    try {
                        ffmpegCapture.run();
                    } catch (...) {
                        throw;
                        return param;
                    }

                    return NULL;
                }

                void VideoCapture::write(int b) {
                }

                int VideoCapture::write(const char* b, int len) {
                    return 0;
                }

                    }
                }
            }
        }
    }
}

