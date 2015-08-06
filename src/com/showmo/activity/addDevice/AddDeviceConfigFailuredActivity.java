package com.showmo.activity.addDevice;

import com.showmo.R;
import com.showmo.base.BaseActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;

public class AddDeviceConfigFailuredActivity extends BaseActivity implements
		View.OnClickListener {
	public static final int RESULTCODE_ADDDEFEAT=1;
	public static final int RESULTCODE_ADDWIFISETERR=2;
	public static final int RESULTCODE_RESEARCH=3;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.init();
	}
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if(keyCode==KeyEvent.KEYCODE_BACK){
			setResult(RESULTCODE_ADDDEFEAT);
			finish();
			slideInFromLeft();
		}
		return true;
	};
	private void init() {
		this.setContentView(R.layout.activity_add_device_config_failured);
		setBarTitle(R.string.add_device_config_title);
		((Button) findViewById(R.id.btn_bar_back)).setOnClickListener(this);
		((Button) findViewById(R.id.add_device_config_err_wifi_err))
				.setOnClickListener(this);
		((Button) findViewById(R.id.add_device_config_err_connect_suc))
				.setOnClickListener(this);

	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.btn_bar_back:
			setResult(RESULTCODE_ADDDEFEAT);
			break;
		case R.id.add_device_config_err_wifi_err:
			setResult(RESULTCODE_ADDDEFEAT);
			break;
		case R.id.add_device_config_err_connect_suc:
			setResult(RESULTCODE_RESEARCH);
			break;
		default:
			break;
		}
		finish();
		slideInFromLeft();
	}
}
