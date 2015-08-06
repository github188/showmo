package com.showmo.ormlite.dao.impl;

import java.sql.SQLException;
import java.util.List;

import android.R.integer;
import android.util.Log;

import com.j256.ormlite.dao.BaseDaoImpl;
import com.j256.ormlite.stmt.PreparedDelete;
import com.j256.ormlite.stmt.Where;
import com.j256.ormlite.support.ConnectionSource;
import com.j256.ormlite.table.DatabaseTableConfig;
import com.showmo.deviceManage.Device;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.ormlite.model.ShowmoAccount;
import com.showmo.util.LogUtils;
import com.showmo.util.StringUtil;

public class DeviceDaoImpl extends BaseDaoImpl<Device, String> implements IDeviceDao{

	public DeviceDaoImpl(Class<Device> dataClass) throws SQLException {
		super(dataClass);

	}

	public DeviceDaoImpl(ConnectionSource connectionSource,
			Class<Device> dataClass) throws SQLException {
		super(connectionSource, dataClass);
	}

	public DeviceDaoImpl(ConnectionSource connectionSource,
			DatabaseTableConfig<Device> tableConfig) throws SQLException {
		super(connectionSource, tableConfig);
	}

	@Override
	public boolean Remove(List<Device> data) {
		// TODO Auto-generated method stub
		if(data==null){
			return false;
		}
		try {
			this.delete(data);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return false;
		}
		return true;
	
	}
	@Override
	public int updateDevice(Device dev){
		try {
			//LogUtils.i("dao", dev.toString());
			return this.update(dev);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return 0;
		}
	}
	@Override
	public boolean RemoveByUniqueId(String UniqueId){
		// TODO Auto-generated method stub
		if(!StringUtil.isNotEmpty(UniqueId)){
			return false;
		}
		try {
			this.deleteById(UniqueId);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return false;
		}
		return true;
	}

	@Override
	public int insert(Device dev)  {
		if(dev == null){
			return 0 ;
		}
		try {
			//LogUtils.i("DeviceDao", "insert-->"+dev.toString());
			return create(dev);
 
		} catch (SQLException e) {
			e.printStackTrace();
			return 0 ;
		}
	}
	public List<Device> queryAllByUseOwner(String userName){
		if(!StringUtil.isNotEmpty(userName)){
			return null;
		}
		List<Device> res=null;
		try {
			res=queryForEq("mUseOwner",userName );
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		if(res!=null){
			for (int i = 0; i < res.size(); i++) {
				Log.v("devicedao", res.get(i).toString());
			}
		}
		return res;
	}
	public Device queryByCameraId(String UniqueId){
		Device res=null;
		try {
			res=queryForId(UniqueId);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return res;
	}
}
