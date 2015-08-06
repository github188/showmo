package com.showmo.util;

import java.io.File;

import com.showmo.base.ShowmoApplication;

import android.os.Environment;


public class PathUtils {
	private static String mExternalDirectory=null;
	private static String mDataDirectory=null;
	private static String mCacheDirectory=null;
	private static String mExternalCacheDirectory=null;
	private static String mThumbnailDirectory=null;
	public static String getExternalDiretory(){
		if(mExternalDirectory==null){
			if(Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())){
				mExternalDirectory=Environment.getExternalStorageDirectory().getAbsolutePath();
			}else {
				mExternalDirectory=null;
			}
		}
		return mExternalDirectory;
	}
	public static String getDataDiretory(){
		if(mDataDirectory==null){
			mDataDirectory=ShowmoApplication.getInstance().getFilesDir().getAbsolutePath();
		}
		return mDataDirectory;
	}
	public static String getCacheDiretory(){
		if(mCacheDirectory==null){
			mCacheDirectory=ShowmoApplication.getInstance().getCacheDir().getAbsolutePath();
		}
		return mCacheDirectory;
	}
	public static String getExternalCacheDiretory(){
		if(mExternalCacheDirectory==null){
			mExternalCacheDirectory=ShowmoApplication.getInstance().getExternalCacheDir().getAbsolutePath();
		}
		return mExternalCacheDirectory;
	}
	public static boolean createFileIfNotExist(String path){
		File file=new File(path);
		if(!file.exists()){
			if(!file.mkdirs()){
				LogUtils.e("io", "create file failured:"+path);
			}
		}
		return true;
	}
	public static String getThumbnailDataPath(){
		if(mThumbnailDirectory==null){
			mThumbnailDirectory=getDataDiretory()+"/thumnail";
			createFileIfNotExist(mThumbnailDirectory);
		}
		return mThumbnailDirectory;
	}
}
