package com.showmo.preference.userSet;

import com.showmo.base.BaseActivity;

import com.showmo.R;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.preference.PreferenceActivity;
import android.preference.PreferenceScreen;
import android.util.Log;
import android.view.View;

public class UserSetPreferenceActivity extends BaseActivity implements View.OnClickListener{
	private PreferenceUserSetFragment m_prefUserSet;
	private FragmentManager fm;
	private Handler m_handler=new Handler();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		m_prefUserSet=new PreferenceUserSetFragment();
		fm=getFragmentManager();
		setContentView(R.layout.activity_preference_user_set);
		findView();
	}
	private void findView(){
		FragmentTransaction tsa=fm.beginTransaction();
		tsa.add(R.id.pref_container, m_prefUserSet, "pref");
		tsa.commit();
		
		
		m_handler.postDelayed(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				Log.v("title", "in acti "+m_prefUserSet.getTitle());
				setBarTitle(m_prefUserSet.getTitle());
			}
		},100);
		
	}
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.btn_bar_back:
			this.finish();
			slideInFromLeft();
			break;
			
		default:
			break;
		}
	}
}
