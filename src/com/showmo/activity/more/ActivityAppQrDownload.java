package com.showmo.activity.more;

import android.os.Bundle;

import com.showmo.R;
import com.showmo.base.BaseActivity;

public class ActivityAppQrDownload extends BaseActivity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_app_share_qr);
		findView();
	}
	private void findView(){
		setBarTitleWithBackFunc(R.string.app_download_qr);
	}
}
