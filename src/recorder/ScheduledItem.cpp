#include "honesconfig.h"

#include <time.h>
#include <stdlib.h>
#include "recorder/ScheduledItem.h"

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace recorder {

                        ScheduledItem::ScheduledItem() {
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
                            time_t t = time(NULL);
                            char dateNow[10];
                            strftime(dateNow, sizeof (dateNow), "%d/%m/%Y", gmtime(&t));
                            beginDate = dateNow;
                            char timeNow[8];
                            strftime(timeNow, sizeof (timeNow), "%H:%M:%S", gmtime(&t));
                            beginTime = timeNow;
                            duration = 0;
                        }

                        ScheduledItem::~ScheduledItem() {
                        }

                        void ScheduledItem::setFrameRate(int frameRate) {
                            if(frameRate > 0)
                                this->frameRate = frameRate;
                        }

                        int ScheduledItem::getFrameRate() {
                            return frameRate;
                        }

                        void ScheduledItem::setSampleRate(int sampleRate) {
                            if(sampleRate > 0)
                                this->sampleRate = sampleRate;
                        }

                        int ScheduledItem::getSampleRate() {
                            return sampleRate;
                        }

                        void ScheduledItem::setVideoBitrate(int videoBitrate) {
                            if(videoBitrate > 0)
                                this->videoBitrate = videoBitrate;
                        }

                        int ScheduledItem::getVideoBitrate() {
                            return videoBitrate;
                        }

                        void ScheduledItem::setAudioBitrate(int audioBitrate) {
                            if(audioBitrate > 0)
                                this->audioBitrate = audioBitrate;
                        }

                        int ScheduledItem::getAudioBitrate() {
                            return audioBitrate;
                        }

                        void ScheduledItem::setAudioChannels(int audioChannels) {
                            if(audioChannels > 0)
                                this->audioChannels = audioChannels;
                        }

                        int ScheduledItem::getAudioChannels() {
                            return audioChannels;
                        }

                        void ScheduledItem::setWidth(int width) {
                            if(width > 0)
                                this->width = width;
                        }

                        int ScheduledItem::getWidth() {
                            return width;
                        }

                        void ScheduledItem::setHeight(int height) {
                            if(height > 0)
                                this->height = height;
                        }

                        int ScheduledItem::getHeight() {
                            return height;
                        }

                        void ScheduledItem::setVideoFormat(const char* videoFormat) {
                            if(videoFormat != NULL)
                                this->videoFormat = videoFormat;
                        }

                        const char* ScheduledItem::getVideoFormat() {
                            return videoFormat.c_str();
                        }

                        void ScheduledItem::setVideoCodec(const char* videoCodec) {
                            if(videoCodec != NULL)
                                this->videoCodec = videoCodec;
                        }

                        const char* ScheduledItem::getVideoCodec() {
                            return videoCodec.c_str();
                        }

                        void ScheduledItem::setAudioCodec(const char* audioCodec) {
                            if(audioCodec != NULL)
                                this->audioCodec = audioCodec;
                        }

                        const char* ScheduledItem::getAudioCodec() {
                            return audioCodec.c_str();
                        }

                        void ScheduledItem::setDestinationFile(const char* destination) {
                            if(destination != NULL)
                                this->destinationFile = destination;
                        }

                        const char* ScheduledItem::getDestinationFile() {
                            return destinationFile.c_str();
                        }

                        void ScheduledItem::setBeginRecordingDate(const char* date) {
                            if(date != NULL)
                                beginDate = date;
                        }

                        const char* ScheduledItem::getBeginRecordingDate() {
                            return beginDate.c_str();
                        }

                        void ScheduledItem::setBeginRecordingTime(const char* time) {
                            if(time != NULL)
                                beginTime = time;
                        }

                        const char* ScheduledItem::getBeginRecordingTime() {
                            return beginTime.c_str();
                        }

                        void ScheduledItem::setEndRecordingDate(const char* date) {
                            if(date != NULL)
                                this->endDate = date;
                        }

                        const char* ScheduledItem::getEndRecordingDate() {
                            return endDate.c_str();
                        }

                        void ScheduledItem::setEndRecordingTime(const char* time) {
                            if(time != NULL)
                                this->endTime = time;
                        }

                        const char* ScheduledItem::getEndRecordingTime() {
                            return endTime.c_str();
                        }

                        void ScheduledItem::setRecordingDuration(double duration) {
                            if(duration > 0)
                                this->duration = duration;
                        }

                        double ScheduledItem::getRecordingDuration() {
                            if (duration > 0) {
                                return duration;
                            } else {
                                if ((endDate.size() > 0) && (endTime.size() > 0) && (beginDate.size() > 0) && (beginTime.size() > 0)) {
                                    string date1 = endDate;
                                    string time1 = endTime;
                                    string date2 = beginDate;
                                    string time2 = beginTime;
                                    int day1, month1, year1, day2, month2, year2;
                                    int hour1, minute1, second1, hour2, minute2, second2;

                                    day1 = atol(date1.substr(0, date1.find("/")).c_str());
                                    date1 = date1.substr(date1.find("/") + 1);
                                    month1 = atol(date1.substr(0, date1.find("/")).c_str());
                                    date1 = date1.substr(date1.find("/") + 1);
                                    year1 = atol(date1.c_str());

                                    hour1 = atol(time1.substr(0, time1.find(":")).c_str());
                                    time1 = time1.substr(time1.find(":") + 1);
                                    minute1 = atol(time1.substr(0, time1.find(":")).c_str());
                                    time1 = time1.substr(time1.find(":") + 1);
                                    second1 = atol(time1.c_str());

                                    day2 = atol(date2.substr(0, date2.find("/")).c_str());
                                    date2 = date2.substr(date2.find("/") + 1);
                                    month2 = atol(date2.substr(0, date2.find("/")).c_str());
                                    date2 = date2.substr(date2.find("/") + 1);
                                    year2 = atol(date2.c_str());

                                    hour2 = atol(time2.substr(0, time2.find(":")).c_str());
                                    time2 = time2.substr(time2.find(":") + 1);
                                    minute2 = atol(time2.substr(0, time2.find(":")).c_str());
                                    time2 = time2.substr(time2.find(":") + 1);
                                    second2 = atol(time2.c_str());

                                    struct tm timeinfo1;
                                    timeinfo1.tm_year = year1 - 1900;
                                    timeinfo1.tm_mon = month1 - 1;
                                    timeinfo1.tm_mday = day1;
                                    timeinfo1.tm_hour = hour1;
                                    timeinfo1.tm_min = minute1;
                                    timeinfo1.tm_sec = second1;

                                    struct tm timeinfo2;
                                    timeinfo2.tm_year = year2 - 1900;
                                    timeinfo2.tm_mon = month2 - 1;
                                    timeinfo2.tm_mday = day2;
                                    timeinfo2.tm_hour = hour2;
                                    timeinfo2.tm_min = minute2;
                                    timeinfo2.tm_sec = second2;

                                    time_t time_t1 = mktime(&timeinfo1);

                                    time_t time_t2 = mktime(&timeinfo2);

                                    return difftime(time_t1, time_t2);
                                } else {
                                    return 0;
                                }
                            }
                        }

                    }
                }
            }
        }
    }
}
