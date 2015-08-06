TEMPLATE = app


SOURCES += \
    ../h264parse/h264parse.cpp \
    ../h264parse/h264sps.cpp \
    ../pwpriprotocol/APTcpConnect.cpp \
    ../pwpriprotocol/APUdpBroadcast.cpp \
    ../pwpriprotocol/ApUdpBroadcastset.cpp \
    ../pwpriprotocol/priprotocol.cpp \
    ../ipc365_app_showmo_jni_jniclient.cpp \
    ../showmo_all_0424.cpp \
    ../StreamAdapter.cpp

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    ../gl_montage_play_lib/avilib.h \
    ../gl_montage_play_lib/montage_pattern.h \
    ../gl_montage_play_lib/montage_play.h \
    ../gl_montage_play_lib/pw_typedef.h \
    ../gl_montage_play_lib/pwerror.h \
    ../gl_montage_play_lib/pwmacro.h \
    ../h264parse/get_bits.h \
    ../h264parse/h264data.h \
    ../h264parse/h264datatype.h \
    ../h264parse/h264def.h \
    ../h264parse/h264macro.h \
    ../h264parse/h264parse.h \
    ../pwnetsdk/proto.h \
    ../pwnetsdk/pw_net_sdk.h \
    ../pwnetsdk/uu_types.h \
    ../pwpriprotocol/APTcpConnect.h \
    ../pwpriprotocol/APUdpBroadcast.h \
    ../pwpriprotocol/ApUdpBroadcastset.h \
    ../pwpriprotocol/PackProtocolType.h \
    ../pwpriprotocol/priprotocol.h \
    ../pwpriprotocol/pw_datatype.h \
    ../ipc365_app_showmo_jni_JniClient.h \
    ../jniutil.h \
    ../jniwraper.h \
    ../StreamAdapter.h \
    ../pwpriprotocol/priptoto.h \
    ../jniHelper/JniField.h \
    ../jniHelper/JniGlobalDef.h \
    ../jniHelper/JniIntField.h \
    ../jniHelper/JniMethod.h \
    ../jniHelper/JniObject.h \
    ../jniHelper/JniObjectField.h \
    ../jniHelper/JniStringField.h \
    ../jniHelper/jniutils.h
