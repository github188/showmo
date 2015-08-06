package com.showmo.deviceManage;

import java.util.ArrayList;
import java.util.List;

import rx.Observable;
import rx.Subscriber;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Func1;
import rx.schedulers.Schedulers;

import com.showmo.alarmManage.Alarm;
import com.showmo.base.ShowmoApplication;
import com.showmo.deviceManage.Device.AlarmSwitch;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.rxErr.NetErrInfo;
import com.showmo.rxcallback.RxCallback;
import com.showmo.util.LogUtils;
import com.showmo.util.ThreadUtils;

import android.R.integer;
import android.os.AsyncTask;
import android.os.SystemClock;
import android.text.format.Time;
import android.util.Log;
import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.CAMERA_ALARM_TYPE;
import ipc365.app.showmo.jni.JniDataDef.LightCtrl;
import ipc365.app.showmo.jni.JniDataDef.Remote_File_Type;
import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;
import ipc365.app.showmo.jni.JniDataDef.SDK_SEARCH;

public class DeviceUseUtils {
	private Device mDevice;
	private IDeviceDao m_dao;
	public DeviceUseUtils(Device dev) {
		mDevice = dev;
		try {
			m_dao=(IDeviceDao)DatabaseHelper.getHelper(ShowmoApplication.getInstance()).getDao(Device.class);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		
	}

	public Device getmDevice() {
		return mDevice;
	}

	public void setmDevice(Device mDevice) {
		this.mDevice = mDevice;
	}

	public boolean upgrade() {
		boolean bres=JniClient.PW_NET_Upgrade(mDevice.getmCameraId(),
				mDevice.getmVersion());
		//LogUtils.e("upgrade", "PW_NET_Upgrade "+mDevice.getmCameraId()+" "+mDevice.getmVersion()+" "+bres);
		if(bres){
			mDevice.setmUpgrading(true);
			IDeviceDao deviceDao=DaoFactory.getDeviceDao(ShowmoApplication.getInstance());
			deviceDao.updateDevice(mDevice);
//			List<Device> devices= deviceDao.queryAllByUseOwner("18257168402");
//			for (int i = 0; i < devices.size(); i++) {
//				LogUtils.e("upgrade", "upgrade "+devices.get(i).ismUpgrading()+" "+devices.get(i).getmUuid());
//			}
			
		}
		
		return bres;

	}



	public boolean brightCtrl(int brightValue) {
		LogUtils.e("light", "bright ctrl before "+brightValue);
		boolean bres=JniClient.PW_NET_BrightCtrl(mDevice.getmCameraId(), brightValue,LightCtrl.set);
		LogUtils.e("light", "bright ctrl after res:"+bres+" errcode"+JniClient.PW_NET_GetLastError());
		return bres;
	}

	public boolean CheckOnlineState() {// 开启在线状态推送
		long timeBf=SystemClock.elapsedRealtime();
		boolean bret = JniClient.PW_NET_OnLineState(mDevice.getmCameraId());
		LogUtils.e("online", "PW_NET_OnLineState " + mDevice.getmCameraId() + " PW_NET_OnLineState ret "
				+ bret+"  api use time----> "+(SystemClock.elapsedRealtime()-timeBf)+"ms");
		return bret;
	}

	public boolean CheckAlarmSwitchState() {
		
		int[] alarmArr={CAMERA_ALARM_TYPE.ALARM_TYPE_DETECTION_MOTION};
		List<AlarmSwitch> alarmList=new ArrayList<Device.AlarmSwitch>();
		for(int i=0;i<alarmArr.length;i++){
			int iret = (int) JniClient.PW_NET_GetAlarmState(mDevice.getmCameraId(),alarmArr[i]);
			LogUtils.e("alarm", " Device "+mDevice.getmDeviceName()+" alarm switch iret:"+iret);
			if (iret == -1) {
				mDevice.setmSwitchStateValid(false);
				return false;
			} else {
				boolean value=false;
				if(iret==0){
					value=false;
				}else{
					value=true;
				}
				Device.AlarmSwitch switchalarm=new Device.AlarmSwitch(alarmArr[i],value);
				alarmList.add(switchalarm);
			} 
		}
		mDevice.setAlarmSwitch(alarmList);
		mDevice.setmSwitchStateValid(true);
		return true;
		
	}

	public boolean SetAlarmSwitchState(int cameraAlarmType, boolean state) {
		boolean bret = JniClient.PW_NET_SetAlarmState(mDevice.getmCameraId(),
				cameraAlarmType, state);
		LogUtils.e("checkalarm", "SetAlarmSwitchState "+bret+"  "+state);
		if (!bret) {
			return false;
		} else {
			List<AlarmSwitch> switchs = mDevice.getmAlarmSwitchs();
			for (int i = 0; i < switchs.size(); i++) {
				if(switchs.get(i).cameraAlarmType==cameraAlarmType){
					switchs.get(i).value=state;
				}
			}
		}
		return true;
	}

	public List<SDK_REMOTE_FILE> getRemoteFiles(Time beginTime, Time endTime) {
		LogUtils.v("timeline", beginTime.format2445() + " " + endTime.format2445());
		SDK_SEARCH searchInfo = new SDK_SEARCH();
		searchInfo.nFileType = Remote_File_Type.SDK_RECORD_ALL;
		searchInfo.startTime = beginTime;
		searchInfo.endTime = endTime;
		return JniClient.PW_NET_SearchRemoteFile(mDevice.getmCameraId(),
				searchInfo);
	}
	

	public void deviceRename(final String newName,final RxCallback<Boolean> callback){
		
		Observable.create(new Observable.OnSubscribe<Boolean>() {
			@Override
			public void call(Subscriber<? super Boolean> t) {
				// TODO Auto-generated method stub
				boolean bres=JniClient.PW_NET_ModifyDevName(mDevice.getmCameraId(), newName);
				LogUtils.e("rx", "call api thread == main? "+ThreadUtils.isMainThread());
				if(!bres){
					t.onError(new NetErrInfo(JniClient.PW_NET_GetLastError()));
				}else{
					t.onNext(Boolean.valueOf(bres));
				}
				t.onCompleted();
			}
		})
		.subscribeOn(Schedulers.io())
		.map(new Func1<Boolean, Boolean>() {
			@Override
			public Boolean call(Boolean t) {
				// TODO Auto-generated method stub
				LogUtils.e("rx", "rename map thread == main? "+ThreadUtils.isMainThread());
				mDevice.setmDeviceName(newName);
				m_dao.updateDevice(mDevice);
				return Boolean.TRUE;
			}
		}).observeOn(AndroidSchedulers.mainThread())
		.subscribe(callback);
	}
	
}
