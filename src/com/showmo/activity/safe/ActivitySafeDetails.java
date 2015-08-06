package com.showmo.activity.safe;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoSystem;
import com.showmo.commonAdapter.PwSafeAdapter;
import com.showmo.commonAdapter.PwSafeTypeAdapter;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.dao.ISafeDao;
import com.showmo.safe.Safe;
import com.showmo.safe.Safe.SafeType;
import com.showmo.util.ToastUtil;
import com.showmo.widget.dialog.PwInfoDialog;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.media.MediaCodec;
import android.os.Bundle;
import android.support.v4.widget.SimpleCursorAdapter.ViewBinder;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;

public class ActivitySafeDetails extends BaseActivity {
	private ListView m_detailList;
	private Safe m_targetSafe;
	public static final String TARGETSAFE="data";
	public static final int ResultCode_delete=2;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_safe_details);
		Intent intent=getIntent();
		m_targetSafe=(Safe)intent.getSerializableExtra(TARGETSAFE);
		findView();
	}

	private void findView() {
		setBarTitleWithBackFunc(R.string.safe_center_title);
		m_detailList=(ListView)findViewById(R.id.safe_details_list);
		List<SafeType> safeTypeList=Safe.getSafeTypesValue(m_targetSafe);
		PwSafeTypeAdapter adapter=new PwSafeTypeAdapter(this, safeTypeList, R.layout.safe_level_details_item);
		adapter.setCurItem(-1);
		m_detailList.setAdapter(adapter);
		Button btn=(Button)findViewAndSet(R.id.btn_delete_sure);
		if(m_targetSafe.SysLevel){
			btn.setVisibility(View.GONE);
		}
		
	}
	@Override
	protected void onClick(int viewId) {
		// TODO Auto-generated method stub
		super.onClick(viewId);
		if(viewId == R.id.btn_delete_sure){
			PwInfoDialog dialog=new PwInfoDialog(this);
			dialog.setContentText(R.string.safe_level_delete);
			dialog.setOkBtnTextAndListener(R.string.safe_level_delete_btn, new PwInfoDialog.OnOkClickListener() {
				
				@Override
				public void onClick() {
					// TODO Auto-generated method stub
					if(m_targetSafe.SysLevel){
						return;
					}
					SharedPreferences sp=getSharedPreferences(ShowmoSystem.SafeConfigXml, Context.MODE_PRIVATE);
					if(sp.getString(ShowmoSystem.SafeCurLevelKey, "").equals(m_targetSafe.safeName)){
						sp.edit().putString(ShowmoSystem.SafeCurLevelKey, Safe.High_safe_level.safeName).commit();
					}
					DatabaseHelper helper=DatabaseHelper.getHelper(ActivitySafeDetails.this);
					ISafeDao dao=null;
					try {
				    	dao=(ISafeDao)helper.getDao(Safe.class);
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
					}
					dao.RemoveBySafeName(m_targetSafe.safeName);
					ToastUtil.toastShort(ActivitySafeDetails.this,R.string.safe_delete_suc);
					Intent intent = new Intent();
					intent.putExtra(TARGETSAFE, (Serializable)m_targetSafe);
					setResult(ResultCode_delete,intent);
					finish();
					slideInFromRight();
				}
			});
			dialog.show();
		}
	}
}
