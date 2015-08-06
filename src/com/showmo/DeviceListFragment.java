package com.showmo;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef;
import ipc365.app.showmo.jni.JniDataDef.OnRealdataCallBackListener;

import java.io.File;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;

import rx.Observable;
import rx.Scheduler;
import rx.Subscriber;
import rx.Observable.OnSubscribe;
import rx.android.plugins.RxAndroidPlugins;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Func1;
import rx.observers.Subscribers;
import rx.schedulers.Schedulers;

import com.showmo.R;
import com.showmo.activity.addDevice.AddDeviceUserEnsure;
import com.showmo.activity.deviceManage.DeviceSettingActivity;
import com.showmo.alarmManage.Alarm;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoSystem;
import com.showmo.commonAdapter.PWDevAdapter;
import com.showmo.dataDef.PWDeviceInfo;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.event.DeviceAddEvent;
import com.showmo.event.TimelineShowEvent;
import com.showmo.eventBus.EventBus;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.playHelper.IDevicePlayer;
import com.showmo.playHelper.PlayHelper;
import com.showmo.playHelper.RealplayOutParams;
import com.showmo.playHelper.StopPlayingDeviceException;
import com.showmo.rxErr.NetErrInfo;
import com.showmo.rxcallback.RxCallback;
import com.showmo.userManage.User;
import com.showmo.util.AnimUtil;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;
import com.showmo.util.PwNetWorkHelper;
import com.showmo.util.PwTimer;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;
import com.showmo.widget.MyWindowManager;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnCancelClickListener;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.R.integer;
import android.R.interpolator;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Fragment;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.IntentFilter;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.database.DataSetObserver;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.nfc.cardemulation.OffHostApduService;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.provider.ContactsContract.CommonDataKinds.Event;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Adapter;
import android.widget.AdapterView;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioGroup;
import android.widget.RelativeLayout;
import android.widget.Toast;

public class DeviceListFragment extends BaseFragment {
	private GridView deviceList;
	private PWDevAdapter m_deviceAdapter=null;
	private RelativeLayout m_addBtn;
	private ShowmoSystem smSys;
	private IDevicePlayer m_playHelper;
	private PwInfoDialog m_longPressDialog=null;
	private PwInfoDialog m_deleteDialog=null;
	private PwInfoDialog m_renameDialog=null;
	private PwInfoDialog m_UpgradeDialog=null;
	private boolean isDetach=false;
	private PwTimer mHideFlowWindowTimer;

	private AlertDialog.Builder builder;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		//LogUtils.v("fragment", "onCreateView");
		//LogUtils.e("devicelist", "onCreateView");

