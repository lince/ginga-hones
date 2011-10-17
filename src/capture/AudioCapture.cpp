#include "honesconfig.h"

#include <sstream>
#include "capture/Exceptions.h"
#include "capture/AudioCapture.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace capture {
                AudioCapture* AudioCapture::instance = NULL;
                GingaFFMpeg AudioCapture::ffmpegCapture;

                AudioCapture::AudioCapture() {
                    running = false;
                    sampleRate = 44100;
                    audioBitrate = 64000;
                    audioChannels = 1;
                    audioFormat = "mp2";
                    audioCodec = "mp2";
                }

                AudioCapture::~AudioCapture() {
                    stop();
                }

                AudioCapture* AudioCapture::AudioCaptureFactory() {
                    if (instance == NULL) {
                        instance = new AudioCapture();
                    }
                    return instance;
                }

                void AudioCapture::AudioCaptureDestructor(){
                    if (instance != NULL) {
                        delete instance;
                        instance = NULL;
                    }
                }

                void AudioCapture::setSampleRate(int sampleRate) {
                    if (sampleRate <= 0) {
                        instance->sampleRate = 44100;
                    } else {
                        instance->sampleRate = sampleRate;
                    }
                }

                int AudioCapture::getSampleRate() {
                    return instance->sampleRate;
                }

                void AudioCapture::setAudioBitrate(int audioBitrate) {
                    if (audioBitrate <= 0) {
                        instance->audioBitrate = 64000;
                    } else {
                        instance->audioBitrate = audioBitrate;
                    }
                }

                int AudioCapture::getAudioBitrate() {
                    return instance->audioBitrate;
                }

                void AudioCapture::setAudioChannels(int audioChannels) {
                    if (audioChannels <= 0) {
                        instance->audioChannels = 1;
                    } else {
                        instance->audioChannels = audioChannels;
                    }
                }

                int AudioCapture::getAudioChannels() {
                    return instance->audioChannels;
                }

                void AudioCapture::setAudioFormat(const char* audioFormat) {
                    if (audioFormat != NULL) {
                        instance->audioFormat = audioFormat;
                    } else {
                        instance->audioFormat = "mp2";
                    }
                }

                const char* AudioCapture::getAudioFormat() {
                    return instance->audioFormat.c_str();
                }

                void AudioCapture::setAudioCodec(const char* audioCodec) {
                    if (audioCodec != NULL) {
                        instance->audioCodec = audioCodec;
                    } else {
                        instance->audioCodec = "mp2";
                    }
                }

                const char* AudioCapture::getAudioCodec() {
                    return instance->audioCodec.c_str();
                }

                void AudioCapture::addDestination(const char* destination) throw () {//TODO: adicionar exceções; quebrar destination em protocolo, etc.
                    string* s = new string(destination);
                    instance->destinations.push_back(s);
                    stop();
                    start();
                }

                void AudioCapture::removeDestination(const char* destination) throw () {
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

                void AudioCapture::addDestination(vector<const char*> destination) throw () {
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

                void AudioCapture::removeDestination(vector<const char*> destination) throw () {
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

                vector<const char*> AudioCapture::getDestinations() {
                    vector<const char*> dest;

                    for (int i = 0; i < instance->destinations.size(); i++){
                        dest.push_back(instance->destinations[i]->c_str());
                    }

                    return dest;
                }

                bool AudioCapture::addOutputStream(OutputStream* stream) {
                    return false;
                }

                bool AudioCapture::removeOutputStream(OutputStream* stream) {
                    return false;
                }

                bool AudioCapture::isRunning(){
                    return running;
                }

                bool AudioCapture::start() {
                    if (isRunning() == false) {
                        if (pthread_create(&thread, NULL, createAudioCaptureThread, this) != 0)
                            return false;

                        pthread_detach(thread);
                        
                        running = true;

                        return true;
                    } else {
                        return false;
                    }
                }

                bool AudioCapture::stop() {
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

                void* AudioCapture::createAudioCaptureThread(void* param) {
                    pthread_testcancel();

                    int i;
                    vector<char*> arguments;
                    char** argv;
                    stringstream sR, aC, aB;
                    string sSR, sAC, sAB;

                    sR << instance->sampleRate;
                    sSR = sR.str();

                    aC << instance->audioChannels;
                    sAC = aC.str();

                    aB << instance->audioBitrate;
                    sAB = aB.str();

                    arguments.push_back("ffmpeg");
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
                    arguments.push_back("-vn");
                    arguments.push_back("-acodec");
                    arguments.push_back((char*) instance->audioCodec.c_str());
                    arguments.push_back("-sameq");
                    arguments.push_back("-f");
                    arguments.push_back((char*) instance->audioFormat.c_str());
                    arguments.push_back((char*) instance->destinations[0]->c_str());

                    argv = &arguments[0];
                    i = 20;

                    ffmpegCapture.set_recording(i, argv);
                    try {
                        ffmpegCapture.run();
                    } catch (...) {
                        throw;
                        return NULL;
                    }

                    return param;
                }

                void AudioCapture::write(int b) {

                }

                int AudioCapture::write(const char* b, int len) {
                    return 0;
                }


                    }
                }
            }
        }
    }
}


