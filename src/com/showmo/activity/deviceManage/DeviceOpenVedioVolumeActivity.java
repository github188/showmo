package com.showmo.activity.deviceManage;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.DeviceVolumState;
import ipc365.app.showmo.jni.JniDataDef.VolumeTypes;

import com.showmo.R;
import com.showmo.R.id;
import com.showmo.R.layout;
import com.showmo.R.menu;
import com.showmo.base.BaseActivity;
import com.showmo.deviceManage.Device;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.userManage.User;
import com.showmo.util.LogUtils;
import com.showmo.util.ToastUtil;
import com.showmo.widget.LoadingDialog;

import android.R.bool;
import android.R.integer;
import android.app.Activity;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.SeekBar.OnSeekBarChangeListener;

public class DeviceOpenVedioVolumeActivity extends BaseActivity implements OnCheckedChangeListener{
	public final static String KeyVolumState="KeyVolumState";
	public final static String KeyDeviceUniq="KeyDeviceUniq";
	private CheckBox mOpenVolumSwitch;
	private ArrayList<DeviceVolumState> volumes;//0¿ª£»1¹Ø
	private HashMap<Integer, DeviceVolumState> mSwitchs;
	private String UniqueId  ;
	private User mCurUser ;
	private IDeviceDao deviceDao;
	private Device device;
	private List<Device> mDeviceList ;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_device_talk_volume);
		Intent in = getIntent();
		if(in != null){
			volumes=(ArrayList<DeviceVolumState>)in.getSerializableExtra(KeyVolumState);
			UniqueId =in.getStringExtra(KeyDeviceUniq);
		}
		mSwitchs=new HashMap<Integer, DeviceVolumState>();
		if(volumes!=null){
			for (DeviceVolumState state : volumes) {
				mSwitchs.put(Integer.valueOf(state.type), state);
			}
		}
		mCurUser = showmoSystem.getCurUser();
		mDeviceList=mCurUser.getDevices();
		for (int i = 0; i < mDeviceList.size(); i++) {
			if(UniqueId.equals(mDeviceList.get(i).getUniqueId())){
				device = mDeviceList.get(i);
				break;
			}
		}
		initView();
		initNetwork();
	}
	
	private void initView() {
		setBarTitle(R.string.device_volume_info);
		mOpenVolumSwitch =(CheckBox)findViewById(R.id.open_volum_switch);
		mOpenVolumSwitch.setChecked(mSwitchs.get(Integer.valueOf(VolumeTypes.STREAM_TYPE)).bSwitch);
		LogUtils.e("switchs", " init Checked "+mOpenVolumSwitch.isChecked());
		mOpenVolumSwitch.setOnCheckedChangeListener(this);
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
	public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
		// TODO Auto-generated method stub
		switch (buttonView.getId()) {
		case R.id.open_volum_switch:
			netTaskSetVolumSwitch(mOpenVolumSwitch,isChecked,VolumeTypes.STREAM_TYPE);
			break;

		default:
			break;
		}
	}
	private void netTaskSetVolumSwitch(final CheckBox targetSwitch,final boolean toValue,final int type){
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
				boolean bres=false;
				if(toValue){
					LogUtils.e("switchs", " netTaskSetVolumSwitch open ");
					bres=JniClient.PW_NET_OpenVolumeSwitch(device.getmCameraId(), type);
				}else{
					LogUtils.e("switchs", " netTaskSetVolumSwitch close ");
					bres=JniClient.PW_NETCloseVolumeSwitch(device.getmCameraId(), type);
				}
				return getResponseInfo(bres);
			}
			@Override
			public void onSuccess(ResponseInfo info) {
				// TODO Auto-generated method stub
				super.onSuccess(info);
				LogUtils.e("switchs", "onSuccess setChecked "+toValue);
				
			}
			@Override
			public void onFailure(ResponseInfo info) {
				// TODO Auto-generated method stub
				super.onFailure(info);
				targetSwitch.setChecked(!toValue);
				if(!handleNetConnectionError((int)info.getErrorCode())){
					if(toValue){
						ToastUtil.toastShort(getContext(), R.string.open_volum_err);
					}else{
						ToastUtil.toastShort(getContext(), R.string.close_volum_err);
					}
				}
			}
			@Override
			public void onFinally() {
				// TODO Auto-generated method stub
				super.onFinally();
				closeLoadingDialog();
			}
		});
	}
	
}
