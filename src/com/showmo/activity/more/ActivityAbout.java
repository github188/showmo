package com.showmo.activity.more;

import java.io.File;
import java.io.FileOutputStream;
import java.util.HashMap;

import cn.sharesdk.framework.Platform;
import cn.sharesdk.framework.PlatformActionListener;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.onekeyshare.OnekeyShare;

import com.showmo.R;
import com.showmo.base.BaseActivity;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

public class ActivityAbout extends BaseActivity {
	private Button sharebtn;
	private static final String FILE_NAME = "/share_pic.jpg";
	public static String SHARE_IMAGE;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);

		this.setContentView(R.layout.activity_about);
		new Thread() {
			public void run() {
				initImagePath();
			}
		}.start();
		findView();

	}
	private void findView(){
		setBarTitleWithBackFunc(R.string.more_about);
		String nowVersion="";
		String appName="";
		try {
			PackageManager pm=getPackageManager();
			PackageInfo info=pm.getPackageInfo(this.getPackageName(), 0);
			nowVersion="v"+info.versionName;
			appName=getResources().getString(info.applicationInfo.labelRes);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		((TextView)findViewById(R.id.about_appversion)).setText(nowVersion);
		((TextView)findViewById(R.id.about_appname)).setText(appName);
		sharebtn=(Button)findViewById(R.id.sharebtn);
		sharebtn.setOnClickListener(this);
	}
	@Override
	protected void onClick(int viewId) {
		// TODO Auto-generated method stub
		switch (viewId) {
		case R.id.sharebtn:
			share();
			break;

		default:
			break;
		}
	}
	
	private void initImagePath() {
		try {
			if (Environment.MEDIA_MOUNTED.equals(Environment
					.getExternalStorageState())
					&& Environment.getExternalStorageDirectory().exists()) {
				SHARE_IMAGE = Environment.getExternalStorageDirectory()
						.getAbsolutePath() + FILE_NAME;
			} else {
				SHARE_IMAGE = getApplication().getFilesDir().getAbsolutePath()
						+ FILE_NAME;
			}
			// 创建图片文件夹
			File file = new File(SHARE_IMAGE);
			if (!file.exists()) {
				file.createNewFile();
				Bitmap pic = BitmapFactory.decodeResource(getResources(),
						R.drawable.ic_launcher);
				FileOutputStream fos = new FileOutputStream(file);
				pic.compress(CompressFormat.JPEG, 100, fos);
				fos.flush();
				fos.close();
			}
		} catch (Throwable t) {
			t.printStackTrace();
			SHARE_IMAGE = null;
		}
	}
	protected  void share() {
		 ShareSDK.initSDK(this);
		 OnekeyShare oks = new OnekeyShare();
		 //关闭sso授权
		oks.disableSSOWhenAuthorize(); 
		 

		 oks.setTitle("我正在使用小末");

		 oks.setTitleUrl("http://www.showmo365.com.cn/index.php/FileServer/AppDownload");

		 oks.setText(getResources().getString(R.string.sms_share));

		 oks.setImagePath(SHARE_IMAGE);

		 oks.setUrl("http://www.showmo365.com.cn/index.php/FileServer/AppDownload");

		 oks.setSite(getString(R.string.app_name));

		 oks.setSiteUrl("http://www.showmo365.com.cn");
		 
		 oks.setDialogMode();
		 
		 oks.show(this);
	}
}
