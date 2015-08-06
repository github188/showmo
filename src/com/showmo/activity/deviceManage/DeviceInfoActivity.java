package com.showmo.activity.deviceManage;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.SDK_WIFI_VALUE;

import java.util.List;

import com.showmo.R;
import com.showmo.R.id;
import com.showmo.R.layout;
import com.showmo.R.menu;
import com.showmo.activity.login.VerificationCodeActivity;
import com.showmo.base.BaseActivity;
import com.showmo.deviceManage.Device;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.userManage.User;
import com.showmo.util.LogUtils;
import com.showmo.util.StringUtil;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

public class DeviceInfoActivity extends BaseActivity {
	private TextView mConnectionWifiTextView;
	private TextView mWifiSignalTextView;
	private TextView mDeviceCurVersionTextView;
	private TextView mDeviceCurIpTextView;
	private TextView mDeviceCurMacTextView;
	SDK_WIFI_VALUE mWifiValue=new SDK_WIFI_VALUE();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_device_info);
		
		Intent in = getIntent();
		if(in != null){
			mWifiValue = (SDK_WIFI_VALUE)in.getSerializableExtra(DeviceSettingActivity.WifiInfoKey);
		}
		initView();
		
	}
	private void initView() {
		setBarTitle(R.string.device_info);
		mConnectionWifiTextView=(TextView)findViewById(R.id.tv_connection_wifi);
		mConnectionWifiTextView.setText(mWifiValue.currentssid);
		mWifiSignalTextView=(TextView)findViewById(R.id.tv_wifi_signal);
		//mWifiSignalTextView.setText(""+mWifiValue.wifi_db);
		mDeviceCurVersionTextView=(TextView)findViewById(R.id.device_cur_version);
		mDeviceCurVersionTextView.setText(mWifiValue.version);
		mDeviceCurIpTextView=(TextView)findViewById(R.id.device_cur_ip);
		mDeviceCurIpTextView.setText(mWifiValue.local_ip);;
		mDeviceCurMacTextView=(TextView)findViewById(R.id.device_cur_mac);
		mDeviceCurMacTextView.setText(mWifiValue.mac);
	}
	@Override
	protected void onClick(int viewId) {
		switch (viewId) {
		case R.id.btn_bar_back:
			onBackPressed();
			break;
		default:
			break;
		}
	}
	
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.device_info, menu);
		return true;
	}

	
}
