package com.showmo.ormlite.dao.impl;

import java.sql.SQLException;
import java.util.List;

import android.util.Log;

import com.j256.ormlite.dao.BaseDaoImpl;
import com.j256.ormlite.support.ConnectionSource;
import com.j256.ormlite.table.DatabaseTableConfig;
import com.showmo.deviceManage.Device;
import com.showmo.ormlite.dao.ISafeDao;
import com.showmo.safe.Safe;
import com.showmo.util.StringUtil;

public class SafeDaoImpl extends BaseDaoImpl<Safe, String> implements ISafeDao{
	public SafeDaoImpl(Class<Safe> dataClass) throws SQLException {
		super(dataClass);

	}

	public SafeDaoImpl(ConnectionSource connectionSource,
			Class<Safe> dataClass) throws SQLException {
		super(connectionSource, dataClass);
	}

	public SafeDaoImpl(ConnectionSource connectionSource,
			DatabaseTableConfig<Safe> tableConfig) throws SQLException {
		super(connectionSource, tableConfig);
	}
	public List<Safe> queryAllSafeLevel(){
		List<Safe> res=null;
		try {
			res=queryForAll();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return res;
	}
	public Safe queryBySafeName(String safeName){
		if(!StringUtil.isNotEmpty(safeName)){
			return null;
		}
		Safe res=null;
		try {
			res=queryForId(safeName);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return res;
	}
	public boolean RemoveBySafeName(String safeName){
		if(!StringUtil.isNotEmpty(safeName)){
			return false;
		}
		try {
			this.deleteById(safeName);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return false;
		}
		return true;
	}
	public boolean Remove(List<Safe> data){
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
	public boolean insertSafe(Safe data){
		if(data==null){
			return false;
		}
		try {
			this.create(data);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return true;
	}
}
