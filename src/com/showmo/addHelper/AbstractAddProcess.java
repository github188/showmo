package com.showmo.addHelper;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import com.showmo.base.ShowmoSystem;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.event.DevOnlineStateEvent;
import com.showmo.eventBus.EventBus;
import com.showmo.userManage.User;
import com.showmo.util.LogUtils;
import com.showmo.util.PwTimer;
import com.showmo.util.safelist.IFindSubcriber;
import com.showmo.util.safelist.IFindSubcriber1;
import com.showmo.util.safelist.SafeList;

import android.os.Handler;
import android.os.Looper;
import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.BindStateInfo;
import ipc365.app.showmo.jni.JniDataDef.Broadcast_wifi_info;
import rx.Observable;
import rx.Subscriber;
import rx.Observable.OnSubscribe;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.schedulers.Schedulers;

public abstract class AbstractAddProcess implements IAddCtrlProcess{
	public enum DEV_BIND_STATE{
		DEV_STATE_ERR(-1),DEV_STATE_BIND_NOBODY(0),DEV_STATE_BIND_BYSELF(1),DEV_STATE_BIND_BYOTHER(2);
		int state=-1;
		private DEV_BIND_STATE(int state){
			this.state = state;
		}
		public static DEV_BIND_STATE get(int state){
			for (DEV_BIND_STATE i : DEV_BIND_STATE.values()) {
				if(i.state == state){
					return i;
				}
			}
			return null;
		}
	}
	public enum ConfigState{
		NoBegin,PreSearching,Configing,Paused,Exited;
	}
	private ConfigState mConfigState=ConfigState.NoBegin;
	private SafeList<String> mPreDevsInLan;
	private SearchRunnable m_searchRun;
	private SetRunnable m_setRun;
	private Thread m_addThread;
	private Thread m_searchThread;
	private Thread m_setThread;
	private Handler m_addHandler;
	private Object lockSet = new Object();
	private Object lockSearch = new Object();
	private Object addWaitLockObject= new Object();
	private SafeList<DevSimpleInfo> m_devsWaitForAdd;
	private SafeList<Device> m_sucDevs;

	private HashMap<String, Integer> mDeviceIdContainer;
	private HashMap<String, Integer> mBindStateContainer;
	private HashMap<String, String> mOtherBindUsers;
	private String m_ssid;
	private String m_psw;
	private String m_keyType;
	protected User mCurUser;

	public AbstractAddProcess(){
		this.init();
	}
	public void setConfigState(ConfigState state){
		mConfigState=state;
	}
	private void init(){
		m_addThread=new Thread(){
			public void run() {
				Looper.prepare();
				m_addHandler=new Handler();
				synchronized (m_addThread) {
					try {
						m_addThread.notifyAll();
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
					}	
				}
				Looper.loop();
			};
		};
		m_addThread.start();
		if(m_addHandler==null){
			synchronized (m_addThread) {
				try {
					m_addThread.wait();
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
				}	
			}
		}
		m_setRun = new SetRunnable();
		m_setThread = new Thread(m_setRun);
		m_searchRun = new SearchRunnable();
		m_searchThread = new Thread(m_searchRun);

		mCurUser = ShowmoSystem.getInstance().getCurUser();
		m_sucDevs=new SafeList<Device>();
		mPreDevsInLan=new SafeList<String>();
		m_devsWaitForAdd=new SafeList<DevSimpleInfo>();
		mDeviceIdContainer=new HashMap<String, Integer>();
		mBindStateContainer=new HashMap<String, Integer>();
		mOtherBindUsers=new HashMap<String, String>();
		mOnlineEventReceiver=new PwEventReceivers();
	}
	private PwEventReceivers mOnlineEventReceiver;
	@Override
	public void beginWork(String ssid,String psw,String keyType) {
		// TODO Auto-generated method stub
		m_ssid=ssid;
		m_psw=psw;
		m_keyType=keyType;
		preSearchDevsInLan();
		EventBus.getDefault().register(mOnlineEventReceiver);
	}

	@Override
	public void pauseWork() {
		// TODO Auto-generated method stub
		setConfigState(ConfigState.Paused);
		if (m_setThread.isAlive()) {
			m_setRun.setThreadPause(true);
		}
		if (m_searchThread.isAlive()) {
			m_searchRun.setThreadPause(true);
		}
	}

	@Override
	public void continueWork() {
		// TODO Auto-generated method stub
		m_setRun.setThreadPause(false);
		m_searchRun.setThreadPause(false);
	}

