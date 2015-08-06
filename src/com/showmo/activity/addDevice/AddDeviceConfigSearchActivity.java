package com.showmo.activity.addDevice;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.mail.internet.NewsAddress;

import rx.Observable;
import rx.Subscriber;
import rx.Observable.OnSubscribe;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.schedulers.Schedulers;

import com.showmo.DeviceListFragment;
import com.showmo.R;
import com.showmo.activity.addDevice.WifiStateBroadcastReciever.OnWifiStateChanged;
import com.showmo.addHelper.AddProcess;
import com.showmo.addHelper.IAddListener;
import com.showmo.addHelper.IAddProcess;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoSystem;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.event.DevOnlineStateEvent;
import com.showmo.event.DeviceAddEvent;
import com.showmo.eventBus.Event;
import com.showmo.eventBus.EventBus;
import com.showmo.network.NetworkHelper;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.rxcallback.RxCallback;
import com.showmo.userManage.User;
import com.showmo.util.LogUtils;
import com.showmo.util.PwNetWorkHelper;
import com.showmo.util.PwTimer;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;
import com.showmo.util.safelist.IFindSubcriber;
import com.showmo.util.safelist.IFindSubcriber1;
import com.showmo.util.safelist.SafeList;
import com.showmo.widget.PwRoundProgressBar;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnCancelClickListener;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.R.integer;
import android.R.interpolator;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IInterface;
import android.os.Looper;
import android.os.Message;
import android.os.Parcelable;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.Broadcast_wifi_info;

