#include "honesconfig.h"

#include <pthread.h>
#include <upnp/ixml.h>
#include "mediarenderer/TVRenderer.h"
#include "mediarenderer/TVRAVTransport.h"
#include "util/Utils.h"

using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediarenderer {
                        
                        const char *TVRAVTransport::SERVICE_TYPE = "urn:schemas-upnp-org:service:AVTransport:1";

                        const char *TVRAVTransport::SCPD =
                                "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\n"
                                "   <specVersion>\n"
                                "      <major>1</major>\n"
                                "      <minor>0</minor>\n"
                                "   </specVersion>\n"
                                "   <actionList>\n"
                                "      <action>\n"
                                "         <name>GetDeviceCapabilities</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>PlayMedia</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>PossiblePlaybackStorageMedia</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>RecMedia</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>PossibleRecordStorageMedia</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>RecQualityModes</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>PossibleRecordQualityModes</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetMediaInfo</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>NrTracks</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>NumberOfTracks</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>MediaDuration</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentMediaDuration</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentURI</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>AVTransportURI</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentURIMetaData</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>AVTransportURIMetaData</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>NextURI</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>NextAVTransportURI</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>NextURIMetaData</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>NextAVTransportURIMetaData</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>PlayMedium</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>PlaybackStorageMedium</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>RecordMedium</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>RecordStorageMedium</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>WriteStatus</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>RecordMediumWriteStatus</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetPositionInfo</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Track</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentTrack</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>TrackDuration</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentTrackDuration</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>TrackMetaData</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentTrackMetaData</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>TrackURI</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentTrackURI</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>RelTime</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>RelativeTimePosition</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>AbsTime</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>AbsoluteTimePosition</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>RelCount</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>RelativeCounterPosition</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>AbsCount</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>AbsoluteCounterPosition</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetTransportInfo</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentTransportState</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>TransportState</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentTransportStatus</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>TransportStatus</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentSpeed</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>TransportPlaySpeed</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetTransportSettings</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>PlayMode</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentPlayMode</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>RecQualityMode</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentRecordQualityMode</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>Next</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>Pause</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>Play</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Speed</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>TransportPlaySpeed</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>Previous</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>Seek</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Unit</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_SeekMode</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Target</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_SeekTarget</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetAVTransportURI</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentURI</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>AVTransportURI</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentURIMetaData</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>AVTransportURIMetaData</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>Stop</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "   </actionList>\n"
                                "   <serviceStateTable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>CurrentPlayMode</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>NORMAL</allowedValue>\n"
                                "            <allowedValue>SHUFFLE</allowedValue>\n"
                                "            <allowedValue>REPEAT_ONE</allowedValue>\n"
                                "            <allowedValue>REPEAT_ALL</allowedValue>\n"
                                "            <allowedValue>RANDOM</allowedValue>\n"
                                "            <allowedValue>DIRECT_1</allowedValue>\n"
                                "            <allowedValue>INTRO</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "         <defaultValue>NORMAL</defaultValue>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>RecordStorageMedium</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>UNKNOWN</allowedValue>\n"
                                "            <allowedValue>DV</allowedValue>\n"
                                "            <allowedValue>MINI-DV</allowedValue>\n"
                                "            <allowedValue>VHS</allowedValue>\n"
                                "            <allowedValue>W-VHS</allowedValue>\n"
                                "            <allowedValue>S-VHS</allowedValue>\n"
                                "            <allowedValue>D-VHS</allowedValue>\n"
                                "            <allowedValue>VHSC</allowedValue>\n"
                                "            <allowedValue>VIDEO8</allowedValue>\n"
                                "            <allowedValue>HI8</allowedValue>\n"
                                "            <allowedValue>CD-ROM</allowedValue>\n"
                                "            <allowedValue>CD-DA</allowedValue>\n"
                                "            <allowedValue>CD-R</allowedValue>\n"
                                "            <allowedValue>CD-RW</allowedValue>\n"
                                "            <allowedValue>VIDEO-CD</allowedValue>\n"
                                "            <allowedValue>SACD</allowedValue>\n"
                                "            <allowedValue>MD-AUDIO</allowedValue>\n"
                                "            <allowedValue>MD-PICTURE</allowedValue>\n"
                                "            <allowedValue>DVD-ROM</allowedValue>\n"
                                "            <allowedValue>DVD-VIDEO</allowedValue>\n"
                                "            <allowedValue>DVD-R</allowedValue>\n"
                                "            <allowedValue>DVD+RW</allowedValue>\n"
                                "            <allowedValue>DVD-RW</allowedValue>\n"
                                "            <allowedValue>DVD-RAM</allowedValue>\n"
                                "            <allowedValue>DVD-AUDIO</allowedValue>\n"
                                "            <allowedValue>DAT</allowedValue>\n"
                                "            <allowedValue>LD</allowedValue>\n"
                                "            <allowedValue>HDD</allowedValue>\n"
                                "            <allowedValue>MICRO-MV</allowedValue>\n"
                                "            <allowedValue>NETWORK</allowedValue>\n"
                                "            <allowedValue>NONE</allowedValue>\n"
                                "            <allowedValue>NOT_IMPLEMENTED</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"yes\">\n"
                                "         <name>LastChange</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>RelativeTimePosition</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>CurrentTrackURI</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>CurrentTrackDuration</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>CurrentRecordQualityMode</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>0:EP</allowedValue>\n"
                                "            <allowedValue>1:LP</allowedValue>\n"
                                "            <allowedValue>2:SP</allowedValue>\n"
                                "            <allowedValue>0:BASIC</allowedValue>\n"
                                "            <allowedValue>1:MEDIUM</allowedValue>\n"
                                "            <allowedValue>2:HIGH</allowedValue>\n"
                                "            <allowedValue>NOT_IMPLEMENTED</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>CurrentMediaDuration</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>AbsoluteCounterPosition</name>\n"
                                "         <dataType>i4</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>RelativeCounterPosition</name>\n"
                                "         <dataType>i4</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_InstanceID</name>\n"
                                "         <dataType>ui4</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>AVTransportURI</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>TransportState</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>STOPPED</allowedValue>\n"
                                "            <allowedValue>PAUSED_PLAYBACK</allowedValue>\n"
                                "            <allowedValue>PAUSED_RECORDING</allowedValue>\n"
                                "            <allowedValue>PLAYING</allowedValue>\n"
                                "            <allowedValue>RECORDING</allowedValue>\n"
                                "            <allowedValue>TRANSITIONING</allowedValue>\n"
                                "            <allowedValue>NO_MEDIA_PRESENT</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>CurrentTrackMetaData</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>NextAVTransportURI</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>PossibleRecordQualityModes</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>CurrentTrack</name>\n"
                                "         <dataType>ui4</dataType>\n"
                                "         <allowedValueRange>\n"
                                "            <minimum>0</minimum>\n"
                                "            <maximum>4294967295</maximum>\n"
                                "            <step>1</step>\n"
                                "         </allowedValueRange>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>AbsoluteTimePosition</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>NextAVTransportURIMetaData</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>PlaybackStorageMedium</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>UNKNOWN</allowedValue>\n"
                                "            <allowedValue>DV</allowedValue>\n"
                                "            <allowedValue>MINI-DV</allowedValue>\n"
                                "            <allowedValue>VHS</allowedValue>\n"
                                "            <allowedValue>W-VHS</allowedValue>\n"
                                "            <allowedValue>S-VHS</allowedValue>\n"
                                "            <allowedValue>D-VHS</allowedValue>\n"
                                "            <allowedValue>VHSC</allowedValue>\n"
                                "            <allowedValue>VIDEO8</allowedValue>\n"
                                "            <allowedValue>HI8</allowedValue>\n"
                                "            <allowedValue>CD-ROM</allowedValue>\n"
                                "            <allowedValue>CD-DA</allowedValue>\n"
                                "            <allowedValue>CD-R</allowedValue>\n"
                                "            <allowedValue>CD-RW</allowedValue>\n"
                                "            <allowedValue>VIDEO-CD</allowedValue>\n"
                                "            <allowedValue>SACD</allowedValue>\n"
                                "            <allowedValue>MD-AUDIO</allowedValue>\n"
                                "            <allowedValue>MD-PICTURE</allowedValue>\n"
                                "            <allowedValue>DVD-ROM</allowedValue>\n"
                                "            <allowedValue>DVD-VIDEO</allowedValue>\n"
                                "            <allowedValue>DVD-R</allowedValue>\n"
                                "            <allowedValue>DVD+RW</allowedValue>\n"
                                "            <allowedValue>DVD-RW</allowedValue>\n"
                                "            <allowedValue>DVD-RAM</allowedValue>\n"
                                "            <allowedValue>DVD-AUDIO</allowedValue>\n"
                                "            <allowedValue>DAT</allowedValue>\n"
                                "            <allowedValue>LD</allowedValue>\n"
                                "            <allowedValue>HDD</allowedValue>\n"
                                "            <allowedValue>MICRO-MV</allowedValue>\n"
                                "            <allowedValue>NETWORK</allowedValue>\n"
                                "            <allowedValue>NONE</allowedValue>\n"
                                "            <allowedValue>NOT_IMPLEMENTED</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>RecordMediumWriteStatus</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>WRITABLE</allowedValue>\n"
                                "            <allowedValue>PROTECTED</allowedValue>\n"
                                "            <allowedValue>NOT_WRITABLE</allowedValue>\n"
                                "            <allowedValue>UNKNOWN</allowedValue>\n"
                                "            <allowedValue>NOT_IMPLEMENTED</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>PossiblePlaybackStorageMedia</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>AVTransportURIMetaData</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>NumberOfTracks</name>\n"
                                "         <dataType>ui4</dataType>\n"
                                "         <allowedValueRange>\n"
                                "            <minimum>0</minimum>\n"
                                "            <maximum>4294967295</maximum>\n"
                                "         </allowedValueRange>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_SeekMode</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>ABS_TIME</allowedValue>\n"
                                "            <allowedValue>REL_TIME</allowedValue>\n"
                                "            <allowedValue>ABS_COUNT</allowedValue>\n"
                                "            <allowedValue>REL_COUNT</allowedValue>\n"
                                "            <allowedValue>TRACK_NR</allowedValue>\n"
                                "            <allowedValue>CHANNEL_FREQ</allowedValue>\n"
                                "            <allowedValue>TAPE-INDEX</allowedValue>\n"
                                "            <allowedValue>FRAME</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_SeekTarget</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>PossibleRecordStorageMedia</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>TransportStatus</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>OK</allowedValue>\n"
                                "            <allowedValue>ERROR_OCCURRED</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>TransportPlaySpeed</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>1</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "   </serviceStateTable>\n"
                                "</scpd>";

                        const char *TVRAVTransport::INSTANCE_ID = "InstanceID";
                        const char *TVRAVTransport::CURRENT_URI = "CurrentURI";
                        const char *TVRAVTransport::CURRENT_URI_METADATA = "CurrentURIMetaData";
                        const char *TVRAVTransport::NR_TRACKS = "NrTracks";
                        const char *TVRAVTransport::MEDIA_DURATION = "MediaDuration";
                        const char *TVRAVTransport::NEXT_URI = "NextURI";
                        const char *TVRAVTransport::NEXT_URI_METADATA = "NextURIMetaData";
                        const char *TVRAVTransport::PLAY_MEDIUM = "PlayMedium";
                        const char *TVRAVTransport::RECORD_MEDIUM = "RecordMedium";
                        const char *TVRAVTransport::WRITE_STATUS = "WriteStatus";
                        const char *TVRAVTransport::NOT_IMPLEMENTED = "NOT_IMPLEMENTED";
                        const int TVRAVTransport::NOT_IMPLEMENTED_I4 = 2147483647;
                        const char *TVRAVTransport::NONE = "NONE";
                        const char *TVRAVTransport::OK = "OK";
                        const char *TVRAVTransport::NETWORK = "NETWORK";
                        const char *TVRAVTransport::CURRENT_TRANSPORT_STATE = "CurrentTransportState";
                        const char *TVRAVTransport::CURRENT_TRANSPORT_STATUS = "CurrentTransportStatus";
                        const char *TVRAVTransport::CURRENT_SPEED = "CurrentSpeed";
                        const char *TVRAVTransport::STOPPED = "STOPPED";
                        const char *TVRAVTransport::PLAYING = "PLAYING";
                        const char *TVRAVTransport::TRANSITIONING = "TRANSITIONING";
                        const char *TVRAVTransport::PAUSED_PLAYBACK = "PAUSED_PLAYBACK";
                        const char *TVRAVTransport::PAUSED_RECORDING = "PAUSED_RECORDING";
                        const char *TVRAVTransport::RECORDING = "RECORDING";
                        const char *TVRAVTransport::NO_MEDIA_PRESENT = "NO_MEDIA_PRESENT";
                        const char *TVRAVTransport::TRACK = "Track";
                        const char *TVRAVTransport::TRACK_DURATION = "TrackDuration";
                        const char *TVRAVTransport::TRACK_METADATA = "TrackMetaData";
                        const char *TVRAVTransport::TRACK_URI = "TrackURI";
                        const char *TVRAVTransport::REL_TIME = "RelTime";
                        const char *TVRAVTransport::ABS_TIME = "AbsTime";
                        const char *TVRAVTransport::REL_COUNT = "RelCount";
                        const char *TVRAVTransport::ABS_COUNT = "AbsCount";
                        const char *TVRAVTransport::PLAY_MEDIA = "PlayMedia";
                        const char *TVRAVTransport::REC_MEDIA = "RecMedia";
                        const char *TVRAVTransport::REC_QUALITY_MODES = "RecQualityModes";
                        const char *TVRAVTransport::NORMAL = "NORMAL";
                        const char *TVRAVTransport::SHUFFLE = "SHUFFLE";
                        const char *TVRAVTransport::REPEAT_ONE = "REPEAT_ONE";
                        const char *TVRAVTransport::REPEAT_ALL = "REPEAT_ALL";
                        const char *TVRAVTransport::RANDOM = "RANDOM";
                        const char *TVRAVTransport::DIRECT_1 = "DIRECT_1";
                        const char *TVRAVTransport::INTRO = "INTRO";
                        const char *TVRAVTransport::PLAY_MODE = "PlayMode";
                        const char *TVRAVTransport::REC_QUALITY_MODE = "RecQualityMode";
                        const char *TVRAVTransport::UNIT = "Unit";
                        const char *TVRAVTransport::TARGET = "Target";

                        const char *TVRAVTransport::PLAYBACK_STORAGE_MEDIUM = "PlaybackStorageMedium";
                        const char *TVRAVTransport::AVTRANSPORT_URI = "AVTransportURI";
                        const char *TVRAVTransport::AVTRANSPORT_URI_METADATA = "AVTransportURIMetaData";
                        const char *TVRAVTransport::CURRENT_MEDIA_DURATION = "CurrentMediaDuration";
                        const char *TVRAVTransport::NUMBER_OF_TRACKS = "NumberOfTracks";
                        const char *TVRAVTransport::TRANSPORT_STATE = "TransportState";
                        const char *TVRAVTransport::TRANSPORT_STATUS = "TransportStatus";
                        const char *TVRAVTransport::TRANSPORT_PLAY_SPEED = "TransportPlaySpeed";
                        const char *TVRAVTransport::CURRENT_TRACK = "CurrentTrack";
                        const char *TVRAVTransport::CURRENT_TRACK_DURATION = "CurrentTrackDuration";
                        const char *TVRAVTransport::CURRENT_TRACK_METADATA = "CurrentTrackMetaData";
                        const char *TVRAVTransport::CURRENT_TRACK_URI = "CurrentTrackURI";
                        const char *TVRAVTransport::RELATIVE_TIME_POSITION = "RelativeTimePosition";
                        const char *TVRAVTransport::ABSOLUTE_TIME_POSITION = "AbsoluteTimePosition";
                        const char *TVRAVTransport::RELATIVE_COUNTER_POSITION = "RelativeCounterPosition";
                        const char *TVRAVTransport::ABSOLUTE_COUNTER_POSITION = "AbsoluteCounterPosition";
                        const char *TVRAVTransport::POSSIBLE_PLAYBACK_STORAGE_MEDIA = "PossiblePlaybackStorageMedia";
                        const char *TVRAVTransport::POSSIBLE_RECORD_STORAGE_MEDIA = "PossibleRecordStorageMedia";
                        const char *TVRAVTransport::POSSIBLE_RECORD_QUALITY_MODES = "PossibleRecordQualityModes";
                        const char *TVRAVTransport::CURRENT_PLAY_MODE = "CurrentPlayMode";
                        const char *TVRAVTransport::CURRENT_RECORD_QUALITY_MODE = "CurrentRecordQualityMode";

                        const char *TVRAVTransport::SET_AVTRANSPORT_URI = "SetAVTransportURI";
                        const char *TVRAVTransport::GET_MEDIA_INFO = "GetMediaInfo";
                        const char *TVRAVTransport::GET_TRANSPORT_INFO = "GetTransportInfo";
                        const char *TVRAVTransport::GET_POSITION_INFO = "GetPositionInfo";
                        const char *TVRAVTransport::GET_DEVICE_CAPABILITIES = "GetDeviceCapabilities";
                        const char *TVRAVTransport::GET_TRANSPORT_SETTINGS = "GetTransportSettings";
                        const char *TVRAVTransport::STOP = "Stop";
                        const char *TVRAVTransport::PLAY = "Play";
                        const char *TVRAVTransport::SEEK = "Seek";
                        const char *TVRAVTransport::NEXT = "Next";
                        const char *TVRAVTransport::PREVIOUS = "Previous";
                        const char *TVRAVTransport::PAUSE = "Pause";


                        TVRAVTransport::TVRAVTransport(TVRenderer *tvr) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.mediarenderer.tvravtransport");
                            LoggerUtil_info(logger, "Entrando no construtor TVRAVTransport(tvr)!");

                            setMediaRenderer(tvr);

                            UPnPService *serv = getService();
                            bool ret = serv->loadSCPD(SCPD);
                            if (ret == false)
                                throw "Invalid description!";
                            serv->setUPnPActionListener(this);
                            serv->setUPnPQueryListener(this);

                        }

                        TVRAVTransport::~TVRAVTransport() {
                        }

                        void TVRAVTransport::initializeStateVariables() {
                            getService()->getUPnPStateVariable(PLAYBACK_STORAGE_MEDIUM)->setValue(NONE);
                            getService()->getUPnPStateVariable(AVTRANSPORT_URI)->setValue("");
                            getService()->getUPnPStateVariable(AVTRANSPORT_URI_METADATA)->setValue("");
                            getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION)->setValue("00:00:00");
                            getService()->getUPnPStateVariable(NUMBER_OF_TRACKS)->setValue(0);
                            getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(NO_MEDIA_PRESENT);
                            getService()->getUPnPStateVariable(TRANSPORT_STATUS)->setValue(OK);
                            getService()->getUPnPStateVariable(TRANSPORT_PLAY_SPEED)->setValue(1);
                            getService()->getUPnPStateVariable(CURRENT_TRACK)->setValue(0);
                            getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION)->setValue("00:00:00");
                            getService()->getUPnPStateVariable(CURRENT_TRACK_METADATA)->setValue("");
                            getService()->getUPnPStateVariable(CURRENT_TRACK_URI)->setValue("");
                            getService()->getUPnPStateVariable(RELATIVE_TIME_POSITION)->setValue("00:00:00");
                            getService()->getUPnPStateVariable(ABSOLUTE_TIME_POSITION)->setValue("00:00:00");
                            getService()->getUPnPStateVariable(RELATIVE_COUNTER_POSITION)->setValue(NOT_IMPLEMENTED_I4);
                            getService()->getUPnPStateVariable(ABSOLUTE_COUNTER_POSITION)->setValue(NOT_IMPLEMENTED_I4);
                            getService()->getUPnPStateVariable(POSSIBLE_PLAYBACK_STORAGE_MEDIA)->setValue("");
                            getService()->getUPnPStateVariable(POSSIBLE_RECORD_STORAGE_MEDIA)->setValue(NOT_IMPLEMENTED);
                            getService()->getUPnPStateVariable(POSSIBLE_RECORD_QUALITY_MODES)->setValue("");
                            getService()->getUPnPStateVariable(CURRENT_PLAY_MODE)->setValue(NORMAL);
                            getService()->getUPnPStateVariable(CURRENT_RECORD_QUALITY_MODE)->setValue(NOT_IMPLEMENTED);
                        }

                        TVRenderer* TVRAVTransport::getMediaRenderer() {
                            return tvRender;
                        }

                        void TVRAVTransport::setMediaRenderer(TVRenderer *tvr) {
                            tvRender = tvr;
                        }

                        UPnPService* TVRAVTransport::getService() {
                            return getMediaRenderer()->getUPnPService(SERVICE_TYPE);
                        }

                        bool TVRAVTransport::actionRequest(UPnPAction *action) {
                            string actionName = action->getName();
                            string id = action->getUPnPArgumentValue(INSTANCE_ID);
                            if (id.compare("0") != 0) {
                                action->setStatus(UPNP_SOAP_E_INVALID_INSTANCE_ID, "Invalid InstanceID");
                                return false;
                            }

                            if (actionName.compare(SET_AVTRANSPORT_URI) == 0)
                                return actionSetAVTransportURI(action);

                            if (actionName.compare(GET_MEDIA_INFO) == 0)
                                return actionGetMediaInfo(action);

                            if (actionName.compare(GET_TRANSPORT_INFO) == 0)
                                return actionGetTransportInfo(action);

                            if (actionName.compare(GET_POSITION_INFO) == 0)
                                return actionGetPositionInfo(action);

                            if (actionName.compare(GET_DEVICE_CAPABILITIES) == 0)
                                return actionGetDeviceCapabilities(action);

                            if (actionName.compare(GET_TRANSPORT_SETTINGS) == 0)
                                return actionGetTransportSettings(action);

                            if (actionName.compare(STOP) == 0)
                                return actionStop(action);

                            if (actionName.compare(PLAY) == 0)
                                return actionPlay(action);

                            if (actionName.compare(SEEK) == 0)
                                return actionSeek(action);

                            if (actionName.compare(NEXT) == 0)
                                return actionNext(action);

                            if (actionName.compare(PREVIOUS) == 0)
                                return actionPrevious(action);

                            if (actionName.compare(PAUSE) == 0)
                                return actionPause(action);

                            return false;
                        }

                        void* prepareMediaThread(void* param){
                            TVRAVTransport* transp = (TVRAVTransport*) param;

                            PlayerHandler::PlayerHandlerFactory()->prepareMedia();

                            string duration = transp->getService()->getUPnPStateVariable(TVRAVTransport::CURRENT_MEDIA_DURATION)->getValue();
                            if (duration.compare(Utils::timeToString(0)) == 0) {
                                duration = Utils::timeToString(PlayerHandler::PlayerHandlerFactory()->getMediaDuration());
                                transp->getService()->getUPnPStateVariable(TVRAVTransport::CURRENT_MEDIA_DURATION)->setValue(duration.c_str());
                                Utils::updateLastChangeStateVariable(transp->getService(), transp->getService()->getUPnPStateVariable(TVRAVTransport::CURRENT_MEDIA_DURATION));
                                transp->getService()->getUPnPStateVariable(TVRAVTransport::CURRENT_TRACK_DURATION)->setValue(duration.c_str());
                                Utils::updateLastChangeStateVariable(transp->getService(), transp->getService()->getUPnPStateVariable(TVRAVTransport::CURRENT_TRACK_DURATION));
                            }

                            string tstate = transp->getService()->getUPnPStateVariable(TVRAVTransport::TRANSPORT_STATE)->getValue();
                            if (tstate.compare(TVRAVTransport::PLAYING) == 0)
                                PlayerHandler::PlayerHandlerFactory()->play();

                            return NULL;
                        }

                        bool TVRAVTransport::actionSetAVTransportURI(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionSetAVTransportURI(action)!");

                            string tstate = getService()->getUPnPStateVariable(TRANSPORT_STATE)->getValue();

                            string currentURI = action->getUPnPArgumentValue(CURRENT_URI);
                            getService()->getUPnPStateVariable(AVTRANSPORT_URI)->setValue(currentURI.c_str());
                            Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(AVTRANSPORT_URI));
                            string currentMetadata = action->getUPnPArgumentValue(CURRENT_URI_METADATA);
                            getService()->getUPnPStateVariable(AVTRANSPORT_URI_METADATA)->setValue(currentMetadata.c_str());
                            Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(AVTRANSPORT_URI_METADATA));

                            getMediaRenderer()->getPlayerHandler()->setURI(currentURI);
                            getMediaRenderer()->getPlayerHandler()->setMetadata(currentMetadata);

                            string duration;

                            if (currentMetadata.length() > 0) {
                                //Utils::parseXMLChars(currentMetadata);
                                IXML_Document* xml = ixmlParseBuffer(currentMetadata.c_str());
                                if (xml != NULL) {
                                    IXML_NodeList* resList = ixmlDocument_getElementsByTagName(xml, "res");
                                    if ((resList != NULL) && (ixmlNodeList_length(resList) > 0)) {
                                        IXML_Node* resNode = ixmlNodeList_item(resList, 0);
                                        if (resNode != NULL) {
                                            duration = Utils::getAttributeValue(resNode, "duration");
                                        }
                                    }
                                }
                            }

                            if (tstate.compare(NO_MEDIA_PRESENT) == 0) {
                                getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(TRANSITIONING);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));

                                pthread_t prepareThread;
                                pthread_create(&prepareThread, NULL, prepareMediaThread, this);
                                pthread_detach(prepareThread);

                                if (duration.length() <= 0)
                                    duration = Utils::timeToString(getMediaRenderer()->getPlayerHandler()->getMediaDuration());

                                getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION)->setValue(duration.c_str());
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION));
                                getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION)->setValue(duration.c_str());
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION));

                                getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(STOPPED);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));
                            } else if (tstate.compare(PLAYING) == 0) {
                                getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(TRANSITIONING);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));

                                pthread_t prepareThread;
                                pthread_create(&prepareThread, NULL, prepareMediaThread, this);
                                pthread_detach(prepareThread);

                                if (duration.length() <= 0)
                                    duration = Utils::timeToString(getMediaRenderer()->getPlayerHandler()->getMediaDuration());

                                getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION)->setValue(duration.c_str());
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION));
                                getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION)->setValue(duration.c_str());
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION));

                                getMediaRenderer()->getPlayerHandler()->play();

                                getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(PLAYING);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));

                                getService()->getUPnPStateVariable(NUMBER_OF_TRACKS)->setValue(1);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(NUMBER_OF_TRACKS));
                                getService()->getUPnPStateVariable(CURRENT_TRACK)->setValue(1);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_TRACK));

                                getService()->getUPnPStateVariable(TRANSPORT_PLAY_SPEED)->setValue(1);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_PLAY_SPEED));
                                getService()->getUPnPStateVariable(TRANSPORT_STATUS)->setValue(OK);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATUS));
                            } else {
                                pthread_t prepareThread;
                                pthread_create(&prepareThread, NULL, prepareMediaThread, this);
                                pthread_detach(prepareThread);

                                if (duration.length() <= 0)
                                    duration = Utils::timeToString(getMediaRenderer()->getPlayerHandler()->getMediaDuration());

                                getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION)->setValue(duration.c_str());
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION));
                                getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION)->setValue(duration.c_str());
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION));
                            }

                            if (currentURI.compare("") == 0) {
                                getService()->getUPnPStateVariable(PLAYBACK_STORAGE_MEDIUM)->setValue(NONE);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(PLAYBACK_STORAGE_MEDIUM));
                            } else {
                                getService()->getUPnPStateVariable(PLAYBACK_STORAGE_MEDIUM)->setValue(NETWORK);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(PLAYBACK_STORAGE_MEDIUM));
                            }

                            return true;
                        }

                        bool TVRAVTransport::actionGetMediaInfo(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetMediaInfo(action)!");

                            action->getUPnPArgument(NR_TRACKS)->setValue(atoi(getService()->getUPnPStateVariable(NUMBER_OF_TRACKS)->getValue()));
                            action->getUPnPArgument(MEDIA_DURATION)->setValue(getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION)->getValue());
                            action->getUPnPArgument(CURRENT_URI)->setValue(getService()->getUPnPStateVariable(AVTRANSPORT_URI)->getValue());
                            action->getUPnPArgument(CURRENT_URI_METADATA)->setValue(getService()->getUPnPStateVariable(AVTRANSPORT_URI_METADATA)->getValue());
                            action->getUPnPArgument(NEXT_URI)->setValue(NOT_IMPLEMENTED);
                            action->getUPnPArgument(NEXT_URI_METADATA)->setValue(NOT_IMPLEMENTED);
                            action->getUPnPArgument(PLAY_MEDIUM)->setValue(getService()->getUPnPStateVariable(PLAYBACK_STORAGE_MEDIUM)->getValue());
                            action->getUPnPArgument(RECORD_MEDIUM)->setValue(NOT_IMPLEMENTED);
                            action->getUPnPArgument(WRITE_STATUS)->setValue(NOT_IMPLEMENTED);

                            return true;
                        }

                        bool TVRAVTransport::actionGetTransportInfo(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetTransportInfo(action)!");

                            action->getUPnPArgument(CURRENT_TRANSPORT_STATE)->setValue(getService()->getUPnPStateVariable(TRANSPORT_STATE)->getValue());
                            action->getUPnPArgument(CURRENT_TRANSPORT_STATUS)->setValue(getService()->getUPnPStateVariable(TRANSPORT_STATUS)->getValue());
                            action->getUPnPArgument(CURRENT_SPEED)->setValue(getService()->getUPnPStateVariable(TRANSPORT_PLAY_SPEED)->getValue());

                            return true;
                        }

                        bool TVRAVTransport::actionGetPositionInfo(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetPositionInfo(action)!");

                            string position = Utils::timeToString(getMediaRenderer()->getPlayerHandler()->getMediaTimePosition());
                            getService()->getUPnPStateVariable(RELATIVE_TIME_POSITION)->setValue(position.c_str());
                            getService()->getUPnPStateVariable(ABSOLUTE_TIME_POSITION)->setValue(position.c_str());

                            string tstate = getService()->getUPnPStateVariable(TRANSPORT_STATE)->getValue();
                            if (tstate.compare(PLAYING) == 0) {
                                double duration = Utils::stringToTime(getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION)->getValue());
                                double position = Utils::stringToTime(getService()->getUPnPStateVariable(RELATIVE_TIME_POSITION)->getValue());
                                if ((duration > 0.0) && (position == 0.0)){
                                    getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(STOPPED);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));
                                }
                            }

                            action->getUPnPArgument(TRACK)->setValue(atoi(getService()->getUPnPStateVariable(CURRENT_TRACK)->getValue()));
                            action->getUPnPArgument(TRACK_DURATION)->setValue(getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION)->getValue());
                            action->getUPnPArgument(TRACK_METADATA)->setValue(getService()->getUPnPStateVariable(CURRENT_TRACK_METADATA)->getValue());
                            action->getUPnPArgument(TRACK_URI)->setValue(getService()->getUPnPStateVariable(CURRENT_TRACK_URI)->getValue());
                            action->getUPnPArgument(REL_TIME)->setValue(getService()->getUPnPStateVariable(RELATIVE_TIME_POSITION)->getValue());
                            action->getUPnPArgument(ABS_TIME)->setValue(getService()->getUPnPStateVariable(ABSOLUTE_TIME_POSITION)->getValue());
                            action->getUPnPArgument(REL_COUNT)->setValue(atoi(getService()->getUPnPStateVariable(RELATIVE_COUNTER_POSITION)->getValue()));
                            action->getUPnPArgument(ABS_COUNT)->setValue(atoi(getService()->getUPnPStateVariable(ABSOLUTE_COUNTER_POSITION)->getValue()));

                            return true;
                        }

                        bool TVRAVTransport::actionGetDeviceCapabilities(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetDeviceCapabilities(action)!");

                            action->getUPnPArgument(PLAY_MEDIA)->setValue(getService()->getUPnPStateVariable(POSSIBLE_PLAYBACK_STORAGE_MEDIA)->getValue());
                            action->getUPnPArgument(REC_MEDIA)->setValue(getService()->getUPnPStateVariable(POSSIBLE_RECORD_STORAGE_MEDIA)->getValue());
                            action->getUPnPArgument(REC_QUALITY_MODES)->setValue(getService()->getUPnPStateVariable(POSSIBLE_RECORD_QUALITY_MODES)->getValue());

                            return true;
                        }

                        bool TVRAVTransport::actionGetTransportSettings(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetTransportSettings(action)!");

                            action->getUPnPArgument(PLAY_MODE)->setValue(getService()->getUPnPStateVariable(CURRENT_PLAY_MODE)->getValue());
                            action->getUPnPArgument(REC_QUALITY_MODE)->setValue(getService()->getUPnPStateVariable(CURRENT_RECORD_QUALITY_MODE)->getValue());

                            return true;
                        }

                        bool TVRAVTransport::actionStop(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionStop(action)!");

                            string tstate = getService()->getUPnPStateVariable(TRANSPORT_STATE)->getValue();

                            if (tstate.compare(STOPPED) == 0) {
                                /* nothing in this state */

                                return true;
                            } else if (tstate.compare(NO_MEDIA_PRESENT) == 0) {
                                /* action not allowed in this state */
                                action->setStatus(701, "Transition not available");

                                return false;
                            } else {
                                    getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(STOPPED);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));

                                    getService()->getUPnPStateVariable(NUMBER_OF_TRACKS)->setValue(0);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(NUMBER_OF_TRACKS));
                                    getService()->getUPnPStateVariable(CURRENT_TRACK)->setValue(0);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_TRACK));
                                    getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION)->setValue("00:00:00");
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_MEDIA_DURATION));
                                    getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION)->setValue("00:00:00");
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_TRACK_DURATION));
                                    getService()->getUPnPStateVariable(RELATIVE_TIME_POSITION)->setValue("00:00:00");
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(RELATIVE_TIME_POSITION));
                                    getService()->getUPnPStateVariable(ABSOLUTE_TIME_POSITION)->setValue("00:00:00");
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(ABSOLUTE_TIME_POSITION));
                                    getService()->getUPnPStateVariable(AVTRANSPORT_URI_METADATA)->setValue("");
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(AVTRANSPORT_URI_METADATA));
                                    
                                    getMediaRenderer()->getPlayerHandler()->stop();
                                    return true;
                            }
                        }

                        bool TVRAVTransport::actionPlay(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionPlay(action)!");

                            string tstate = getService()->getUPnPStateVariable(TRANSPORT_STATE)->getValue();

                            if (tstate.compare(PLAYING) == 0) {
                                getService()->getUPnPStateVariable(TRANSPORT_PLAY_SPEED)->setValue(1);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_PLAY_SPEED));

                                return true;
                            } else if ((tstate.compare(STOPPED) == 0) || (tstate.compare(PAUSED_PLAYBACK) == 0)) {
                                    getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(TRANSITIONING);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));

                                    getMediaRenderer()->getPlayerHandler()->play();

                                    getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(PLAYING);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));
                                    getService()->getUPnPStateVariable(TRANSPORT_PLAY_SPEED)->setValue(1);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_PLAY_SPEED));
                                    
                                    getService()->getUPnPStateVariable(NUMBER_OF_TRACKS)->setValue(1);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(NUMBER_OF_TRACKS));
                                    getService()->getUPnPStateVariable(CURRENT_TRACK)->setValue(1);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CURRENT_TRACK));
                                    getService()->getUPnPStateVariable(TRANSPORT_STATUS)->setValue(OK);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATUS));
                                    
                                    return true;
                            } else {
                                /* action not allowed in these states */
                                action->setStatus(701, "Transition not available");

                                return false;
                            }
                        }

                        bool TVRAVTransport::actionSeek(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionSeek(action)!");

                            string tstate = getService()->getUPnPStateVariable(TRANSPORT_STATE)->getValue();
                            bool retValue = false;

                            getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(TRANSITIONING);
                            Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));

                            if ((tstate.compare(STOPPED) == 0) || (tstate.compare(PLAYING) == 0)) {
                                string mode = action->getUPnPArgumentValue(UNIT);

                                if ((mode.compare("REL_TIME") == 0) || (mode.compare("ABS_TIME") == 0)) {
                                    string time = action->getUPnPArgumentValue(TARGET);
                                    if (getMediaRenderer()->getPlayerHandler()->seek(Utils::stringToTime(time)))
                                        retValue = true;
                                }
                            } else {
                                /* action not allowed in these states */
                                action->setStatus(701, "Transition not available");
                            }

                            getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(tstate.c_str());
                            Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));

                            return retValue;
                        }

                        bool TVRAVTransport::actionNext(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionNext(action)!");

                            return true;
                        }

                        bool TVRAVTransport::actionPrevious(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionPrevious(action)!");

                            return true;
                        }

                        bool TVRAVTransport::actionPause(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionPause(action)!");

                            string tstate = getService()->getUPnPStateVariable(TRANSPORT_STATE)->getValue();

                            if (tstate.compare(PLAYING) == 0) {
                                if (getMediaRenderer()->getPlayerHandler()->pause()) {
                                    getService()->getUPnPStateVariable(TRANSPORT_STATE)->setValue(PAUSED_PLAYBACK);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(TRANSPORT_STATE));
                                    return true;
                                } else {
                                    return false;
                                }
                            }

                            action->setStatus(701, "Transition not available");
                            return false;
                        }

                        bool TVRAVTransport::variableRequest(UPnPStateVariable *stateVar) {
                            LoggerUtil_info(logger, "Entrando no método queryControlReceived(stateVar)!");
                            LoggerUtil_debug(logger, "stateVar nome: " << stateVar->getName() << " valor: " << stateVar->getValue());

                            UPnPStateVariable* serviceStateVar = getService()->getUPnPStateVariable(stateVar->getName());
                            if(serviceStateVar == NULL)
                                return false;

                            stateVar->setValue(serviceStateVar->getValue());

                            return true;
                        }

                    }
                }
            }
        }
    }
}
