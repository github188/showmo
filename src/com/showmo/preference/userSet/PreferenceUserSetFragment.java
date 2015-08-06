package com.showmo.preference.userSet;


import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoApplication;
import com.showmo.base.ShowmoSystem;
import com.showmo.userManage.User;
import com.showmo.util.LogUtils;
import com.tencent.android.tpush.XGPushManager;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceFragment;
import android.util.Log;

public class PreferenceUserSetFragment extends BasePreferenceFragment {
	private String title;
	private CheckBoxPreference m_xg_push_switch;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		addPreferencesFromResource(R.xml.preference_user_set);
		title=getPreferenceScreen().getTitle().toString();
		m_xg_push_switch= (CheckBoxPreference)findPreference("xg_push_switch");
		m_xg_push_switch.setOnPreferenceChangeListener(new OnPreferenceChangeListener() {
			@Override
			public boolean onPreferenceChange(Preference preference, Object newValue) {
				// TODO Auto-generated method stub
				LogUtils.e("prefchange", "onPreferenceChange newValue:"+newValue.toString());
				if(preference == m_xg_push_switch){
					Boolean value=(Boolean)newValue;
					if(value.booleanValue()){
						ShowmoSystem.getInstance().registerCurUserXgPush();
					}else{
						ShowmoSystem.getInstance().unregisterXgPush();
					}
					return true;
				}
				return false;
			}
		});
		Log.v("title", title);
	}
	public String getTitle(){
		return title;
	}
}
