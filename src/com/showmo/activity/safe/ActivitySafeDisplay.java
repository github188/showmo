package com.showmo.activity.safe;

import java.util.List;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.commonAdapter.PwSafeLevelDisplayAdapter;
import com.showmo.commonAdapter.PwSafeTypeAdapter;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.dao.ISafeDao;
import com.showmo.safe.Safe;
import com.showmo.safe.Safe.SafeType;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;
import com.showmo.widget.dialog.PwInfoDialog;

import android.content.Intent;
import android.os.Bundle;
import android.preference.PreferenceActivity.Header;
import android.util.Log;
import android.widget.Button;
import android.widget.ListView;

public class ActivitySafeDisplay extends BaseActivity {
	private ListView mTypeList;
	private PwInfoDialog infoDialog;
	private ISafeDao safeDao;
	PwSafeLevelDisplayAdapter adapter;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_safe_display_level);
		infoDialog=new PwInfoDialog(this);
		try {
			safeDao=(ISafeDao)DatabaseHelper.getHelper(this).getDao(Safe.class);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		
		findView();
	}

	private void findView() {
		setBarTitleWithBackFunc(R.string.safe_center_title);
		mTypeList=(ListView)findViewById(R.id.safe_type_list);
		List<SafeType> safeTypeList=Safe.getSafeTypesValue(new Safe("",false,false,false));
		adapter=new PwSafeLevelDisplayAdapter(this, safeTypeList, R.layout.safe_type_list_item);
		adapter.setCurItem(-1);
		mTypeList.setAdapter(adapter);
		Button btn=(Button)findViewAndSet(R.id.btn_select_sure);
	}
	private void showNameDialog(){
		infoDialog.setDialogTitle(R.string.safe_name_your_level);
		infoDialog.setInputMode(true, "");
		infoDialog.setOkBtnTextAndListener(R.string.safe_add_ok, new PwInfoDialog.OnOkClickListener() {
			
			@Override
			public void onClick() {
				// TODO Auto-generated method stub
				String safeName=infoDialog.getInputText();
				if(!StringUtil.isNotEmpty(safeName)){
					infoDialog.show();
					ToastUtil.toastShort(ActivitySafeDisplay.this, R.string.safe_name_not_null);
					return;
				}
				List<Safe> safeList=safeDao.queryAllSafeLevel();
				safeList.addAll(Safe.getSysLevel());
				for (Safe safe : safeList) {
					if(safe.safeName.equals(safeName)){
						infoDialog.show();
						ToastUtil.toastShort(ActivitySafeDisplay.this, R.string.safe_name_already_exist);
						return;
					}
				}
				Safe disSafe = adapter.getDisSafe();
				disSafe.safeName=safeName;
				Log.v("display", disSafe.toString());
				if(safeDao.insertSafe(disSafe)){
					ToastUtil.toastShort(ActivitySafeDisplay.this, R.string.safe_add_suc);
					finish();
					slideInFromRight();
				}
			}
		});
		infoDialog.show();
	}
	
	@Override
	protected void onClick(int viewId) {
		// TODO Auto-generated method stub
		super.onClick(viewId);
		if(viewId==R.id.btn_select_sure){
			showNameDialog();
		}
	}
}
