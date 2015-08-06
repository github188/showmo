package com.showmo.base;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import javax.mail.internet.NewsAddress;

import com.showmo.alarmManage.Alarm;
import com.showmo.deviceManage.Device;
import com.showmo.eventBus.EventBus;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.dao.IAlarmDao;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.ormlite.dao.impl.AlarmDaoImpl;
import com.showmo.playHelper.IDevicePlayer;
import com.showmo.playHelper.PlayHelper;
import com.showmo.safe.Safe;
import com.showmo.service.PwMsgCallbackDealService;
import com.showmo.service.PwMsgCallbackDealService.OnMgrStateListener;
import com.showmo.service.PwMsgCallbackDealService.PwMSGCB;
import com.showmo.service.PwMsgCallbackDealService.PwMsgBinder;
import com.showmo.userManage.IUserObject;
import com.showmo.userManage.User;
import com.showmo.util.LogUtils;
import com.showmo.util.PwNetWorkHelper;
import com.showmo.util.ScreenUtil;
import com.showmo.util.StringUtil;
import com.tencent.android.tpush.XGPushManager;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.widget.Toast;
import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.CAMERA_ALARM_TYPE;
import ipc365.app.showmo.jni.JniDataDef.ClientDeviceQueryRet;
import ipc365.app.showmo.jni.JniDataDef.ClientRegister2Login;
import ipc365.app.showmo.jni.JniDataDef.ClienteLoginReq;
import ipc365.app.showmo.jni.JniDataDef.Device_alarm_server_upload_msg;
import ipc365.app.showmo.jni.JniDataDef.MSGBroadcastActions;
import ipc365.app.showmo.jni.JniDataDef.MSGID;
import ipc365.app.showmo.jni.JniDataDef.OnDebugDataCallbackListener;
import ipc365.app.showmo.jni.JniDataDef.OnMsgDataCallBackListener;
import ipc365.app.showmo.jni.JniDataDef.Remote_Message;
import ipc365.app.showmo.jni.JniDataDef.ResetPasswordByVerifyReq;
import ipc365.app.showmo.jni.JniDataDef.ResetPasswordReq;
import ipc365.app.showmo.jni.JniDataDef.Update_2_mobile_invite;

