package com.showmo.activity.deviceManage;

import com.showmo.R;
import com.showmo.base.BaseActivity;

import android.os.Bundle;

public class MotionDetectionActivity extends BaseActivity{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_motion_detecion);
		initView();
	}

	private void initView() {
		findViewAndSet(R.id.btn_bar_back);
	    
	    
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
}
