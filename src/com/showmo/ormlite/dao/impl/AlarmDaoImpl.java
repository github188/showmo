package com.showmo.ormlite.dao.impl;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.sql.SQLException;
import java.util.List;

import android.util.Log;

import com.j256.ormlite.dao.BaseDaoImpl;
import com.j256.ormlite.dao.GenericRawResults;
import com.j256.ormlite.dao.RawRowMapper;
import com.j256.ormlite.stmt.Where;
import com.j256.ormlite.support.ConnectionSource;
import com.j256.ormlite.table.DatabaseTableConfig;
import com.showmo.alarmManage.Alarm;
import com.showmo.deviceManage.Device;
import com.showmo.ormlite.dao.IAlarmDao;

public class AlarmDaoImpl extends BaseDaoImpl<Alarm, Integer> implements IAlarmDao{
	public AlarmDaoImpl(Class<Alarm> dataClass) throws SQLException {
		super(dataClass);
	}

	public AlarmDaoImpl(ConnectionSource connectionSource,
		Class<Alarm> dataClass) throws SQLException {
		super(connectionSource, dataClass);
	}
	
	public AlarmDaoImpl(ConnectionSource connectionSource,
		DatabaseTableConfig<Alarm> tableConfig) throws SQLException {
		super(connectionSource, tableConfig);
	}
	@Override
	public int insert(Alarm info){
		if(info==null){
			return 0;
		}
		try {
			return create(info);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return 0;
		}
		
	}
	@Override
	public int updateInfo(Alarm info) {
		// TODO Auto-generated method stub
		if(info==null){
			return 0;
		}
		try {
			return update(info);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return 0;
		}
	}
	@Override
	public List<Alarm> queryLastestItems(int deviceId, int limit) {
		// TODO Auto-generated method stub
		try {
			String rawSql="select * from tb_alarms where deviceId = "+deviceId
					+" order by endTime Desc limit "+limit;
			Log.v("sql", rawSql);
			
			GenericRawResults<Alarm> RawResults=queryRaw(rawSql,new PwRawRowMapper());
			return RawResults.getResults();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return null;
	}
	@Override
	public List<Alarm> queryLastestItems(int deviceId,long beginTime, int limit){
		try {
			String rawSql="select * from tb_alarms where deviceId = "+deviceId
					+" and beginTime < "+beginTime
					+" order by endTime Desc limit "+limit;
			Log.v("sql", rawSql);
			
			GenericRawResults<Alarm> RawResults=queryRaw(rawSql,new PwRawRowMapper());
			return RawResults.getResults();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return null;
	}
	
	@Override
	public List<Alarm> queryLimitByTime(int deviceId,long beginTime,long endTime,int limit){
		try {
			String rawSql="select * from tb_alarms where deviceId = "+deviceId+
					" and beginTime >= "+beginTime+" and endTime <= "+endTime
					+" order by endTime Desc limit "+limit;
			Log.v("sql", rawSql);
			
			GenericRawResults<Alarm> RawResults=queryRaw(rawSql,new PwRawRowMapper());
			return RawResults.getResults();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		
		
		return null;
	}
	
	public class PwRawRowMapper implements RawRowMapper<Alarm>{
		public Alarm mapRow(String[] columnNames, String[] resultColumns){
			
			Alarm alarm=new Alarm();
			Class<Alarm> clazz=Alarm.class;
			for (int i = 0; i < columnNames.length; i++) {
				Field fid=null;
				try {
					fid=clazz.getDeclaredField(columnNames[i]);
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
					return null;
				}
				fid.setAccessible(true);
				String ftype=fid.getGenericType().toString();
				if(ftype.equals("int")){
					try {
						fid.setInt(alarm, Integer.valueOf(resultColumns[i]).intValue());
					}catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
						return null;
					}
				}
				else if(ftype.equals("long")){
					try {
						fid.setLong(alarm, Long.valueOf(resultColumns[i]).longValue());
					}catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
						return null;
					}
				}else if (ftype.equals("boolean")) {
					try {
						fid.setBoolean(alarm, Boolean.valueOf(resultColumns[i]).booleanValue());
					}catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
						return null;
					}
				}
				else {
					try {
						fid.set(alarm, resultColumns[i]);
					}catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
						return null;
					}
				}
			}
			
			
			return alarm;
		}
	}
	
	@Override
	public List<Alarm> queryAllByDeviceId(int deviceId){
		
		try {
			return queryForEq("deviceId", deviceId);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return null;
		}
	}
	@Override
	public Alarm queryByRecordId(int id){
		try {
			return queryForId(id);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return null;
		}
		
	}
	@Override
	public int RemoveByRecordId(int id){
		try {
			return deleteById(id);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return 0;
		}
		
	}
	@Override
	public int Remove(List<Alarm> data){
		
		try {
			return this.delete(data);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return 0;
		}
	}
	@Override
	public int RemoveByDeviceId(int deviceId){
		List<Alarm> alarmlistAlarms=queryAllByDeviceId(deviceId);
		int deletecount=0;
		try {
			deletecount=delete(alarmlistAlarms);
		} catch (Exception e) {
			e.printStackTrace();
			// TODO: handle exception
		}
		return deletecount;
	}
}
