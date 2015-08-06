package com.showmo.playHelper;

import java.sql.Time;
import java.util.List;

import javax.mail.internet.NewsAddress;

import rx.Observable;
import rx.Scheduler;
import rx.Subscriber;
import rx.Subscription;
import rx.android.app.AppObservable;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Func1;
import rx.schedulers.Schedulers;

import com.showmo.R.id;
import com.showmo.base.ShowmoSystem;
import com.showmo.deviceManage.Device;
import com.showmo.event.PlaybackCompleteEvent;
import com.showmo.event.PlaybackEvent;
import com.showmo.eventBus.EventBus;
import com.showmo.playHelper.IDevicePlayer.EnumStreamType;
import com.showmo.playHelper.IDevicePlayer.PLAYER_STATUS;
import com.showmo.util.LogUtils;
import com.showmo.util.PwNetWorkHelper;
import com.showmo.util.PwTimer;
import com.tencent.android.tpush.horse.r;

import android.R.bool;
import android.R.integer;
import android.R.interpolator;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.provider.ContactsContract.CommonDataKinds.Event;
import android.util.Log;
import android.view.View;
import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.OnRealdataCallBackListener;
import ipc365.app.showmo.jni.JniDataDef.Remote_PlayBack_Action;
import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;

public class PlayHelper extends IDevicePlayer {


	private RealplayInParams m_curWaitPlayInParam = null;
	private OnRealplayListener mOnRealplayListener = null;


	//	private PlayerStatusObserver mStatusObserver=null;
	private OnStopRealplayListener m_onStopListener=null;
	//private RealplayAsynTask m_RealplayAsynTask;
	private PlaybackAsyncTask m_playbackAsyncTask;
	private Handler m_handler;	
	private ShowmoSystem m_System;
	private EnumStreamType m_streamType=EnumStreamType.FLUENCY;		
	private Subscription mRealplaySubscription=null;
	private StreamRxTimer mStreamRxTimer;
	public PLAYER_STATUS mCurPlayerStatus=PLAYER_STATUS.NOPLAY;
	private boolean isRecording=false;
	private Device mCurDeviceInfo=null;
	private OnPlaybackListener m_PlaybackListener=null;
	private boolean mbPublic = false;
	private Object statusLock=new Object();
	private Object deviceLock=new Object();


	@Override
	public EnumStreamType getStreamType(){
		return m_streamType;
	}
	public  PLAYER_STATUS getStatus(){
		return mCurPlayerStatus;
	}
	@Override
	public boolean isPublic() {
		return mbPublic;
	}


	//	public interface PlayerStatusObserver{
	//		void onStatusChanged(PLAYER_STATUS status);
	//		
	//	}


	private void setPublic(boolean bPublic) {
		mbPublic = bPublic;
	}

	private void clearPlayState(){
		setPlayerStatus(PLAYER_STATUS.NOPLAY,null);
	}

	private  class StopResRunnable implements Runnable{
		private boolean mRes;
		private int merrcode;
		public StopResRunnable(boolean res,int errCode){
			mRes=res;
			merrcode=errCode;
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			if(m_onStopListener!=null){
				m_onStopListener.onstopRealplayRes(mRes, merrcode);
			}
		}
	}
	private  class StopPlaybackResRunnable implements Runnable{
		private boolean mRes;
		private int merrcode;
		public StopPlaybackResRunnable(boolean res,int errCode){
			mRes=res;
			merrcode=errCode;
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			if(m_PlaybackListener!=null){
				if(!bStopPlaybackBySelf){
					m_PlaybackListener.onStopPlayback(mRes, merrcode);
				}else{
					bStopPlaybackBySelf=false;
					postStopRes(mRes,merrcode);
				}
				//m_onStopListener.onstopRealplayRes(mRes, merrcode);
			}
		}
	}
	private void postStopPlaybackRes(boolean res,int errCode){
		m_handler.post(new StopPlaybackResRunnable(res,errCode));
	}
	private void postStopRes(boolean res,int errCode){
		m_handler.post(new StopResRunnable(res,errCode));
	}

	//	public PlayerStatusObserver getmStatusObserver() {
	//		return mStatusObserver;
	//	}
	//
	//	public void setmStatusObserver(PlayerStatusObserver mStatusObserver) {
	//		this.mStatusObserver = mStatusObserver;
	//	}

