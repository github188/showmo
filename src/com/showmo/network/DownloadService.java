package com.showmo.network;

import java.io.File;

import com.showmo.activity.more.ActivityAppUpdate;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class DownloadService extends Service {
	public static final int Flag_Init = 0; 
	public static final int Flag_Down = 1; 
	public static final int Flag_Pause = 2;
	public static final int Flag_Done = 3; 

	String url;
	

	private int progress = 0;

	public int getProgress() {
		return progress;
	}


	private int flag;

	public int getFlag() {
		return flag;
	}

	DownThread mThread;
	Downloader downloader;

	private static DownloadService instance;

	public static DownloadService getInstance() {
		return instance;
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	@Override
	public void onCreate() {

		instance = this;
		flag = Flag_Init;
		super.onCreate();
	}

	@Override
	public void onStart(Intent intent, int startId) {
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		String msg = intent.getExtras().getString("flag");
		url = intent.getExtras().getString("url");
		if (mThread == null){
			mThread = new DownThread();
		}
		if(downloader == null)
		{
			downloader = new Downloader(this, url, "showmo.apk");
		}
		downloader.setDownloadListener(downListener);
		
		if (msg.equals("start"))
		{
			startDownload();
		} 
		else if (msg.equals("pause")) 
		{
			downloader.pause();
		}
		else if (msg.equals("resume")) 
		{
			downloader.resume();
		} 
		else if (msg.equals("stop"))
		{
			downloader.cancel();
			stopSelf();
		}

		return super.onStartCommand(intent, flags, startId);
	}

	private void startDownload() {
		if (flag == Flag_Init || flag == Flag_Pause) {
			if (mThread != null && !mThread.isAlive()) {
				mThread = new DownThread();
			}
			mThread.start();

		}
	}

	@Override
	public void onDestroy() {
		Log.e("jlf", "service...........onDestroy");
		try {
			flag = 0;
			mThread.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		mThread = null;
		super.onDestroy();
	}

	class DownThread extends Thread {
		
		@Override
		public void run() {
			
			if (flag == Flag_Init || flag == Flag_Done) {
				flag = Flag_Down;
			}
			
			downloader.start();
		}
	}
	
	private DownloadListener downListener = new DownloadListener() {
		
		int fileSize;
		Intent intent = new Intent();
		
		@Override
		public void onSuccess(File file) {
			intent.setAction(ActivityAppUpdate.ACTION_DOWNLOAD_SUCCESS);
			intent.putExtra("progress", 100);
			intent.putExtra("file", file);
			sendBroadcast(intent);
		}
		
		@Override
		public void onStart(int fileByteSize) {
			fileSize = fileByteSize;
			flag = Flag_Down;
		}
		
		@Override
		public void onResume() {
			flag = Flag_Down;
		}
		
		@Override
		public void onProgress(int receivedBytes) {
			if(flag == Flag_Down)
			{
				progress = (int)((receivedBytes / (float)fileSize) * 100);
				intent.setAction(ActivityAppUpdate.ACTION_DOWNLOAD_PROGRESS);
				intent.putExtra("progress", progress);
				sendBroadcast(intent);
				
				if (progress == 100) {
					flag = Flag_Done;
				}
			}
		}
		
		@Override
		public void onPause() {
			flag = Flag_Pause;
		}
		
		@Override
		public void onFail() {
			intent.setAction(ActivityAppUpdate.ACTION_DOWNLOAD_FAIL);
			sendBroadcast(intent);
			flag = Flag_Init;
		}
		
		@Override
		public void onCancel(){
			progress = 0;
			flag = Flag_Init;
		}
	};
}