	@Override
	public void exitWork() {
		// TODO Auto-generated method stub
		if(mConfigState == ConfigState.NoBegin){
			return;
		}
		setConfigState(ConfigState.Exited);
		if(m_setThread!=null){
			if (m_setThread.isAlive()) {
				m_setRun.setThreadExit(true);
			}
		}
		if(m_searchThread!=null){
			if (m_searchThread.isAlive()) {
				m_searchRun.setThreadExit(true);
			}
		}
		m_setThread=null;
		m_searchThread=null;
		m_setRun=null;
		m_searchRun=null;
		m_sucDevs.clear();
		if(m_addHandler!=null){
			m_addHandler.getLooper().quit();
			m_addHandler=null;
		}
		EventBus.getDefault().unregister(mOnlineEventReceiver);
	}

	private void preSearchDevsInLan(){//在操作之前搜索局域网内已经存在的设备
		Observable.create(new OnSubscribe<Void>() {
			@Override
			public void call(Subscriber<? super Void> t) {
				// TODO Auto-generated method stub
				List<String> tlist= JniClient.PW_PRI_SearchDevInLan(5);
				if(tlist!=null){
					for (String string : tlist) {
						LogUtils.e("addDevice", "device pre search uuid:"+string.toUpperCase());
						//						postAddReq(string);
						mPreDevsInLan.add(string.toUpperCase().trim());
					}
				}
				t.onNext(null);
			}
		}).subscribeOn(Schedulers.io())
		.observeOn(AndroidSchedulers.mainThread())
		.subscribe(new Action1<Void>() {
			@Override
			public void call(Void t) {
				// TODO Auto-generated method stub
				
				if(mConfigState!=ConfigState.Exited){
					setConfigState(ConfigState.Configing);
					startConfig();
				}
				//				String preLiString="";
				//				for (String string : mPreDevsInLan) {
				//					preLiString +=string.toUpperCase()+"\n";
				//				}
				//				mPreTextView.setText((CharSequence)preLiString);
				LogUtils.fe(LogUtils.LogAppFile,  "startConfig(); presearch size:"+mPreDevsInLan.size());
			}
		});
		setConfigState(ConfigState.PreSearching);
	}
	private void startConfig(){
		if(m_setRun==null){
			m_setRun = new SetRunnable();
		}
		if(m_setThread==null){
			m_setThread = new Thread(m_setRun);
		}
		if(m_searchRun==null){
			m_searchRun = new SearchRunnable();
			LogUtils.e("hashcode", "instartConfig "+m_searchRun.hashCode());
		}
		if(m_searchThread==null){
			m_searchThread = new Thread(m_searchRun);
		}
		m_setThread.start();
		m_searchThread.start();
	}
	private class SetRunnable implements Runnable {
		private boolean bSetThreadExit = false;
		private boolean bSetThreadPause = false;
		public synchronized void setThreadPause(boolean pause) {
			//bSetThreadPause = pause;
			JniClient.PW_PRI_SetPauseCycleBroadcastToDev(pause);
		}
		public synchronized void setThreadExit(boolean exit) {
			//bSetThreadExit = exit;
			LogUtils.fe(LogUtils.LogAppFile, "PW_PRI_StopCycleBroadcastToDev");
			JniClient.PW_PRI_StopCycleBroadcastToDev();
		}
		public void run() {
			Broadcast_wifi_info info = new
					Broadcast_wifi_info(m_ssid,
							m_psw, m_keyType, "s");
			synchronized (lockSet) {
				while(!JniClient.PW_PRI_BeginCycleBroadcastToDev(info));
			}
		}
	}

	private class SearchRunnable implements Runnable {
		private boolean bSearchThreadExit = false;
		private DeviceUseUtils utils = new DeviceUseUtils(null);

		public synchronized void setThreadExit(boolean exit) {
			bSearchThreadExit = exit;
		}

		private boolean bSearchThreadPause = false;

		public synchronized void setThreadPause(boolean pause) {
			bSearchThreadPause = pause;
		}

