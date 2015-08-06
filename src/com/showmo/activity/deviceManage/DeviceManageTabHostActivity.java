package com.showmo.activity.deviceManage;

import ipc365.app.showmo.jni.JniDataDef;
import ipc365.app.showmo.jni.JniDataDef.MSGID;

import java.util.ArrayList;
import java.util.List;

import com.showmo.MainActivity;
import com.showmo.R;
import com.showmo.activity.deviceManage.AlarmSwitchActivity;
import com.showmo.activity.deviceManage.DeviceManageAdpater;
import com.showmo.activity.register.RegisterActivity;
import com.showmo.base.BaseActivity;
import com.showmo.deviceManage.Device;
import com.showmo.userManage.User;
import com.showmo.util.AnimUtil;
import com.showmo.util.StringUtil;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TabHost;
import android.widget.TabHost.OnTabChangeListener;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class DeviceManageTabHostActivity extends BaseActivity {
	
	private ListView mLvDevList;
	private DeviceManageAdpater mDevManageAdapter ;	
	private List<Device> mDeviceList ;
	private User mCurUser ;
	private TextView mTvNoDeviceHint;
	private Handler handler;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_device_manage_tab_host);
		initView();  
		initNetwork();

	}
	private void initView() {
		setBarTitle(R.string.device_manage);
		findViewAndSet(R.id.btn_bar_back);		   
        mLvDevList = (ListView)findViewById(R.id.lv_dev_list);             
        mTvNoDeviceHint = (TextView)findViewById(R.id.tv_dev_no_device_hint);              
		mDevManageAdapter = new DeviceManageAdpater( mDeviceList,this);
		mLvDevList.setAdapter(mDevManageAdapter);
		mLvDevList.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				
				Device device = mDeviceList.get(position);
				///slideInFromRight
				slideInFromRight(DeviceSettingActivity.class,device.getmCameraId(),device.getUniqueId());

			}

		});
	
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.device_manage_tab_host, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
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
	
 
   
    @Override
    protected void onDestroy() {
    	//unregisterReceiver(mDeviceMsgUpdateReceiver);	
    	
    	super.onDestroy();
    }
    @Override
	protected void onResume() {
    	mCurUser = mShowmoSys.getCurUser();
		if(mCurUser != null){
			mDeviceList = mCurUser.getDevices();
		}
	//	Log.e("1111111111", "111deviceqqqqqqqqqqq"+mDeviceList.size());
		if(mDeviceList !=null && mDeviceList.size()!=0){
		//	Log.e("1111111111", "1111deviceqqqqqqqqqqq"+mDeviceList.get(0).toString());
			mDevManageAdapter.notifyDataSetChanged();
			mDevManageAdapter.setDeviceList(mDeviceList);
		}else{		
			mTvNoDeviceHint.setVisibility(View.VISIBLE);
		}		
		super.onResume();
		
	}

}
