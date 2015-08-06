package com.showmo.util;

import java.lang.reflect.Field;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Callable;
import java.util.concurrent.Future;
import java.util.concurrent.FutureTask;

import android.R.bool;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.style.SuggestionSpan;
import android.util.Log;

public abstract class PwTimer extends Timer {
	private long m_RTime;
	private boolean m_bRepeat;
	private Handler m_mainHanlder=new Handler(Looper.getMainLooper());
	private Handler m_threadHandler;
	private Thread m_workThread;
	private boolean m_bWorkInThread;
	private Message m_WorkMsg;
	private final int MSGWHAT=12211;
	private WorkRunable m_WorkRunable;
	private Runnable m_WorkRun;
	private boolean isRuning;
	public PwTimer(boolean bWorkInThread){
		m_RTime=0;
		m_bRepeat=false;
		m_bWorkInThread=bWorkInThread;
		isRuning=false;
		if(bWorkInThread){
			m_WorkRun=new Runnable() {
				@Override
				public void run() {
					// TODO Auto-generated method stub
					Looper.prepare();
					m_threadHandler=new Handler(Looper.myLooper());
					try {
						synchronized (m_WorkRun) {
							m_WorkRun.notifyAll();
						}
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
					}
					Looper.loop();
				}
			};
			m_workThread=new Thread(m_WorkRun);
			m_workThread.start();
			try {
				synchronized (m_WorkRun) {
					m_WorkRun.wait();
				}	
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
			
		}
		
	}
	private Message getMessage(){
		m_WorkRunable=new WorkRunable();
		m_WorkMsg=Message.obtain();
		m_WorkMsg.what=MSGWHAT;
		
		Class<Message> clazz=Message.class;
		Field callbackField=null;
		try {
			callbackField=clazz.getDeclaredField("callback");
		} catch (Exception e) {
			// TODO: handle exception
			//LogUtils.v("timer", "getDeclaredField failured");
			e.printStackTrace();
		}
		callbackField.setAccessible(true);
		try {
			callbackField.set(m_WorkMsg, m_WorkRunable);
		} catch (Exception e) {
			// TODO: handle exception
			//LogUtils.v("timer", "set Field failured");
			e.printStackTrace();
		}
		return m_WorkMsg;
	}
	public long getRepeatTime(){
		return m_RTime;
	}
	public synchronized void start(long msec){
		stopIfStarted();
		m_RTime=msec;
		postRun();
		setRuning(true);
	}
	private synchronized void postRun(){
//		Class<Message> clazz=Message.class;
//		Field cbF=null;
//		try {
//			cbF=clazz.getDeclaredField("callback");
//		} catch (Exception e) {
//			// TODO: handle exception
//			e.printStackTrace();
//		}
//		cbF.setAccessible(true);
//		try {
//			Object cbObj=cbF.get(m_WorkMsg);
//			WorkRunable runable=(WorkRunable)cbObj;
//			LogUtils.v("timer", runable.toString());
//		} catch (Exception e) {
//			// TODO: handle exception
//			e.printStackTrace();
//		}
		
		
		
//		LogUtils.v("timer", m_WorkMsg.toString());
		
		if(m_bWorkInThread){
			m_threadHandler.sendMessageDelayed(getMessage(), m_RTime);
		}else{
			m_mainHanlder.sendMessageDelayed(getMessage(), m_RTime);
		}
	}
	public synchronized void start(long msec, boolean repeat){
		//LogUtils.v("timer", "start sotp");
		stopIfStarted();
		m_bRepeat=repeat;	
		m_RTime=msec;
		postRun();
		setRuning(true);
	}
	private boolean findMsgInQueue(){
		if(m_bWorkInThread){
			return m_threadHandler.hasMessages(MSGWHAT);
		}
		return m_mainHanlder.hasMessages(MSGWHAT);
	}
	private void stopMsgFromQueue(){
		if(m_bWorkInThread){
			m_threadHandler.removeCallbacks(m_WorkRunable);
		}
	      m_mainHanlder.removeCallbacks(m_WorkRunable);
	}
	public synchronized  void stopIfStarted(){
		m_bRepeat=false;
		//LogUtils.v("timer", "stopIfStarted stop");
		setRuning(false);
		if(findMsgInQueue()){
			stopMsgFromQueue();
		}
	}
	private class WorkRunable implements Runnable{
		@Override
		public void run() {
			// TODO Auto-generated method stub
			setRuning(false);
			doInTask();
			if(m_bRepeat){
				postRun();
			}
		}
	}
	protected synchronized void setRepeat(boolean b){
		m_bRepeat=b;
	}
	private synchronized void setRuning(boolean b){
		isRuning=b;
	}
	public synchronized boolean getRunning(){
		return isRuning;
	}
	public boolean getRepeat(){
		return m_bRepeat;
	}
	public abstract void  doInTask();
	
}