		public void run() {
			//			List<String> tlist= JniClient.PW_PRI_SearchDevInLan(3);
			//			
			//			LogUtils.e("addDevice", "pre search over "+JniClient.PW_PRI_GetLastError());
			//			if(tlist!=null){
			//				for (String string : tlist) {
			//					LogUtils.e("presearch", "device pre uuid:"+string.toUpperCase());
			//					mPreDevsInLan.add(string.toUpperCase().trim());
			//				}
			//			}
			while (!bSearchThreadExit) {
				if (!bSearchThreadPause) {
					List<String> ssidList;
					synchronized (lockSearch) {
						ssidList = JniClient.PW_PRI_SearchDevInLan(3);
					}
					if (ssidList == null) {
						LogUtils.i("adddevice", "PW_PRI_SearchDevInLan ssidList == null "+JniClient.PW_PRI_GetLastError());
					} else {
						List<String> templist=new ArrayList<String>();
						for (int i=0;i<ssidList.size();i++) {
							LogUtils.fe(LogUtils.LogAppFile,"searched in lan:"+ssidList.get(i));
							boolean bflag=false;//是否已经在等待添加，或者已经添加完成
							Object findObject=null;
							findObject=mPreDevsInLan.find(
									new IFindSubcriber1<String,String>() {
										public boolean eqJudge(String item, String para) {
											boolean eq=para.toUpperCase().trim().equals(item.toUpperCase());
											if(eq){
												int bindstate=obtainBindState(item);//查看是否还有为绑定的设备
												if( bindstate==0){
													postAddReq(item);
												}
												LogUtils.fe(LogUtils.LogAppFile, " find device in pre uuid:"+item);
											}
											return eq;
										};
									},ssidList.get(i));
							if(findObject==null){
								findObject=m_devsWaitForAdd.find(
										new IFindSubcriber1<DevSimpleInfo,String>() {
											@Override
											public boolean eqJudge(DevSimpleInfo item,
													String para) {
												// TODO Auto-generated method stub
												boolean eq=para.toUpperCase().trim().equals(item.uuid.toUpperCase());
												if(eq){
													LogUtils.fe(LogUtils.LogAppFile," find device in wait list uuid:"+item);
												}
												return eq;
											}
										},ssidList.get(i));
							}
							if(findObject==null){
								findObject=m_sucDevs.find(
										new IFindSubcriber1<Device,String>() {
											@Override
											public boolean eqJudge(Device item,
													String para) {
												// TODO Auto-generated method stub
												return para.toUpperCase().trim().equals(item.getmUuid().toUpperCase());
											}
										},ssidList.get(i));
							}
							if(findObject!=null){
								bflag=true;
							}

							if(!bflag){
								
								LogUtils.fe(LogUtils.LogAppFile, "postAddReq to add uuid:"+ssidList.get(i).toUpperCase());
								postAddReq(ssidList.get(i).toUpperCase());
							}
						}
					}
				}else{
					try {
						Thread.sleep(100);
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
					}
				}
			}
			//LogUtils.i("hashcode", "runnable exit "+this.hashCode());
		}
	}
	private synchronized int obtainDeviceId(String uuid){// <0代表获取失败
		Integer cameraIdObj=mDeviceIdContainer.get(uuid);
		int deviceId=0;
		if(cameraIdObj==null){
			deviceId=(int)JniClient.PW_NET_GetDeviceid(uuid);
			if(deviceId >= 0){
				mDeviceIdContainer.put(uuid, Integer.valueOf(deviceId));
				LogUtils.fe(LogUtils.LogAppFile, "PW_NET_GetDeviceid online uuid:"+uuid+" deviceId:"+deviceId);
			}
		}else{
			deviceId=cameraIdObj.intValue();
			//LogUtils.i("addDevice", "PW_NET_GetDeviceid uuid:"+uuid+" deviceId:"+deviceId);
		}

		return deviceId;
	}
	protected String obtainOtherBindUser(String uuid){
		return mOtherBindUsers.get(uuid);
	}
	private synchronized int obtainBindState(String uuid){
		Integer bindsate=mBindStateContainer.get(uuid);
		int mBindSate=-1;
		if(bindsate == null){
			BindStateInfo stateinfo=JniClient.PW_NET_BindState(uuid);
			if(stateinfo == null){
				return -1;
			}else {
				//LogUtils.e("addDevice", "PW_NET_BindState online uuid:"+uuid+" mBindSate:"+mBindSate);
				mBindStateContainer.put(uuid, Integer.valueOf((int)stateinfo.state));
				if(stateinfo.state == 2){
					mOtherBindUsers.put(uuid, stateinfo.deviceCurUser);
				}
			}
		}else{
			mBindSate=bindsate.intValue();
		//	LogUtils.i("addDevice", "PW_NET_BindState uuid:"+uuid+" mBindSate:"+mBindSate);
		}
		return mBindSate;
	}
	private void postAddReq(String uuid){
		LogUtils.i("adddevice", "postAddReq uuid:"+uuid);
		if(m_addHandler!=null){
			m_addHandler.post(new AddRunnable(uuid));
		}
	}
	public class AddRunnable implements Runnable{//请求在线状态
		private String uuid;
		public AddRunnable(String uuid){
			this.uuid=uuid;
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			final DevSimpleInfo info=getDevStateInfo(uuid);
			if(info!=null){
				DevSimpleInfo findObj=findDevInWaitList(info.deviceid);
				if(findObj == null){
					m_devsWaitForAdd.add(info);
				}else {
					return;
				}
			}else {//状态获取失败或者被过滤
				return;
			}
			if(!JniClient.PW_NET_OnLineState(info.deviceid)){
				if(!JniClient.PW_NET_OnLineState(info.deviceid)){
					LogUtils.fe(LogUtils.LogAppFile, "PW_NET_OnLineState ERR "+info.deviceid+" "+uuid);
					m_devsWaitForAdd.remove(info);
					return;
				}
			}
			//在线状态2秒内没有返回就直接从添加列表删除
			PwTimer checkOnlineTimer=new PwTimer(false) {
				@Override
				public void doInTask() {
					// TODO Auto-generated method stub
					if(findDevInWaitList(info.deviceid)!=null){
						LogUtils.e("adddevice", " online state out of time");
						m_devsWaitForAdd.remove(info);
					}
				}
			};
			checkOnlineTimer.start(2000,false);
		}
	} 

