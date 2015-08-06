package com.showmo.activity.safe;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoSystem;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.widget.TextView;

public class ActivitySafeCenter extends BaseActivity {
	private TextView mCurLevel;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_safe_center);
		findView();
	}

	private void findView() {
		setBarTitleWithBackFunc(R.string.safe_center_title);
		findViewAndSet(R.id.safe_level);
		findViewAndSet(R.id.display_safe);
		mCurLevel=(TextView)findViewById(R.id.safe_level_summary);
	}
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		SharedPreferences sp=getSharedPreferences(ShowmoSystem.SafeConfigXml, Context.MODE_PRIVATE);
		String curname=sp.getString(ShowmoSystem.SafeCurLevelKey, "");
		String str=getResources().getString(R.string.safe_cur_level);
		str =str+curname;
		mCurLevel.setText(str);
	}
	
	

	@Override
	protected void onClick(int viewId) {
		// TODO Auto-generated method stub
		super.onClick(viewId);
		switch (viewId) {
		case R.id.safe_level:
			slideInFromRight(ActivitySafeSelect.class);
			break;
		case R.id.display_safe:
			slideInFromRight(ActivitySafeDisplay.class);
			break;
		default:
			break;
		}
	}
}