	public PlayHelper() {
		//m_RealplayAsynTask = null;
		m_handler=new Handler(Looper.getMainLooper());
		m_System=ShowmoSystem.getInstance();
		mStreamRxTimer=new  StreamRxTimer();
	}
	@Override
	public void streamFlowAdd(long rxStreamSize) {
		// TODO Auto-generated method stub
		mStreamRxTimer.addRx(rxStreamSize);
	}
	private boolean isAdapter_Fluency=true;
	public class StreamRxTimer extends PwTimer{
		public StreamRxTimer(){
			super(true);
		}
		private long totalBytes=0;

		private synchronized void addRx(long bytes){
			totalBytes+=bytes;
		}
		private synchronized void clearRx(){
			totalBytes=0;
		}
		@Override
		public void doInTask() {
			// TODO Auto-generated method stub
			long secCount=getRepeatTime() /1000;
			long averSpeed=(long)(totalBytes * 1.0 / secCount);
			LogUtils.e("averSpeed", "averSpeed "+averSpeed/1024.0f);
			if(averSpeed <= MinQualitySpeed){
				try {
					LogUtils.e("stream", "threshold <= MinQualitySpeed will changeto FLUENCY");
					if(mCurDeviceInfo==null){
						return;
					}
					if(JniClient.PW_NET_SetStreamType(mCurDeviceInfo.getmCameraId(), STREAM_TYPE_SUB)){
						isAdapter_Fluency=true;
						stopIfStarted();
					}
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
				}
			}
//			if(isAdapter_Fluency || getStreamType() == EnumStreamType.ADAPTER){
//				if(averSpeed >= MaxFluencySpeed){
//					try {
//						LogUtils.e("stream", "threshold >= MaxFluencySpeed will changeto QUALITY");
//						//setStreamType(EnumStreamType.QUALITY);
//						if(mCurDeviceInfo==null){
//							return;
//						}
//						if(JniClient.PW_NET_SetStreamType(mCurDeviceInfo.getmCameraId(), STREAM_TYPE_MAIN)){
//							isAdapter_Fluency=false;
//						}
//					} catch (Exception e) {
//						// TODO: handle exception
//						e.printStackTrace();
//					}
//				}
//			}else{
//				if(averSpeed <= MinQualitySpeed){
//					try {
//						LogUtils.e("stream", "threshold <= MinQualitySpeed will changeto FLUENCY");
//						if(mCurDeviceInfo==null){
//							return;
//						}
//						if(JniClient.PW_NET_SetStreamType(mCurDeviceInfo.getmCameraId(), STREAM_TYPE_SUB)){
//							isAdapter_Fluency=true;
//						}
//					} catch (Exception e) {
//						// TODO: handle exception
//						e.printStackTrace();
//					}
//				}
//			}
			clearRx();
		}
		@Override
		public synchronized void stopIfStarted() {
			// TODO Auto-generated method stub
			super.stopIfStarted();
			clearRx();
		}
	}
	@Override
	public void setOnRealplayListener(OnRealplayListener stateListener) {
		// TODO Auto-generated method stub
		mOnRealplayListener=stateListener;
	}

	private RealplayInParams mBeforeRealplayParam=null;
	private PlaybackInParams mBeforeplaybackParams=null;