	private DevSimpleInfo findDevInWaitList(final int deviceid){
		return m_devsWaitForAdd.find(new IFindSubcriber<DevSimpleInfo>() {	
			@Override
			public boolean eqJudge(DevSimpleInfo item) {
				// TODO Auto-generated method stub
				return deviceid==item.deviceid;
			}
		});
	}
	//获取设备绑定信息以及deviceid
	protected DevSimpleInfo getDevStateInfo(String uuid){
		int bindState= obtainBindState(uuid);
		if(!onDevStateFilter(DEV_BIND_STATE.get(bindState))){
			return null;
		}
		int deviceid=obtainDeviceId(uuid);
		if(deviceid<0){
			return null;
		}
		return new DevSimpleInfo(uuid,deviceid);
	}
	/*
	 * 过滤 设备状态，即绑定或者配置操作针对哪种状态的设备进行
	 */
	protected abstract boolean onDevStateFilter(DEV_BIND_STATE state);
	/*
	 * 针对不同状态的设备进行操作
	 */
			
	protected abstract Device performBindBySelfDev(DevSimpleInfo sdevinfo);
	protected abstract Device performBindByOtherDev(DevSimpleInfo sdevinfo);
	protected abstract Device performBindByNoBodyDev(DevSimpleInfo sdevinfo);
	
	protected void addOnlineDev(DevSimpleInfo sdevinfo){
		if(sdevinfo!=null){
			int state=obtainBindState(sdevinfo.uuid);
			if(state == 1){//已被自己绑定
				Device newDevice=performBindBySelfDev(sdevinfo);
				//添加成功之后获取设备状态
				if(newDevice!=null)
					JniClient.PW_NET_OnLineState(newDevice.getmDeviceId());
				return;
			}else if(state == 2){//设备已被他人绑定过
				LogUtils.fe(LogUtils.LogAppFile, "dev bind by other failured uuid:"+sdevinfo.uuid);
				performBindByOtherDev(sdevinfo);
				mPreDevsInLan.add(sdevinfo.uuid);
				return;
			}else if (state == 0) {
				Device device=performBindByNoBodyDev(sdevinfo);
				if(device!=null){
					m_sucDevs.add(device);
				}
			}
			
		}
	}
	public class PwEventReceivers{
		public void onEventAsync(final DevOnlineStateEvent ev) {
			// TODO Auto-generated method stub
			LogUtils.fe(LogUtils.LogAppFile, "onEventAsync online deviceid:"+ev.info.device_id+" state "+ev.online);
			synchronized(addWaitLockObject){
				if(m_sucDevs.size()>=1){
					return;
				}
				DevSimpleInfo sdevinfo=findDevInWaitList(ev.info.device_id);
				if(!ev.online){
					m_devsWaitForAdd.remove(sdevinfo);
					return;
				}
				addOnlineDev(sdevinfo);
				m_devsWaitForAdd.remove(sdevinfo);
			}
		}
	}
}
