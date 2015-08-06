package com.showmo.preference.userSet;

import com.showmo.R;
import com.showmo.base.BaseActivity;

import android.app.Activity;
import android.preference.Preference;
import android.preference.PreferenceCategory;
import android.preference.PreferenceFragment;
import android.preference.PreferenceScreen;

public class BasePreferenceFragment extends PreferenceFragment {
	protected BaseActivity m_activity;
	
	@Override
	public void onAttach(Activity activity) {
		// TODO Auto-generated method stub
		super.onAttach(activity);
		m_activity=(BaseActivity)activity;
	}
	@Override
	public void addPreferencesFromResource(int preferencesResId) {
		// TODO Auto-generated method stub
		super.addPreferencesFromResource(preferencesResId);
		PreferenceScreen pScreen=this.getPreferenceScreen();
		setLayoutResource(pScreen);

	}
	protected void setLayoutResource(Preference preference) {
        if (preference instanceof PreferenceScreen) {
            PreferenceScreen ps = (PreferenceScreen) preference;
            ps.setLayoutResource(R.layout.preference_screen);
            int cnt = ps.getPreferenceCount();
            for (int i = 0; i < cnt; ++i) {
                Preference p = ps.getPreference(i);
                setLayoutResource(p);
            }
        } else if (preference instanceof PreferenceCategory) {
            PreferenceCategory pc = (PreferenceCategory) preference;
            pc.setLayoutResource(R.layout.preference_category);
            int cnt = pc.getPreferenceCount();
            for (int i = 0; i < cnt; ++i) {
                Preference p = pc.getPreference(i);
                setLayoutResource(p);
            }
        } else {
            preference.setLayoutResource(R.layout.preference_item);
        }
    }
}