	@Override
	public boolean playBeforePlaybackDevcice() {
		// TODO Auto-generated method stub
		if(mBeforeplaybackParams==null){
			return false;
		}else{
			playback(mBeforeplaybackParams.file, mBeforeplaybackParams.m_dataCallback, mBeforeplaybackParams.pos);
			//mBeforeplaybackParams=null;
		}
		return true;
	}
	@Override
	public  boolean playbeforeRealplayDevice(){
		if(mBeforeRealplayParam==null){
			return false;
		}else{
			try {
				realplay(mBeforeRealplayParam.getDevInfo(),mBeforeRealplayParam.getDataCallback());
				//mBeforeRealplayParam=null;
			} catch (StopPlayingDeviceException e) {
				// TODO: handle exception
				e.printStackTrace();
				return false;
			}
		}
		return true;
	}
	@Override
	public synchronized boolean realplay(Device device,
			OnRealdataCallBackListener datacallback)throws StopPlayingDeviceException {
		LogUtils.i("realplay ","realPlay rx begin target device CameraId:"+device.getmCameraId()+" uuid:"+device.getmUuid());
		if(!stop()){
			throw new StopPlayingDeviceException("stop pre playing exception!");
		}

		if(mOnRealplayListener!=null){
			mOnRealplayListener.onRealplayBeforeListener();
		}
		if(mRealplaySubscription!=null){
			LogUtils.i("realplay ","mRealplaySubscription.isUnsubscribed:"+mRealplaySubscription.isUnsubscribed());
			if(!mRealplaySubscription.isUnsubscribed()){
				mRealplaySubscription.unsubscribe();
				//return false;
			}
		}
		LogUtils.fe(LogUtils.LogAppFile, "realplay uuid: "+device.getmUuid()+" cameraid:"+device.getmCameraId());
		isContinuePlayWhileErr=true;
		mBeforeRealplayParam=new RealplayInParams(REALPLAY_CMD_ORDER,device, datacallback);
		mRealplaySubscription=RxRealplay(mBeforeRealplayParam);
		return true;
	}
	@Override
	public void setOnStopPlayListener(OnStopRealplayListener listener){
		m_onStopListener=listener;
	}
	@Override
	public boolean stop() {
		// TODO Auto-generated method stub
		boolean bres=true;
		try {
			if(mCurPlayerStatus==PLAYER_STATUS.REALPLAYING){
				bres=this.stopRealplay();
			}
			else if(mCurPlayerStatus==PLAYER_STATUS.PLAYBACKING){
				bres=this.stopPlayBack();
			}
		} catch (NoDeviceIsPlayingException e) {
			// TODO: handle exception
			e.printStackTrace();
			setPlayerStatus(PLAYER_STATUS.NOPLAY,null);
		}
		return bres;
	}

	public boolean stopRealplay() throws NoDeviceIsPlayingException{
		boolean bres=true;
		//LogUtils.i("realplay", "停止当前设备 "+mCurDeviceInfo.getmCameraId());
		mStreamRxTimer.stopIfStarted();
		if(mCurDeviceInfo==null){
			throw new NoDeviceIsPlayingException("no device playing in palyer!");
		}
		synchronized(deviceLock){
			bres=JniClient.PW_NET_StopRealPlay(mCurDeviceInfo.getmCameraId());
			//LogUtils.v("realplay", "停止当前设备  "+mCurDeviceInfo.getmCameraId()+" ret "+bres);
			//			if(bres){
			setPlayerStatus(PLAYER_STATUS.NOPLAY,null);
			//			}
			int err=0;
			//			if(!bres){
			//				err=(int)JniClient.PW_NET_GetLastError();
			//			}
			postStopRes(true, err);
		}
		return bres;
	}
	private void startNewPlaybackTask(PlaybackInParams in){
		m_playbackAsyncTask=new PlaybackAsyncTask();
		m_playbackAsyncTask.execute(in);
	}

	private class PlaybackAsyncTask extends AsyncTask<PlaybackInParams, Void, Boolean>{

		@Override
		protected Boolean doInBackground(PlaybackInParams... params) {
			// TODO Auto-generated method stub
			boolean bres=JniClient.PW_NET_PlayBack(params[0].dev.getmCameraId(),
					params[0].file, params[0].m_dataCallback, 0);
			//LogUtils.v("playback","begintime:"+params[0].file.startTime.format2445()+"end:"+params[0].file.endTime.format2445());
			//LogUtils.v("playback", "begin hour:"+params[0].file.startTime.hour+" end hour: "+params[0].file.endTime.hour);
			//LogUtils.v("playback", "file:"+params[0].file.sFileName+" pos: "+params[0].pos);
			if(bres && params[0].pos!=0){
				bres=JniClient.PW_NET_PlayBackControl(params[0].dev.getmCameraId(), 
						Remote_PlayBack_Action.SDK_PLAY_BACK_SEEK_PERCENT, params[0].pos);
				if(!bres){
					JniClient.PW_NET_StopPlayBack(params[0].dev.getmCameraId());
				}
				try {
					Thread.sleep(2000);
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
				}

			}
			if(bres){
				EventBus.getDefault().post(new PlaybackEvent(true));
				setPlayerStatus(PLAYER_STATUS.PLAYBACKING, params[0].dev);
			}
			return new Boolean(bres);
		}
		@Override
		protected void onPreExecute() {
			// TODO Auto-generated method stub
			if(m_PlaybackListener!=null){
				m_PlaybackListener.onPlaybackPre();
			}
		}
		@Override
		protected void onPostExecute(Boolean result) {
			// TODO Auto-generated method stub
			if (!result.booleanValue()) {
				if (isContinuePlaybackWhileErr) {
					if(mPrePlaybackParams!=null){
						startNewPlaybackTask(mPrePlaybackParams);
						return;
					}
				}
			}
			if(m_PlaybackListener!=null){
				m_PlaybackListener.onPlaybackOver(result.booleanValue(),(int)JniClient.PW_NET_GetLastError());
			}
		}
	}
	private PlaybackInParams mPrePlaybackParams=null;
	private boolean isContinuePlaybackWhileErr=true;

