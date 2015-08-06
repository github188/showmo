package com.showmo.base;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.OnDebugDataCallbackListener;
import ipc365.app.showmo.jni.JniDataDef.QueryAppVersionRet;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.Thread.UncaughtExceptionHandler;
import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.ShowmoSplashActivity;
import com.showmo.activity.alarmInfo.DeviceListActivity;
import com.showmo.activity.login.LoginActivity;
import com.showmo.eventBus.EventBus;
import com.showmo.playHelper.PlayHelper;
import com.showmo.service.PwSendExceptionMailService;
import com.showmo.util.LogUtils;
import com.showmo.util.MailUtils;
import com.showmo.util.PathUtils;
import com.showmo.util.ScreenUtil;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;
import com.tencent.android.tpush.XGNotifaction;
import com.tencent.android.tpush.XGPushManager;
import com.tencent.android.tpush.XGPushNotifactionCallback;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlarmManager;
import android.app.Application;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.os.SystemClock;
import android.util.Log;
import android.widget.Toast;

public class ShowmoApplication extends Application {
	private static ShowmoApplication m_Instance;
	private List<Activity> m_activityList;
	private static Context context;
	private Handler m_Handler=new Handler();
	static {
		long loadLibBefore=SystemClock.elapsedRealtime();
		System.loadLibrary("montage_play");
		System.loadLibrary("pw_magic_show_2");
		System.loadLibrary("StreamReader");
		System.loadLibrary("pwnetsdk");

		Log.i("call","loadLibrary  usetime----> "+(SystemClock.elapsedRealtime()-loadLibBefore)+"ms");
	}
	public static final String GLOBAL_TAG = "SHOWMO";

	public static ShowmoApplication getInstance() {
		return m_Instance;
	}

	public void addActivity(Activity ac) {
		m_activityList.add(ac);
	}

