package com.showmo.playHelper;

import ipc365.app.showmo.jni.JniDataDef.OnRealdataCallBackListener;
import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;

import com.showmo.deviceManage.Device;

public abstract class IDevicePlayer implements OnAdapterStreamListener{
	public static enum EnumStreamType{
		QUALITY,FLUENCY,ADAPTER
	}
	public static enum EnumRealplayCmd{MGRSIGNIN, GETDEVIP, REALPLAY,MGRRECONNECT}
	public final static EnumRealplayCmd[] REALPLAY_CMD_ORDER = { EnumRealplayCmd.MGRSIGNIN,
		 EnumRealplayCmd.GETDEVIP, EnumRealplayCmd.REALPLAY };
	public enum PLAYER_STATUS{
		NOPLAY,PLAYBACKING,REALPLAYING
	}
	public static final String SP_STREAM_ADAPTER="adapter";
	public static final String SP_STREAM_FLUENCY="FLUENCY";
	public static final String SP_STREAM_QUALITY="QUALITY";
	public static final String SP_STREAM_KEY = "stream";
	public static final int STREAM_TYPE_MAIN = 0;
	public static final int STREAM_TYPE_SUB = 1;
	public static final int STREAM_TYPE_JPEG = 2;
	public final static int MaxFluencySpeed = 25*1024;//流畅优先下达到这个值就切换到画质优先
	public final static int MinQualitySpeed=40*1024;//画质优先下达到这个值就切换到流畅优先
	/*
	 * 播放设备device，数据通过OnRealdataCallBackListener回调,stateListener播放状态回调
	 */
	public abstract boolean realplay(Device device,OnRealdataCallBackListener datacallback)throws StopPlayingDeviceException;
	
	public abstract void setOnRealplayListener(OnRealplayListener stateListener);
	public abstract boolean playbeforeRealplayDevice();
	
	public abstract void cancelRealplay();
	public abstract  boolean playback(SDK_REMOTE_FILE file,OnRealdataCallBackListener datacallback,int pos);
	
	public abstract boolean playBeforePlaybackDevcice();
	public abstract void setOnPlaybackListener(OnPlaybackListener playbackListener);
	public abstract void cancelPlayback();
	
	public abstract boolean stop();
	
	public abstract void setOnStopPlayListener(OnStopRealplayListener playbackListener);
	
	public abstract Device getmCurDeviceInfo();
	
	public abstract int setStreamType(EnumStreamType streamType) throws NoDeviceIsPlayingException;
	
	public abstract boolean isPublic();
	public abstract void streamFlowAdd(long rxStreamSize);
	public abstract EnumStreamType getStreamType();
	public abstract PLAYER_STATUS getStatus();
}
