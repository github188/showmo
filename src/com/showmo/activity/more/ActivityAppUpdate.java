package com.showmo.activity.more;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Timer;
import java.util.TimerTask;

import javax.net.ssl.HttpsURLConnection;


















import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.network.DownloadService;
import com.showmo.util.AppStateCheck;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;

import android.R.integer;
import android.app.AlertDialog;
import android.app.ActionBar.LayoutParams;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.PopupWindow;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.PopupWindow.OnDismissListener;

public class ActivityAppUpdate extends BaseActivity{
	public static final String NewVersionKey="newversion";
	public static final String NewFeatureKey="newfeature";
	private String m_newVersion;
	private String m_newFeature;
	private PopupWindow downloadpw;
	private View downloadvw;
	private String urlStr="http://www.showmo365.com/TemporaryFileDir/Android/showmo365_android.apk";
	public static final String ACTION_DOWNLOAD_PROGRESS = "my_download_progress";
	public static final String ACTION_DOWNLOAD_SUCCESS = "my_download_success";
	public static final String ACTION_DOWNLOAD_FAIL = "my_download_fail";
	MyReceiver receiver;
	private TextView downloadtv;
	private TextView feature,cancel;
	private ProgressBar pb;
	private  boolean ifdownsucces=false;
	private File file;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_new_version);
		Intent intent=getIntent();
		m_newVersion=intent.getStringExtra(NewVersionKey);
		m_newFeature=intent.getStringExtra(NewFeatureKey);
		findView();
		receiver = new MyReceiver();
		check();
		initpopwindow();
		
		
		
	

	}
	@Override
	protected void onStart() {
		super.onStart();
		IntentFilter filter = new IntentFilter();
		filter.addAction(ACTION_DOWNLOAD_PROGRESS);
		filter.addAction(ACTION_DOWNLOAD_SUCCESS);
		filter.addAction(ACTION_DOWNLOAD_FAIL);
		registerReceiver(receiver, filter);
	}

	@Override
	protected void onStop() {
		super.onStop();
		unregisterReceiver(receiver);
	}
	
	public void initpopwindow(){

		downloadvw=LayoutInflater.from(this).inflate(R.layout.download_popupwindow, null);
		downloadtv=(TextView)downloadvw.findViewById(R.id.progressno);
		pb=(ProgressBar)downloadvw.findViewById(R.id.progressbar);
		cancel=(TextView)downloadvw.findViewById(R.id.cancel);
		cancel.setOnClickListener(this);
		downloadpw=new PopupWindow(downloadvw, LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
		downloadpw.setFocusable(true);
		downloadpw.setOutsideTouchable(false);
		downloadpw.update();
		downloadpw.setOnDismissListener(new OnDismissListener() {
		      
		      @Override
		      public void onDismiss() {
		    	  WindowManager.LayoutParams params=ActivityAppUpdate.this.getWindow().getAttributes();  
			      params.alpha=1f;  
			      ActivityAppUpdate.this.getWindow().setAttributes(params);  
		      }
		    });
	}
	
	
	private boolean check(){
		String  filePath = "/sdcard/showmodownload/showmo.apk";
		File file=new File(filePath);
		if(!file.exists()){
		 ifdownsucces=false;
		}
		else{
			PackageManager packageManager = getPackageManager();  
			PackageInfo packageInfo = packageManager.getPackageArchiveInfo(filePath, PackageManager.GET_ACTIVITIES); 
			Log.e("1",""+packageInfo.versionCode+packageInfo.versionName);
			if(m_newVersion.equals(packageInfo.versionName)){
				ifdownsucces=true;
			}
			else {
				
				ifdownsucces=false;
			}
		}
		return ifdownsucces;
	}
	
	private void findView(){
		setBarTitleWithBackFunc(R.string.more_newversoin);
		findViewAndSet(R.id.btn_update);
		((TextView)findViewById(R.id.new_versoin)).setText(m_newVersion);
		 feature= ((TextView)findViewById(R.id.new_feature));
		feature.setText(m_newFeature);
		if(!StringUtil.isNotEmpty(m_newFeature)){
			feature.setVisibility(View.GONE);
		}
	}
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		super.onClick(v);
		switch (v.getId()) {
		case R.id.btn_update:
			if(ifdownsucces==false){
			downloadpw.showAtLocation(feature, Gravity.CENTER, 0, 20);
			 WindowManager windowManager = ActivityAppUpdate.this.getWindowManager();  
		     Display display =  windowManager.getDefaultDisplay();  
			 WindowManager.LayoutParams params=ActivityAppUpdate.this.getWindow().getAttributes();  
		     params.alpha=0.7f; 
		     ActivityAppUpdate.this.getWindow().setAttributes(params);  
			startDownloadService();
			}
			else {
				if(file==null){
					file = new File("/sdcard/showmodownload/showmo.apk");
				}
				Toast.makeText(this, "已检测到下载了最新版本", Toast.LENGTH_SHORT).show();
				openFile(file);
			}
			break;
		case R.id.cancel:

			 stopDownloadService();
			 downloadpw.dismiss();
			 Toast.makeText(this, "取消下载", Toast.LENGTH_SHORT).show();
		default:
			break;
		}
	}
	
	void startDownloadService() {
		if (DownloadService.getInstance() != null
				&& DownloadService.getInstance().getFlag() != DownloadService.Flag_Init) {
			Toast.makeText(this, "已经在下载", 0).show();
			return;
		}
		 Intent it = new Intent(this, DownloadService.class);
		it.putExtra("flag", "start");
		it.putExtra("url", urlStr);
		startService(it);

	}

	class MyReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (action.equals(ACTION_DOWNLOAD_PROGRESS)) {
				int pro = intent.getExtras().getInt("progress");
				pb.setProgress(pro);
				downloadtv.setText(String.valueOf(pro)+"%");
			}
			else if(action.equals(ACTION_DOWNLOAD_SUCCESS)){
				ifdownsucces=true;
				downloadpw.dismiss();
				stopDownloadService();
				Toast.makeText(ActivityAppUpdate.this, "下载完成", Toast.LENGTH_SHORT).show();
				  file=(File)intent.getExtras().getSerializable("file");
				openFile(file);
			}
			else if(action.equals(ACTION_DOWNLOAD_FAIL))
			{
			}
		}

	}
	void stopDownloadService() {
		Intent it = new Intent(this, DownloadService.class);
		it.putExtra("flag", "stop");
		startService(it);
		//progBar.setProgress(0);
	}
	private void openFile(File f) {
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(android.content.Intent.ACTION_VIEW);
		String type = "application/vnd.android.package-archive";
		intent.setDataAndType(Uri.fromFile(f),type );
		startActivity(intent);
	}


}
