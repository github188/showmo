package com.showmo.activity.addDevice;

import com.showmo.util.LogUtils;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.net.wifi.WifiManager;
import android.os.Parcelable;
import android.util.Log;

public class WifiStateBroadcastReciever extends BroadcastReceiver {
	
	private Context bContext;
	private OnWifiStateChanged m_WifiStateChanged=null;
	boolean bRegister=false;
	public WifiStateBroadcastReciever(Context context){
		bContext=context;
		
	}
	public void unregisterSelf(){
		if(bRegister){
			bRegister=false;
			try {
				bContext.unregisterReceiver(this);
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
		}
		
	}
	public void registerSelf(){
		IntentFilter filter=new IntentFilter();
		filter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
		filter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
		LogUtils.v("wifi", "registerSelf");
		bContext.registerReceiver(this, filter);
		bRegister=true;
	}
	
	public void setOnWifiStateChanged(OnWifiStateChanged lis){
		m_WifiStateChanged=lis;
	}
	public interface OnWifiStateChanged{
		void onWifiEnableChanged(boolean enbaled);
		void onWifiConnectivityChanged(boolean bConnected);
	}
	@Override
	public void onReceive(Context context, Intent intent) {
		// TODO Auto-generated method stub
		if (WifiManager.WIFI_STATE_CHANGED_ACTION.equals(intent.getAction())) {
			int wifistate=intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, -1);
			switch (wifistate) {
			case WifiManager.WIFI_STATE_DISABLED:	
				if(m_WifiStateChanged!=null){
					LogUtils.v("wifi", "onWifiEnableChanged");
					m_WifiStateChanged.onWifiEnableChanged(false);
				}
				break;
			case WifiManager.WIFI_STATE_ENABLED:
				if(m_WifiStateChanged!=null){
					m_WifiStateChanged.onWifiEnableChanged(true);
				}
				break;
			default:
				break;
			}
		}
		
		if (WifiManager.NETWORK_STATE_CHANGED_ACTION.equals(intent
				.getAction())) {
			Parcelable parcelableExtra = intent
					.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);
			if (null != parcelableExtra) {
				NetworkInfo networkInfo = (NetworkInfo) parcelableExtra;
				State state = networkInfo.getState();
				boolean isConnected = (state == State.CONNECTED);// 当然，这边可以更精确的确定状态
				if(networkInfo.getType()==ConnectivityManager.TYPE_WIFI){
					if(m_WifiStateChanged!=null){
						m_WifiStateChanged.onWifiConnectivityChanged(isConnected);
					}
				}
			}
		}
	}
	
}