		return inflater.inflate(R.layout.fragment_device_list, container, false);
	}
	//	@Override
	//	public void onStop() {
	//		// TODO Auto-generated method stub
	//		LogUtils.e("devicelist", "onStop");
	//		super.onStop();
	//	}
	//	@Override
	//	public void onStart() {
	//		LogUtils.e("devicelist", "onStart");
	//		// TODO Auto-generated method stub
	//		super.onStart();
	//	}
	@Override
	public void onDetach() {
		// TODO Auto-generated method stub
		//LogUtils.e("devicelist", "onDetach");
		super.onDetach();
		isDetach=true;
		EventBus.getDefault().unregister(this);
		//LogUtils.v("device", "device onDetached");
	}
	@Override
	public void onAttach(Activity activity){
		super.onAttach(activity);
		//LogUtils.e("devicelist", "onAttach");
		//LogUtils.v("device", "device onAttached");
		EventBus.getDefault().register(this);
		isDetach=false;
	}

	public PWDevAdapter getAdapter(){
		return m_deviceAdapter;
	}
	public DeviceListFragment(){
		//m_handler=new Handler();

		smSys=ShowmoSystem.getInstance();
		m_playHelper=smSys.getPlayer();
		mHideFlowWindowTimer=new PwTimer(false) {
			@Override
			public void doInTask() {
				// TODO Auto-generated method stub
				MyWindowManager.removeSmallWindow(m_activity);
			}
		};
	}
	public enum DialogEnum{
		longpress,delete,rename
	}
	private void showDialog(DialogEnum type)//"longpress" m_longPressDialog "delete" m_deleteDialog "rename" m_renameDialog
	{
		if(type==DialogEnum.longpress){
			if(m_longPressDialog==null){
				m_longPressDialog=new PwInfoDialog(m_activity);
				m_longPressDialog.setOkBtnTextAndListener(null, new OnOkClickListener() {
					@Override
					public void onClick() {
						// TODO Auto-generated method stub
						RadioGroup rg=(RadioGroup)m_longPressDialog.findViewById(R.id.dev_operate_group);
						int radioBtn=rg.getCheckedRadioButtonId();
						if(radioBtn==R.id.radio_btn_delete){
							showDialog(DialogEnum.delete);
						}else if (radioBtn==R.id.radio_btn_rename) {
							showDialog(DialogEnum.rename);
						}
					}
				})
				.setDialogContentView(R.layout.dialog_content_dev_long_press);
			}
			m_longPressDialog.show();
		}else if (type==DialogEnum.delete) {
			if(m_deleteDialog==null){
				m_deleteDialog=new PwInfoDialog(m_activity);
				m_deleteDialog.setContentText(R.string.present_camera_will_be_unbind)
				.setOkBtnTextAndListener(null, new OnOkClickListener() {
					@Override
					public void onClick() {
						// TODO Auto-generated method stub
						LogUtils.v("delete", m_deviceAdapter.getCurData().toString());
						Device deleDev=m_deviceAdapter.getCurData();


						//删除告警信息

						List<Alarm> alarmList=DaoFactory.getAlarmDao(m_activity).queryAllByDeviceId(deleDev.getmDeviceId());
						Log.e("alarmList", "111111111:："+alarmList.size());
						if (alarmList!=null&&alarmList.size()>0) {
							for (int j = 0; j < alarmList.size(); j++) {
								int recordID=alarmList.get(j).getRecordId();
								//Log.e("111111111", "111111111:："+recordID);
								String alarmImgPath=Environment.getExternalStorageDirectory().getAbsolutePath();
								alarmImgPath+="/alarmImg/"+recordID+".jpg";
								File fileimg=new File(alarmImgPath);
								//Log.e("alarmImgPath", "111111111:："+alarmImgPath);
								//删除本地图片
								fileimg.delete();
							}

						}
						DaoFactory.getAlarmDao(m_activity).RemoveByDeviceId(deleDev.getmDeviceId());


						Device curPlayDev=m_playHelper.getmCurDeviceInfo();
						if(curPlayDev!=null){
							if(deleDev.getmCameraId()==curPlayDev.getmCameraId()){
								m_playHelper.stop();
							}
						}
						m_activity.showLoadingDialog();
						deleteDev(deleDev,new RxCallback<Void>() {
							@Override
							public void onNext(Void t) {
								// TODO Auto-generated method stub
								m_activity.closeLoadingDialog();
								super.onNext(t);
								Intent intent=new Intent(User.DEVICE_REMOVE_ACTION);
								m_activity.sendBroadcast(intent);
							}
							@Override
							public void onError(Throwable e) {
								// TODO Auto-generated method stub
								super.onError(e);
								m_activity.closeLoadingDialog();
								boolean breserr=((MainActivity)m_activity).handleNetConnectionError((int)JniClient.PW_NET_GetLastError());
								if(!breserr){
									ToastUtil.toastShort(m_activity, "删除设备失败");
									LogUtils.e("remove", "remove err errcode:"+JniClient.PW_NET_GetLastError());
								}
							}
						});

					}
				});
			}
			m_deleteDialog.show();
		}else if (type==DialogEnum.rename) {
			if(m_renameDialog==null){
				m_renameDialog=new PwInfoDialog(m_activity);
				m_renameDialog.setDialogTitle(R.string.add_device_name_device_dialog_title);
				m_renameDialog.setInputMode(true,"")
				.setContentText(R.string.present_camera_will_be_unbind)
				.setOkBtnTextAndListener(null, new OnOkClickListener() {
					@Override
					public void onClick() {
						// TODO Auto-generated method stub
						//LogUtils.v("rename", "onClick");
						Device dev=m_deviceAdapter.getCurData();
						DeviceUseUtils utils=new DeviceUseUtils(dev);
						utils.deviceRename(m_renameDialog.getInputText(),new RxCallback<Boolean>() {

							@Override
							public void onNext(Boolean result) {
								// TODO Auto-generated method stub
								ToastUtil.toastShort(m_activity, R.string.device_rename_suc);
								m_deviceAdapter.notifyDataSetChanged();
							}
							@Override
							public void onError(Throwable result) {
								// TODO Auto-generated method stub
								if (result instanceof NetErrInfo) {
									NetErrInfo info=(NetErrInfo)result;
									m_activity.handleNetConnectionError((int)info.netErrCode);
									return;
								}
								ToastUtil.toastShort(m_activity, R.string.device_rename_fai);
							}
						});
						Intent intent=new Intent(Device.DEVICE_RENAME_ACTION);
						m_activity.sendBroadcast(intent);
					}
				});

			}
			m_renameDialog.setInputText("");

			m_renameDialog.show();
		}
	}

	private void deleteDev(final Device dev,final RxCallback<Void> retcb){

		Observable.create(new OnSubscribe<Void>() {
			@Override
			public void call(Subscriber<? super Void> t) {
				// TODO Auto-generated method stub
				boolean bres=ShowmoSystem.getInstance().getCurUser().unbindDevice(dev);
				if(!bres){
					retcb.onError(new Throwable());
				}else{
					retcb.onNext(null);
				}
			}
		}).subscribeOn(Schedulers.io())
		.map(new Func1<Void, Void>() {
			@Override
			public Void call(Void t) {
				// TODO Auto-generated method stub
				DaoFactory.getAlarmDao(m_activity).RemoveByDeviceId(dev.getmDeviceId());
				DaoFactory.getDeviceDao(m_activity).RemoveByUniqueId(dev.getUniqueId());
				LogUtils.e("unbind", "DaoFactory.getDeviceDao over");
				return null;
			}
		}).observeOn(AndroidSchedulers.mainThread())
		.subscribe(retcb);
	}


	public void setAdapter(PWDevAdapter adapter){
		if(adapter==null){
			return;
		}
		resizeDeviceList(adapter.getCount());
		m_deviceAdapter=adapter;
		m_deviceAdapter.setCurItemBackgroundDrawable(m_activity.getResources().getDrawable(R.drawable.device_selected_item));
		m_deviceAdapter.registerDataSetObserver(new DataSetObserver() {
			public void onChanged() {
				// Do nothing
				if(!isDetach){
					resizeDeviceList(m_deviceAdapter.getCount());
				}
			}
		});
		//LogUtils.e("curItem", "deviceList.setAdapter hashcode:"+hashCode());
		deviceList.setAdapter(adapter);
		m_deviceAdapter.setCurItem(0);
	}
	public void resizeDeviceList(int deviceCount){
		float size;
		if(deviceCount <=1){
			size=deviceCount*1.1f;
		}else {
			size=0.4f + deviceCount;
		}
		float gridWid = size
				* this.getResources().getDimension(
						R.dimen.dimen_device_item_width);
		//LogUtils.v("grid", "gridWid " + (int) gridWid);
		//LogUtils.v("grid", "size " + deviceCount);

		LinearLayout.LayoutParams lparam = new LinearLayout.LayoutParams(
				(int) gridWid, HexTrans.dip2px(m_activity, 120));
		deviceList.setLayoutParams(lparam);
	}
	@Override
	public void onActivityCreated(Bundle savedInstaceState) {
		// TODO Auto-generated method stub
		super.onActivityCreated(savedInstaceState);

		builder =new AlertDialog.Builder(m_activity);
		deviceList = (GridView) m_activity.findViewById(R.id.device_list_fragment);
		m_addBtn=(RelativeLayout)m_activity.findViewById(R.id.add_device_ly);
		m_addBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent intent=new Intent(m_activity,AddDeviceUserEnsure.class);
				m_activity.startActivity(intent);
				AnimUtil.slideInFromRight(m_activity);
			}
		});

		deviceList.setOnItemClickListener(new PwDeviceItemClickListener());
		deviceList.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
			public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id){
				m_deviceAdapter.setCurItem(position);
				showDialog(DialogEnum.longpress);
				return true;
			}
		});
		if (m_deviceAdapter!=null) {
			setAdapter(m_deviceAdapter);
		}
		if(showmoSystem.getCurUser().isExperience()){
			experienceUserChange();
		}

		//builder.setTitle("提示").setMessage("请把设备更新到最新版本！").setPositiveButton("确定"
	}
	public void onEventMainThread(DeviceAddEvent ev){
		//LogUtils.v("EventReciever", "DeviceAddEvent onEventMainThread ");
		try {
			m_playHelper.realplay(ev.device, (OnRealdataCallBackListener)m_activity);
		} catch (StopPlayingDeviceException e) {
			// TODO: handle exception]
			e.printStackTrace();
			ToastUtil.toastShort(m_activity, R.string.stop_play_err);
		}catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
	private void experienceUserChange(){
		m_addBtn.setVisibility(View.GONE);
		deviceList.setOnItemLongClickListener(null);
	}
	private void canCloseDialog(DialogInterface dialogInterface, boolean close) {  
		try {  
			Field field = dialogInterface.getClass().getSuperclass().getDeclaredField("mShowing");  
			field.setAccessible(true);  
			field.set(dialogInterface, close);  
		} catch (Exception e) {  
			e.printStackTrace();  
		}  
	}  

	private class PwDeviceItemClickListener implements AdapterView.OnItemClickListener{
		@Override
		public void onItemClick(AdapterView<?> arg0,View arg1,int position,long rowId){


			SharedPreferences pref=PreferenceManager.getDefaultSharedPreferences(m_activity);
			boolean bWifi=pref.getBoolean("wifi_check", false);
			if(bWifi){
				PwNetWorkHelper  netWorkHelper=PwNetWorkHelper.getInstance();
				if(!netWorkHelper.getWifiEnabled()){
					ToastUtil.toastShort(m_activity, R.string.add_device_wifi_unable);
					return;
				}else if(!netWorkHelper.getWifiConnectState()){
					ToastUtil.toastShort(m_activity, R.string.add_device_wifi_not_connect);
					return;
				}
			}

			final Device info=(Device)arg0.getItemAtPosition(position);
			//ToastUtil.toastShort(m_activity,"pos:"+position+" uuid "+info.getmUuid());
			if((!StringUtil.isNotEmpty(info.getmVersion())) ){//当前为最新版本
				//	Log.e("11111111111111", "1111111111112222222222224444444444");
				m_deviceAdapter.setCurItem(position);
				info.UseFreqIncrease();
				DaoFactory.getDeviceDao(m_activity).updateDevice(info);
				if(info.getmCameraId()<=0){
					return;
				}
				mHideFlowWindowTimer.stopIfStarted();
				String netType=getNetworkClassByType(getNetwork());
				if("2G".equals(netType) || "3G".equals(netType) || "4G".equals(netType)){
					MyWindowManager.createSmallWindow(m_activity);
					mHideFlowWindowTimer.start(3000, false);
				}

				try {
					m_playHelper.realplay(info, (OnRealdataCallBackListener)m_activity);
				} catch (StopPlayingDeviceException e) {
					// TODO: handle exception]
					e.printStackTrace();
					ToastUtil.toastShort(m_activity, R.string.stop_play_err);
				}
			}else{
				if(info.ismUpgrading()){
					showToastShort(R.string.is_upgrading);	
				}else{
					m_UpgradeDialog = new PwInfoDialog(m_activity);
					m_UpgradeDialog.setContentText(R.string.upgrade_ipc_tip);
					m_UpgradeDialog.removeCancelBtn();
					m_UpgradeDialog.setOkBtnTextAndListener(R.string.goto_upgrade, new OnOkClickListener() {
						@Override
						public void onClick() {
							// TODO Auto-generated method stub
							Intent intent = new Intent(m_activity, DeviceSettingActivity.class);
							intent.putExtra("INTENT_KEY_STRINGONE", info.getmCameraId());
							intent.putExtra("INTENT_KEY_STRINGTWO", info.getUniqueId());
							intent.putExtra("INTENT_KEY_STRINGTO_DEVICESETTING", "devicesetting");
							startActivity(intent);
						}
					});
					m_UpgradeDialog.addBtn(R.string.continue_play, new View.OnClickListener() {

						@Override
						public void onClick(View v) {
							// TODO Auto-generated method stub
							//ToastUtil.toastShort(m_activity,"cameraID:"+info.getmCameraId()+" uuid "+info.getmUuid());
							try {
								m_playHelper.realplay(info, (OnRealdataCallBackListener)m_activity);
							} catch (StopPlayingDeviceException e) {
								// TODO: handle exception]
								e.printStackTrace();
								ToastUtil.toastShort(m_activity, R.string.stop_play_err);
							}
						}
					});
					m_UpgradeDialog.show();
				}
			}
		}
	}



	static class NetworkType{
		public static final int NETWORK_TYPE_WIFI = 16;
		public static final int NETWORK_TYPE_UNKNOWN = 0;
		public static final int NETWORK_TYPE_GPRS = 1;
		public static final int NETWORK_TYPE_EDGE = 2;
		public static final int NETWORK_TYPE_UMTS = 3;
		public static final int NETWORK_TYPE_CDMA = 4;
		public static final int NETWORK_TYPE_EVDO_0 = 5;
		public static final int NETWORK_TYPE_EVDO_A = 6;
		public static final int NETWORK_TYPE_1xRTT = 7;
		public static final int NETWORK_TYPE_HSDPA = 8;
		public static final int NETWORK_TYPE_HSUPA = 9;
		public static final int NETWORK_TYPE_HSPA = 10;
		public static final int NETWORK_TYPE_IDEN = 11;
		public static final int NETWORK_TYPE_EVDO_B = 12;
		public static final int NETWORK_TYPE_LTE = 13;
		public static final int NETWORK_TYPE_EHRPD = 14;
		public static final int NETWORK_TYPE_HSPAP = 15;
	}

	private String getNetworkClassByType(int networkType){
		switch(networkType){
		case NetworkType.NETWORK_TYPE_WIFI:
			return "wifi";
		case NetworkType.NETWORK_TYPE_GPRS:
		case NetworkType.NETWORK_TYPE_EDGE:
		case NetworkType.NETWORK_TYPE_CDMA:
		case NetworkType.NETWORK_TYPE_1xRTT:
		case NetworkType.NETWORK_TYPE_IDEN:
			return "2G";
		case NetworkType.NETWORK_TYPE_EVDO_0:
		case NetworkType.NETWORK_TYPE_EVDO_A:
		case NetworkType.NETWORK_TYPE_UMTS:
		case NetworkType.NETWORK_TYPE_HSDPA:
		case NetworkType.NETWORK_TYPE_HSUPA:
		case NetworkType.NETWORK_TYPE_HSPA:
		case NetworkType.NETWORK_TYPE_EVDO_B:
		case NetworkType.NETWORK_TYPE_EHRPD:
		case NetworkType.NETWORK_TYPE_HSPAP:
			return "3G";
		case NetworkType.NETWORK_TYPE_LTE:
			return "4G";
		default:
			return "";

		}
	}

	private int getNetwork(){
		int networkType = 0;
		NetworkInfo network=((ConnectivityManager)m_activity.getSystemService(Context.CONNECTIVITY_SERVICE)).getActiveNetworkInfo();
		if(network!=null && network.isAvailable() && network.isConnected()){
			int type=network.getType();
			if(type==ConnectivityManager.TYPE_WIFI){
				networkType = NetworkType.NETWORK_TYPE_WIFI;
			}else if(type==ConnectivityManager.TYPE_MOBILE){
				//TelephonyManager telephonyManager=(TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
				//networkType = telephonyManager.getNetworkType();
				networkType = network.getSubtype();
			}
		}
		return networkType;
	}
	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub

		super.onDestroy();
	}
}
