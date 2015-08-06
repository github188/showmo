package com.showmo.playHelper;

import com.showmo.deviceManage.Device;

import ipc365.app.showmo.jni.JniDataDef.OnRealdataCallBackListener;
import ipc365.app.showmo.jni.JniDataDef.Remote_File_Type;
import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;

public class PlaybackInParams {
	 Device dev;
	 SDK_REMOTE_FILE file;
	 OnRealdataCallBackListener m_dataCallback = null;
	 int pos;
}
