package com.showmo.playHelper;

import java.io.File;
import java.io.FileOutputStream;
import java.util.Timer;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import javax.mail.internet.NewsAddress;

import rx.Observable;
import rx.Subscriber;
import rx.Subscription;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.functions.Func1;
import rx.schedulers.Schedulers;

import com.showmo.rxErr.NetErrInfo;
import com.showmo.rxcallback.RxCallback;
import com.showmo.util.LogUtils;
import com.showmo.util.PwTimer;

import android.R.integer;
import android.graphics.Camera;
import android.os.AsyncTask;
import android.os.Environment;
import android.text.TextDirectionHeuristic;
import ipc365.app.showmo.jni.JniClient;

public class IntercomApiWraper {
	public static IntercomRunnable m_IntercomRunnable;
	public static final int API_ERR=0;
	public static final int DEV_STATE_ON=1;
	public static final int DEV_OPEN_ERR=2;
	public static int cameraId;
	private static Subscription m_subcri;
	private static StopTimer mStopTimer=null;
	
	private static State openState=State.NoOpen;
	private static Lock closeWaitLock=new ReentrantLock();
	private static Condition closeCondition=closeWaitLock.newCondition();
	private static enum State{
		NoOpen,Opening,Opened,Closeing;
	}
	static{
		mStopTimer=new StopTimer();
	}
	
	private  static class StopTimer extends PwTimer{
		private RxCallback<Void> mstopcb;
		public StopTimer(){
			super(true);
		}
		public void setStopCb(RxCallback<Void> cb){
			mstopcb=cb;
		}
		@Override
		public void doInTask() {
			// TODO Auto-generated method stub
			stop(mstopcb);
		}
		
	}
	public static class IntercomErrThrowable extends Throwable{
		public int errtype; 
		public int netErrCode;
		public IntercomErrThrowable(int errtype){
			this.errtype=errtype;
		}
		public IntercomErrThrowable(int errtype,int neterrcode){
			this.errtype=errtype;
			netErrCode=neterrcode;
		}
	}
	public static void init(){
		AudioEncodeAndRecord.RecordInit();
	}
	public static void uninit(){
		AudioEncodeAndRecord.RecordUninit();
	}

