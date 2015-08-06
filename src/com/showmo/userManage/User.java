package com.showmo.userManage;

import com.showmo.MainActivity;
import com.showmo.R;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.ClientDeviceAddReq;
import ipc365.app.showmo.jni.JniDataDef.ClientDeviceAddRet;
import ipc365.app.showmo.jni.JniDataDef.ClientDeviceQueryRet;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

import com.showmo.base.ShowmoApplication;
import com.showmo.commonAdapter.PWDevAdapter;
import com.showmo.deviceManage.Device;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.util.LogUtils;

import android.R.integer;
import android.util.Log;

public class User implements IUserBehavior, IUserObject, Serializable {
	public final static int USER_TYPE_PHONE = 0;
	public final static int USER_TYPE_EMAIL = 1;
	public final static int USER_NOT_EXIST = 0;
	public final static int USER_EXIST = 1;
	public final static int ACTION_VERI_REGISTER = 0;
	public final static int ACTION_VERI_RESET_PSW = 1;

	public static final String DEVICE_ADD_ACTION = "ipc365.app.shomo.DEVICE_ADD_ACTION";
	public static final String DEVICE_REMOVE_ACTION = "ipc365.app.shomo.DEVICE_REMOVE_ACTION";

	private String m_username;
	private String m_psw;
	private String m_verifyCode;
	private List<Device> m_deviceList;
	private int m_userType; // 0 phoneNumber 1 email
	private int actionForVeri; // 0 register 1 reset psw

	private boolean mExprience=false;
	
	private IDeviceDao m_DeviceDao = null;
	private DatabaseHelper m_dbHelper = null;
	
	public User(){
		
	}
	public User(List<Device> m_deviceList){
		this.m_deviceList = m_deviceList;
	}
	public User(int actionForVeri){
		this.actionForVeri = actionForVeri;
	}
	public User(String username, String verifyCode,int actionForVeri){
		m_username = username;
		m_verifyCode = verifyCode;
		this.actionForVeri = actionForVeri;
	}
	public User(String username, /*String psw,*/int actionForVeri){
		m_username = username;
		/*m_psw = psw;*/
		this.actionForVeri = actionForVeri;
	}
	public User(String username, String psw, boolean isExperience,List<Device> list) {
		// TODO Auto-generated constructor stub
		m_username = username;
		m_psw = psw;
		m_DeviceDao=DaoFactory.getDeviceDao(ShowmoApplication.getInstance());
		m_deviceList=list;
		mExprience = isExperience;
	}

	public User(String username, String psw, boolean isExperience, int userType,
			int actionForVeri) {
		m_username = username;
		m_psw = psw;
		m_deviceList = new ArrayList<Device>();
		m_DeviceDao=DaoFactory.getDeviceDao(ShowmoApplication.getInstance());
		mExprience = isExperience;
		m_userType = userType;
		this.actionForVeri = actionForVeri;
	}

	public boolean isExperience() {
		return mExprience;
	}

	@Override
	public String getUserName() {
		// TODO Auto-generated method stub
		return m_username;
	}

	@Override
	public void setUserName(String user) {
		// TODO Auto-generated method stub
		m_username = user;
	}
	@Override
	public void sortDevice(){
		if(m_deviceList == null){
			return;
		}
		if(m_deviceList.size() == 0){
			return;
		}
		Collections.sort(m_deviceList);
	}
	@Override
	public String getPsw() {
		// TODO Auto-generated method stub
		return m_psw;
	}

	@Override
	public void setPsw(String psw) {
		// TODO Auto-generated method stub
		m_psw = psw;
	}


	@Override
	public synchronized void setDevices(List<Device> devices){
		m_deviceList=devices;
//		List<Device> devFromDb = m_DeviceDao.queryAllByUseOwner(this
//				.getUserName());
//		if (devFromDb != null) {
//			for (int i = 0; i < devFromDb.size(); i++) {
//				//LogUtils.e("device", "from db cameraId:"+devFromDb.get(i).getmCameraId()+" filename:"+devFromDb.get(i).getmTinyImgFilePath());
//				for (int j = 0; j < m_deviceList.size(); j++) {
//					if (devFromDb.get(i).getmCameraId() ==m_deviceList.get(j).getmCameraId() ) {
//						String imgPh = devFromDb.get(i).getmTinyImgFilePath();
//						m_deviceList.get(j).setmTinyImgFilePath(imgPh);
//					}
//				}
//			}
//			m_DeviceDao.Remove(devFromDb);
//		}
//		for (int j = 0; j < m_deviceList.size(); j++) {
//			m_DeviceDao.insert(m_deviceList.get(j));
//		}
	}

	@Override
	public synchronized List<Device> getDevices() {// return null while
		// failure
		return m_deviceList;
	}

