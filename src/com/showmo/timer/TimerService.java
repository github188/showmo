package com.showmo.timer;

import java.util.Timer;
import java.util.TimerTask;

import com.showmo.base.BaseActivity;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.util.Log;

public class TimerService extends Service {
	
	public static final String TIME_INIT = "TIME_INIT";
	
	public static final String TIME_RESET = "TIME_RESET";
	
	public static final String TIME_STOP = "TIME_STOP";
	
	public static final String TIME_START = "TIME_START";
	
	private static int TIME_MAX = 120 ;
	
	private static int TIME_TEST = 90 ;
	
	private int timeLeft = -1  ;
	
	private Timer timer ;
	
	private  TimerTask timerTask  ;

	private Intent timeIntent;

	private TimeControlReceiver receiver;

	private class MyTimerTask extends TimerTask{

		@Override
		public void run() {
		 	if(timeLeft > 0){
	    		timeLeft--;
	    		//发送广播  
	    		sendTimeChangedBroadcast();  
	    	} else{
	    		if(timerTask != null){
					 timerTask.cancel();
					 timerTask = null ;
				}
	    	}
		}
		
	}
	
 
	@Override
	public void onCreate() {
		Log.e("out", "service_onCreate");
		timer = new Timer();  
		registerBroadcastReceiver();
		super.onCreate();
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		sendTimeChangedBroadcast( );
		return super.onStartCommand(intent, flags, startId);
		
	}
	
	private void startTimerTask(){
		if(timerTask == null){
			timeLeft = TIME_TEST;
			timerTask = new MyTimerTask();
			timer.schedule(timerTask, 1000,1000);  
		}
	}

	 /** 
     * 发送广播，通知UI层时间已改变 
     */  
    private void sendTimeChangedBroadcast( ){  
    	if(timeIntent == null){
    		timeIntent = new Intent();  
    		timeIntent.setAction(BaseActivity.TIME_CHANGED_ACTION);  
    	}
    	timeIntent.putExtra(BaseActivity.INTENT_KEY_STRING, timeLeft+"");
        //发送广播，通知UI层时间改变了  
        sendBroadcast(timeIntent);  
    }  
    
	private  class  TimeControlReceiver extends BroadcastReceiver{  

		@Override
		public void onReceive(android.content.Context context, Intent intent) {
			String str = intent.getStringExtra(BaseActivity.INTENT_KEY_STRING);
			 if(str.equals(TIME_STOP)){
				if(timerTask != null){
					 timerTask.cancel();
					 timerTask = null ;
				}
			}else if(str.equals(TIME_START)){
				startTimerTask();
			}
		}  
	} 
    
	
	@Override
	public void onDestroy() {
		timer.cancel();
		unregisterReceiver(receiver);
		super.onDestroy();
	}
	
	/** 
	 * 注册广播 
	 */  
	private void registerBroadcastReceiver(){  
		receiver = new TimeControlReceiver();  
		IntentFilter filter = new IntentFilter(BaseActivity.TIME_CONTROL_ACTION);  
		registerReceiver(receiver, filter);  
	}  


	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
 

}
	 
    