public class AddDeviceConfigSearchActivity extends BaseActivity implements
OnClickListener {
	private WifiStateBroadcastReciever m_WifiStateBroadcastReciever;
	private String m_ssid;
	private String m_psw;
	private String m_keyType;
	public static final String SSID = "ssid";
	public static final String PSW = "psw";
	public static final String KEYTYPE = "keyType";

	private TextView mPreTextView;
	private TextView mSearchTextView;

	public final static int RESULTCODE_ADDSUC = 1;
	public final static int RESULTCODE_ADDDEFEAT = 2;
	public final static int RESULTCODE_ADDNETCHANGE = 3;
	public final static int RESULTCODE_ADDWIFISETERR = 4;
	public final static int RESULTCODE_ADDBOUNDBYOTHER=5;

	private User m_curUser;
	//private 	Device dev;

	private PwInfoDialog m_wifiErrDialog;
	private PwRoundProgressBar m_ProgressBar;
	private  PwInfoDialog m_renameDialog=null;
	private IAddProcess mAddProcess;
	private PwTimer m_setTimer;
	private Button m_CompleteBtn;
	//	private TextView m_counterText;

	private boolean isRearching=false;//是否当前是在进行重新搜索的操作
	public static final int SETTIME = 100;
	public static final int RESEARCHTIME=20;
	private static final int HANDLE_WIFI_CHANGED = 0;
	private static final int HANDLE_ADD_FAI = 1;
	private static final int HANDLE_ADD_SUC = 2;
	private static final int HANDLE_PROGRESS = 3;
	private static final int HANDLE_BINDERR = 4;
	private static final int HANDLE_DEV_COMP=5;
	private static final int HANDLE_RESEARCH_ERR=6;
	private static final int HANDLE_BIND_BY_OTHER=7;

	private Handler m_handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case HANDLE_WIFI_CHANGED:
				mAddProcess.exitWork();
				m_setTimer.stopIfStarted();
				m_wifiErrDialog.show();
				break;
			case HANDLE_RESEARCH_ERR:
				ToastUtil.toastShort(AddDeviceConfigSearchActivity.this,  "添加设备失败");
				setResult(RESULTCODE_ADDSUC);
				finish();
				break;
			case HANDLE_ADD_FAI:
				Toast.makeText(AddDeviceConfigSearchActivity.this, "添加设备失败",
						Toast.LENGTH_SHORT).show();
				Intent intent = new Intent(AddDeviceConfigSearchActivity.this,
						AddDeviceConfigFailuredActivity.class);
				startActivityForResult(intent, 0);

				break;
			case HANDLE_BINDERR:
				Toast.makeText(AddDeviceConfigSearchActivity.this, "绑定失败",
						Toast.LENGTH_SHORT).show();
				Intent intent1 = new Intent(AddDeviceConfigSearchActivity.this,
						AddDeviceConfigFailuredActivity.class);
				startActivityForResult(intent1, 0);
				break;
			case HANDLE_ADD_SUC:
				/*Toast.makeText(AddDeviceConfigSearchActivity.this, "添加设备成功",
						Toast.LENGTH_SHORT).show();
				Intent intentAddSuc = new Intent(User.DEVICE_ADD_ACTION);
				sendBroadcast(intentAddSuc);
				setResult(RESULTCODE_ADDSUC);
				finish();*/
				break;

			case HANDLE_PROGRESS:
				m_ProgressBar.setProgress(msg.arg1);
				break;
			case HANDLE_DEV_COMP:
				/*				String counttext=getString(R.string.add_device_config_device_counter);
				counttext=counttext+" "+m_sucDevs.size();
				m_counterText.setText(counttext);*/
				/*			if(m_sucDevs.size()==0){
					m_CompleteBtn.setVisibility(View.GONE);
				}else {
					m_CompleteBtn.setVisibility(View.VISIBLE);
				}*/
				m_WifiStateBroadcastReciever.unregisterSelf();
				showDialog((Device)msg.obj);
				/*Toast.makeText(AddDeviceConfigSearchActivity.this, "添加设备成功",
						Toast.LENGTH_SHORT).show();
				Intent intentAddSuc = new Intent(User.DEVICE_ADD_ACTION);
				sendBroadcast(intentAddSuc);
				setResult(RESULTCODE_ADDSUC);
				finish();*/
				break;
			case HANDLE_BIND_BY_OTHER:
				showDevBoundByOther((OtherBindDevInfo)msg.obj);
				break;
			default:
				break;
			}
		}
	};

	private void init() {

		m_CompleteBtn=setBarTitleWithRightBtn(R.string.add_device_config_title, R.string.add_device_complete);
		m_CompleteBtn.setVisibility(View.GONE);
		((TextView) findViewById(R.id.btn_bar_back)).setOnClickListener(this);
		m_ProgressBar = (PwRoundProgressBar) findViewById(R.id.add_device_config_progress);

		m_WifiStateBroadcastReciever = new WifiStateBroadcastReciever(this);
		m_WifiStateBroadcastReciever
		.setOnWifiStateChanged(new MyWifiStateChanged());
		m_wifiErrDialog = new PwInfoDialog(this);
		m_wifiErrDialog.setCancelable(false);
		m_wifiErrDialog.removeCancelBtn();
		m_wifiErrDialog
		.setContentText(R.string.add_device_wifi_changed_config_err);
		m_wifiErrDialog.setOkBtnTextAndListener(null, new OnOkClickListener() {

			@Override
			public void onClick() {
				// TODO Auto-generated method stub
				setResult(RESULTCODE_ADDNETCHANGE);
				finish();
				slideInFromLeft();
			}
		});
		m_curUser = ShowmoSystem.getInstance().getCurUser();
		m_setTimer = new SetTimer();
		mPreTextView=(TextView)findViewById(R.id.presearch_text);
		mSearchTextView=(TextView)findViewById(R.id.search_text);
		mAddProcess=new AddProcess();
		mAddProcess.setAddListener(new PwAddDevListener());
	}

	private Device mSucDevice;
	private class OtherBindDevInfo{
		String uuid;
		String user;
		OtherBindDevInfo(String uuid,String user){
			this.uuid=uuid;
			this.user=user;
		}
	}
	public class PwAddDevListener implements IAddListener{
		@Override
		public void addedByOther(String uuid,String user) {
			// TODO Auto-generated method stub
			mAddProcess.pauseWork();
			m_setTimer.stopIfStarted();
			PostMsgToMainHandler(HANDLE_BIND_BY_OTHER,new OtherBindDevInfo(uuid,user));
		}
		@Override
		public void addedSuccess(Device dev) {
			// TODO Auto-generated method stub
			mAddProcess.pauseWork();
			m_setTimer.stopIfStarted();
			PostMsgToMainHandler(HANDLE_DEV_COMP,dev);
			mSucDevice=dev;
		}
	}
	private class MyWifiStateChanged implements OnWifiStateChanged {
		public void onWifiEnableChanged(boolean enbaled) {
			if (!enbaled) {
				if (!m_wifiErrDialog.isShowing()) {
					Message msg = m_handler.obtainMessage();
					msg.what = HANDLE_WIFI_CHANGED;
					m_handler.sendMessage(msg);
				}
			}
		}
		public void onWifiConnectivityChanged(boolean bConnected) {
			if (!bConnected) {
				if (!m_wifiErrDialog.isShowing()) {
					Message msg = m_handler.obtainMessage();
					msg.what = HANDLE_WIFI_CHANGED;
					m_handler.sendMessage(msg);
				}
			}
		}
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "config onCreate");
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_add_device_config_and_search);
		this.init();
		Intent intent = getIntent();

		m_ssid = intent.getStringExtra(SSID);
		m_psw = intent.getStringExtra(PSW);
		m_keyType = intent.getStringExtra(KEYTYPE);
		if (m_ssid == null || m_keyType == null) {
			m_ssid = savedInstanceState.getString(SSID);
			m_psw = savedInstanceState.getString(PSW);
			m_keyType = savedInstanceState.getString(KEYTYPE);
		}
		if (!StringUtil.isNotEmpty(m_ssid) || !StringUtil.isNotEmpty(m_keyType)) {
			Toast.makeText(this, R.string.add_device_wifi_info_exception,
					Toast.LENGTH_SHORT).show();
			setResult(RESULTCODE_ADDWIFISETERR);
			finish();
			slideInFromLeft();
		}

		m_WifiStateBroadcastReciever.registerSelf();
		//EventBus.getDefault().register(this);
		m_setTimer = new SetTimer();
		mAddProcess.beginWork(m_ssid, m_psw, m_keyType);
		m_setTimer.start(1000,true);
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "config onRestoreInstanceState");
		super.onRestoreInstanceState(savedInstanceState);
	}

	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "config onRestart");
		super.onRestart();
	}

	@Override
	protected void onStart() {
		LogUtils.v("activity", "config onStart");
		super.onStart();	
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "config onResume");
		super.onResume();
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "config onSaveInstanceState");

		super.onSaveInstanceState(outState);
		outState.putString(SSID, m_ssid);
		outState.putString(PSW, m_psw);
		outState.putString(KEYTYPE, m_keyType);
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "config onPause");
		super.onPause();
	}

	@Override
	protected void onStop() {
		LogUtils.v("activity", "config onStop");
		super.onStop();
	};

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "config onDestroy");
		m_WifiStateBroadcastReciever.unregisterSelf();
		//EventBus.getDefault().unregister(this);
		mAddProcess.exitWork();
		m_setTimer.stopIfStarted();
		super.onDestroy();
	}
	private int mSetTime = 0;
	private class SetTimer extends PwTimer {
		//	private int mSetTime = 0;
		public SetTimer() {
			super(true);
		}
		public void doInTask() {
			int timercount=SETTIME;
			if(isRearching){
				timercount=RESEARCHTIME;
			}	
			mSetTime++;
			Message msg1 = m_handler.obtainMessage();
			msg1.what = HANDLE_PROGRESS;
			//LogUtils.e("progress", "mSetTime "+mSetTime+" timercount "+timercount+" prog "+(mSetTime*100/timercount));
			msg1.arg1 = mSetTime*100/timercount;
			m_handler.sendMessage(msg1);

			if (mSetTime >= timercount) {
				mAddProcess.pauseWork();
				m_setTimer.stopIfStarted();
				if(mSucDevice==null){
					if(isRearching){
						m_handler.sendEmptyMessage(HANDLE_RESEARCH_ERR);
					}else{
						m_handler.sendEmptyMessage(HANDLE_ADD_FAI);
					}
				}else{
					m_handler.sendEmptyMessage(HANDLE_ADD_SUC);
				}
			}
		}
	}

	private void onMyBackPressed() {
		mAddProcess.pauseWork();
		m_setTimer.stopIfStarted();
		PwInfoDialog dialog = new PwInfoDialog(this);
		dialog.setOkBtnTextAndListener(null, new OnOkClickListener() {
			public void onClick() {
				setResult(RESULTCODE_ADDWIFISETERR);
				finish();
				slideInFromLeft();
			}
		});

		dialog.setCancelBtnTextAndListener(null, new OnCancelClickListener() {

			@Override
			public void onClick() {
				// TODO Auto-generated method stub
				mAddProcess.continueWork();
				m_setTimer.start(1000,true);
			}
		});
		dialog.setContentText(R.string.add_device_config_back);
		dialog.show();
	}


	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			LogUtils.v("wifi", "onMyBackPressed");
			onMyBackPressed();
		}
		return true;
	};
	private void PostMsgToMainHandler(int msgid,Object obj){
		Message msg=m_handler.obtainMessage();
		msg.what=msgid;
		msg.obj=obj;
		msg.sendToTarget();
	}
	private void showDevBoundByOther(final OtherBindDevInfo info){
		PwInfoDialog BeBoundOther=new PwInfoDialog(this);
		BeBoundOther.setCancelable(false);
		String strInfo=getString(R.string.add_device_bound_by_other);
		BeBoundOther.removeCancelBtn();

		BeBoundOther.setContentText(String.format(strInfo, info.uuid));
		BeBoundOther.addBtn(R.string.add_device_quit, new View.OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				LogUtils.e("click", "setCancelBtnTextAndListener");
				mAddProcess.exitWork();
				m_setTimer.stopIfStarted();
				setResult(RESULTCODE_ADDBOUNDBYOTHER);
				finish();
			}
		});
		BeBoundOther.setOkBtnTextAndListener(R.string.add_device_continue, new OnOkClickListener() {

			@Override
			public void onClick() {
				// TODO Auto-generated method stub
				LogUtils.e("click", "setOkBtnTextAndListener");
				//				isAddSuccess=false;
				//				mPreDevsInLan.add(uuid);
				//				continueWorker();
				mAddProcess.continueWork();
				m_setTimer.start(1000,true);
			}
		});
		BeBoundOther.show();
	}
	private void showDialog(final Device dev){
		LogUtils.v("dialog", "showdialog "+dev.getmUuid());
		m_renameDialog=new PwInfoDialog(this);
		m_renameDialog.removeCancelBtn();
		m_renameDialog.setCancelable(false);
		m_renameDialog.setDialogTitle(R.string.add_device_name_device_dialog_title);
		m_renameDialog.setInputMode(true,"")
		.setContentText(R.string.present_camera_will_be_unbind)
		.setOkBtnTextAndListener(null, new OnOkClickListener() {
			@Override
			public void onClick() {
				if(!StringUtil.isNotEmpty(m_renameDialog.getInputText())){
					ToastUtil.toastShort(AddDeviceConfigSearchActivity.this, R.string.add_device_name_not_null);
					showDialog(dev);
					return;
				}
				renameDevice(dev);													
			}
		});
		m_renameDialog.show();
	}
	private int renamecount=0;
	private void renameDevice(final Device dev){
		final DeviceUseUtils utils=new DeviceUseUtils(dev);
		showLoadingDialog();
		utils.deviceRename(m_renameDialog.getInputText(), new RxCallback<Boolean>() {
			@Override
			public void onNext(Boolean t) {
				// TODO Auto-generated method stub
				super.onNext(t);
				closeLoadingDialog();
				Toast.makeText(AddDeviceConfigSearchActivity.this, "添加设备成功",
						Toast.LENGTH_SHORT).show();
				addDeviceOver(dev);
				finish();
			}
			@Override
			public void onError(Throwable e) {
				// TODO Auto-generated method stub
				super.onError(e);
				if(renamecount >=3){
					closeLoadingDialog();
					ToastUtil.toastShort(AddDeviceConfigSearchActivity.this, "命名失败了！！");
					addDeviceOver(dev);						
					finish();
				}else{
					renameDevice(dev);
					renamecount++;
				}	
				//ToastUtil.toastShort(AddDeviceConfigSearchActivity.this, "命名失败了！！");
				//addDeviceOver(dev);						
				//finish();
			}
		});	
	}

	private void addDeviceOver(Device dev){
		Intent intent=new Intent(Device.DEVICE_RENAME_ACTION);
		sendBroadcast(intent);
		Intent intentAddSuc = new Intent(User.DEVICE_ADD_ACTION);
		sendBroadcast(intentAddSuc);
		setResult(RESULTCODE_ADDSUC);
		LogUtils.e("addDeviceOver", "addDeviceOver uuid:"+dev.getmUuid()+" cameraid:"+dev.getmCameraId());
		EventBus.getDefault().post(new DeviceAddEvent(dev));
		finish();
	}


	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.btn_bar_back:
			onMyBackPressed();
			break;
		case R.id.btn_common_title_next:
			m_handler.sendEmptyMessage(HANDLE_ADD_SUC);
			break;
		default:
			break;
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "config onActivityResult " + resultCode);
		switch (resultCode) {
		case AddDeviceConfigFailuredActivity.RESULTCODE_ADDDEFEAT:
			setResult(RESULTCODE_ADDDEFEAT);
			finish();
			slideInFromLeft();
			break;
		case AddDeviceConfigFailuredActivity.RESULTCODE_ADDWIFISETERR:
			setResult(RESULTCODE_ADDWIFISETERR);
			finish();
			slideInFromLeft();
			break;
		case AddDeviceConfigFailuredActivity.RESULTCODE_RESEARCH:
			mSetTime=0;
			isRearching=true;
			//			m_devsWaitForAdd.clear();
			//			startConfig();
			m_ProgressBar.setProgress(0);
			mAddProcess.continueWork();
			m_setTimer.start(1000,true);
			break;
		default:
			break;
		}
	}

}
