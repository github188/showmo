package com.showmo.widget;

import com.showmo.R;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FloatWindowSmallView extends LinearLayout {
	public static int viewWidth;
	public static int viewHeight;
	private WindowManager.LayoutParams mParams;
	
	public FloatWindowSmallView(Context context) {
		super(context);
		LayoutInflater.from(context).inflate(R.layout.float_window_small, this);
		View view = findViewById(R.id.small_window_layout);
		viewWidth = view.getLayoutParams().width;
		viewHeight = view.getLayoutParams().height;
		TextView percentView = (TextView) findViewById(R.id.percent);
		percentView.setText(R.string.net_use_alarm);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			MyWindowManager.removeSmallWindow(getContext());
			break;
	
		default:
			break;
		}
		return true;
	}

	public void setParams(WindowManager.LayoutParams params) {
		mParams = params;
	}

}
