package com.showmo.network;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import android.app.Activity;
import android.app.DownloadManager;
import android.app.DownloadManager.Request;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.Cursor;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;
import android.webkit.MimeTypeMap;

public class Downloader {

	String urlStr;
	String filePath;
	String fileName;

	DownloadListener downloadListener;

	public void setDownloadListener(DownloadListener listener) {
		this.downloadListener = listener;
	}

	public Downloader(Context context, String url, String filePath,
			String fileName) {
		this.urlStr = url;
		this.filePath = filePath;
		this.fileName = fileName;
	}

	public Downloader(Context context, String url, String fileName) {
		this(context, url, "/showmodownload/", fileName);
	}

	public void start() {

		URL url = null;
		try {			
			url = new URL(urlStr);
			HttpURLConnection urlCon = (HttpURLConnection) url.openConnection();
			urlCon.setDoInput(true);
			urlCon.setRequestMethod("GET");
			urlCon.setRequestProperty("Content-Type",
					"application/x-www-form-urlencoded;charset=utf-8");

			urlCon.connect();
			int length = urlCon.getContentLength();
			downloadListener.onStart(length);

			if (urlCon.getResponseCode() == 200) {

				File path = Environment
						.getExternalStoragePublicDirectory(filePath);
				if(!path.exists()){
					path.mkdir();
				}
				File file = new File(path, fileName);
				BufferedInputStream is = new BufferedInputStream(
						urlCon.getInputStream());
				  				  
				BufferedOutputStream out = new BufferedOutputStream(
						new FileOutputStream(file));
				byte[] buffer = new byte[10240];
				int len = 0;
				int receivedBytes = 0;
label:				
				while(true)
				{

					if(isPause) downloadListener.onPause();
					if(isCancel) {
						downloadListener.onCancel();
						break label;
					}
					try {
						Thread.sleep(50);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					
					while (!isPause && (len = is.read(buffer)) > 0) {
						out.write(buffer, 0, len);
						receivedBytes += len;
						downloadListener.onProgress(receivedBytes);
						if(receivedBytes == length){
							downloadListener.onSuccess(file);
							break label;
						}
						if(isCancel) {
							downloadListener.onCancel();
							file.delete();
							break label;
						}
					}
				}

				is.close();
				out.close();
			} else {
				Log.e("jlf", "ResponseCode:" + urlCon.getResponseCode()
						+ ", msg:" + urlCon.getResponseMessage());
			}

		} catch (MalformedURLException e) {
			e.printStackTrace();
			downloadListener.onFail();
		} catch (IOException e) {
			e.printStackTrace();
			downloadListener.onFail();
		} 
	}

	private boolean isPause;

	public void pause() {
		isPause = true;
	}

	public void resume() {
		isPause = false;
		isCancel = false;
		downloadListener.onResume();
	}
	
	private boolean isCancel;
	public void cancel()
	{
		isCancel = true;
	}
}
