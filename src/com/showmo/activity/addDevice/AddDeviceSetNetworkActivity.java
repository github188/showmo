package com.showmo.activity.addDevice;

import java.security.KeyStore.PrivateKeyEntry;

import com.showmo.R;
import com.showmo.activity.addDevice.WifiStateBroadcastReciever.OnWifiStateChanged;
import com.showmo.base.BaseActivity;
import com.showmo.util.LogUtils;
import com.showmo.util.PwNetWorkHelper;
import com.showmo.widget.PWEditCompoundView;
import com.showmo.widget.PWIpEditView;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.R.anim;
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
import android.os.Message;
import android.os.Parcelable;
import android.util.Log;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnCreateContextMenuListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import ipc365.app.showmo.jni.JniDataDef.Broadcast_wifi_info;

public class AddDeviceSetNetworkActivity extends BaseActivity {
	private TextView m_ssidName;
	private PWEditCompoundView m_ssidPswEdit;
	private Button m_nextBtn;
	private boolean m_pswVisibleFlag;
	private PwNetWorkHelper m_wifiHelper;
	private String m_keyType;
	private WifiStateBroadcastReciever m_WifiStateBroadcastReciever;
	public final static int ADDDEVICEQUESTCODE=10101;
	
	private static final int HANDLE_WIFI_CHANGED = 0;
	private static final int HANDLE_WIFI_DISABLED = 1;
	private Handler m_handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {// wifi改变，提示重新配置，改变之后检测是否是连接状态，wifi关闭则回退到上个页面
			if (HANDLE_WIFI_CHANGED == msg.what) {
				if (!m_wifiHelper.getWifiConnectState()) {
					return;
				}
				Toast.makeText(AddDeviceSetNetworkActivity.this,
						R.string.add_device_wifi_changed, Toast.LENGTH_SHORT)
						.show();
				m_ssidName.setText(m_wifiHelper.getCurSsid());
				m_keyType = m_wifiHelper.getCurWifiCapability();
				m_ssidPswEdit.setText("");
			}
		}
	};

	

	private void init() {
		m_pswVisibleFlag = false;
		//setBarTitleWithRightBtn(R.string.add_device_title_set_wifi);
		setBarTitle(R.string.add_device_title_set_wifi);
		findViewAndSet(R.id.btn_set_net_next);
		m_wifiHelper = PwNetWorkHelper.getInstance();

		m_ssidName = (TextView) findViewById(R.id.add_device_wifi_name);
		
		m_ssidPswEdit = (PWEditCompoundView) findViewById(R.id.add_device_wifi_psw);
		m_ssidPswEdit
				.setOnRightImgClickListener(new PWEditCompoundView.OnRightImgClickListener() {
					@Override
					public void onRightImgClickListener(View v) {
						// TODO Auto-generated method stub
						if (!m_pswVisibleFlag) {
							m_pswVisibleFlag = true;
							v.setBackgroundDrawable(getResources().getDrawable(
									R.drawable.login_psw_visible));
							m_ssidPswEdit.setPswVisible(true);
						} else {
							m_pswVisibleFlag = false;
							v.setBackgroundDrawable(getResources().getDrawable(
									R.drawable.login_psw_invisible));
							m_ssidPswEdit.setPswVisible(false);
						}
					}
				});
		m_WifiStateBroadcastReciever = new WifiStateBroadcastReciever(this);
		m_WifiStateBroadcastReciever
				.setOnWifiStateChanged(new MyOnWifiStateChanged());
		
	}

	public class MyOnWifiStateChanged implements OnWifiStateChanged {
		public void onWifiEnableChanged(boolean enbaled) {

		}
		public void onWifiConnectivityChanged(boolean bConnected) {
			if (bConnected) {
				LogUtils.v("wifi", "onWifiConnectivityChanged");
				if (!m_wifiHelper.getCurSsid().equals(m_ssidName.getText())) {
					Message msg = m_handler.obtainMessage();
					msg.what = HANDLE_WIFI_CHANGED;
					m_handler.sendMessage(msg);
				}
			}
		}
	}

	private class MyWifiErrOkListener implements OnOkClickListener{
		public void onClick(){
			if(!m_wifiHelper.getWifiEnabled() || !m_wifiHelper.getWifiConnectState()){
				finish();
			}else{
				workInit();
			}
		}
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "setwifi onCreate");
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_add_device_set_network);
		this.init();
		
	}
	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "setwifi onRestoreInstanceState");
		super.onRestoreInstanceState(savedInstanceState);
	}
	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "setwifi onRestart");
		super.onRestart();
	}
	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "setwifi onStart");
		super.onStart();
		if(!m_wifiHelper.getWifiEnabled()){
			PwInfoDialog dialog=new PwInfoDialog(this);
			dialog.setContentText(R.string.add_device_wifi_unable);
			dialog.setCancelable(false);
			dialog.removeCancelBtn();
			dialog.setOkBtnTextAndListener(null,new MyWifiErrOkListener());
			dialog.show();
			return;
		}
		if(!m_wifiHelper.getWifiConnectState()){
			PwInfoDialog dialog=new PwInfoDialog(this);
			dialog.setCancelable(false);
			dialog.setContentText(R.string.add_device_wifi_not_connect);
			dialog.removeCancelBtn();
			dialog.setOkBtnTextAndListener(null,new MyWifiErrOkListener());
			dialog.show();
			return;
		}
		workInit();
	}
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "setwifi onResume");
		super.onResume();
	}
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "setwifi onSaveInstanceState");
		super.onSaveInstanceState(outState);
	}
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "setwifi onPause");
		super.onPause();
	}
	@Override
	protected void onStop() {
		LogUtils.v("activity", "setwifi onStop");
		// TODO Auto-generated method stub
		super.onStop();
		m_WifiStateBroadcastReciever.unregisterSelf();
	}
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "setwifi onDestroy");
		super.onDestroy();
		PwNetWorkHelper.destoryInstance();
	}
	private void workInit(){
		m_WifiStateBroadcastReciever.registerSelf();
		String CurSsid = m_wifiHelper.getCurSsid();
		m_keyType = m_wifiHelper.getCurWifiCapability();
		m_ssidName.setText(CurSsid);
		m_ssidPswEdit.setText("");
	}
	
	

	@Override
	protected void onClick(int viewId) {
		// TODO Auto-generated method stub
		super.onClick(viewId);
		switch (viewId) {
		case R.id.btn_bar_back:
			onBackPressed();
			break;
		case R.id.btn_set_net_next:
			String ssid = m_ssidName.getText().toString();
			String ssidPsw = m_ssidPswEdit.getEditText().getText().toString();
			if (!m_wifiHelper.getWifiEnabled()) {
				PwInfoDialog dialog = new PwInfoDialog(this);
				dialog.removeCancelBtn();
				dialog.setContentText(R.string.add_device_wifi_unable);
				dialog.setOkBtnTextAndListener(null, null);
				dialog.show();
				return;
			}
			if (!m_wifiHelper.getWifiConnectState()) {
				PwInfoDialog dialog = new PwInfoDialog(this);
				dialog.removeCancelBtn();
				dialog.setContentText(R.string.add_device_wifi_not_connect);
				dialog.setOkBtnTextAndListener(null, null);
				dialog.show();
				return;
			}
			Intent intent = new Intent(this,
					AddDeviceConfigSearchActivity.class);
			intent.putExtra(AddDeviceConfigSearchActivity.SSID, ssid);
			intent.putExtra(AddDeviceConfigSearchActivity.PSW, ssidPsw);
			intent.putExtra(AddDeviceConfigSearchActivity.KEYTYPE, m_keyType);
			this.startActivityForResult(intent, ADDDEVICEQUESTCODE);
			slideInFromRight();
			LogUtils.v("add", ssid + "  " + ssidPsw);
			break;
		default:
			break;
		}
	}
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		LogUtils.v("activity", "setwifi onActivityResult");
		if(requestCode==ADDDEVICEQUESTCODE){
			switch (resultCode) {
			case AddDeviceConfigSearchActivity.RESULTCODE_ADDSUC:
			case AddDeviceConfigSearchActivity.RESULTCODE_ADDDEFEAT:
			case AddDeviceConfigSearchActivity.RESULTCODE_ADDBOUNDBYOTHER:	
				finish();
				break;
			case AddDeviceConfigSearchActivity.RESULTCODE_ADDNETCHANGE:
				if(!m_wifiHelper.getWifiEnabled()){//如果在onstart之前启动
					finish();
				}else if(!m_wifiHelper.getWifiConnectState()){
					finish();
				}else {//切换网络的话，不用处理什么，onstart会处理
					
				}
				break;
			case AddDeviceConfigSearchActivity.RESULTCODE_ADDWIFISETERR:
				break;
			default:
				break;
			}
			
		}
		super.onActivityResult(requestCode, resultCode, data);
	}
	
}
