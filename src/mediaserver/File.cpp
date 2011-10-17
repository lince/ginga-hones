#include "honesconfig.h"

#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "mediaserver/File.h"
#include "util/Utils.h"
using namespace br::ufscar::lince::ginga::hones::util;
#ifdef HAVE_AVFORMAT
#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>
#ifdef __cplusplus
}
#endif
#endif

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                        File::File(const char* path) {
                            struct stat st;
                            readable = false;
                            directory = false;
                            length = (off_t) 0;
                            type = NULL;
                            dlnaAdditionalInfo = NULL;
                            duration = title = author = album = genre = comment = NULL;
                            bitrate = channels = samplerate = width = height = track = year = 0;
                            if (stat(path, &st) >= 0) {
                                if (access(path, R_OK) >= 0) {
                                    readable = true;
                                }
                                length = st.st_size;
                                directory = S_ISDIR(st.st_mode);
                                lastModified = st.st_mtime;
                            }
                            setPath(path);
                            string tmp = path;
                            tmp = tmp.substr(tmp.find_last_of('/') + 1);
                            setName(tmp.c_str());
                            if (directory == false) {
                                suffix = tmp.substr(tmp.find_last_of('.') + 1);
                                const char* mime = Utils::getMimeType(path);
                                if (mime != NULL) {
                                    type = strdup(mime);
                                    upnpClass = Utils::getUPnPClass(mime);
                                    dlnaAdditionalInfo = Utils::getDLNAAdditionalInfo(path);
                                    initializeMediaProperties();
                                }
                            }
                        }

                        File::~File() {
                            if (type != NULL)
                                delete type;
                            if (dlnaAdditionalInfo != NULL)
                                free(dlnaAdditionalInfo);
                        }

                        const char* File::getName() {
                            return name.c_str();
                        }

                        void File::setName(const char* name) {
                            this->name = name;
                        }

                        const char* File::getMimeType() {
                            return type;
                        }

                        const char* File::getUPnPClass() {
                            return upnpClass;
                        }

                        const char* File::getDLNAAdditionalInfo() {
                            return dlnaAdditionalInfo;
                        }

                        const char* File::getSuffix() {
                            return suffix.c_str();
                        }

                        void File::setPath(const char* path) {
                            this->path = path;
                        }

                        const char* File::getPath() {
                            return path.c_str();
                        }

                        void File::setID(const char* id) {
                            this->id = id;
                        }

                        const char* File::getID() {
                            return id.c_str();
                        }

                        void File::setParentID(const char* id) {
                            this->parentId = id;
                        }

                        const char* File::getParentID() {
                            return parentId.c_str();
                        }

                        off_t File::getLength() {
                            return length;
                        }

                        bool File::isReadable() {
                            return readable;
                        }

                        bool File::isDirectory() {
                            return directory;
                        }

                        const time_t File::getLastModified() {
                            return lastModified;
                        }

                        bool File::isAudio() {
                            if (type != NULL) {
                                string mtype = type;
                                return (mtype.find("audio") == 0);
                            } else {
                                return false;
                            }
                        }

                        bool File::isVideo() {
                            if (type != NULL) {
                                string mtype = type;
                                return (mtype.find("video") == 0);
                            } else {
                                return false;
                            }
                        }

                        bool File::isImage() {
                            if (type != NULL) {
                                string mtype = type;
                                return (mtype.find("image") == 0);
                            } else {
                                return false;
                            }
                        }

                        const char* File::getDuration() {
                            return duration;
                        }

                        const char* File::getTitle() {
                            return title;
                        }

                        const char* File::getAuthor() {
                            return author;
                        }

                        const char* File::getAlbum() {
                            return album;
                        }

                        const char* File::getGenre() {
                            return genre;
                        }

                        const char* File::getComment() {
                            return comment;
                        }

                        int File::getBitrate() {
                            return bitrate;
                        }

                        int File::getChannels() {
                            return channels;
                        }

                        int File::getSamplerate() {
                            return samplerate;
                        }

                        int File::getWidth() {
                            return width;
                        }

                        int File::getHeight() {
                            return height;
                        }

                        int File::getTrack() {
                            return track;
                        }

                        int File::getYear() {
                            return year;
                        }

                        void File::initializeMediaProperties() {
#ifdef HAVE_AVFORMAT
                            av_register_all();
                            AVFormatContext* ctx;
                            if ((av_open_input_file(&ctx, getPath(), NULL, 0, NULL) == 0) && (av_find_stream_info(ctx) >= 0)) {
                                
                                // duration
                                if (ctx->duration != AV_NOPTS_VALUE)
                                    duration = strdup(Utils::timeToString(ctx->duration / AV_TIME_BASE).c_str());
                                // bitrate
                                if (ctx->bit_rate > 0)
                                    bitrate = ctx->bit_rate / 8;

                                for (unsigned int i = 0; i < ctx->nb_streams; i++) {
                                    // samplerate
                                    if(samplerate <= 0)
                                        samplerate = ctx->streams[i]->codec->sample_rate;
                                    // channels
                                    if(channels <= 0)
                                        channels = ctx->streams[i]->codec->channels;
                                    // width
                                    if(width <= 0)
                                        width = ctx->streams[i]->codec->width;
                                    // height
                                    if(height <= 0)
                                        height = ctx->streams[i]->codec->height;
                                    if ((samplerate > 0) && (channels > 0) && (width > 0) && (height > 0))
                                        i = ctx->nb_streams;
                                }

#if LIBAVFORMAT_VERSION_INT < (53<<16)
                                // title
                                if(ctx->title != NULL)
                                    title = strdup(ctx->title);
                                // artist
                                if(ctx->author != NULL)
                                    author = strdup(ctx->author);
                                // album
                                if(ctx->album != NULL)
                                    album = strdup(ctx->album);
                                // genre
                                if(ctx->genre != NULL)
                                    genre = strdup(ctx->genre);
                                // description / comment
                                if(ctx->comment != NULL)
                                    comment = strdup(ctx->comment);
                                // track number
                                if(ctx->track > 0)
                                    track = ctx->track;
                                // date / year
                                if(ctx->year > 0)
                                    year = ctx->year;
#else
                                av_metadata_conv(ctx, NULL, ctx->iformat->metadata_conv);
                                AVMetadataTag *mtitle, *mauthor, *malbum, *mgenre, *mcomment, *mtrack, *myear;
                                mtitle = av_metadata_get(ctx->metadata, "title", NULL, 0);
                                mauthor = av_metadata_get(ctx->metadata, "author", NULL, 0);
                                malbum = av_metadata_get(ctx->metadata, "album", NULL, 0);
                                mgenre = av_metadata_get(ctx->metadata, "genre", NULL, 0);
                                mcomment = av_metadata_get(ctx->metadata, "comment", NULL, 0);
                                mtrack = av_metadata_get(ctx->metadata, "track", NULL, 0);
                                myear = av_metadata_get(ctx->metadata, "year", NULL, 0);
                                // title
                                if(mtitle != NULL)
                                    title = strdup(mtitle->value);
                                // artist
                                if(mauthor != NULL)
                                    author = strdup(mauthor->value);
                                // album
                                if(malbum != NULL)
                                    album = strdup(malbum->value);
                                // genre
                                if(mgenre != NULL)
                                    genre = strdup(mgenre->value);
                                // description / comment
                                if(mcomment != NULL)
                                    comment = strdup(mcomment->value);
                                // track number
                                if(mtrack != NULL)
                                    track = Utils::stringToInteger(mtrack->value);
                                // date / year
                                if(myear != NULL)
                                    year = Utils::stringToInteger(myear->value);
#endif //LIBAVFORMAT_VERSION_INT
                            }
                            av_close_input_file(ctx);
#endif //HAVE_AVFORMAT
                        }

                    }
                }
            }
        }
    }
}
