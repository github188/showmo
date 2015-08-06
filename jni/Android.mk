
LOCAL_PATH := $(call my-dir)

include_path := ./inc  
montage_path := ./gl_montage_play_lib/armeabi
netsdk_path  := ./pwnetsdk/armeabi
pri_path 	 := ./pwpriprotocol
pri_exchange := ./pwpriprotocol/exchange/lib

include $(CLEAR_VARS)
LOCAL_MODULE := pw_net_sdk
LOCAL_SRC_FILES := $(netsdk_path)/libpwnetsdk.a
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := _json
LOCAL_SRC_FILES := ./pwpriprotocol/exchange/lib/libjson.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := _exchange
LOCAL_SRC_FILES := $(pri_exchange)/libexchange.a
include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := _exchange
#LOCAL_SRC_FILES := $(pri_exchange)/libexchange.a
#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := pw_pri_proto
LOCAL_CPPFLAGS += -std=c++11
LOCAL_C_INCLUDES := $(include_path)
LOCAL_CPPFLAGS+= -fexceptions
LOCAL_SRC_FILES :=$(pri_path)/APUdpBroadcast.cpp\
		$(pri_path)/ApUdpBroadcastset.cpp\
		$(pri_path)/APTcpConnect.cpp\
        $(pri_path)/priprotocol.cpp	
 LOCAL_STATIC_LIBRARIES:= _exchange\
 _json
#       $(pri_path)/exchange/Source/CameraExchange.cpp\
#		$(pri_path)/exchange/Source/CaptureExchange.cpp\
#		$(pri_path)/exchange/Source/ChainExchange.cpp\
#		$(pri_path)/exchange/Source/CommExchange.cpp\
#		$(pri_path)/exchange/Source/Exchange.cpp\
#		$(pri_path)/exchange/Source/ExchangeKind.cpp\
#		$(pri_path)/exchange/Source/GeneralExchange.cpp\
#		$(pri_path)/exchange/Source/GUIExchange.cpp\
#		$(pri_path)/exchange/Source/MediaExchange.cpp\
#		$(pri_path)/exchange/Source/NetIPAbilitySet.cpp\
#		$(pri_path)/exchange/Source/NetIPDeviceInfo.cpp\
#		$(pri_path)/exchange/Source/NetIPOperation.cpp\
#		$(pri_path)/exchange/Source/NetPlatform.cpp\
#		$(pri_path)/exchange/Source/NetWorkExchange.cpp\
#		$(pri_path)/exchange/Source/StorageExchange.cpp\
#		$(pri_path)/exchange/Source/ManagerExchange.cpp\
#		$(pri_path)/exchange/Source/json/json_reader.cpp\
#		$(pri_path)/exchange/Source/json/json_value.cpp\
#		$(pri_path)/exchange/Source/json/json_writer.cpp\
	

include $(BUILD_STATIC_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE := _montagplay
LOCAL_SRC_FILES := $(montage_path)/libmontage_play.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := tpnsSecurity
LOCAL_SRC_FILES := ./xgnative/libtpnsSecurity.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := tpnsWatchdog
LOCAL_SRC_FILES := ./xgnative/libtpnsWatchdog.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := _montagshow
LOCAL_SRC_FILES :=  $(montage_path)/libpw_magic_show_2.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := _streamReader
LOCAL_SRC_FILES :=  $(montage_path)/libStreamReader.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := pwnetsdk
LOCAL_C_INCLUDES := $(include_path)
LOCAL_CPPFLAGS +=-frtti -std=c++11
LOCAL_SRC_FILES := ipc365_app_showmo_jni_jniclient.cpp\
                   ./avi/aviconvert.cpp\
                   ./h264parse/h264parse.cpp\
                   ./h264parse/h264sps.cpp\
                   ./StreamAdapter.cpp\
                   ./jniHelper/JniObject.cpp\
                   ./jniHelper/JniField.cpp\
                   ./jniHelper/JniMethod.cpp\
                   ./jniHelper/jniutils.cpp\
			
LOCAL_LDLIBS := -llog 
LOCAL_STATIC_LIBRARIES:= pw_net_sdk\
			pw_pri_proto

LOCAL_SHARED_LIBRARIES:= _montagplay\
			_montagshow\
			_streamReader
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/support)
#include $(LOCAL_PATH)/foo/Android.mk
