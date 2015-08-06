package com.showmo.activity.alarmInfo;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.util.StringUtil;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.widget.ImageView;

public class InfoImgActivity extends BaseActivity{

	private ImageView mImgPic;
	
	private String mImgPath ;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_alarm_info_img);
		
		Intent in = getIntent();
		mImgPath = in.getStringExtra(INTENT_KEY_STRING);
		initView();
	}

	private void initView() {
		setBarTitle(R.string.alarm_info_pic);
		findViewAndSet(R.id.btn_bar_back);
		mImgPic = (ImageView) findViewById(R.id.img_ainfo_pic);
		if(StringUtil.isNotEmpty(mImgPath)){
			Bitmap deviceImg = BitmapFactory.decodeFile(mImgPath);
			BitmapDrawable drawable=new BitmapDrawable(deviceImg);
			mImgPic.setImageBitmap(deviceImg);
		}
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
