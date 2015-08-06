package com.showmo.activity.addDevice;

import com.showmo.R;
import com.showmo.R.id;
import com.showmo.R.layout;
import com.showmo.base.BaseActivity;

import android.R.string;
import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

public class AddDeviceWithoutTipActivity extends BaseActivity implements View.OnClickListener {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_add_device_without_tip);
		init();
	}
	
	
	private void init(){
		setBarTitleWithRightBtn(R.string.add_device_ensure_without_tip_title);
		this.findViewById(R.id.btn_common_title_next).setVisibility(View.INVISIBLE);;
	}

	public void onClick(View v){
		switch (v.getId()) {
		case R.id.btn_bar_back:
			onBackPressed();
			break;
		}
	}
}
