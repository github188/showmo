package com.showmo.activity.deviceManage;

import java.util.List;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.network.NetWorkErrorCode;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.playHelper.IDevicePlayer;
import com.showmo.playHelper.PlayHelper;
import com.showmo.userManage.User;
import com.showmo.util.StringUtil;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import ipc365.app.showmo.jni.JniDataDef;
import ipc365.app.showmo.jni.JniDataDef.MSGID;


public class DeviceManageActivity extends BaseActivity{

	private ListView mLvDevList;
	private DeviceManageAdpater mDevManageAdapter ;
	private List<Device> mDeviceList ;
	private Button btnFirmwareUpdate;
	private User mCurUser ;
	private PwInfoDialog mComfirmDialog;
	private Button btnAlarmSwitch;
	private DeviceMsgUpdateReceiver mDeviceMsgUpdateReceiver;
	private TextView mTvNoDeviceHint;
	private Button btnDelete;


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_device_manage);

		initView();
		initNetwork();
		initReceiver();
		mCurUser = mShowmoSys.getCurUser();
		if(mCurUser != null){
			mDeviceList = mCurUser.getDevices();
		}
		if(mDeviceList !=null){

			mDevManageAdapter.setDeviceList(mDeviceList);

		}else{
			mTvNoDeviceHint.setVisibility(View.VISIBLE);
 
		}

	}






	private void initView() {
		setBarTitle(R.string.device_manage);
		findViewAndSet(R.id.btn_bar_back);
		btnDelete  = (Button)findViewAndSet(R.id.btn_dev_delete);
		btnFirmwareUpdate  = (Button)findViewAndSet(R.id.btn_dev_firmware_update);
		btnAlarmSwitch  = (Button)findViewAndSet(R.id.btn_dev_alarmswitch);
		mTvNoDeviceHint = (TextView)findViewById(R.id.tv_dev_no_device_hint);
		
		mLvDevList = (ListView)findViewById(R.id.lv_dev_list);
		mDevManageAdapter = new DeviceManageAdpater( mDeviceList,this);
		mLvDevList.setAdapter(mDevManageAdapter);
		mLvDevList.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				
				btnDelete.setEnabled(true);
				//设置选中项颜色
				mDevManageAdapter.updateColor(position, view);
				updateBottomButton(  position );

			}

		});
		setBottomBtnEnabled(false);
		

	}
	
	private void setBottomBtnEnabled(boolean enabled){
		btnDelete.setEnabled(enabled);
		btnFirmwareUpdate.setEnabled(enabled);
		btnAlarmSwitch.setEnabled(enabled);
	}
	
	/**
	 * 更新底部按键的状态
	 * @param position
	 */
	private void updateBottomButton(int position ){
		Device device = mDeviceList.get(position);

		//当 设备正在更新 或者 当前没有新版本 时 固件升级按键不可用
		if(  device.ismUpgrading() ||   !StringUtil.isNotEmpty(device.getmVersion())){
			btnFirmwareUpdate.setEnabled(false);
		}
		else{
			btnFirmwareUpdate.setEnabled(true);
		}
		btnAlarmSwitch.setEnabled(device.ismSwitchStateValid());

	}



	private void initReceiver() {
		Log.e("out", "initReceiver");
		mDeviceMsgUpdateReceiver = new DeviceMsgUpdateReceiver();
		IntentFilter filter = new IntentFilter( );
		filter.addAction(JniDataDef.MSGBroadcastActions.UPDATING_MOBILE_INVITE_ACK);
		filter.addAction(JniDataDef.MSGBroadcastActions.UPDATE_MOBILE_INVITE_INFO);
		registerReceiver(mDeviceMsgUpdateReceiver, filter);

	}

	private class DeviceMsgUpdateReceiver extends BroadcastReceiver{

		@Override
		public void onReceive(Context context, Intent intent) {
			Log.e("upgrade", "DeviceMsgUpdateReceiver.onReceive");
			//固件升级响应
			if(intent.getAction() == JniDataDef.MSGBroadcastActions.UPDATING_MOBILE_INVITE_ACK){

				// 1 msg received; 2 update packet downloaded; 
				// 4update failed ; 8 update succeed
				
				JniDataDef.SDK_CAMERA_UPDATE  data = (JniDataDef.SDK_CAMERA_UPDATE) intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
				Log.e("upgrade", "DeviceMsgUpdateReceiver.onReceive deviceid:"+data.cameraid);
				String deviceName = null ;
				for (Device dev : mDeviceList) {
					if(dev.getmDeviceId() == data.cameraid){
						deviceName = dev.getmDeviceName();
						break ;
					}
				}
				
				switch (data.cmd ) {
				case MSGID.UPDATE_FAILED:
					showToastShort( deviceName + getString(R.string.upgrade_failure)  );
					break;
				case MSGID.DOWNLOAD_PIC_SUCCESS:
					showToastShort( deviceName + getString(R.string.upgrade_success)  );
					break;
				}


			}else if(intent.getAction() == JniDataDef.MSGBroadcastActions.UPDATE_MOBILE_INVITE_INFO){//固件可更新
				

			}
			mDevManageAdapter.notifyDataSetChanged();
			int position = mDevManageAdapter.getSelectPos();
			if( position == -1 ){
				return ;
			}
			updateBottomButton(position);

		}

	}




	@Override
	protected void onClick(int viewId) {
		switch (viewId) {
		case R.id.btn_bar_back:
			onBackPressed();
			break;
		case R.id.btn_dev_alarmswitch:
			slideInFromRight(AlarmSwitchActivity.class,
					mDeviceList.get(mDevManageAdapter.getSelectPos()).getmDeviceId()  );
			break;
		case R.id.btn_dev_delete:
			if(mComfirmDialog == null){
				mComfirmDialog = buildCustomDialog(R.string.hint, R.string.present_camera_will_be_unbind,null,null,
						new OnOkClickListener(){

					@Override
					public void onClick() {
						
						netTaskDeviceUnbind();
					}

				},null);
			}
			mComfirmDialog.show();
			break;
		case R.id.btn_dev_firmware_update:
			
			netTaskFirmwareUpdate();
			break;
 

		default:
			break;
		}

	}

	private void netTaskFirmwareUpdate(){
		Log.e("DeviceManageActivity", "netTaskFirmwareUpdate");
		mNetHelper.newNetTask(new RequestCallBack() {
			
			@Override
			public ResponseInfo doInBackground() {
				Device device = mDeviceList.get(mDevManageAdapter.getSelectPos());
				DeviceUseUtils utils = new DeviceUseUtils(device);
				return getResponseInfo(utils.upgrade());
			}
			
			@Override
			public void onSuccess(ResponseInfo info) {
				showToastShort(R.string.is_upgrading);
				btnFirmwareUpdate.setEnabled(false);
				mDevManageAdapter.notifyDataSetChanged();
			}
			
			
			@Override
			public void onFailure(ResponseInfo info) {
				int errorCode = (int) info.getErrorCode();
				if(handleNetConnectionError(errorCode)){
					return ;
				}
				switch (errorCode) {


				default:
					LogUntreatedError(errorCode);
					break;
				}
 
			}
			
 
			
		});
		
		
	}

	private void netTaskDeviceUnbind(){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public void onPrepare() {
				showLoadingDialog();
			}

			@Override
			public ResponseInfo doInBackground() {
				IDevicePlayer playHelper = mShowmoSys.getPlayer() ;
				
				Device device = mDeviceList.get(mDevManageAdapter.getSelectPos());
				
				Device curPlayDev=playHelper.getmCurDeviceInfo();
				if(curPlayDev!=null){
					if(device.getmCameraId()==curPlayDev.getmCameraId()){
						playHelper.stop();
					}
				}
				
				Log.e("DeviceManageActivity","netTaskDeviceUnbind.device-->"+device);
				return getResponseInfo(	mCurUser.unbindDevice(device));
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				showToastShort(R.string.unbind_success);
				mDevManageAdapter.resetColor();
				mDevManageAdapter.notifyDataSetChanged();
				Intent intent=new Intent(User.DEVICE_REMOVE_ACTION);
				sendBroadcast(intent);
				setBottomBtnEnabled(false);
			}

			@Override
			public void onFailure(ResponseInfo info) {
				int errorCode = (int) info.getErrorCode();
				if(handleNetConnectionError(errorCode)){
					return ;
				}
				switch (errorCode) {
				case NetWorkErrorCode. CSP_ERROR_GET_DEVICE_ID :

					break;

				default:
					break;
				}
 
			}

			@Override
			public void onFinally() {
				closeLoadingDialog();
			}
		});


	}

 
	@Override
	protected void onDestroy() {
		unregisterReceiver(mDeviceMsgUpdateReceiver);		
		super.onDestroy();
	}

}