	public void cancelPlayback(){
		isContinuePlaybackWhileErr=false;
	}
	@Override
	public void setOnPlaybackListener(OnPlaybackListener playbackListener) {
		// TODO Auto-generated method stub
		m_PlaybackListener=playbackListener;
	}
	private boolean bStopPlaybackBySelf=false;
	@Override
	public boolean playback(SDK_REMOTE_FILE file,OnRealdataCallBackListener datacallback,int pos) {
		if(file==null){
			return false;
		}
		if(mCurDeviceInfo==null){
			return false;
		}
		mStreamRxTimer.stopIfStarted();
		Device playbackDev=mCurDeviceInfo;
		if(mCurPlayerStatus==PLAYER_STATUS.PLAYBACKING){
			bStopPlaybackBySelf = true;
		}
		stop();

		if(m_PlaybackListener!=null){
			m_PlaybackListener.onPlaybackPre();
		}
		mCurDeviceInfo=playbackDev;
		isContinuePlaybackWhileErr=true;
		mPrePlaybackParams=new PlaybackInParams();
		mPrePlaybackParams.dev=mCurDeviceInfo;
		mPrePlaybackParams.m_dataCallback=datacallback;
		mPrePlaybackParams.file=file;
		mPrePlaybackParams.pos=pos;
		mBeforeplaybackParams=mPrePlaybackParams;
		if(m_playbackAsyncTask==null){
			startNewPlaybackTask(mPrePlaybackParams);
		}else{
			switch (m_playbackAsyncTask.getStatus()) {
			case PENDING:
				m_playbackAsyncTask.cancel(false);
				startNewPlaybackTask(mPrePlaybackParams);
				break;
			case RUNNING:
				return false;
			case FINISHED:
				startNewPlaybackTask(mPrePlaybackParams);
				break;
			default:
				break;
			}
		}
		return true;
	}

	public void onEventMainThread(PlaybackCompleteEvent ev){
		EventBus.getDefault().post(new PlaybackEvent(false));
		JniClient.PW_NET_StopPlayBack(mCurDeviceInfo.getmCameraId());
		setPlayerStatus(PLAYER_STATUS.NOPLAY,null);
		if(m_PlaybackListener!=null){
			m_PlaybackListener.onPlaybackCompleted();
		}
	}

	public boolean stopPlayBack() throws NoDeviceIsPlayingException{
		if(mCurDeviceInfo==null){
			throw new NoDeviceIsPlayingException("no device playbacking in palyer!");
		}
		boolean bres=JniClient.PW_NET_StopPlayBack(mCurDeviceInfo.getmCameraId());
		//		if(bres){
		EventBus.getDefault().post(new PlaybackEvent(false));
		setPlayerStatus(PLAYER_STATUS.NOPLAY,null);
		//		}
		int err=0;
		//		if(!bres){
		//			err=(int)JniClient.PW_NET_GetLastError();
		//		}
		postStopPlaybackRes(true, err);
		return bres;
	}

	public boolean onRecord(Device device) {
		return false;
	}

	public boolean onStopRecord(Device device) {
		return false;
	}

	public boolean onCapture(Device device) {
		return false;
	}

