package com.showmo.activity.alarmInfo;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import rx.Scheduler;
import rx.Subscriber;
import rx.Observable.OnSubscribe;
import rx.android.schedulers.AndroidSchedulers;
import rx.schedulers.Schedulers;

import com.showmo.R;
import com.showmo.alarmManage.Alarm;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoApplication;
import com.showmo.base.ShowmoSystem;
import com.showmo.deviceManage.Device;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.ormlite.model.ShowmoAccount;
import com.showmo.rxcallback.RxCallback;
import com.showmo.userManage.User;
import com.showmo.util.AESUtil;
import com.showmo.util.LogUtils;
import com.showmo.util.ToastUtil;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.Observable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.TextView;
import ipc365.app.showmo.jni.JniDataDef;

public class DeviceListActivity extends BaseActivity{
	
	public static final int REQUEST_CODE_ALARM_INFO = 100 ;
	
	private ListView mLvDevices;
	
	private DeviceListAdapter mDeviceAdapter;

	private ArrayList<Device> mDeviceList ;
	
	private User mCurUser ;
	
	private NewAlarmBroadcastReceiver mNewAlarmReciver ;
	
	private TextView mTvNoDeviceHint;

	private Device mSelectedDevice;

	private IDeviceDao mDeviceDao;
	
	private boolean isInvokeByXg=false;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_alarm_device_list);
		mShowmoSys = ShowmoSystem.getInstance();
		mCurUser = mShowmoSys.getCurUser();
		mDeviceDao = (IDeviceDao)getDao(Device.class);
		initView();
		if(mCurUser == null){
			loginXgRegAcount();
		}else{
			initListView();
			initReciver();
		}
	}
	
	private void loginXgRegAcount(){
		showLoadingDialog();
		rx.Observable.create(new OnSubscribe<Void>() {
			@Override
			public void call(Subscriber<? super Void> t) {
				// TODO Auto-generated method stub
				String regAccount=getCommonSharedPreferences().getString(BaseActivity.SP_KEY_REG_ACCOUNT, "");
				if(regAccount.equals("")){
					t.onError(null);
					return;
				}
				List<ShowmoAccount> res = DaoFactory.getUserDao(ShowmoApplication.getInstance()).queryByUserName(regAccount);
				String psw=null;
				if(res != null){
					String pswNotAes = res.get(0).getPsssword();
					byte[] pswbyte = AESUtil.decrypt(AESUtil.parseHexStr2Byte(pswNotAes), AESUtil.KEY_AES);
					psw=new String(pswbyte);
					boolean blogin=false;
					try {
						blogin=ShowmoSystem.getInstance().userLogin(regAccount, psw, ShowmoSystem.SHOWMO_USER, false);
						LogUtils.e("alarm", "userLogin "+blogin);
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
						t.onError(null);
					}finally{
						LogUtils.e("alarm", "finally "+blogin);
						if(blogin){
							t.onNext(null);
						}else {
							t.onError(null);
						}
					}
				}
			}
		}).subscribeOn(Schedulers.io())
		.observeOn(AndroidSchedulers.mainThread())
		.subscribe(new RxCallback<Void>() {
			@Override
			public void onNext(Void t) {
				// TODO Auto-generated method stub
				closeLoadingDialog();
				isInvokeByXg=true;
				initListView();
				initReciver();
				super.onNext(t);
			}
			@Override
			public void onError(Throwable e) {
				// TODO Auto-generated method stub
				closeLoadingDialog();
				super.onError(e);
				ToastUtil.toastShort(DeviceListActivity.this, "获取告警信息失败！");
				finish();
			}
		});
	}
 
	private void initReciver() {
		mNewAlarmReciver = new NewAlarmBroadcastReceiver();
		registerBroadcastReceiver(mNewAlarmReciver, JniDataDef.MSGBroadcastActions.CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG);

	}


	private void initView() {
		
		setBarTitle(R.string.add_device_search_ssid_list);
		findViewAndSet(R.id.btn_bar_back);
		mTvNoDeviceHint = (TextView)findViewById(R.id.tv_alarm_info_no_device_hint);
		//测试 自动发送广播
//		findViewById(R.id.btn_test_open).setOnClickListener(new OnClickListener() {
//			
//			@Override
//			public void onClick(View v) {
//				mShowmoSys.TEST_sendMsgAckInThread(47742);
//			}
//		});
//		
//		findViewById(R.id.btn_test_close).setOnClickListener(new OnClickListener() {
//			
//			@Override
//			public void onClick(View v) {
//				mShowmoSys.stopTest_send();
//			}
//		});
		
		
	}

	private void initListView(){
		//初始化listview
		mCurUser= mShowmoSys.getCurUser();
		mLvDevices = (ListView)findViewById(R.id.lv_alarm_info_device_list);
		 List<Device> list=null;
		if(mCurUser!=null){
			list = mCurUser.getDevices();
		 }
		//for test start  插入测试数据 一个设备对象
//		Device [UniqueId=1506874951048535, mCameraId=48535, mDeviceId=48535, mUuid=DC07C1F52BC7, mDeviceName=演示1:前台(壁装), mTinyImgFilePath=, mVersion=, mUseOwner=15068749510, mHaveNewAlarmInfo=false, mOnlineState=false]
		
//		mDeviceList = new ArrayList<Device>();
//		Device device = new Device("111",2222, 48535, "DC07C1F52BC7", "前台(壁装)", null, null);
//		mDeviceList.add(device);
		//for test end 
		
		if(list == null || list.size() == 0 ){
			mLvDevices.setVisibility(View.GONE);
			mTvNoDeviceHint.setVisibility(View.VISIBLE);
		}else{
			mDeviceList = new ArrayList<Device>();
			mDeviceList.addAll(list);
			sortListByHaveNewAlarm(mDeviceList);
			mDeviceAdapter = new DeviceListAdapter(mDeviceList, this);
			mLvDevices.setAdapter(mDeviceAdapter);
		}
		

		mLvDevices.setOnItemClickListener(new OnItemClickListener() {


			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				
				mSelectedDevice = mDeviceList.get(position) ;
				Intent intent = new Intent(DeviceListActivity.this,InfoListActivity.class);
				intent.putExtra(INTENT_KEY_INT,mSelectedDevice .getmDeviceId());
				startActivityForResult(intent,REQUEST_CODE_ALARM_INFO );
				slideInFromRight();
				
//				slideInFromRight(InfoListActivity.class,mDeviceList.get(position).getmDeviceId());	
				
			}
		});


	}
	
	private void sortListByHaveNewAlarm(List<Device> lsit){
        Collections.sort(lsit, new Comparator<Device>() {
            public int compare(Device arg0, Device arg1) {
            	int i = arg0.getHaveNewAlarmInfo() ? 1 : 0 ;
            	int j = arg0.getHaveNewAlarmInfo()  ? 1 : 0 ;
 
                return Integer.valueOf(j).compareTo(i);
            }
        });
		
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		
		if(resultCode == RESULT_OK){
			mSelectedDevice.setHaveNewAlarmInfo(false);
			mDeviceAdapter.notifyDataSetChanged();
			dbTaskModHaveNewAlarm();
		} 
	}
	
	private void dbTaskModHaveNewAlarm(){
		mNetHelper.newNetTask(new RequestCallBack() {
			
			@Override
			public ResponseInfo doInBackground() {
				int res = mDeviceDao.updateDevice(mSelectedDevice);
				
				if(res == 0 ){
					Log.e("DeviceListActivity", "dbTaskModHaveNewAlarm.res-->fail");
				}
				return null ;
			}
			
		});
		
		
		
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

	private void registerBroadcastReceiver( BroadcastReceiver receiver ,  String action){  
		IntentFilter filter = new IntentFilter(action);  
		registerReceiver(receiver, filter);  
	}  
	
	/**
	 * 当新的告警信息来到时，会收到广播，此时更新adapter
	 * @author Administrator
	 *
	 */
	private class NewAlarmBroadcastReceiver extends BroadcastReceiver{

		@Override
		public void onReceive(Context context, Intent intent) {
			if(intent  == null){
				return ;
			}
			Alarm alarm =    (Alarm) intent.getSerializableExtra(JniDataDef.MSGBroadcastActions.DataKey);
			
			if(alarm == null){
				return ;
			}
			
			mDeviceAdapter.notifyDataSetChanged();
			
		}

	}
 
	private void unregisterBroadcastReceiver(){
		unregisterReceiver(mNewAlarmReciver);
		
	}
	
	
	@Override
	protected void onDestroy() {
		unregisterBroadcastReceiver();
		if(isInvokeByXg){
			showmoSystem.userLogout();
		}
		super.onDestroy();
	}
	
	
	
	
	

}