public class ShowmoSystem implements OnMgrStateListener {
	public final static int SHOWMO_USER = 1;
	public final static int IPC365_USER = 0;
	private User m_currentUser;
	private boolean m_bInit;
	private static ShowmoSystem m_instance;
	private PlayHelper m_playHelper;
	private Context mContext;
	private Intent m_msgCbService;
	private ServiceConnection msgCbSc;
	private PwMsgCallbackDealService MsgService = null;
	private boolean MgrConnected = false;
	private Object mgrStateLock;
	private Object initLock;
	// 当需要mgr连接的操作进行时。如果mgr没有连接上，那么可以在这个锁上等待，MGR连接失败。或者连接上的时候会唤醒所有等待者,等待者可以选择在失败的时候继续等待
	public static final String SafeConfigXml="safe_config";
	public static final String SafeCurLevelKey="current_safe_level";
	BroadcastReceiver mNetReceiver;
	private static final int HANDLETOAST = 1;
	private Handler m_Handler = new Handler(Looper.getMainLooper()) {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case HANDLETOAST:
				Toast.makeText(mContext, (String) msg.obj, Toast.LENGTH_SHORT)
				.show();
				break;

			default:
				break;
			}
		}
	};

	public synchronized static ShowmoSystem getInstance() {
		if (m_instance == null) {
			m_instance = new ShowmoSystem();
		}
		return m_instance;
	}

	private class PwMsgCbSc implements ServiceConnection {
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			// TODO Auto-generated method stub

			PwMsgBinder binder = (PwMsgBinder) service;
			MsgService = binder.getService();
			MsgService.connectMgrServer(ShowmoSystem.this);
			MsgService.queryDevicesStates(m_currentUser.getDevices());//获取完设备之后才连接服务。这时候立即获取可能失败，所以要等确定服务连上再询问
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			// TODO Auto-generated method stub

		}
	}

	private void toastToUI(String str) {
		Message msg = m_Handler.obtainMessage();
		msg.what = HANDLETOAST;
		msg.obj = str;
		msg.sendToTarget();
	}

	@Override
	public void onMgrStateChanged(boolean state) {
		LogUtils.v("sys", "onMgrStateChanged " + state);
		setMgrConnected(state);
		if (!state) {
			//toastToUI("您与服务器的连接已经断开");
			if (m_currentUser != null) {
				//if(PwNetWorkHelper.getInstance().getNetConnectState()){
					if (m_currentUser != null) {
						if (MsgService != null) {
							MsgService.connectMgrServer(ShowmoSystem.this);
						}
					}
				//}
			}
		} else {
			//LogUtils.v("mgr", "获取mgr的锁");
			synchronized (mgrStateLock) {
				LogUtils.v("mgr","解开所有等待mgr的锁");
				mgrStateLock.notifyAll();
			}
			//toastToUI("您已经连上服务器");
		}

	}



	@Override
	public void onMgrSignInFailured() {//唤醒所有等待的锁，防止一直等待
		synchronized (mgrStateLock) {
			mgrStateLock.notifyAll();
		}
	}

	private ShowmoSystem() {
		m_bInit = false;
		m_currentUser = null;
		msgCbSc = new PwMsgCbSc();
		mgrStateLock = new Object();
		initLock=new Object();
		mNetReceiver=new NetWorkConnectivityReceiver();
	}

	public Object getMgrStateLock() {//返回mgr状态的锁，
		return mgrStateLock;
	}

	public boolean waitForMgrState(int count) {// 等待mgr连接count (count<=0 则一直阻塞直到连上) 次，如果连接上则返回true否则返回false
		int mcount = count;
		boolean bres = false;
		MsgService.connectMgrServer(this);
		if(mcount<=0){
			while (!isMgrConnected()) {//没有连接上则循环等待
				try {
					LogUtils.v("sys", "mgrStateLock before wait");
					synchronized (mgrStateLock) {
						mgrStateLock.wait();
					}
				} catch (InterruptedException e) {
					// TODO: handle exception
					e.printStackTrace();
				}
			}
			bres=true;
		}else{
			do {//连续mcount次等待都失败则返回
				mcount--;
				bres = isMgrConnected();
				if (!bres) {
					try {
						LogUtils.v("sys", "mgrStateLock before wait");
						synchronized (mgrStateLock) {
							mgrStateLock.wait();
						}
					} catch (InterruptedException e) {
						// TODO: handle exception
						e.printStackTrace();
					}
				} else {
					break;
				}
				bres = isMgrConnected();
			} while (!bres && mcount > 0);
		}
		return bres;
	}

	public boolean isMgrConnected() {
		return MgrConnected;
	}

	private void setMgrConnected(boolean mgrConnectState) {
		synchronized (mgrStateLock) {
			MgrConnected = mgrConnectState;
		}
	}

	public IDevicePlayer getPlayer() {
		return m_playHelper;
	}

	public void setPlayer(PlayHelper m_playHelper) {
		this.m_playHelper = m_playHelper;
	}
	/*
	 * @param username psw   usertype SHOWMO IPC365 
	 */
	public synchronized boolean userLogin(String username,String psw,int userType,boolean isExperience) throws NotInitException ,AlreadyLoginException{
		checkInit();
		if(m_currentUser!=null){
			if(m_currentUser.getUserName().equals(username)){
				LogUtils.fi(LogUtils.LogAppFile, "have been login by self");
				return true;
			}else {
				LogUtils.fi(LogUtils.LogAppFile, "have been login by other");
				throw new AlreadyLoginException("already login ,please logout first!");
			}
		}
		boolean bLogin;
		ClienteLoginReq req = new ClienteLoginReq();

		req.user_name = username;
		req.pass = psw;
		req.net_type = userType;
		req.padding = 0;
		LogUtils.fi(LogUtils.LogAppFile, "before login api");
		bLogin = JniClient.PW_NET_Login(req);
		LogUtils.fi(LogUtils.LogAppFile, "after login "+bLogin);
		if(bLogin){
			LogUtils.fi(LogUtils.LogAppFile, "before login getUserDevicesFromServer");
			List<Device> devlist=getUserDevicesFromServer(username);
			LogUtils.fi(LogUtils.LogAppFile, "after login getUserDevicesFromServer "+devlist);
			if(devlist == null){
				if(bLogin){
					JniClient.PW_NET_Logout();
				}
				return false;
			}
			compareWithDao(username,devlist);
			m_currentUser=new User(username, psw, isExperience,devlist);
			registerReceivers();
			registerMsgCallback();
		}
		LogUtils.fi(LogUtils.LogAppFile, "login over ");
		return bLogin;
	}
	private void registerReceivers(){
		IntentFilter filter=new IntentFilter();
		filter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
		mContext.registerReceiver(mNetReceiver, filter);
	}
	private void unregisterReceivers(){
		mContext.unregisterReceiver(mNetReceiver);
	}
	private boolean registerMsgCallback(){
		if(m_currentUser!=null){
			if(m_currentUser.getDevices()!=null){
				LogUtils.v("bind", "bindService registerMsgCallback");
				mContext.bindService(m_msgCbService, msgCbSc,
						Context.BIND_AUTO_CREATE);
			}else{
				return false;
			}
		}else {
			return false;
		}
		return true;
	}
	public boolean userLogout() {
		boolean res = false;
		if(MsgService!=null){
			if(MsgService!=null){
				MsgService.setmMgrSignInExitFlag(true);
				MsgService.setmQueryOnlineExitStateFlag(true);
				MsgService.setmQueryAlarmSwitchsExitFlag(true);	
				mContext.unbindService(msgCbSc);
			}
			setMgrConnected(false);
			MsgService=null;
		}
		synchronized (mgrStateLock) {
			mgrStateLock.notifyAll();
		}
		unregisterReceivers();
		if (JniClient.PW_NET_Logout()) {
			m_currentUser = null;
			res = true;
		}
		return res;
	}

	public long getLastErrorCode() {
		return JniClient.PW_NET_GetLastError();
	}

	public boolean userRegister(IUserObject user, String vertifyCode,int userType) {
		LogUtils.e("signUp", "username "+user.getUserName()+" getPsw "
				+user.getPsw()+" vertifyCode "+vertifyCode);
		return JniClient
				.PW_NET_SignUp(new ClientRegister2Login(user.getUserName(),
						user.getPsw(), vertifyCode, "", userType));

	}

	public boolean modifyPsw(IUserObject user, String newPsw) {
		ResetPasswordReq req=new ResetPasswordReq();
		req.user_name=user.getUserName();
		req.oldpass=user.getPsw();
		req.newpass=newPsw;
		boolean bres= JniClient.PW_NET_ResetPassword(req);
		return bres;
	}

	public boolean resetPsw(IUserObject user, String vertifyCode, String newPsw) {

		return JniClient.PW_NET_ResetPasswordEx(new ResetPasswordByVerifyReq(
				user.getUserName(), vertifyCode, newPsw));
	}
	public long checkVerifyCode(String strUserName,String strVerifyCode) {

		Log.e("checkVerifyCode", strUserName+"  "+strVerifyCode);
		return JniClient.PW_NET_CheckVerifyCode(strUserName, strVerifyCode);
	}

	public long userExistQuery(String username) { // 0 not exist 1 exist -1
		// failure
		long ires=JniClient.PW_NET_VerityAccount(username);
		return ires;

	}

	public boolean getVerifyCode(IUserObject user) {
		boolean bres=JniClient.PW_NET_GetVerifyCode(user.getUserName());
		return bres;
	}

	public synchronized boolean init(Context appContext) {
		if (!m_bInit) {
			String libPath=appContext.getFilesDir().getParentFile().getAbsolutePath()+"/lib/";
			String content="magic_show.txt";
			LogUtils.i("init", libPath);
			LogUtils.i("init", libPath);
			LogUtils.i("init", content);
			JniClient.native_mpgl_init(libPath, libPath, content);
			JniClient.native_mpgl_setPpi(ScreenUtil.getScreenInfo(appContext).densityDpi);
			m_bInit =JniClient.PW_NET_Init();
			LogUtils.fi(LogUtils.LogAppFile,
					"PW_NET_Init " + m_bInit + " "
							+ JniClient.PW_NET_GetLastError());
		}
		if (m_bInit) {
			mContext = appContext;
			m_msgCbService = new Intent(mContext,
					PwMsgCallbackDealService.class);
			//	mContext.startService(m_msgCbService);
			SharedPreferences sp= mContext.getSharedPreferences(SafeConfigXml, Context.MODE_PRIVATE);
			String curLevelName = sp.getString(SafeCurLevelKey, "");
			if(!StringUtil.isNotEmpty(curLevelName)){
				Editor editor=sp.edit();
				editor.putString(SafeCurLevelKey, Safe.High_safe_level.safeName);
				editor.commit();
			}
		}
		return m_bInit;
	}

	public User getCurUser() {
		return m_currentUser;
	}
	private void queryAllCurUserDevicesStates(){
		if(m_currentUser!=null){
			if(MsgService!=null){
				MsgService.queryDevicesStates(m_currentUser.getDevices());
			}
		}
	}
	public void queryDevicesStates(List<Device> devList){
		if(m_currentUser!=null){
			if(MsgService!=null){
				MsgService.queryDevicesStates(devList);
			}
		}
	}

	private void checkInit() throws NotInitException {
		if (!m_bInit) {
			LogUtils.fi(LogUtils.LogAppFile, "Showmo System not init");
			throw new NotInitException("Showmo System not init");
		}
	}
	public  void sendMailInThread(Runnable run){
		new Thread(run).start();
	}

	private TestThread testthread=null;
	public void TEST_sendMsgAckInThread(int cameraId){
		// 1 msg received; 2 update packet downloaded; 4
		// update failed ; 8 update succeed
		stopTest_send();
		testthread=new TestThread(cameraId);
		testthread.start();
	}
	public void stopTest_send(){
		if(testthread!=null){
			if(testthread.isAlive()){
				testthread.setStop(true);
			}
		}
	}
	public class TestThread extends Thread{
		private int cameraId;
		private boolean exitFlag;
		public TestThread(int id){//20320
			cameraId=id;
		}
		public void setStop(boolean b){
			exitFlag=b;
		}
		public void run(){
			LogUtils.v("teset", "TestThread   1");
			int count=1;
			while(!exitFlag){
				PwMSGCB msgsb=MsgService.new PwMSGCB();
				Device_alarm_server_upload_msg alarmMsg=new Device_alarm_server_upload_msg();
				alarmMsg.recordId=10001;
				alarmMsg.cameraId=cameraId;
				alarmMsg.deviceId=cameraId;
				alarmMsg.channelNo=10002;
				alarmMsg.alarmType=CAMERA_ALARM_TYPE.ALARM_TYPE_DETECTION_MOTION;
				alarmMsg.beginTime=System.currentTimeMillis();
				alarmMsg.endTime=System.currentTimeMillis();
				alarmMsg.alarmMode=1;
				alarmMsg.alarmCode=count++;
				alarmMsg.Ccid=10004;

				msgsb.onMsgDataCallBack(alarmMsg , 20320);
				try {
					Thread.sleep(3000);
				} catch (Exception e) {
				}
			}
		}
	}

	public long getAlarmPicture(String path,Alarm alarm){
		long ires=JniClient.PW_NET_GetAlarmPic(path, alarm.getRecordId(), 
				alarm.getCameraId(), alarm.getAlarmType(), 
				alarm.getAlarmCode(), alarm.getCcid(), 
				(int)alarm.getBeginTime(), (int)alarm.getEndTime());

		LogUtils.v("getAlarmPic", "PW_NET_GetAlarmPic "+alarm.getCameraId()+"  "+ires);

		if(ires==-1 || ires==0 || ires == 2){
			return ires;
		}
		alarm.setmImgDownloading(true);
		IAlarmDao dao=null;
		try {
			dao=(IAlarmDao)DatabaseHelper.getHelper(mContext).getDao(Alarm.class);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		dao.updateInfo(alarm);
		return ires;
	}
	
	
	private void compareWithDao(String username,List<Device> devFromServer){
		IDeviceDao deviceDao=DaoFactory.getDeviceDao(mContext);
		List<Device> devFromDb = deviceDao.queryAllByUseOwner(username);
		LogUtils.fi(LogUtils.LogAppFile, "queryAllByUseOwner  from db over size:"+devFromDb.size()+"from server size:"+devFromServer.size());
		if (devFromDb != null) {
			for (int i = 0; i < devFromDb.size(); i++) {
				//LogUtils.e("device", "from db cameraId:"+devFromDb.get(i).getmCameraId()+" filename:"+devFromDb.get(i).getmTinyImgFilePath());
				for (int j = 0; j < devFromServer.size(); j++) {
					if (devFromDb.get(i).getmCameraId() ==devFromServer.get(j).getmCameraId() ) {
						devFromServer.set(j, devFromDb.get(i));
					}
				}
			}
			LogUtils.fi(LogUtils.LogAppFile, "compare loop over:");
			deviceDao.Remove(devFromDb);
		}
		LogUtils.fi(LogUtils.LogAppFile,"compare over");
		for (int j = 0; j < devFromServer.size(); j++) {
			deviceDao.insert(devFromServer.get(j));
		}
		LogUtils.fi(LogUtils.LogAppFile,"update database over");
	}
	private List<Device> getUserDevicesFromServer(String username){
		ArrayList<ClientDeviceQueryRet> lpDeviceList = null;
		lpDeviceList= JniClient.PW_NET_GetDeviceList();
		if(lpDeviceList!=null){
			List<Device> curDeviceList=new ArrayList<Device>();
			for (int i = 0; i < lpDeviceList.size(); i++) {
				ClientDeviceQueryRet tmp = lpDeviceList.get(i);
				Device device = new Device(username,
						(int) tmp.device_id, (int) tmp.device_id,
						tmp.device_sn, tmp.device_name, "", "");
				curDeviceList.add(0, device);
				//m_deviceList.add(device);
			}
			return curDeviceList;
		}
		return null;
	}
	private class NetWorkConnectivityReceiver extends BroadcastReceiver{
		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
			if(intent.getAction().equals(ConnectivityManager.CONNECTIVITY_ACTION)){
				if(PwNetWorkHelper.getInstance().getNetConnectState()){
					if (m_currentUser != null) {
						if (MsgService != null) {
							MsgService.connectMgrServer(ShowmoSystem.this);
						}
					}
				}
			}
		}
	}
	public void unregisterXgPush(){
		if(m_currentUser == null){
			return;
		}
		SharedPreferences sPreferences=mContext.getSharedPreferences(BaseActivity.SHAREDPERENCES_NAME, Context.MODE_PRIVATE);
		String regedAccount=sPreferences.getString(BaseActivity.SP_KEY_REG_ACCOUNT, "");
		if(!regedAccount.equals("")){
			sPreferences.edit().putString(BaseActivity.SP_KEY_REG_ACCOUNT, "").commit();
			XGPushManager.unregisterPush(mContext);
		}
	}
	public void registerCurUserXgPush(){
		if(m_currentUser == null){
			return;
		}
		SharedPreferences sPreferences=mContext.getSharedPreferences(BaseActivity.SHAREDPERENCES_NAME, Context.MODE_PRIVATE);
		XGPushManager.registerPush(mContext,m_currentUser.getUserName());
		sPreferences.edit().putString(BaseActivity.SP_KEY_REG_ACCOUNT, m_currentUser.getUserName()).commit();
	}
}
