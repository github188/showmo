package com.showmo.activity.safe;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoSystem;
import com.showmo.commonAdapter.PwSafeAdapter;
import com.showmo.commonAdapter.PwSafeAdapter.OnLevelDetailsClick;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.dao.ISafeDao;
import com.showmo.safe.Safe;
import com.showmo.widget.dialog.PwInfoDialog;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.widget.ListView;

public class ActivitySafeSelect extends BaseActivity implements OnLevelDetailsClick{
	private ListView m_levelList;
	private PwSafeAdapter adapter;
	private List<Safe> safeList;
	private static final int QuestDetailCode=1;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_safe_select_list);
		findView();
	}

	private void findView() {
		setBarTitleWithBackFunc(R.string.safe_center_select);
		findViewAndSet(R.id.btn_select_check);
		m_levelList=(ListView)findViewById(R.id.safe_level_list);
		safeList=new ArrayList<Safe>();
		safeList.add(Safe.High_safe_level);
		safeList.add(Safe.Mid_safe_level);
		safeList.add(Safe.Low_safe_level);
		DatabaseHelper m_dbHelper=DatabaseHelper.getHelper(this);
		ISafeDao safeDao=null;
		try {
		    safeDao=(ISafeDao)m_dbHelper.getDao(Safe.class);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		List<Safe> disSafe=safeDao.queryAllSafeLevel();
		safeList.addAll(disSafe);
		
		adapter=new PwSafeAdapter(this, safeList, R.layout.safe_select_list_item);
		adapter.setOnLevelDetailsClick(this);
		m_levelList.setAdapter(adapter);
		setCurLevel();
	}
	
	private void setCurLevel(){
		SharedPreferences sp= getSharedPreferences(ShowmoSystem.SafeConfigXml, Context.MODE_PRIVATE);
		String curSafeName=sp.getString(ShowmoSystem.SafeCurLevelKey, "");
		for (int i=0;i<safeList.size();i++) {
			if(safeList.get(i).safeName.equals(curSafeName)){
				adapter.setCurItem(i);
			}
		}
	}
	@Override
	public void onLevelDetailsClick(Safe data) {
		// TODO Auto-generated method stub
		Log.v("click", "onLevelDetailsClick "+data.safeName);
		Intent intent=new Intent(this,ActivitySafeDetails.class);
		intent.putExtra(ActivitySafeDetails.TARGETSAFE, (Serializable)data);
		startActivityForResult(intent, QuestDetailCode);
		slideInFromRight();
		
	}
	@Override
	protected void onClick(int viewId) {
		// TODO Auto-generated method stub
		super.onClick(viewId);
		switch (viewId) {
		case R.id.btn_select_check:
			PwInfoDialog dialog=new PwInfoDialog(this);
			String context=getResources().getString(R.string.safe_select_query);
			context+=adapter.getCurData().safeName;
			dialog.setContentText(context);
			dialog.setOkBtnTextAndListener(null, new PwInfoDialog.OnOkClickListener() {
				
				@Override
				public void onClick() {
					// TODO Auto-generated method stub
					SharedPreferences sp= getSharedPreferences(ShowmoSystem.SafeConfigXml, Context.MODE_PRIVATE);
					sp.edit().putString(ShowmoSystem.SafeCurLevelKey, adapter.getCurData().safeName).commit();
				}
			});
			dialog.show();
			break;
		default:
			break;
		}
	}
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		if(resultCode==ActivitySafeDetails.ResultCode_delete){
			Safe safeData=(Safe)data.getSerializableExtra(ActivitySafeDetails.TARGETSAFE);
			 adapter.getDataList().remove(safeData);
			for (Safe d : safeList) {
				if(d.safeName.equals(safeData.safeName)){
					safeList.remove(d);
					break;
				}
			}
			setCurLevel();
			adapter.notifyDataSetChanged();
		}
	}
}
