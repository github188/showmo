package com.showmo.ormlite.dao;

import java.util.List;

import com.showmo.alarmManage.Alarm;

import android.R.interpolator;


public interface IAlarmDao {
	public int insert(Alarm info);
	public List<Alarm> queryAllByDeviceId(int deviceId);
	public List<Alarm> queryLimitByTime(int deviceId,long beginTime,long endTime,int limit);//返回begintime到endtime之间最新的limit条信息
	
	public List<Alarm> queryLastestItems(int deviceId,int limit);//返回数据表里面最新的limit条
	public List<Alarm> queryLastestItems(int deviceId,long beginTime, int limit);//返回数据表里面beginTime之前的limit条
	
	public Alarm queryByRecordId(int id);
	public int RemoveByRecordId(int id);
	public int Remove(List<Alarm> data);
	public int RemoveByDeviceId(int deviceId);
	public int updateInfo(Alarm info);
}
