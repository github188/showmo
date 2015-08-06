package com.showmo.addHelper;

import com.showmo.deviceManage.Device;
import com.showmo.util.LogUtils;

import ipc365.app.showmo.jni.JniClient;

public class AddProcess extends AbstractAddProcess implements IAddProcess{

	@Override
	protected boolean onDevStateFilter(DEV_BIND_STATE state) {
		if((state == DEV_BIND_STATE.DEV_STATE_BIND_NOBODY) || 
				(state == DEV_BIND_STATE.DEV_STATE_BIND_BYOTHER)){
			return true;
		}
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	protected Device performBindBySelfDev(DevSimpleInfo sdevinfo) {
		// TODO Auto-generated method stub
		Device newDevice=null;
		for(Device devcur:mCurUser.getDevices()){
			if(devcur.getmDeviceId() == sdevinfo.deviceid){
				LogUtils.e("adddevice", "performBindBySelfDev deviceid:"+devcur.getmDeviceId());
				newDevice=devcur;
				break;
			}
		}
		if(newDevice==null){
			LogUtils.e("adddevice", "device is bind by self but not in curUser list:"+sdevinfo.deviceid);
			newDevice =new Device(mCurUser.getUserName(), 
					sdevinfo.deviceid, sdevinfo.deviceid, sdevinfo.uuid, sdevinfo.uuid, 
					"", "");
			mCurUser.addDevice(newDevice);
		}
		return newDevice;
	}

	@Override
	protected Device performBindByOtherDev(DevSimpleInfo sdevinfo) {
		// TODO Auto-generated method stub
		LogUtils.v("adddevice", "performBindByOtherDev uuid:"+sdevinfo.uuid);
		if(mAddListener!=null){
			mAddListener.addedByOther(sdevinfo.uuid,obtainOtherBindUser(sdevinfo.uuid));
		}
		return null;
	}

	@Override
	protected Device performBindByNoBodyDev(DevSimpleInfo sdevinfo) {
		// TODO Auto-generated method stub
		LogUtils.v("adddevice", "performBindByNoBodyDev uuid:"+sdevinfo.uuid);
		Device dev= mCurUser.bindDevice(sdevinfo.uuid, sdevinfo.uuid,sdevinfo.deviceid);
		if(dev!=null){//添加成功
			LogUtils.e("bind","UUID:"+sdevinfo.uuid+" bindDevice succuess uuid: "+sdevinfo.uuid);
			if(mAddListener!=null){
				mAddListener.addedSuccess(dev);
			}
			JniClient.PW_NET_OnLineState(sdevinfo.deviceid);
			//PostMsgToMainHandler(HANDLE_DEV_COMP,dev);
		}else{//添加失败
			LogUtils.e("bind","UUID:"+sdevinfo.uuid+" bindDevice err "+JniClient.PW_NET_GetLastError());
		}
		return dev;
	}
	private IAddListener mAddListener=null;
	@Override
	public void setAddListener(IAddListener lis) {
		// TODO Auto-generated method stub
		mAddListener = lis;
	}

	@Override
	public IAddListener getAddListener() {
		// TODO Auto-generated method stub
		return mAddListener;
	}
	
}