	public static interface IntercomCallback{
		void onCompleted();
		void onError(Throwable e);
		void onVolumChanged(int volum);
	}
	public static boolean beginIntercom(final int cameraId,final IntercomCallback cb){
		LogUtils.e("IntercomApiWraper", "beginIntercom");
		if(mStopTimer.getRunning()){
			mStopTimer.stopIfStarted();
		//	LogUtils.e("IntercomApiWraper", "beginIntercom: stop timer is still running!");
			cb.onCompleted();
			return true;
		}
		if(openState==State.Closeing){
			//LogUtils.e("IntercomApiWraper", "beginIntercom: State.Closeing ");
			closeWaitLock.lock();
			try {
			//	LogUtils.e("IntercomApiWraper", "beginIntercom: wait for close over");
				closeCondition.await();
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
			closeWaitLock.unlock();
		//	LogUtils.e("IntercomApiWraper", "beginIntercom: close is over,intercom open continue");
		}
		openState=State.Opening;
		m_IntercomRunnable=new IntercomRunnable(cameraId,cb);
		try {
			AudioEncodeAndRecord.RecordStart();	
		} catch (Exception e) {
			// TODO: handle exception
			//LogUtils.e("IntercomApiWraper", "beginIntercom: AudioEncodeAndRecord.RecordStart err ");
			e.printStackTrace();
			//cb.onError(e);
		}
		m_subcri=Observable.create(new Observable.OnSubscribe<Void>(){
			@Override
			public void call(Subscriber<? super Void> t) {
				// TODO Auto-generated method stub
			//	LogUtils.e("IntercomApiWraper", "PW_NET_TalkCtrl open begin");
				boolean bres=JniClient.PW_NET_TalkCtrl(cameraId, true);
			//	LogUtils.e("IntercomApiWraper", "open rx begin: PW_NET_TalkCtrl bres:"+bres);
				if(!bres)  //
				{
					int devState=(int)JniClient.PW_NET_GetTalkState(cameraId);
				//	LogUtils.e("IntercomApiWraper", "open rx begin: PW_NET_GetTalkState state:"+devState);
					if(devState == -1)  //// 0 close 1 open -1 failed
					{
						t.onError(new IntercomErrThrowable(API_ERR,(int)JniClient.PW_NET_GetLastError()));
						return;
					}
					t.onError(new IntercomErrThrowable(DEV_OPEN_ERR,(int)JniClient.PW_NET_GetLastError()));
					return;
				}else{
					t.onNext(null);
					return;
				}
			}
		}).subscribeOn(Schedulers.io())
		.observeOn(AndroidSchedulers.mainThread())
		.subscribe(new Subscriber<Void>() {
			@Override
			public void onNext(Void t) {
				// TODO Auto-generated method stub
				AsyncTask.execute(m_IntercomRunnable);
			//	LogUtils.e("IntercomApiWraper", "open rx begin: AsyncTask.execute suc open over");
				cb.onCompleted();
				openState=State.Opened;
			}
			@Override
			public void onError(Throwable e) {//对讲过程发生错误，则进行清理，不再由stop接口进行清理
				// TODO Auto-generated method stub
				AudioEncodeAndRecord.RecordStop();
				m_IntercomRunnable=null;
				openState=State.NoOpen;
				//LogUtils.e("IntercomApiWraper", "open rx begin: open error over");
				cb.onError(e);
			}
			@Override
			public void onCompleted() {
				// TODO Auto-generated method stub
					
			}
		});
		IntercomApiWraper.cameraId=cameraId;
		return true;
	}
	private static class IntercomRunnable implements Runnable{
		private boolean bstop=false;
		private boolean bpause=false;
		private IntercomCallback cb;
		int cameraId;
		public IntercomRunnable(int cameraId,IntercomCallback cb){
			this.cb=cb;
			this.cameraId=cameraId;
		}
		public synchronized void stop(){
			bstop=true;
		}
		public  synchronized void pause(){
			bpause=true;
		}
		public synchronized void conti(){
			bpause = false;
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
		//	LogUtils.e("IntercomApiWraper", "intercom runnable: senddata begin");
			while(!bstop){
				if(!bpause){
					byte[] data= AudioEncodeAndRecord.RecordInputData();
	
					String path=Environment.getExternalStorageDirectory().getAbsolutePath();
					//LogUtils.e("intercomdata", "IntercomRunnable PW_NET_SendTalkData before ");
	
					boolean bres=JniClient.PW_NET_SendTalkData(cameraId, data, data.length,path+"/intercom2.pcm");
					LogUtils.e("intercomdata", "IntercomRunnable PW_NET_SendTalkData after ");
					//LogUtils.e("intercomdata", "PW_NET_SendTalkData res "+bres+" size "+data.length+" cameraId "+cameraId);
					cb.onVolumChanged(AudioEncodeAndRecord.GetAmplitude());
				}else{
					try {
						Thread.sleep(40);
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
					}
				}
			}
		//	LogUtils.e("IntercomApiWraper", "intercom runnable: senddata exit");
			cb.onVolumChanged(0);
		}
	};
	public static void  closeSound(){
		if(openState==State.Opened){
			m_IntercomRunnable.pause();
		}
	}
	public static void openSound(){
		if(openState==State.Opened){
			m_IntercomRunnable.conti();
		}
	}
	public static boolean endIntercomIfOPen(final RxCallback<Void> stopcb){	
		if(mStopTimer==null){
			mStopTimer=new StopTimer();
		}
		mStopTimer.setStopCb(stopcb);
		if(openState==State.Opened){
			if(!mStopTimer.getRunning()){
			//	LogUtils.e("IntercomApiWraper", "endIntercomIfOPen: mStopTimer.start");
				mStopTimer.start(300, false);
			}
		}else if(openState==State.Opening){
			m_subcri.unsubscribe();
		//	LogUtils.e("IntercomApiWraper", "endIntercomIfOPen:rxing m_subcri.unsubscribe");
			AudioEncodeAndRecord.RecordStop();
		}
		return true;
	}

	private static void stop(final RxCallback<Void> stopcb){
		if(openState==State.Opened){
		//	LogUtils.e("IntercomApiWraper", "intercom stop: close begin");
			closeWaitLock.lock();
			openState=State.Closeing;
			closeWaitLock.unlock();
			m_IntercomRunnable.stop();
			m_IntercomRunnable=null;
			AudioEncodeAndRecord.RecordStop();
		//	LogUtils.e("IntercomApiWraper", "intercom stop: stop senddata");
			Observable.create(new Observable.OnSubscribe<Void>(){
				@Override
				public void call(Subscriber<? super Void> t) {
					// TODO Auto-generated method stub
//					try {
//						Thread.sleep(2000);
//					} catch (Exception e) {
//						// TODO: handle exception
//						e.printStackTrace();
//					}
					
					boolean bres=JniClient.PW_NET_TalkCtrl(cameraId, false);
				//	LogUtils.e("IntercomApiWraper", "intercom stop: PW_NET_TalkCtrl close "+bres);
					if(bres){
						//LogUtils.e("IntercomApiWraper", "intercom stop: t.onNext");
						t.onNext(null);
					}else{
				//		LogUtils.e("IntercomApiWraper", "intercom stop: t.onError");
						t.onError(new NetErrInfo(JniClient.PW_NET_GetLastError()));
					}
				}
			}).subscribeOn(Schedulers.io())
			.subscribe(new RxCallback() {
				@Override
				public void onNext(Object t) {
					// TODO Auto-generated method stub
				//	LogUtils.e("IntercomApiWraper", "intercom stop:  suc,notify all waiter ");
					openState=State.NoOpen;
					closeWaitLock.lock();
				//	LogUtils.e("IntercomApiWraper", "closeWaitLock.lock() suc");
					try {
						closeCondition.signalAll();
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
					}
					closeWaitLock.unlock();
			//		LogUtils.e("IntercomApiWraper", "stopcb.onNext(null)");
					stopcb.onNext(null);
				}
				@Override
				public void onError(Throwable e) {
					// TODO Auto-generated method stub
				//	LogUtils.e("IntercomApiWraper", "intercom stop:  fai,notify all waiter ");
					openState=State.NoOpen;
					closeWaitLock.lock();
					try {
						closeCondition.signalAll();
					} catch (Exception e2) {
						// TODO: handle exception
						e2.printStackTrace();
					}
					closeWaitLock.unlock();
					stopcb.onError(null);
				}
			});
		}
	}

}
