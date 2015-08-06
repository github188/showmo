package com.showmo.activity.deviceManage;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef;
import ipc365.app.showmo.jni.JniDataDef.CAMERA_ALARM_TYPE;
import ipc365.app.showmo.jni.JniDataDef.DeviceVolumState;
import ipc365.app.showmo.jni.JniDataDef.LightCtrl;
import ipc365.app.showmo.jni.JniDataDef.MSGID;
import ipc365.app.showmo.jni.JniDataDef.SDK_WIFI_VALUE;
import ipc365.app.showmo.jni.JniDataDef.VolumeTypes;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.DeviceListFragment.DialogEnum;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoSystem;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.network.NetWorkErrorCode;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.playHelper.IDevicePlayer;
import com.showmo.rxErr.NetErrInfo;
import com.showmo.rxcallback.RxCallback;
import com.showmo.userManage.IUserObject;
import com.showmo.userManage.User;
import com.showmo.util.LogUtils;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;
import com.showmo.widget.MyTextView;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.R.integer;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class DeviceSettingActivity extends BaseActivity {
	//private ImageView imgFirmwarUpdate;
	public final static String WifiInfoKey="WifiInfoKey";
	private int TaskCount=0;
	private boolean mBrightState;
	private boolean mIsBrightGetSuc=false;
	private boolean mIsAlarmGetSuc=false;
	
	private ImageView imgAlarmSwitch;
	private ImageView imgLightSwitch;
	private ImageView imgVolumeSwitch;

	private TextView dev_name;
	private TextView dev_version;
	private ImageView mRedTipPoint;
	private TextView switch_state;
	private TextView light_state;
	private RadioButton radio_top_loading;
	private RadioButton radio_wall;

	private PwInfoDialog mComfirmDialog;
	private IUserObject mUserObj ;
	private int mCameraId = -1 ;
	private String UniqueId  ;
	private User mCurUser ;
	private IDeviceDao deviceDao;
	private Device device;
	private Handler handler;
	private List<Device> mDeviceList ;
	private String deviceSettingFlag;

	private int valueBrightness;
	private boolean alarmSwitchFlag=false;
	private int brightnessFlag=0;
	private PwInfoDialog m_renameDialog=null;
	private int valueVolume;


	private DeviceMsgUpdateReceiver mDeviceMsgUpdateReceiver;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_device_setting);
		Intent in = getIntent();
		if(in != null){
			mCameraId = in.getExtras().getInt(INTENT_KEY_STRINGONE, -1 );
			UniqueId = in.getExtras().getString(INTENT_KEY_STRINGTWO, "null" );
			deviceSettingFlag=in.getExtras().getString("INTENT_KEY_STRINGTO_DEVICESETTING","null");
			LogUtils.e("upgrade", "mCameraId:"+mCameraId+"UniqueId:"+UniqueId+"deviceSettingFlag:"+deviceSettingFlag);
		}
		mCurUser = showmoSystem.getCurUser();
		deviceDao=DaoFactory.getDeviceDao(DeviceSettingActivity.this);
		LogUtils.e("setting", "showmoSystem "+showmoSystem+" "+mCurUser);
		mDeviceList=mCurUser.getDevices();
		LogUtils.e("setting", "af "+mDeviceList);
		for (int i = 0; i < mDeviceList.size(); i++) {
			if(UniqueId.equals(mDeviceList.get(i).getUniqueId())){
				device = mDeviceList.get(i);
				break;
			}
		}
		initReceiver();
		initNetwork();
		initView();
		
	}

	private void initView() {
		setBarTitle(R.string.device_setting);
		imgAlarmSwitch=(ImageView)findViewById(R.id.img_set_dev_alarm_switch);
		imgLightSwitch=(ImageView)findViewById(R.id.img_set_dev_switch);
		imgVolumeSwitch=(ImageView)findViewById(R.id.img_set_dev_volume);

		dev_name=(TextView)findViewById(R.id.set_dve_name);
		dev_version=(TextView)findViewById(R.id.set_dve_firmware_version);
		mRedTipPoint=(ImageView)findViewById(R.id.tip_point);
		switch_state=(TextView)findViewById(R.id.switch_state);
		light_state=(TextView)findViewById(R.id.brightness_state);

		findViewAndSet(R.id.rel_dev_delete);
		findViewAndSet(R.id.rel_dev_firmware_update);
		findViewAndSet(R.id.rel_dev_alarmswitch);
		findViewAndSet(R.id.device_wifi_info);
		findViewAndSet(R.id.light_switch);
		findViewAndSet(R.id.re_dev_name);
		findViewAndSet(R.id.device_talk_volume);

		radio_top_loading=(RadioButton)findViewById(R.id.radio_top_loading);
		radio_wall=(RadioButton)findViewById(R.id.radio_wall);

//		switch_state.setOnClickListener(new OnClickListener() {//告警开关状态需要获取则点击之后获取
//			@Override
//			public void onClick(View arg0) {
//				// TODO Auto-generated method stub
//				netTaskGetSwitchValue();
//			}
//		});
//		light_state.setOnClickListener(new OnClickListener() {//LED开关状态获取
//			@Override
//			public void onClick(View arg0) {
//				// TODO Auto-generated method stub
//				netTaskDeviceBrightnessStartState();
//			}
//		});

		dev_name.setText(device.getmDeviceName());
		if(StringUtil.isNotEmpty(device.getmVersion())){
			if(device.ismUpgrading()){//正在更新
				String posString=getString(R.string.upgrading_pos, device.getmDownUpgradePkgPos());
				dev_version.setText(posString);
			}else{//有新的版本可以更新
				dev_version.setText(device.getmVersion());
				mRedTipPoint.setVisibility(View.VISIBLE);
			}
		}else{//没有新的版本可以更新
			dev_version.setText(R.string.current_firmware_new_version);
		}
		if(device.ismSwitchStateValid()){
			mIsAlarmGetSuc=true;
			List<Device.AlarmSwitch> switchs=device.getmAlarmSwitchs();
			Device.AlarmSwitch motionDetectSwitch=null;
			for (int i = 0; i < switchs.size(); i++) {
				if(switchs.get(i).cameraAlarmType == CAMERA_ALARM_TYPE.ALARM_TYPE_DETECTION_MOTION){
					motionDetectSwitch=switchs.get(i);
					break;
				}
			}
			if (motionDetectSwitch.value) {
				alarmSwitchFlag=true;
				switch_state.setVisibility(View.GONE);
				imgAlarmSwitch.setImageResource(R.drawable.switch_on);
			}else {
				alarmSwitchFlag=false;
				switch_state.setVisibility(View.GONE);
				imgAlarmSwitch.setImageResource(R.drawable.switch_off);
			}
		}else {
			netTaskGetSwitchValue();
		}


		handler=new Handler(){
			@Override
			public void handleMessage(Message msg) {

				if (msg.what==123) {
					String posString=getString(R.string.upgrading_pos, device.getmDownUpgradePkgPos());
					dev_version.setText(posString);
				}

				super.handleMessage(msg);   
			}

		};
		if ("devicesetting".equals(deviceSettingFlag)) {
			netTaskFirmwareUpdate();
		}
		netTaskDeviceBrightnessStartState();
//		handler.postDelayed(new Runnable() {
//
//			@Override
//			public void run() {
//				// TODO Auto-generated method stub
//				netTaskDeviceBrightnessStartState();
//				if ("devicesetting".equals(deviceSettingFlag)) {
//
//					netTaskFirmwareUpdate();
//				}
//				if(!device.ismSwitchStateValid()){
//					netTaskGetSwitchValue();
//				}
//			}
//		}, 40);

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.device_setting, menu);
		return true;
	}


	@Override
	protected void onClick(int viewId) {
		switch (viewId) {
		case R.id.btn_bar_back:
			onBackPressed();
			break;

		case R.id.rel_dev_delete:
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
		case R.id.rel_dev_firmware_update:
			if(!StringUtil.isNotEmpty(device.getmVersion())){			
				showToastShort(R.string.temporarily_not_have_upgrade);
			}else{	
				if(device.ismUpgrading()){
					showToastShort(R.string.is_upgrading);
				}else{
					netTaskFirmwareUpdate();	

				}
			}
			break;		
		case R.id.rel_dev_alarmswitch:	
			if(mIsAlarmGetSuc){
				netTaskSetAlarmSwitch();
			}else{
				netTaskGetSwitchValue();
			}

			break;
		case R.id.light_switch:
			if(mIsBrightGetSuc){
				netTaskDeviceBrightness();
			}else{
				netTaskDeviceBrightnessStartState();
			}

			break;
		case R.id.re_dev_name:
			showRenameDialog();
			break;
		case R.id.device_wifi_info:
			netTaskGetWifiInfo();

			break;
		case R.id.device_talk_volume:
			netTaskGetTalkVolume();
			break;
		default:
			break;
		}

	}
	private void netTaskGetTalkVolume(){
		mNetHelper.newNetTask(new RequestCallBack() {	
			@Override
			public void onPrepare() {
				// TODO Auto-generated method stub
				super.onPrepare();
				showLoadingDialog();
			}
			@Override
			public ResponseInfo doInBackground() {
				ResponseInfo resInfo=new ResponseInfo();
				List<DeviceVolumState> infos=JniClient.PW_NET_GetVolumeSwitchs(device.getmCameraId());
				if (infos!=null) {
					resInfo.setIsSuccess(true);
					resInfo.setObj(infos);
				}else {
					resInfo.setIsSuccess(false);
				}
				return resInfo;
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				closeLoadingDialog();
				ArrayList<DeviceVolumState> states=(ArrayList<DeviceVolumState>)info.getObj();
				Intent intent=new Intent(getContext(),DeviceOpenVedioVolumeActivity.class);
				intent.putExtra(DeviceOpenVedioVolumeActivity.KeyVolumState, states);
				intent.putExtra(DeviceOpenVedioVolumeActivity.KeyDeviceUniq, UniqueId);
				startActivity(intent);
				slideInFromRight();
			}
			@Override
			public void onFailure(ResponseInfo info) {
				showToastShort(R.string.volume_switch_state_set_err);
				//	slideInFromRight(DeviceOpenVedioVolumeActivity.class, 0,UniqueId);
				closeLoadingDialog();
			}
		});

	}
	private void netTaskGetWifiInfo(){
		mNetHelper.newNetTask(new RequestCallBack() {
			@Override
			public void onPrepare() {
				showLoadingDialog();
			}
			@Override
			public ResponseInfo doInBackground() {
				SDK_WIFI_VALUE wifiinfo=JniClient.PW_NET_GetWifiValue(device.getmCameraId());
				boolean bres=false;
				if(wifiinfo==null){
					bres=false;
				}else{
					bres=true;
				}
				ResponseInfo responseInfo=new ResponseInfo();
				responseInfo.setIsSuccess(bres);
				responseInfo.setObj(wifiinfo);
				if(!bres){
					responseInfo.setErrorCode(JniClient.PW_NET_GetLastError());
				}
				return responseInfo;
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				Intent intent =new Intent(getContext(),DeviceInfoActivity.class);
				intent.putExtra(WifiInfoKey, (Serializable)info.getObj());
				startActivity(intent);
				slideInFromRight();
			}
			@Override
			public void onFailure(ResponseInfo info) {
				int errorCode = (int) info.getErrorCode();
				if(handleNetConnectionError(errorCode)){
					return ;
				}

			}
			@Override
			public void onFinally() {
				closeLoadingDialog();
			}
		});


	}

	private void netTaskGetSwitchValue(){
		mNetHelper.newNetTask(new RequestCallBack() {
			@Override
			public void onPrepare() {
				// TODO Auto-generated method stub
				showLoadingDialog();
				super.onPrepare();
			}
			@Override
			public ResponseInfo doInBackground() {
				// TODO Auto-generated method stub
				ResponseInfo resInfo=new ResponseInfo();
				DeviceUseUtils utils = new DeviceUseUtils(device);
				resInfo.setIsSuccess(utils.CheckAlarmSwitchState());
				return resInfo;
			}
			@Override
			public void onFailure(ResponseInfo info) {
				imgAlarmSwitch.setVisibility(View.GONE);
				switch_state.setVisibility(View.VISIBLE);	
				switch_state.setText(R.string.switch_state);
				showToastShort(R.string.switch_state);	
				closeLoadingDialog();
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				switch_state.setVisibility(View.GONE);	
				imgAlarmSwitch.setVisibility(View.VISIBLE);
				mIsAlarmGetSuc=true;
				List<Device.AlarmSwitch> switchs= device.getmAlarmSwitchs();
				for (int i = 0; i < switchs.size(); i++) {
					if(switchs.get(i).cameraAlarmType == CAMERA_ALARM_TYPE.ALARM_TYPE_DETECTION_MOTION){
						if(switchs.get(i).value){
							imgAlarmSwitch.setImageResource(R.drawable.switch_on);
							alarmSwitchFlag=true;
						}else{	
							imgAlarmSwitch.setImageResource(R.drawable.switch_off);
							alarmSwitchFlag=false;
						}
						break;
					}

				}

				imgAlarmSwitch.setImageResource(R.drawable.switch_on);
				alarmSwitchFlag=true;
				closeLoadingDialog();
			}
		});

	}


	private void netTaskSetAlarmSwitch(){
		mNetHelper.newNetTask(new RequestCallBack() {
			@Override
			public void onPrepare() {
				showLoadingDialog();

			}

			@Override
			public ResponseInfo doInBackground() {
				boolean bres=false;
				Device target=null;
				for (int i = 0; i < mDeviceList.size(); i++) {
					if (UniqueId.equals(mDeviceList.get(i).getUniqueId())) {												
						target = mDeviceList.get(i);
						break;
					}
				}
				LogUtils.e("checkalarm", "ser alarm before");
				bres = new DeviceUseUtils(target).SetAlarmSwitchState(CAMERA_ALARM_TYPE.ALARM_TYPE_DETECTION_MOTION,!alarmSwitchFlag);
				if(bres){
					alarmSwitchFlag = !alarmSwitchFlag;
				}
				return getResponseInfo(bres);
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				if(!alarmSwitchFlag){
					imgAlarmSwitch.setImageResource(R.drawable.switch_off);
				}else{
					imgAlarmSwitch.setImageResource(R.drawable.switch_on);
				}
			}
			@Override
			public void onFailure(ResponseInfo info) {
				int errorCode = (int) info.getErrorCode();
				if(handleNetConnectionError(errorCode)){
					return ;
				}

			}
			@Override
			public void onFinally() {
				closeLoadingDialog();
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

				Device curPlayDev=playHelper.getmCurDeviceInfo();
				if(curPlayDev!=null){
					if(mCameraId==curPlayDev.getmCameraId()){
						playHelper.stop();
					}
				}

				return getResponseInfo(	mCurUser.unbindDevice(UniqueId));
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				showToastShort(R.string.unbind_success);
				Intent intent=new Intent(User.DEVICE_REMOVE_ACTION);
				sendBroadcast(intent);
				onBackPressed();
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
	private void netTaskDeviceBrightness(){
		mNetHelper.newNetTask(new RequestCallBack() {	
			@Override
			public void onPrepare() {
				// TODO Auto-generated method stub
				super.onPrepare();
				showLoadingDialog();
			}
			@Override
			public ResponseInfo doInBackground() {
				boolean bres=false;
				if(mBrightState){
					DeviceUseUtils utils=new DeviceUseUtils(device);
					bres=utils.brightCtrl(0);
					if(bres){
						mBrightState=false;
					}
				}else{
					DeviceUseUtils utils=new DeviceUseUtils(device);
					bres=utils.brightCtrl(30);
					if(bres){
						mBrightState=true;
					}
				}
				return getResponseInfo(bres);
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				closeLoadingDialog();
				imgLightSwitch.setVisibility(View.VISIBLE);
				light_state.setVisibility(View.GONE);
				setSwitchState(mBrightState);
			}
			@Override
			public void onFailure(ResponseInfo info) {
				if(!handleNetConnectionError((int)info.getErrorCode())){
					showToastShort(R.string.light_switch_state_set_err);
				}
				closeLoadingDialog();
			}
		});


	}
	private  void setSwitchState(boolean bstate){
		mBrightState=bstate;
		if(bstate){
			imgLightSwitch.setImageResource(R.drawable.switch_on);		
		}else{
			imgLightSwitch.setImageResource(R.drawable.switch_off);
		}
	}

	@Override
	public synchronized void showLoadingDialog() {
		// TODO Auto-generated method stub
		LogUtils.e("task", "asyn task ++");
		TaskCount++;
		super.showLoadingDialog();
	}
	@Override
	public synchronized void closeLoadingDialog() {
		// TODO Auto-generated method stub
		LogUtils.e("task", "asyn task --");
		TaskCount--;
		if(TaskCount<=0)
			super.closeLoadingDialog();
	}
	private void netTaskDeviceBrightnessStartState(){
		mNetHelper.newNetTask(new RequestCallBack() {	
			@Override
			public void onPrepare() {
				// TODO Auto-generated method stub
				showLoadingDialog();
				super.onPrepare();
			}
			@Override
			public ResponseInfo doInBackground() {
				ResponseInfo resInfo=new ResponseInfo();
				valueBrightness=JniClient.PW_NET_Brightness(device.getmCameraId());

				if (valueBrightness!=-1) {
					resInfo.setIsSuccess(true);
				}else {
					resInfo.setIsSuccess(false);
				}
				return resInfo;
			}

			@Override
			public void onSuccess(ResponseInfo info) {	
				mIsBrightGetSuc=true;
				imgLightSwitch.setVisibility(View.VISIBLE);
				light_state.setVisibility(View.GONE);
				if (valueBrightness==0) {
					setSwitchState(false);			
				}else {
					setSwitchState(true);		
				}
				closeLoadingDialog();
			}

			@Override
			public void onFailure(ResponseInfo info) {
				//imgLightSwitch.setImageResource(R.drawable.switch_off);
				imgLightSwitch.setVisibility(View.GONE);
				light_state.setVisibility(View.VISIBLE);
				showToastShort(R.string.light_switch_state_get_err);	
				closeLoadingDialog();
			}
		});


	}


	private void netTaskFirmwareUpdate(){		
		mNetHelper.newNetTask(new RequestCallBack() {		
			@Override
			public void onPrepare() {
				// TODO Auto-generated method stub
				super.onPrepare();
				showLoadingDialog();
			}
			@Override
			public ResponseInfo doInBackground() {
				DeviceUseUtils utils = new DeviceUseUtils(device);
				return getResponseInfo(utils.upgrade());
			}			
			@Override
			public void onSuccess(ResponseInfo info) {
				closeLoadingDialog();
				dev_version.setText(getString(R.string.upgrading_pos,0));
				mRedTipPoint.setVisibility(View.GONE);
				showToastShort(R.string.is_upgrading);		
			}

			@Override
			public void onFailure(ResponseInfo info) {	
				closeLoadingDialog();
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
	private void initReceiver() {
		//Log.e("out", "11111initReceiver");
		mDeviceMsgUpdateReceiver = new DeviceMsgUpdateReceiver();
		IntentFilter filter = new IntentFilter( );
		filter.addAction(JniDataDef.MSGBroadcastActions.UPDATING_MOBILE_INVITE_ACK);
		filter.addAction(JniDataDef.MSGBroadcastActions.UPDATE_SUCCESS_MOBILE_INVITE_ACK);
		filter.addAction(JniDataDef.MSGBroadcastActions.UPDATE_FAILE_MOBILE_INVITE_ACK);
		filter.addAction(JniDataDef.MSGBroadcastActions.UPDATE_MOBILE_INVITE_INFO);
		//sendDBBroadcast(MSGBroadcastActions.UPDATE_MOBILE_INVITE_ACK, ftpAck);
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

				final JniDataDef.SDK_CAMERA_UPDATE  data = (JniDataDef.SDK_CAMERA_UPDATE) intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
				//Log.e("upgrade", "111111DeviceMsgUpdateReceiver.onReceive deviceid:"+data.cameraid+"   "+data.downpos+"    "+(data.downpos>=0&&data.downpos<=100));
				if(data.cameraid!=device.getmCameraId()){
					return;
				}
				Message msg=new Message();
				msg.arg1=data.downpos;
				msg.what=123;
				handler.sendMessage(msg);
			}else if(intent.getAction() == JniDataDef.MSGBroadcastActions.UPDATE_MOBILE_INVITE_INFO){//固件可更新

			}else if(intent.getAction() == JniDataDef.MSGBroadcastActions.UPDATE_SUCCESS_MOBILE_INVITE_ACK){
				
				JniDataDef.SDK_CAMERA_UPDATE  data = (JniDataDef.SDK_CAMERA_UPDATE) intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
				if(data.cameraid!=device.getmCameraId()){
					return;
				}
				LogUtils.fi(LogUtils.LogAppFile, "upgrade success cameraid:"+data.cameraid);
				String deviceName = null ;				
				deviceName = device.getmDeviceName();
				//imgFirmwarUpdate.setImageResource(R.drawable.arrow_grey_down);
				dev_version.setText(R.string.current_firmware_new_version);
				//imgFirmwarUpdate.setVisibility(View.GONE);
				mRedTipPoint.setVisibility(View.INVISIBLE);
				showToastShort(getString(R.string.upgrade_success,device.getmDeviceName()));
			}else if(intent.getAction() == JniDataDef.MSGBroadcastActions.UPDATE_FAILE_MOBILE_INVITE_ACK){
				JniDataDef.SDK_CAMERA_UPDATE  data = (JniDataDef.SDK_CAMERA_UPDATE) intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
				if(data.cameraid!=device.getmCameraId()){
					return;
				}
				LogUtils.fi(LogUtils.LogAppFile, "upgrade failured cameraid:"+data.cameraid+" errcode "+data.errcode);
				String deviceName = null ;				
				deviceName = device.getmDeviceName();
				dev_version.setText(device.getmVersion());
				//imgFirmwarUpdate.setVisibility(View.GONE);
				mRedTipPoint.setVisibility(View.GONE);
				showToastShort( getString(R.string.upgrade_failure,device.getmDeviceName()));
			}
		}
	}

	@Override
	protected void onDestroy() {
		unregisterReceiver(mDeviceMsgUpdateReceiver);
		super.onDestroy();
	}
	/*private void showUpdateDialog(){

	 if (progressDialog!=null) {
		//设置最大值为100  
			progressDialog.setMax(100);  
			progressDialog.setCancelable(false);
			//设置进度条风格STYLE_HORIZONTAL  
			progressDialog.setProgressStyle(  ProgressDialog.STYLE_HORIZONTAL);  
			progressDialog.setTitle("固件升级进度条");  
			progressDialog.setProgress(0);
			 progressDialog.show();
	 }

    }*/
	private void showRenameDialog(){
		if(m_renameDialog==null){
			m_renameDialog=new PwInfoDialog(this);
			m_renameDialog.setDialogTitle(R.string.add_device_name_device_dialog_title);
			m_renameDialog.setInputMode(true,"")
			.setContentText(R.string.present_camera_will_be_unbind)
			.setOkBtnTextAndListener(null, new OnOkClickListener() {
				@Override
				public void onClick() {
					DeviceUseUtils utils=new DeviceUseUtils(device);
					utils.deviceRename(m_renameDialog.getInputText(),new RxCallback<Boolean>() {


						@Override
						public void onNext(Boolean result) {
							// TODO Auto-generated method stub
							if(mCurUser != null){
								Device	device2=deviceDao.queryByCameraId(UniqueId);
								for (int i = 0; i < mDeviceList.size(); i++) {
									if (device2.getmUuid().equals(mDeviceList.get(i).getmUuid())) {
										mDeviceList.get(i).setmDeviceName(m_renameDialog.getInputText().toString().trim());									
										mCurUser.setDevices(mDeviceList);
									}
								}
							}
							dev_name.setText(m_renameDialog.getInputText().toString().trim());
							ToastUtil.toastShort(DeviceSettingActivity.this, R.string.device_rename_suc);
						}
						@Override
						public void onError(Throwable result) {
							// TODO Auto-generated method stub
							if (result instanceof NetErrInfo) {
								NetErrInfo info=(NetErrInfo)result;
								DeviceSettingActivity.this.handleNetConnectionError((int)info.netErrCode);
								return;
							}
							ToastUtil.toastShort(DeviceSettingActivity.this, R.string.device_rename_fai);
						}
					});
					Intent intent=new Intent(Device.DEVICE_RENAME_ACTION);
					DeviceSettingActivity.this.sendBroadcast(intent);
				}
			});

		}
		m_renameDialog.setInputText("");

		m_renameDialog.show();
	}
}
