package com.showmo.ormlite.dao;

import android.accounts.Account;
import android.content.Context;

import com.j256.ormlite.dao.Dao;
import com.showmo.alarmManage.Alarm;
import com.showmo.deviceManage.Device;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.model.ShowmoAccount;
import com.showmo.safe.Safe;

public class DaoFactory {
	public static IAlarmDao getAlarmDao(Context context){
		return (IAlarmDao)getDao(context, Alarm.class);
	}
	public static  IDeviceDao getDeviceDao(Context context){
		return (IDeviceDao)getDao(context, Device.class);
	}
	public  static ISafeDao getSafeDao(Context context){
		return (ISafeDao)getDao(context, Safe.class);
	}
	public static  AccountDao getUserDao(Context context){
		return (AccountDao)getDao(context, ShowmoAccount.class);
	}
	private static Dao getDao(Context context,Class clazz){
		Dao dao=null;
		try {
			dao=DatabaseHelper.getHelper(context).getDao(clazz);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return dao;
	}
}