	@Override
	public synchronized Device bindDevice(String devName, String uuid,int device) {
		// TODO Auto-generated method stub
		ClientDeviceAddReq in_lpAddDevice = new ClientDeviceAddReq();
		in_lpAddDevice.device_name = devName;
		in_lpAddDevice.custom_id = 0;
		in_lpAddDevice.device_passwd = "123456";
		in_lpAddDevice.device_sn = uuid;
		Device dev=null;
		ClientDeviceAddRet out_lpDevice = null;

		//long bindState=JniClient.PW_NET_BindState(uuid);
		out_lpDevice = JniClient.PW_NET_AddDevice(in_lpAddDevice);
		LogUtils.i("addDevice", "PW_NET_AddDevice uuid: "+uuid+" result: "+(out_lpDevice == null ?false:true)
				+" errcode: "+JniClient.PW_NET_GetLastError());
		if (out_lpDevice == null) {
			//LogUtils.v("add", "PW_NET_AddDevice err:"+JniClient.PW_NET_GetLastError());
			return null;
		}// String user,int cameraId,int deviceId,String uuid,String
		// deviceName,String imgFilename,String version
		dev = new Device(m_username, (int) out_lpDevice.camera_id,
				(int) out_lpDevice.device_id, uuid, devName, "", "");
		for (Device ddd:m_deviceList) {
			if(ddd.getmCameraId() == dev.getmCameraId()){
				return dev;
			}
		}
		LogUtils.v("bind", "dev "+dev);
		IDeviceDao dao=DaoFactory.getDeviceDao(ShowmoApplication.getInstance());
		if(dao!=null){
			dao.insert(dev);
		}else{
			LogUtils.e("bind", "IDeviceDao dao=null");
		}
		m_deviceList.add(0, dev);		
		return dev;
	}
	public void addDevice(Device dev){
		int iret = m_DeviceDao.insert(dev);
		m_deviceList.add(0, dev);
	}
	
	/*public synchronized List<Device> bindDevice(List<String> uuidList) {
		// TODO Auto-generated method stub
		List<Device> devList=new ArrayList<Device>();
		for (int i = 0; i < uuidList.size(); i++) {
			long ires=JniClient.PW_NET_BindState(uuidList.get(i).toUpperCase());
			if(ires==2){//被其他用户绑定
				//continue;
			}else if (ires==1) {//已被该用户绑定
				continue;
			}else if (ires==-1) {//失败
				continue;
			}


			Device dev=bindDevice("小末摄像机", uuidList.get(i).toUpperCase());

			if(dev!=null){
				devList.add(dev);
			}
		}
		m_deviceList.addAll(devList);
		return devList;
	}*/
	/*public updatedeviceList(){
		
	}*/
	@Override
	public synchronized boolean unbindDevice(String UniqueId){
		// TODO Auto-generated method stub
				Device device=m_DeviceDao.queryByCameraId(UniqueId);
				if (device!=null) {
					boolean bret = JniClient.PW_NET_DeleteDevice(device.getmUuid());
					//LogUtils.e("unbind", "111JniClient.PW_NET_DeleteDevice "+bret+" "+JniClient.PW_NET_GetLastError());
					if (!bret) {
						return false;
					}
					bret = m_DeviceDao.RemoveByUniqueId(device.getUniqueId());
					//LogUtils.e("unbind", "111m_DeviceDao.RemoveByUniqueId "+bret);
					if (!bret) {
						return false;
					}
					for (int i = 0; i < m_deviceList.size(); i++) {
						Device device2=m_deviceList.get(i);
						if (device2.getUniqueId().equals(UniqueId)) {
							//Log.e("11111", "111"+UniqueId+"   ");
							m_deviceList.remove(device2);
						}
					}
					
					//Log.e("111111", "11112222"+m_deviceList.size());
					return true;
				}else {
					Log.e("111", "11113333444");
					return false;
				}
				
	}
	
	@Override
	public synchronized boolean unbindDevice(Device dev) {
		// TODO Auto-generated method stub
		boolean bret = JniClient.PW_NET_DeleteDevice(dev.getmUuid());
		LogUtils.e("unbind", "JniClient.PW_NET_DeleteDevice "+bret+" "+JniClient.PW_NET_GetLastError());
		if (!bret) {
			return false;
		}
		bret = m_DeviceDao.RemoveByUniqueId(dev.getUniqueId());
		LogUtils.e("unbind", "m_DeviceDao.RemoveByUniqueId "+bret);
		if (!bret) {
			return false;
		}
		m_deviceList.remove(dev);
		return true;
	}

	@Override
	public int getUserType() {
		return m_userType;
	}

	@Override
	public void setUserType(int userType) {
		m_userType = userType;
	}

	@Override
	public int getActionForVeri() {
		return actionForVeri;
	}

	@Override
	public void setActionForVeri(int actionForVeri) {
		this.actionForVeri = actionForVeri;
	}

	@Override
	public String getverifyCode() {
		// TODO Auto-generated method stub
		return m_verifyCode;
	}

	@Override
	public void setverifyCode(String verifyCode) {
		// TODO Auto-generated method stub
		this.m_verifyCode=verifyCode;
	}
}
