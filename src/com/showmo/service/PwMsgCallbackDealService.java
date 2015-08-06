package com.showmo.service;

import java.io.File;
import java.io.FileOutputStream;
import java.io.Serializable;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Executor;
import java.util.concurrent.FutureTask;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

import javax.mail.internet.NewsAddress;

import com.showmo.alarmManage.Alarm;
import com.showmo.alarmManage.OnAlarmPictureDownloadListener;
import com.showmo.base.ShowmoApplication;
import com.showmo.base.ShowmoSystem;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.event.DevOnlineStateEvent;
import com.showmo.event.PlayDisconnectEvent;
import com.showmo.event.PlaybackCompleteEvent;
import com.showmo.eventBus.EventBus;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.dao.IAlarmDao;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.userManage.User;
import com.showmo.util.LogUtils;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.SDK_CAMERA_UPDATE;
import ipc365.app.showmo.jni.JniDataDef.alarm_data_download;
import ipc365.app.showmo.jni.JniDataDef.alarm_data_download_progress;
import ipc365.app.showmo.jni.JniDataDef.alarm_data_state;
import ipc365.app.showmo.jni.OnMsgDataCallBackWraper;
import ipc365.app.showmo.jni.JniDataDef.Device_alarm_server_upload_msg;
import ipc365.app.showmo.jni.JniDataDef.MSGBroadcastActions;
import ipc365.app.showmo.jni.JniDataDef.MSGID;
import ipc365.app.showmo.jni.JniDataDef.OnMsgDataCallBackListener;
import ipc365.app.showmo.jni.JniDataDef.Remote_Message;
import ipc365.app.showmo.jni.JniDataDef.Update_2_mobile_invite;
import ipc365.app.showmo.jni.JniDataDef.User_2_mgr_disconn_device;
import android.R.interpolator;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.AsyncTask;
import android.os.Binder;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class PwMsgCallbackDealService extends Service {
	//	private DeviceUpgradeMsgReceiever devUpgradeMsgReceiver;
	//	private DeviceUpgradeAckReceiever devUpgradeAckReceiver;
	//	private AlarmMsgReceiever alarmMsgReceiver;
	private OnMgrStateListener m_mgrStateListener=null;

	private IBinder m_binder;
	private Thread m_connectMgrThread=null;
	private boolean mMgrSignInExitFlag=false;
	private connectMgrRunnable mMgrRunnable;
	private Thread m_queryOnlineThread=null;
	private Thread m_queryAlarmThread=null;
	private boolean mQueryOnlineExitStateFlag=false;
	private boolean mQueryAlarmSwitchsExitFlag=false;
	private QueryDevicesStatesRunable mQueryOnlineStateRunable;
	//private QueryDevicesStatesRunable mQueryAlarmSwitchRunable;
	private ShowmoSystem mSys;
	private OnAlarmPictureDownloadListener  mAlarmDataListener=null;
	private IAlarmDao m_AlarmDao;
	private IDeviceDao m_DeviceDao;
	private Handler m_handler=new Handler();
	private Handler m_alarmHandler=null;
	private Object objWait=new Object();
	public interface OnMgrStateListener{
		public void onMgrStateChanged(boolean state);
		public void onMgrSignInFailured();
	}

	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		LogUtils.v("msgservice", "onBind");
		return m_binder;
	}
	@Override
	public void onRebind(Intent intent) {
		// TODO Auto-generated method stub
		LogUtils.v("msgservice", "onRebind");
		super.onRebind(intent);
	}
	@Override
	public void onTaskRemoved(Intent rootIntent) {
		// TODO Auto-generated method stub]
		LogUtils.v("msgservice", "onTaskRemoved");
		super.onTaskRemoved(rootIntent);
	}
	public class PwMsgBinder extends Binder {
		public PwMsgCallbackDealService getService() {
			return PwMsgCallbackDealService.this;
		}
	}
	public void setAlarmDownloadDataListener(OnAlarmPictureDownloadListener listener){
		mAlarmDataListener=listener;
	}
	private Thread getNewThreadWhenNoActiveOrNull(Thread thread,Runnable target){//当Thread线程在运行的时候返回NULL
		Thread res;
		if(thread==null){
			res=new Thread(target);
		}else{
			if(!thread.isAlive()){
				LogUtils.v("msgservice", "thread not isAlive");
				//m_connectMgrThread.
				res=new Thread(target);
			}else{
				return null;
			}
		}
		return res;
	}

	public void queryDevicesStates(List<Device> devList){
		LogUtils.v("mgr", "查询所有的设备状态");
		mQueryOnlineStateRunable.setListToQuery(devList);//如果线程正在运行，则直接将列表丢给线程
		m_queryOnlineThread=getNewThreadWhenNoActiveOrNull(m_queryOnlineThread, mQueryOnlineStateRunable);
		if(m_queryOnlineThread!=null){
			setmQueryOnlineExitStateFlag(false);
			m_queryOnlineThread.start();
		}
		//		mQueryAlarmSwitchRunable.setListToQuery(devList);
		//		m_queryAlarmThread=getNewThreadWhenNoActiveOrNull(m_queryAlarmThread, mQueryAlarmSwitchRunable);
		//		if(m_queryAlarmThread!=null){
		//			setmQueryAlarmSwitchsExitFlag(false);
		//			m_queryAlarmThread.start();
		//		}
	}
	private interface StateRunable{
		boolean onRun(DeviceUseUtils userUtils);//状态询问
		boolean exitFlag();//线程退出标志
		public boolean execWhileErrHappen();//当有设备状态询问失败，是否继续询问
	}

	public class QueryDevicesStatesRunable implements Runnable{
		private AtomicInteger atoInt=new AtomicInteger();
		private List<Device> listToQuery=null;
		private DeviceUseUtils utils=new DeviceUseUtils(null);
		private StateRunable m_stateRun;
		public QueryDevicesStatesRunable(StateRunable stateQueryRun){
			m_stateRun=stateQueryRun;
		}
		public synchronized List<Device> getListToQuery() {
			return listToQuery;
		}
		public  synchronized void setListToQuery(List<Device> listToQuery) {
			LogUtils.v("online", "setListToQuery locked ");
			atoInt.getAndSet(0);
			this.listToQuery=new ArrayList<Device>();
			this.listToQuery.addAll(listToQuery);
			//this.listToQuery = listToQuery;
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			queryrun:
				while(!m_stateRun.exitFlag()){
					synchronized(this){
						if(listToQuery==null || listToQuery.size()==0){
							return;
						}else{
							if (!mSys.isMgrConnected()) {
								//LogUtils.i("mgr", "online等待mgr的锁");
								mSys.waitForMgrState(0);//一直阻塞直到mgr连上
								//LogUtils.i("mgr", "onlinemgr的已经被解开");
							}
							//LogUtils.i("mgr", "mgr 仍是连接状态 ");
							int curIndex=atoInt.getAndIncrement();
							if(listToQuery.size()>0){//size大于零，说明列表中还有设备没有查询成功
								if(curIndex>=listToQuery.size()){//curIndex==size说明已经查询到最后一项，并且还有查询失败的设备
									if(m_stateRun.execWhileErrHappen()){
										atoInt.getAndSet(0);
										continue queryrun;
									}else{
										return;
									}
								}
								//LogUtils.i("mgr", "curIndex "+curIndex+" list size ");
								Device dev=listToQuery.get(curIndex);
								utils.setmDevice(dev);
								if(!m_stateRun.onRun(utils)){
									if(!m_stateRun.onRun(utils)){
										if(!m_stateRun.onRun(utils)){
											m_stateRun.onRun(utils);
										}
									}
								}
								atoInt.getAndDecrement();
								listToQuery.remove(curIndex);
							
//								if(!m_stateRun.onRun(utils)){//查询状态，在线，或者告警。false为失败了，失败了就查下一项，成功了就从列表中移除;
//									continue queryrun;
//								}else{
//									atoInt.getAndDecrement();
//									listToQuery.remove(curIndex);ss
//								}
							}else{
								return;
							}
						}
					}
				}
		}
	}

	public void connectMgrServer(OnMgrStateListener listener){
		//LogUtils.v("msgservice", "connectMgrServer");
		m_mgrStateListener=listener;
		m_connectMgrThread=getNewThreadWhenNoActiveOrNull(m_connectMgrThread, mMgrRunnable);
		if(m_connectMgrThread!=null){
			setmMgrSignInExitFlag(false);
			m_connectMgrThread.start();
		}
	}

	private class connectMgrRunnable implements Runnable{
		public void run(){
			while(!mMgrSignInExitFlag){
				//LogUtils.v("msgservice", "PW_NET_Mgr_SignIn"+Thread.currentThread().getId());
				if(!JniClient.PW_NET_Mgr_SignIn()){//如果失败则每两秒连接一次
					LogUtils.v("msgservice", "PW_NET_Mgr_SignIn fai");
					if(m_mgrStateListener!=null){
						try {
							Thread.sleep(1000);
						} catch (Exception e) {
							// TODO: handle exception
							e.printStackTrace();
						}

						m_mgrStateListener.onMgrSignInFailured();
					}
				}else{//连接成功则退出
					LogUtils.v("msgservice", "PW_NET_Mgr_SignIn suc");
					if(m_mgrStateListener!=null){
						m_mgrStateListener.onMgrStateChanged(true);
					}
					return;
				}
			}
		}
	}
	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		//		registerReceivers();
		LogUtils.v("msgservice", "onCreate");
		m_binder=new PwMsgBinder();
		mMgrRunnable=new connectMgrRunnable();
		mQueryOnlineStateRunable=new QueryDevicesStatesRunable(new StateRunable(){
			public boolean onRun(DeviceUseUtils userUtils){
				return userUtils.CheckOnlineState();
			}
			public  boolean exitFlag(){
				return mQueryOnlineExitStateFlag;
			}
			public boolean execWhileErrHappen(){
				return true;
			}
		});
		//		mQueryAlarmSwitchRunable=new QueryDevicesStatesRunable(new StateRunable(){
		//			public boolean onRun(DeviceUseUtils userUtils){
		//				return userUtils.CheckAlarmSwitchState();
		//			}
		//			public  boolean exitFlag(){
		//				return mQueryAlarmSwitchsExitFlag;
		//			}
		//			public boolean execWhileErrHappen(){
		//				return false;
		//			}
		//		});
		mSys=ShowmoSystem.getInstance();
		try {
			m_AlarmDao=(IAlarmDao)DatabaseHelper.getHelper(this).getDao(Alarm.class);
			m_DeviceDao=(IDeviceDao)DatabaseHelper.getHelper(this).getDao(Device.class);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		OnMsgDataCallBackWraper liWrap=OnMsgDataCallBackWraper.getInstance(new PwMSGCB());
		JniClient.PW_NET_SetMsgDataCallBack(liWrap, 0);
		//		int poolSize=Runtime.getRuntime().availableProcessors();
		//		m_Executor=new ThreadPoolExecutor(poolSize, poolSize*2+1, 1, TimeUnit.SECONDS, 
		//				new LinkedBlockingQueue<Runnable>(10));

		new Thread(){
			public void run() {
				Looper.prepare();
				m_alarmHandler=new Handler(Looper.myLooper());
				synchronized(objWait){
					try {
						objWait.notifyAll();
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
					}
				}
				Looper.loop();
			};
		}.start();
		synchronized(objWait){
			try {
				objWait.wait();
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
		}
		super.onCreate();
	}
	private void sendDBBroadcast(String Action,Object data){
		Intent intent=new Intent(Action);
		Bundle bundle=new Bundle();
		bundle.putSerializable(MSGBroadcastActions.DataKey, (Serializable)data);
		intent.putExtras(bundle);
		this.sendBroadcast(intent);
	}
	private void sendDBBroadcast(String Action,int data){
		Intent intent=new Intent(Action);
		intent.putExtra(MSGBroadcastActions.DataKey, data);
		this.sendBroadcast(intent);
	}

	private Device findDevice(List<Device> DevList,int deviceID){
		for (Device device : DevList) {
			if(device.getmDeviceId()==deviceID){
				return device;
			}
		}
		return null;
	}

	public class PwMSGCB  implements OnMsgDataCallBackListener{
		@Override
		public void onMsgDataCallBack(Object pBuffer,long lmsgid) {
			// TODO Auto-generated method stub
			//pBuffer.getClass().getMethod(name,Float.TYPE)
			User curUser=mSys.getCurUser();
			if(curUser==null){
				return;
			}
			List<Device> devList=curUser.getDevices();
			int msgid=(int)lmsgid;
			switch (msgid) {
			case MSGID.UPDATE_MOBILE_INVITE_INFO://升级
				Update_2_mobile_invite inviteMsg=(Update_2_mobile_invite)pBuffer;
				Device dev=findDevice(devList, inviteMsg.deviceId);
				if(dev!=null){
					dev.setmVersion(inviteMsg.softwareVersion);
					m_DeviceDao.updateDevice(dev);
					sendDBBroadcast(MSGBroadcastActions.UPDATE_MOBILE_INVITE_INFO, inviteMsg);
				}
				//LogUtils.v("upgrade", "get upgrade info deviceId:"+inviteMsg.deviceId+"softversion:"+inviteMsg.softwareVersion);
				break;
			case MSGID.UPDATE_MOBILE_INVITE_ACK://升级响应
				SDK_CAMERA_UPDATE ftpAck=(SDK_CAMERA_UPDATE)pBuffer;
				LogUtils.i("upgrade", "get upgrade  ack deviceId:"+ftpAck.cameraid+"status:"+ftpAck.cmd+" pos "+ftpAck.downpos);
				Device devAck=findDevice(devList, ftpAck.cameraid);
				if(ftpAck.cmd==MSGID.UPDATE_SUCCESS){// 1 msg received; 2 update packet downloaded; 4update failed ; 8 update succeed
					if(devAck!=null){
						devAck.setmUpgrading(false);
						devAck.setmVersion("");
						m_DeviceDao.updateDevice(devAck);
					}
					sendDBBroadcast(MSGBroadcastActions.UPDATE_SUCCESS_MOBILE_INVITE_ACK, ftpAck);
				}else if (ftpAck.cmd==MSGID.UPDATE_FAILED) {
					if(devAck!=null){
						devAck.setmUpgrading(false);
						m_DeviceDao.updateDevice(devAck);
					}
					sendDBBroadcast(MSGBroadcastActions.UPDATE_FAILE_MOBILE_INVITE_ACK, ftpAck);
				}else if(ftpAck.cmd == MSGID.UPDATE_DOWNPOS){
					if(devAck!=null){
						devAck.setmUpgrading(true);
						devAck.setmDownUpgradePkgPos(ftpAck.downpos);
						m_DeviceDao.updateDevice(devAck);
					}
					sendDBBroadcast(MSGBroadcastActions.UPDATING_MOBILE_INVITE_ACK, ftpAck);
				}
				//sendDBBroadcast(MSGBroadcastActions.UPDATE_MOBILE_INVITE_ACK, ftpAck);
				break;
			case MSGID.HIST_VIDEO_CAMERA_ACK_MSG://回放结束
				Remote_Message remoteMsg=(Remote_Message)pBuffer;
				//LogUtils.v("playback", "playbackover"+remoteMsg.toString());
				EventBus.getDefault().post(new PlaybackCompleteEvent());
				//sendDBBroadcast(MSGBroadcastActions.HIST_VIDEO_CAMERA_ACK_MSG, remoteMsg);
				break;
			case MSGID.CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG://告警消息
				Device_alarm_server_upload_msg alarmMsg=(Device_alarm_server_upload_msg)pBuffer;
				//LogUtils.v("alarm", "get a alarm msg: "+alarmMsg.toString());
				//LogUtils.v("msgcb", alarmMsg.toString());
				Device alarmDev=findDevice(devList, alarmMsg.deviceId);
				if(alarmDev!=null){
					Alarm alarm=new Alarm(alarmMsg.clientId, 
							alarmMsg.recordId, alarmMsg.deviceId,
							alarmMsg.cameraId, alarmMsg.channelNo, 
							alarmMsg.alarmType, alarmMsg.alarmMode,
							alarmMsg.beginTime,alarmMsg.endTime,
							alarmMsg.alarmCode, alarmMsg.Ccid);
					Alarm alarminfof= m_AlarmDao.queryByRecordId(alarmMsg.recordId);
					if(alarminfof!=null){
						LogUtils.e("findAlarmDb", "FindAlarmDb: "+alarminfof.toString());
						LogUtils.e("receiveAlarm", "FindService: "+alarm.toString());
					}
					m_AlarmDao.insert(alarm);
					alarmDev.setHaveNewAlarmInfo(true);
					m_DeviceDao.updateDevice(alarmDev);
					sendDBBroadcast(MSGBroadcastActions.CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG, alarm);
				}
				break;

			case MSGID.CLIENT_MGR_CAMERA_OFFLINE_MSG:
				User_2_mgr_disconn_device deviceOffInfo=(User_2_mgr_disconn_device)pBuffer;
				//LogUtils.e("online", "CLIENT_MGR_CAMERA_OFFLINE_MSG "+deviceOffInfo.toString());
				final Device devOffline=findDevice(devList, deviceOffInfo.device_id);
				if(devOffline!=null){
					devOffline.setmOnlineState(false);
					m_handler.post(new Runnable() {
						@Override
						public void run() {
							// TODO Auto-generated method stub
							m_alarmHandler.removeMessages(devOffline.getmCameraId());
							//LogUtils.i("alarm","remove runnable from handler cameraid:"+devOffline.getmCameraId());
						}
					});
				}
				EventBus.getDefault().post(new DevOnlineStateEvent(false,deviceOffInfo));
				sendDBBroadcast(MSGBroadcastActions.CLIENT_MGR_CAMERA_ONLINE_STATE_CHANGE_MSG, deviceOffInfo);

				break;
			case MSGID.CLIENT_MGR_CAMERA_ONLINE_MSG:
				User_2_mgr_disconn_device deviceOnInfo=(User_2_mgr_disconn_device)pBuffer;
				//	LogUtils.e("online", "ONLINE_MSG:: "+deviceOnInfo.toString());
				Device devOnline=findDevice(devList, deviceOnInfo.device_id);

				if(devOnline!=null){
					devOnline.setmOnlineState(true);
					//AsyncTask.execute(new AlarmStateRunnable(devOnline));
				//	LogUtils.v("alarm", "CLIENT_MGR_CAMERA_ONLINE_MSG post alarm task");
					AlarmStateRunnable asrun=new AlarmStateRunnable(devOnline);
					Message msg=getAlarmMessage(asrun, devOnline.getmCameraId());
					m_alarmHandler.removeMessages(devOnline.getmCameraId());
					m_alarmHandler.sendMessageDelayed(msg, 0);
					//LogUtils.i("alarm","post runnable to handler cameraid:"+devOnline.getmCameraId());
					//LogUtils.e("broadcast", "devOnline:: "+devOnline);
				}
				EventBus.getDefault().post(new DevOnlineStateEvent(true,deviceOnInfo));
				sendDBBroadcast(MSGBroadcastActions.CLIENT_MGR_CAMERA_ONLINE_STATE_CHANGE_MSG, deviceOnInfo);
				break;
			case MSGID.MGR_OUTLINE:
				//LogUtils.e("msgcb", "mgr disconnect");
				if(m_mgrStateListener!=null){
					m_mgrStateListener.onMgrStateChanged(false);
				}
				break;
			case MSGID.PLAY_STOP:
				EventBus.getDefault().post(new PlayDisconnectEvent());
				break;
			case MSGID.DOWNLOAD_PIC_POS:
				alarm_data_download_progress pros=(alarm_data_download_progress)pBuffer;
				//LogUtils.v("alarm", "DOWNLOAD_PIC_POS recordId:"+pros.recordId+" pos: "+pros.pos);
				sendDBBroadcast(MSGBroadcastActions.CAMERA_ALARM_DATA_DOWLOAD_POS, pros);
				break;
			case MSGID.DOWNLOAD_PIC_FAILED:
				alarm_data_download download=(alarm_data_download)pBuffer;
				//LogUtils.v("alarm", "DOWNLOAD_PIC_POS recordId:"+download.recordId+" state: "+download.state);
				sendDBBroadcast(MSGBroadcastActions.CAMERA_ALARM_DATA_DOWLOAD_fai_MSG, download);
				break;
			case MSGID.DOWNLOAD_PIC_SUCCESS://告警图片数据
				alarm_data_download alarmPicInfo=(alarm_data_download)pBuffer;
				alarm_data_state state=new alarm_data_state();
				state.recordId=alarmPicInfo.recordId;
				IAlarmDao dao=null;
				try {
					dao=(IAlarmDao)DatabaseHelper.getHelper(PwMsgCallbackDealService.this).getDao(Alarm.class);
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
				}
				Alarm alarm=dao.queryByRecordId(alarmPicInfo.recordId);
				if(alarm==null){
					LogUtils.v("alarm","not exist alarm in db "+ alarmPicInfo.recordId+ alarm);
					return;
				}
				alarm.setmImgDownloading(false);
				state.cameraId=alarm.getCameraId();

			//	LogUtils.v("alarm","get alarm data info:recordId "+ state.recordId+" cameraID "+alarm.getCameraId());
				if(alarmPicInfo.alarmImgFilename==null){
					state.state=-1;
				}else{
					alarm.setmImgPath(alarmPicInfo.alarmImgFilename);
					dao.updateInfo(alarm);
					state.state=1;
				}
				sendDBBroadcast(MSGBroadcastActions.CAMERA_ALARM_DATA_UPLOAD_MSG, state);
				break;
			default:
				break;
			}
		}
	}
	private Message getAlarmMessage(Runnable run,int what){
		Message msg=Message.obtain(m_alarmHandler, run);
		msg.what=what;
		return msg;

	}
	private class AlarmStateRunnable implements Runnable{
		private Device dev;
		public AlarmStateRunnable(Device dev){
			this.dev=dev;
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			DeviceUseUtils util=new DeviceUseUtils(dev);
			//LogUtils.i("alarm", "AlarmStateRunnable cameraId:"+dev.getmCameraId());

			if(!util.CheckAlarmSwitchState()){
				if(!util.CheckAlarmSwitchState()){
					util.CheckAlarmSwitchState();
				}
			}
		}
	}

	@Override
	public void onStart(Intent intent, int startId) {
		// TODO Auto-generated method stub
		LogUtils.v("msgservice", "onStart");
		super.onStart(intent, startId);
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		// TODO Auto-generated method stub
		LogUtils.v("msgservice", "onStartCommand");

		return super.onStartCommand(intent, flags, startId);
	}

	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		//		unregisterReceivers();
		//LogUtils.v("msgservice", "onDestroy");
		m_alarmHandler.getLooper().quit();
		m_alarmHandler=null;
		if(m_connectMgrThread!=null){
			if(m_connectMgrThread!=null){
				setmMgrSignInExitFlag(true);
			}
		}
		if(m_queryOnlineThread!=null){
			if(m_queryOnlineThread.isAlive()){
				setmQueryOnlineExitStateFlag(true);
			}
		}
		JniClient.PW_NET_SetMsgDataCallBack(null, 0);
		super.onDestroy();
	}

	@Override
	public boolean onUnbind(Intent intent) {
		// TODO Auto-generated method stub
		//LogUtils.v("msgservice", "onUnbind");

		return super.onUnbind(intent);
	}
	public boolean ismMgrSignInExitFlag() {
		return mMgrSignInExitFlag;
	}
	public synchronized void setmMgrSignInExitFlag(boolean mMgrSignInExitFlag) {
		this.mMgrSignInExitFlag = mMgrSignInExitFlag;
	}
	public boolean ismQueryOnlineExitStateFlag() {
		return mQueryOnlineExitStateFlag;
	}
	public synchronized  void setmQueryOnlineExitStateFlag(boolean mQueryOnlineStateFlag) {
		this.mQueryOnlineExitStateFlag = mQueryOnlineStateFlag;
	}
	public boolean ismQueryAlarmSwitchsExitFlag() {
		return mQueryAlarmSwitchsExitFlag;
	}
	public synchronized void setmQueryAlarmSwitchsExitFlag(boolean mQueryAlarmSwitchs) {
		this.mQueryAlarmSwitchsExitFlag = mQueryAlarmSwitchs;
	}

}