	public void exit() {
		for (int i = 0; i < m_activityList.size(); i++) {
			//Log.v("exit", m_activityList.get(i).toString());
			m_activityList.get(i).finish();
		}
		m_activityList.clear();
		System.exit(0);
	}
	public boolean checkAppNewVersion(String newversion){
//		QueryAppVersionRet verion=new QueryAppVersionRet();
//		boolean bres=JniClient.PW_ENT_GetAppVersion(1, verion);
//		if(!bres){
//			LogUtils.e("err", "app auto upgrade check failured:"+JniClient.PW_NET_GetLastError());
//			return false;
//		}
		String newVersion=newversion;
		String curVersion="0" ;
		
		try {
			PackageManager pm=getPackageManager();
			PackageInfo info=pm.getPackageInfo(getPackageName(), 0);
			curVersion=info.versionName;
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		//LogUtils.e("upgrade", "newVersion:"+newVersion+"curVersion:"+curVersion);
		List<String> newVersionList=StringUtil.splitWithChar(newVersion, '.');
		List<String> curVersionList=StringUtil.splitWithChar(curVersion, '.');
		
		if(newVersionList.size() != curVersionList.size()){
			return false;
		}
		else {
			for (int i = 0; i < newVersionList.size(); i++) {
				//LogUtils.e("upgrade","i "+i+" new "+newVersionList.get(i)+" cur "+curVersionList.get(i));
				if(Integer.valueOf(newVersionList.get(i)).intValue()>
				Integer.valueOf(curVersionList.get(i)).intValue()){
					
					return true;
				}
			}
		}
		return false;
	}
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		context = getApplicationContext();  
		m_activityList = new ArrayList<Activity>();
		m_Instance = this;
		setExceptionHandle();
		ShowmoSystem system=ShowmoSystem.getInstance();
		PlayHelper playerHelper=new PlayHelper();
		EventBus.getDefault().register(playerHelper);
		system.setPlayer(playerHelper);
		new Thread(){
			public void run() {
				long initSdkBefore=SystemClock.elapsedRealtime();
				ShowmoSystem.getInstance().init(ShowmoApplication.getInstance());
				//LogUtils.e("screen", ScreenUtil.getScreenInfo(this).width+","+ScreenUtil.getScreenInfo(this).height);
				LogUtils.fi(LogUtils.LogAppFile,"init netsdk  usetime----> "+(SystemClock.elapsedRealtime()-initSdkBefore)+"ms");
			};
		}.start();
		

		m_Handler.postDelayed(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				JniClient.PW_NET_SetDebugDatacallback(new OnDebugDataCallbackListener() {

					@Override
					public void onDebugDataCallback(String debugmsg) {
						// TODO Auto-generated method stub
						LogUtils.fi(LogUtils.LogSdkFile, debugmsg);
					}
				});
			}
		}, 1000);
		interceptXgNotification();
		
	}

	private void setExceptionHandle() {
		Thread.setDefaultUncaughtExceptionHandler(new SendExceptionHandle());
	}
	public static Context getContextObject(){  
		return context;  
	}  
	public class SendExceptionHandle implements UncaughtExceptionHandler {
		@Override
		public void uncaughtException(Thread thread, Throwable ex) {

			StringWriter sw = new StringWriter();
			PrintWriter pw = new PrintWriter(sw);
			ex.printStackTrace(pw);
			StringBuilder sb = new StringBuilder();
			sb.append("user is:");
			sb.append(ShowmoSystem.getInstance().getCurUser().getUserName() + "\n");
			sb.append("version code is ");
			sb.append(Build.VERSION.SDK_INT + "\n");
			sb.append("Model is ");
			sb.append(Build.MODEL + "\n");
			sb.append(sw.toString());
			LogUtils.fi(LogUtils.LogAppFile, sb.toString());
			//LogUtils.e("exception", "uncaughtException ");
			ex.printStackTrace();
//			Intent intent = new Intent(PwSendExceptionMailService.ACTION_STRING);
//			intent.putExtra(PwSendExceptionMailService.EXCEPTION_STR_KEY,
//					sb.toString());
//			startService(intent);
//
//			Intent reintent = new Intent(ShowmoApplication.this,
//					LoginActivity.class);
//			PendingIntent restartIntent = PendingIntent.getActivity(
//					ShowmoApplication.this, 0, reintent,
//					Intent.FLAG_ACTIVITY_NEW_TASK);
			// 退出程序
			//			AlarmManager mgr = (AlarmManager) ShowmoApplication.this
			//					.getSystemService(Context.ALARM_SERVICE);
			//			mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 1000,
			//					restartIntent); // 1秒钟后重启应用
			exit();

		}
	}

	public boolean isMainProcess() {
		ActivityManager am = ((ActivityManager) getSystemService(Context.ACTIVITY_SERVICE));
		List<RunningAppProcessInfo> processInfos = am.getRunningAppProcesses();
		String mainProcessName = getPackageName();
		int myPid = android.os.Process.myPid();
		for (RunningAppProcessInfo info : processInfos) {
			if (info.pid == myPid && mainProcessName.equals(info.processName)) {
				return true;
			}
		}
		return false;
	}
	private boolean isBackgroundProcess(){
		LogUtils.e("appActive", "isBackgroundProcess");
		ActivityManager aManager=(ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);
		List<ActivityManager.RunningAppProcessInfo> appProcessInfos=aManager.getRunningAppProcesses();
		if(appProcessInfos.size()==0){
			LogUtils.e("appActive", "appProcessInfos.size()==0");
			return false;
		}
		for (RunningAppProcessInfo process:appProcessInfos) {
			//LogUtils.e("appActive", "process:"+process.processName);
			if(process.processName.equals(getPackageName())){//Activity进程名与Application一致，并且默认是程序包名
				if (process.importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {
					LogUtils.e("appActive", "foreground");
				}else if (process.importance == RunningAppProcessInfo.IMPORTANCE_BACKGROUND) {
					LogUtils.e("appActive", "background");
					return true;
				}else if (process.importance ==RunningAppProcessInfo.IMPORTANCE_VISIBLE){
					LogUtils.e("appActive", "visible");
				}else if (process.importance ==RunningAppProcessInfo.IMPORTANCE_EMPTY){
					LogUtils.e("appActive", "empty");
				}
				else if (process.importance ==RunningAppProcessInfo.IMPORTANCE_SERVICE){
					LogUtils.e("appActive", "service");
				}else if (process.importance ==RunningAppProcessInfo.IMPORTANCE_PERCEPTIBLE){
					LogUtils.e("appActive", "PERCEPTIBLE");
				}
				LogUtils.e("appActive", "else");
				return false;
			}
		}
		return false;
	}
	
	public void interceptXgNotification(){
		if (isMainProcess()) {
			// 为保证弹出通知前一定调用本方法，需要在application的onCreate注册
			// 收到通知时，会调用本回调函数。
			// 相当于这个回调会拦截在信鸽的弹出通知之前被截取
			// 一般上针对需要获取通知内容、标题，设置通知点击的跳转逻辑等等
			XGPushManager.setNotifactionCallback(new XGPushNotifactionCallback() {
				
				@Override
				public void handleNotify(XGNotifaction xGNotifaction) {
					// TODO Auto-generated method stub
				//	Log.i("test", "intercept处理信鸽通知：" + xGNotifaction);
					// 获取标签、内容、自定义内容
					String title = xGNotifaction.getTitle();
					String content = xGNotifaction.getContent();
					String customContent = xGNotifaction
							.getCustomContent();
					
					if (isBackgroundProcess()||m_activityList.size()==0) {
					// 其它的处理
					// 如果还要弹出通知，可直接调用以下代码或自己创建Notifaction，否则，本通知将不会弹出在通知栏中。
					NotificationManager nm=(NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);

					Intent intent1=new Intent(ShowmoApplication.this,DeviceListActivity.class);
					//Intent intent2=new Intent(ShowmoApplication.this,DeviceListActivity.class);
					Intent[] intents={intent1};
					PendingIntent pi=PendingIntent.getActivities(ShowmoApplication.this, 
							1, intents, PendingIntent.FLAG_UPDATE_CURRENT);

					Notification nf=new Notification.Builder(ShowmoApplication.this)
					.setSmallIcon(R.drawable.ic_launcher)
					.setContentTitle(title)
					.setContentText(content)
					.setContentIntent(pi)
					.setAutoCancel(true)
					.getNotification();
					nf.defaults=Notification.DEFAULT_SOUND;
					nm.notify(0, nf);
				}
				}
			});
			
		}
	}
}
