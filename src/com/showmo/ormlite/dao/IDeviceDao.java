package com.showmo.ormlite.dao;

import java.util.List;

import com.showmo.deviceManage.Device;

import android.graphics.Camera;


public interface IDeviceDao {
	public int insert(Device dev);
	public List<Device> queryAllByUseOwner(String userName);
	public Device queryByCameraId(String UniqueId);
	public boolean RemoveByUniqueId(String UniqueId);
	public boolean Remove(List<Device> data);
	public int updateDevice(Device dev);
}
