package com.showmo.util;

import android.annotation.SuppressLint;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

@SuppressLint("SimpleDateFormat")
public class TimeUtil {
	
	public  static final String  DATE_FORMAT_DEF="yyyy/MM/dd HH:mm:ss";
	
	public static SimpleDateFormat mSimpleDateFormat ;
	
	public static  Date parse( String strDate  ){
		if(mSimpleDateFormat == null){
			mSimpleDateFormat = new SimpleDateFormat(DATE_FORMAT_DEF);
		}
		
		try {
			return mSimpleDateFormat.parse(strDate);
		} catch (ParseException e) {
			e.printStackTrace();
			return null ;
		}
		
	}
	
	public static  String format( long seconds  ){
		if(mSimpleDateFormat == null){
			mSimpleDateFormat = new SimpleDateFormat(DATE_FORMAT_DEF);
		}
		
		Calendar canl=Calendar.getInstance();
		int offsetTime=canl.get(Calendar.ZONE_OFFSET)+canl.get(Calendar.DST_OFFSET);

		Date date=new Date(seconds*1000-offsetTime);
		return mSimpleDateFormat.format(date);
	}
	
	
}