	public class RealplayErrInfo extends Throwable{
		public EnumRealplayCmd mCmd;
		public int merrcode;
		public RealplayErrInfo(EnumRealplayCmd cmd,int errcode){
			mCmd=cmd;
			merrcode=errcode;
		}
	}
	public void publishRealplayState(final RealplayOutParams para){
		m_handler.post(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				if(mOnRealplayListener!=null){
					mOnRealplayListener.onRealplayStateListener(para);
				}
			}
		});
	}

	private Subscription RxRealplay(final RealplayInParams inpara){
		//LogUtils.e("devicelist", "m_playHelper.rxrealplay begin");
		return Observable.create(new Observable.OnSubscribe<Void>() {
			@Override
			public void call(Subscriber<? super Void> t) {
				// TODO Auto-generated method stub
				boolean bres = m_System.isMgrConnected();
				if(!bres){
					LogUtils.fe(LogUtils.LogAppFile, "mgr not connected");
					publishRealplayState(new RealplayOutParams(EnumRealplayCmd.MGRRECONNECT,bres));
					bres=m_System.waitForMgrState(3);
				}
				LogUtils.fe(LogUtils.LogAppFile, "onCompleted waitForMgrState "+bres);
				publishRealplayState(new RealplayOutParams(EnumRealplayCmd.MGRSIGNIN,bres));
				if(!bres){
					t.onError(new RealplayErrInfo(EnumRealplayCmd.MGRRECONNECT,(int)JniClient.PW_NET_GetLastError()));
					return;
				}else{
					t.onNext(null);
				}
			}
		}).subscribeOn(Schedulers.io())
				.flatMap(new Func1<Void, Observable<? extends Void>>() {
					@Override
					public Observable<? extends Void> call(Void t) {
						// TODO Auto-generated method stub
						return Observable.create(new Observable.OnSubscribe<Void>() {
							@Override
							public void call(Subscriber<? super Void> t) {
								// TODO Auto-generated method stub
								boolean bres=JniClient.PW_NET_GetLocalVerity(inpara.getDevInfo()
										.getmCameraId());
								t.onNext(null);
							}

						});
					}
				})
				.flatMap(new Func1<Void, Observable<? extends Boolean>>() {
					@Override
					public Observable<? extends Boolean> call(Void t) {
						// TODO Auto-generated method stub
						return Observable.create(new Observable.OnSubscribe<Boolean>() {
							@Override
							public void call(Subscriber<? super Boolean> t) {
								// TODO Auto-generated method stub
								LogUtils.fe(LogUtils.LogAppFile, "PW_NET_StartRealPlay uuid:"+inpara.getDevInfo().getmUuid()+"cameraId:"+inpara.getDevInfo().getmCameraId());
								int iRes = JniClient.PW_NET_StartRealPlay(
										inpara.getDevInfo().getmCameraId(), inpara.getDataCallback(), 0);
								LogUtils.fe(LogUtils.LogAppFile, "PW_NET_StartRealPlay over ");
								LogUtils.fe(LogUtils.LogAppFile, "local ip from android:"+PwNetWorkHelper.getInstance().getIp());
								RealplayOutParams outParams=null;
								if(iRes>=0){
									outParams=new RealplayOutParams(EnumRealplayCmd.REALPLAY,true,0);
								}else{
									outParams=new RealplayOutParams(EnumRealplayCmd.REALPLAY,false,(int)JniClient.PW_NET_GetLastError());
								}
								publishRealplayState(outParams);
								boolean bLocal=false;
								if (iRes>=0) {
									if(iRes==0){
										LogUtils.fe(LogUtils.LogAppFile, "public ");
										bLocal=false;
									}else{
										LogUtils.fe(LogUtils.LogAppFile, "local ");
										bLocal=true;
									}
									mbPublic = !bLocal;
									t.onNext(Boolean.valueOf(bLocal));
									t.onCompleted();
								}else{
									
									t.onError(new RealplayErrInfo(EnumRealplayCmd.REALPLAY,(int)JniClient.PW_NET_GetLastError()));
								}
							}
						});
					}
				}).observeOn(AndroidSchedulers.mainThread())
				.subscribe(new Subscriber<Boolean>() {
					@Override
					public void onNext(Boolean t) {//local
						// TODO Auto-generated method stub
						if(mOnRealplayListener!=null){
							setPlayerStatus(PLAYER_STATUS.REALPLAYING,inpara.getDevInfo());
							RealplayOutParams para=new RealplayOutParams(EnumRealplayCmd.REALPLAY,true);
							para.setArg2(t.booleanValue());
							para.setDevice(inpara.getDevInfo());
							mOnRealplayListener.onRealplayResultListener(para);
						}
					//	LogUtils.i("realplay", "realplay onNext subscribe over");
					}
					@Override
					public void onError(Throwable e) {
						// TODO Auto-generated method stub
						boolean isDeviceNotOnline=false;
						if(e instanceof RealplayErrInfo){
							RealplayErrInfo info=(RealplayErrInfo)e;
							isDeviceNotOnline = (info.merrcode == 2005);
							if(isContinuePlayWhileErr && !isDeviceNotOnline){
								//LogUtils.i("realplay", "isContinuePlayWhileErr$$$$$$$$$$$$$$");
								LogUtils.e(LogUtils.LogAppFile, "replay RxRealplay");
								RxRealplay(inpara);
								return;
							}
						}
						
						if(e instanceof RealplayErrInfo){
							RealplayErrInfo info=(RealplayErrInfo)e;
							if(mOnRealplayListener!=null){
								LogUtils.e("playerr", "play err1:"+info.merrcode);
								RealplayOutParams para=new RealplayOutParams(EnumRealplayCmd.REALPLAY,false);
								para.setErrcode(info.merrcode);
								mOnRealplayListener.onRealplayResultListener(para);
							}
						}else{
							e.printStackTrace();
							JniClient.PW_NET_StopRealPlay(inpara.getDevInfo().getmCameraId());
							mOnRealplayListener.onRealplayResultListener(new RealplayOutParams(EnumRealplayCmd.REALPLAY,false));
						}

						LogUtils.i("realplay", "realplay onError subscribe over");
					}
					@Override
					public void onCompleted() {
						// TODO Auto-generated method stub
						LogUtils.i("realplay", "realplay onCompleted subscribe over");
					}
				});

	}
	private boolean isContinuePlayWhileErr=true;

	public  void cancelRealplay(){
		isContinuePlayWhileErr = false;
	}
	private  void setCurWaitPlayInParam(RealplayInParams inpara) {
		synchronized(deviceLock){
			m_curWaitPlayInParam = inpara;
		}
	}

	private  RealplayInParams getCurWaitPlayInParam() {
		return m_curWaitPlayInParam;
	}

	private  void setPlayerStatus(PLAYER_STATUS STAT,Device curPlayingDev){
		synchronized(statusLock){
			//LogUtils.e("clear", "setPlayerStatus lock");
			if(mCurPlayerStatus!=STAT){
				//				if(mStatusObserver!=null){
				//					mStatusObserver.onStatusChanged(STAT);
				//				}
			}
			mCurDeviceInfo=curPlayingDev;
			mCurPlayerStatus=STAT;
		}
		//LogUtils.e("clear", "setPlayerStatus unlock");
	}
	@Override
	public Device getmCurDeviceInfo() {
		return mCurDeviceInfo;
	}

	private void setmCurDeviceInfo(Device mCurDeviceInfo) {
		this.mCurDeviceInfo = mCurDeviceInfo;
	}

	@Override
	public synchronized int setStreamType(EnumStreamType streamType) throws NoDeviceIsPlayingException{
		boolean bRes=false;
		int ires=-1;
		if(mCurDeviceInfo==null){
			throw new NoDeviceIsPlayingException("no device is playing!");
		}
		mStreamRxTimer.stopIfStarted();
		switch (streamType) {
		case FLUENCY:
			bRes = JniClient.PW_NET_SetStreamType(mCurDeviceInfo.getmCameraId(), STREAM_TYPE_SUB);
			LogUtils.e("stream", "STREAM_TYPE_SUB "+bRes);
			ires=STREAM_TYPE_SUB;
			break;
		case QUALITY:
			//			if(mbPublic){
			//				bRes= JniClient.PW_NET_SetStreamType(mCurDeviceInfo.getmCameraId(), STREAM_TYPE_JPEG);
			//				ires=STREAM_TYPE_JPEG;
			//			}else {
			bRes = JniClient.PW_NET_SetStreamType(mCurDeviceInfo.getmCameraId(), STREAM_TYPE_MAIN);
			LogUtils.e("stream", "STREAM_TYPE_MAIN "+bRes);
			ires=STREAM_TYPE_MAIN;
			//			}
			break;	
		case ADAPTER:
			bRes=JniClient.PW_NET_SetStreamType(mCurDeviceInfo.getmCameraId(), STREAM_TYPE_MAIN);
			ires=2;
			mStreamRxTimer.start(5000,true);
			break;
		default:
			break;
		}
		if(bRes){
			m_streamType=streamType;
		}else{
			ires=-1;
		}
		return ires;
	}
}
