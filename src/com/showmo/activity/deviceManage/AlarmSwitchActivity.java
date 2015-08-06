package com.showmo.activity.deviceManage;

import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.deviceManage.Device.AlarmSwitch;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.CheckBox;
import android.widget.ListView;

/**
 * 告警开关列表 
 * 根据deviceId获得开关的列表
 * @author Administrator
 *
 */
public class AlarmSwitchActivity extends BaseActivity{

	private ListView mLvAlarmType;

	private List<AlarmSwitch> mAlarmList ;

	private PwInfoDialog mComfirmDialog ;

	private Device mDevice ;

	private AlarmTypeAdapter mAlarmTypeAdapter;

	private DeviceUseUtils mDeviceUseUtils ;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_alarm_switch);

		Intent in = getIntent();
		int devid = -1 ;
		if(in != null){
			devid = in.getExtras().getInt(INTENT_KEY_INT, -1 );
		}
		List<Device> list = mShowmoSys.getCurUser().getDevices();
		if(devid != -1){
			for (Device dev :list ) {
				if(dev.getmDeviceId() == devid ){
					mDevice = dev ;
				}
			}
		}

		initView();
		initNetwork();

	}

	private void initView() {
		setBarTitle(R.string.alarm_type);
		findViewAndSet(R.id.btn_bar_back);

		mLvAlarmType = (ListView)findViewById(R.id.lv_alarm_list);
		mAlarmTypeAdapter  = new AlarmTypeAdapter(  this) ;
		mAlarmList = mDevice.getmAlarmSwitchs();
		mAlarmTypeAdapter.setAlarmTypeList(mAlarmList);

		mLvAlarmType.setAdapter(mAlarmTypeAdapter);
		mLvAlarmType.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {

				final CheckBox checkBox = ((CheckBox)view.findViewById(R.id.check_alarm_switch));
				String strOperate = null ;
				strOperate = checkBox.isChecked() ? getString( R.string.comfirm_close) : getString( R.string.comfirm_open); 
				String contextText = strOperate +getString(  mAlarmList.get(position).alarmNameResId)  +"?" ;
				final AlarmSwitch alarmSwitch = mAlarmList.get(position);

				if(mComfirmDialog == null){
					mComfirmDialog = buildCustomDialog(R.string.hint,null,null,
							new OnOkClickListener(){

						@Override
						public void onClick() {
							if(mDeviceUseUtils == null){
								mDeviceUseUtils = new DeviceUseUtils(mDevice);
							}

							checkBox.setChecked(!checkBox.isChecked());
							netTaskSwitchAlarm(alarmSwitch.cameraAlarmType, checkBox.isChecked());

						}
					},null);
				}
				mComfirmDialog.setContentText(contextText);
				mComfirmDialog.show();

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

	private void netTaskSwitchAlarm(final int alarmType ,final boolean operate){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public ResponseInfo doInBackground() {

				return getResponseInfo(
						mDeviceUseUtils.SetAlarmSwitchState(alarmType, operate));
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


			@Override
			public void onFinally() {
				mAlarmTypeAdapter.notifyDataSetChanged();
			}

		});





	}


}
