#include "honesconfig.h"

#include "capture/Exceptions.h"
#include "capture/ImageCapture.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace capture {

                ImageCapture* ImageCapture::instance = NULL;
                GingaFFMpeg ImageCapture::ffmpegCapture;

                ImageCapture::ImageCapture() {
                    width = 1024;
                    height = 768;
                    imageFormat = "png";
                }

                ImageCapture::~ImageCapture() {
                }

                ImageCapture* ImageCapture::ImageCaptureFactory() {
                    if (instance == NULL) {
                        instance = new ImageCapture();
                    }
                    return instance;
                }

                void ImageCapture::ImageCaptureDestructor() {
                    if (instance != NULL) {
                        delete instance;
                        instance = NULL;
                    }
                }

                bool ImageCapture::capture() throw () {
                    int i;
                    vector<char*> arguments;
                    char** argv;
                    stringstream sw, sh;
                    string resolution;

                    sw << instance->width;
                    sh << instance->height;
                    resolution = sw.str() + "x" + sh.str();

                    arguments.push_back("ffmpeg");
                    arguments.push_back("-f");
                    arguments.push_back("fbgrab");
                    arguments.push_back("-r");
                    arguments.push_back("1");
                    arguments.push_back("-i");
                    arguments.push_back(":");
                    arguments.push_back("-f");
                    arguments.push_back("rawvideo");
                    arguments.push_back("-s");
                    arguments.push_back((char*) resolution.c_str());
                    arguments.push_back("-vcodec");
                    arguments.push_back((char*) instance->imageFormat.c_str());
                    arguments.push_back("-vframes");
                    arguments.push_back("1");
                    arguments.push_back("-an");
                    arguments.push_back((char*) instance->destination.c_str());

                    argv = &arguments[0];
                    i = 17;

                    ffmpegCapture.set_recording(i, argv);
                    try {
                        ffmpegCapture.run();
                    } catch (...) {
                        throw;
                        return false;
                    }
                    ffmpegCapture.stop_recording();

                    return true;
                }

                void ImageCapture::setWidth(int width) {
                    if (width > 0) {
                        instance->width = width;
                    }
                }

                int ImageCapture::getWidth() {
                    return instance->width;
                }

                void ImageCapture::setHeight(int height) {
                    if (height > 0) {
                        instance->height = height;
                    }
                }

                int ImageCapture::getHeight() {
                    return instance->height;
                }

                void ImageCapture::setImageFormat(const char* imageFormat) {
                    if (imageFormat != NULL) {
                        instance->imageFormat = imageFormat;
                    }
                }

                const char* ImageCapture::getImageFormat() {
                    return instance->imageFormat.c_str();
                }

                void ImageCapture::setDestination(const char* destination) throw () {
                    if (destination != NULL) {
                        instance->destination = destination;
                    }
                }

                const char* ImageCapture::getDestination() {
                    return instance->destination.c_str();
                }

                bool ImageCapture::addOutputStream(OutputStream* stream) {
                    return false;
                }

                bool ImageCapture::removeOutputStream(OutputStream* stream) {
                    return false;
                }

                void ImageCapture::write(int b) {

                }

                int ImageCapture::write(const char* b, int len) {
                    return 0;
                }

                    }
                }
            }
        }
    }
}


