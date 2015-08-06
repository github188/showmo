package com.showmo.userManage;

import java.util.ArrayList;
import java.util.List;

import com.showmo.deviceManage.Device;

import android.R.integer;
public interface IUserObject {
	public String getUserName();
	public void setUserName(String user);
	public String getPsw();
	public void setPsw(String psw);
	public String getverifyCode();
	public void setverifyCode(String verifyCode);
	public boolean isExperience();//showmo ipc365
	List<Device> getDevices();
	public void setDevices(List<Device> devices);
	public int getUserType();
	public void setUserType(int userType); // 0 phoneNumber 1 email  2 quickAccount
	public int getActionForVeri(); // 0 register 1 reset psw
	public void setActionForVeri(int actionForVeri);
	
}
