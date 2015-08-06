package com.showmo.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import android.text.format.Time;
import android.util.Log;

public class LogUtils {
	public static final String TagFile="yxj";
	public static final String LogSdkFile=PathUtils.getDataDiretory()+"/showmoSdkLog.txt";
	public static final String LogAppFile=PathUtils.getDataDiretory()+"/showmoAppLog.txt";
	public static void v(String tag,String msg){
		Log.v(tag, msg);
	}
	public  static void w(String tag,String msg){
		Log.w(tag, msg);
	}
	public  static void e(String tag,String msg){
		Log.e(tag, msg);
	}
	public static  void i(String tag,String msg){
		Log.i(tag, msg);
	}
	public  static void d(String tag,String msg){
		Log.d(tag, msg);

	}
	public static void fi(String filename,String msg)//记录到文件并打印
	{
		printToFile(filename,":\t->"+msg+"\n");
		LogUtils.i(TagFile, msg);
	}
	public static void fe(String filename,String msg)//记录到文件并打印
	{
		printToFile(filename,":\t->"+msg+"\n");
		LogUtils.e(TagFile, msg);
	}
	public synchronized static void printToFile(String filename,String str){
		String logfile= filename;
		File file=new File(logfile);
		try {
			if(!file.exists()){
				file.createNewFile();
			}else{
				FileInputStream inputStream=new FileInputStream(file);
				if(inputStream.available() >=1000*1024){
					file.delete();
					file=new File(logfile);
					file.createNewFile();
				}
			}
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		FileOutputStream oStream=null;
		try {
			oStream=new FileOutputStream(file,true);
			Time time=new Time();
			time.setToNow();
			String logString="\t"+time.toMillis(false)+str;
			oStream.write(logString.getBytes());
			oStream.close();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
	 public static String getLineInfo() {
		 StackTraceElement[] steArray = Thread.currentThread().getStackTrace();
         StackTraceElement ste = new Throwable().getStackTrace()[0];
         return (steArray[steArray.length-1].getFileName() + ": Line " + steArray[steArray.length-1].getLineNumber()+" ::");
     }
}
