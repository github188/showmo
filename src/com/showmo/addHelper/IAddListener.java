package com.showmo.addHelper;

import com.showmo.deviceManage.Device;

public interface IAddListener {
	public void addedByOther(String uuid,String user);
	public void addedSuccess(Device dev);
}
