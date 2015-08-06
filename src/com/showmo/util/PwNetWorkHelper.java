package com.showmo.util;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

import com.showmo.base.ShowmoApplication;

import android.R.interpolator;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiConfiguration.KeyMgmt;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.IBinder;
import android.os.Parcelable;
import android.util.Log;

public class PwNetWorkHelper {
	private Context m_appContext;

	private static PwNetWorkHelper m_instance = null;

	private PwNetWorkHelper(Context context) {
		m_appContext = context;
		init();
	}

	public synchronized static PwNetWorkHelper getInstance() {
		if (m_instance == null) {
			m_instance = new PwNetWorkHelper(ShowmoApplication.getInstance());
		}
		return m_instance;
	}

	public synchronized static void destoryInstance() {
		if (m_instance != null) {
			// m_instance.m_appContext.unregisterReceiver(m_instance.m_wifiReceiver);
			m_instance = null;
		}
	}

	//private WifiSateReceiver m_wifiReceiver;
	private WifiManager m_wm;
	private WifiScanResultListener m_scanResultHolderListener;
	private WifiSwitchListener m_switchListener;
	private ConnectivityManager m_cm;
	private Timer m_switchTimer;
	private TimerTask m_switchTimerTask;

	public interface WifiScanResultListener {
		void onScanResultListener(List<ScanResult> rl);
	}

	public void setScanResultListener(WifiScanResultListener listener) {
		m_scanResultHolderListener = listener;
	}

	public interface WifiSwitchListener {
		void onWifiSwitchListener(boolean bres);
	}

	public void setWifiSwitchListener(WifiSwitchListener listener) {
		m_switchListener = listener;
	}
	public String getIp()
	{
		int type = getNetType();
		switch (type) {
		case NET_TYPE_UNKOWN:
			return null;
		case NET_TYPE_WIFI:
			return getWifiIp();
		case NET_TYPE_MOBILE:
			return getNetMobileIp();
		default:
			break;
		}
		return null;
	}
	private String getWifiIp(){
		WifiInfo wInfo=m_wm.getConnectionInfo();
		int ipaddr=wInfo.getIpAddress();
		return intToIp(ipaddr);
	}
	private String intToIp(int i){
		return (i & 0xFF ) + "." +       
		        ((i >> 8 ) & 0xFF) + "." +       
		        ((i >> 16 ) & 0xFF) + "." +       
		        ( i >> 24 & 0xFF) ;
	}
	private String getNetMobileIp(){
		try  
        {  
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();)  
            {  
               NetworkInterface intf = en.nextElement();  
               for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();)  
               {  
                   InetAddress inetAddress = enumIpAddr.nextElement();  
                   if (!inetAddress.isLoopbackAddress() && inetAddress instanceof Inet4Address)  
                   {  
                       return inetAddress.getHostAddress().toString();  
                   }  
               }  
           }  
        }  
        catch (SocketException ex)  
        {  
            Log.e("WifiPreference IpAddress", ex.toString());  
        }  
        return null;  
	}
	
	public final static int NET_TYPE_UNKOWN=0;
	public final static int NET_TYPE_WIFI=1;
	public final static int NET_TYPE_MOBILE=2;
	public int getNetType(){
		NetworkInfo networkInfo=m_cm.getActiveNetworkInfo();
		if(networkInfo == null){
			return NET_TYPE_UNKOWN;
		}
		if (networkInfo.getType() == ConnectivityManager.TYPE_WIFI) {
			LogUtils.e("net", "Current net type == TYPE_WIFI");
			return NET_TYPE_WIFI;
		}
		if (networkInfo.getType() == ConnectivityManager.TYPE_MOBILE) {
			LogUtils.e("net", "Current net type == TYPE_MOBILE");
			return NET_TYPE_MOBILE;
		}
		return NET_TYPE_UNKOWN;
	}
	
	public void init() {
		LogUtils.v("PwNetWorkService", "onCreate");
		//m_wifiReceiver = new WifiSateReceiver();
		// IntentFilter intentFilter = new IntentFilter();
		// intentFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
		// intentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
		// intentFilter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
		// intentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
		// m_appContext.registerReceiver(m_wifiReceiver, intentFilter);
		m_wm = (WifiManager) m_appContext
				.getSystemService(Context.WIFI_SERVICE);
		m_cm=(ConnectivityManager)m_appContext
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		m_switchTimer = new Timer();

	}

	public WifiInfo getCurWifiInfo() {
		if (!getWifiConnectState()) {
			return null;
		}
		return m_wm.getConnectionInfo();
	}

	public String getCurSsid() {
		WifiInfo curInfo = m_wm.getConnectionInfo();
		return eraseStringQuotes(curInfo.getSSID());
	}

	public String getCurCipherType() {
		if (!getWifiConnectState()) {
			return null;
		}
		WifiInfo curInfo = m_wm.getConnectionInfo();
		return getCipherType(curInfo.getSSID());
	}

	public String getCurWifiCapability() {
		String capability=this.getCurCipherType();
		if(capability ==null){
			return null;
		}
		if (capability.contains("WEP")) {
			return "WEPAUTO";
		} else if (capability.contains("WPA-EAP")) {
			if (capability.contains("TKIP+CCMP") || capability.contains("CCMP")) {
				return "WPA";
			} else {
				return "WPA";
			}
		} else if (capability.contains("WPA2-EAP")) {
			if (capability.contains("TKIP+CCMP") || capability.contains("CCMP")) {
				return "WPA2";
			} else {
				return "WPA2";
			}

		} else if (capability.contains("WPA-PSK")) {
			if (capability.contains("TKIP+CCMP") || capability.contains("CCMP")) {
				return "WPA";
			} else {
				return "WPA";
			}
		} else if (capability.contains("WPA2-PSK")) {
			if (capability.contains("TKIP+CCMP") || capability.contains("CCMP")) {
				return "WPA2";
			} else {
				return "WPA2";
			}
		} else {
			return "OPEN";
		}
	}

	private String eraseStringQuotes(String target) {
		String strOut = target.trim();
		return strOut.replace("\"", "");
	}

	public String getCurWifiSecurity() {
		if (!getWifiConnectState()) {
			return null;
		}
		WifiInfo curInfo = m_wm.getConnectionInfo();
		String curSsid = eraseStringQuotes(curInfo.getSSID());
		List<WifiConfiguration> configList = m_wm.getConfiguredNetworks();
		for (WifiConfiguration configuration : configList) {
			if (configuration.SSID.contains(curSsid)) {
				return getSecurity(configuration);
			}
		}
		return null;
	}

	public String getSecurity(WifiConfiguration config) {// 安全性
	
		if (config.allowedKeyManagement.get(KeyMgmt.WPA_PSK)) {
			return new String("WPA_PSK");
		} else if (config.allowedKeyManagement.get(KeyMgmt.WPA_EAP)) {
			return new String("WPA_EAP");
		} else if (config.allowedKeyManagement.get(KeyMgmt.IEEE8021X)) {
			return new String("IEEE8021X");
		} else if (config.allowedKeyManagement.get(KeyMgmt.NONE)) {
			return new String("NONE");
		}
		return null;
	}

	public String getCipherType(String ssid) {
		List<ScanResult> list = m_wm.getScanResults();
		String curSsid = eraseStringQuotes(ssid);
		for (ScanResult scanResult : list) {
			// LogUtils.v("CipherType",scanResult.SSID+
			// "  "+scanResult.capabilities);
			if (scanResult.SSID.contains(curSsid)) {
				return scanResult.capabilities;
			}
		}
		return null;
	}

	public boolean addNetwork(String SSID, String Password) {// 添加一网络并连接
		LogUtils.v("PwNetWorkService", "addNetwork 1" + SSID + Password);
		boolean isFound = false, bRes = false;
		int wifiConfigurationId, i, iConfigureNetworkCount = 0;
		Vector<Integer> ConfigureNetwork = new Vector<Integer>();
		List<WifiConfiguration> wifiConfiList = m_wm.getConfiguredNetworks();
		for (i = 0; i < wifiConfiList.size(); i++) {
			String ssidtemp = "\"" + SSID + "\"";
			// LogUtils.v("PW_LOG","@addNetwork 2"+m_wifiManager.getConfiguredNetworks().get(i).SSID+" "+m_wifiManager.getConfiguredNetworks().get(i).BSSID);
			// LogUtils.v("PW_LOG","@addNetwork 2"+m_wifiManager.getConfiguredNetworks().get(i).status);
			if (wifiConfiList.get(i).SSID.equals(ssidtemp)) {// m_wifiConfigurationList出来的SSID带""引号
				ConfigureNetwork.addElement(new Integer(i));
			}
		}
		if (ConfigureNetwork.size() != 0) {
			isFound = true;
		}
		LogUtils.v("PW_LOG", "@addNetwork3 ssid " + SSID + "  " + Password + "  "
				+ isFound + " " + iConfigureNetworkCount);
		if (isFound) {
			// iTimes = iTimes/iConfigureNetworkCount;
			for (i = 0; i < ConfigureNetwork.size(); i++) {
				WifiConfiguration tmp = m_wm.getConfiguredNetworks().get(
						ConfigureNetwork.get(i));
				wifiConfigurationId = tmp.networkId;
				LogUtils.v("PW_LOG", "SSID " + tmp.SSID);
				try {
					boolean b = m_wm.enableNetwork(wifiConfigurationId, true);
				} catch (Exception ex) {
					LogUtils.v("PW_LOG", "@addNetwork 4 ");
					ex.printStackTrace();
				}
			}
		} else {
			LogUtils.v("PW_LOG", "@addNetwork 5");
			WifiConfiguration wifiConfiguration = createWifiInfo(SSID,
					Password, 3);
			wifiConfigurationId = m_wm.addNetwork(wifiConfiguration);
			boolean b = m_wm.enableNetwork(wifiConfigurationId, true);
		}
		m_switchTimerTask = new PWSwitchTimerTask();
		m_switchTimer.schedule(m_switchTimerTask, 15000);
		return bRes;
	}

	public WifiConfiguration createWifiInfo(String SSID, String Password,
			int Type) {
		WifiConfiguration wConfig = new WifiConfiguration();

		wConfig.allowedAuthAlgorithms.clear();
		wConfig.allowedGroupCiphers.clear();
		wConfig.allowedKeyManagement.clear();
		wConfig.allowedPairwiseCiphers.clear();
		wConfig.allowedProtocols.clear();

		wConfig.SSID = "\"" + SSID + "\"";
		// WifiConfiguration tempWConfig = this.isExists(SSID);
		// if(tempWConfig != null) {
		// wifiManage.removeNetwork(tempWConfig.networkId);
		// }

		if (Type == 1) {// WIFICIPHER_NOPASS(wifi密码 未通过)
			wConfig.wepKeys[0] = "";
			wConfig.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
			wConfig.wepTxKeyIndex = 0;
		}

		if (Type == 2) {// WIFICIPHER_WEP(wifi密码 有线等效保密)
			wConfig.hiddenSSID = true;
			wConfig.wepKeys[0] = "\"" + Password + "\"";

			wConfig.allowedAuthAlgorithms
					.set(WifiConfiguration.AuthAlgorithm.SHARED);
			wConfig.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
			wConfig.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
			wConfig.allowedGroupCiphers
					.set(WifiConfiguration.GroupCipher.WEP40);
			wConfig.allowedGroupCiphers
					.set(WifiConfiguration.GroupCipher.WEP104);
			wConfig.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);

			wConfig.wepTxKeyIndex = 0;
		}

		if (Type == 3) {// WIFICIPHER_WPA(wifi密码 WiFi网络安全接入)
			wConfig.preSharedKey = "\"" + Password + "\"";
			// wConfig.hiddenSSID = true;

			wConfig.allowedAuthAlgorithms
					.set(WifiConfiguration.AuthAlgorithm.OPEN);
			wConfig.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
			wConfig.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
			wConfig.allowedPairwiseCiphers
					.set(WifiConfiguration.PairwiseCipher.TKIP);
			// wConfig.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
			wConfig.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
			wConfig.allowedPairwiseCiphers
					.set(WifiConfiguration.PairwiseCipher.CCMP);
			wConfig.status = WifiConfiguration.Status.ENABLED;
		}

		return wConfig;
	}

	public boolean removeNetwork(String ssid) {
		List<WifiConfiguration> m_wifiList = m_wm.getConfiguredNetworks();
		int netId;
		boolean flag = false;
		for (int i = 0; i < m_wifiList.size(); i++) {
			if (m_wifiList.get(i).SSID.contains(ssid)) {// m_wifiConfigurationList出来的SSID带""引号
				netId = m_wifiList.get(i).networkId;
				flag = m_wm.removeNetwork(netId);
				// LogUtils.v("PW_LOG","remove success");
				break;
			}
		}
		return flag;
	}

	public void startScanWifi() {
		m_wm.startScan();
	}
	
	public boolean getNetConnectState(){
		ConnectivityManager cm = (ConnectivityManager) m_appContext
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo wifiNetworkInfo = cm
				.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
		NetworkInfo mobileNetworkInfo = cm
				.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
		return (wifiNetworkInfo.isConnected() || mobileNetworkInfo.isConnected());
	}
	
	public boolean getWifiConnectState() {
		ConnectivityManager cm = (ConnectivityManager) m_appContext
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo wifiNetworkInfo = cm
				.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
		if (wifiNetworkInfo.isConnected()) {
			return true;
		}
		return false;
	}

	public boolean getWifiEnabled() {
		return m_wm.isWifiEnabled();
	}

	private class PWSwitchTimerTask extends TimerTask {
		public void run() {
			if (!getWifiConnectState()) {
				if (m_switchListener != null) {
					m_switchListener.onWifiSwitchListener(false);
				}
			}
			this.cancel();
		}
	}

//	private class WifiSateReceiver extends BroadcastReceiver {
//		@Override
//		public void onReceive(Context context, Intent intent) {
//			// TODO Auto-generated method stub
//			if (WifiManager.WIFI_STATE_CHANGED_ACTION
//					.equals(intent.getAction())) {// 这个监听wifi的打开与关闭，与wifi的连接无关
//				int wifiState = intent.getIntExtra(
//						WifiManager.EXTRA_WIFI_STATE, 0);
//				switch (wifiState) {
//				case WifiManager.WIFI_STATE_DISABLED:
//					break;
//				case WifiManager.WIFI_STATE_DISABLING:
//					break;
//				//
//				}
//			}
//			// 这个监听wifi的连接状态即是否连上了一个有效无线路由，当上边广播的状态是WifiManager.WIFI_STATE_DISABLING，和WIFI_STATE_DISABLED的时候，根本不会接到这个广播。
//			// 在上边广播接到广播是WifiManager.WIFI_STATE_ENABLED状态的同时也会接到这个广播，当然刚打开wifi肯定还没有连接到有效的无线
//			if (WifiManager.NETWORK_STATE_CHANGED_ACTION.equals(intent
//					.getAction())) {
//				Parcelable parcelableExtra = intent
//						.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);
//				if (null != parcelableExtra) {
//					NetworkInfo networkInfo = (NetworkInfo) parcelableExtra;
//					State state = networkInfo.getState();
//					boolean isConnected = (state == State.CONNECTED);// 当然，这边可以更精确的确定状态
//					// LogUtils.v("PW_LOG", "networkInfo.isConnected"+
//					// networkInfo.isConnected());
//					LogUtils.v("PwNetWorkService",
//							"############WifiStateBroadcast isConnected"
//									+ isConnected);
//					if (isConnected) {
//						if (m_switchListener != null) {
//							m_switchListener.onWifiSwitchListener(true);
//						}
//					}
//				}
//			}
//			if (WifiManager.SCAN_RESULTS_AVAILABLE_ACTION.equals(intent
//					.getAction())) {
//				LogUtils.v("PwNetWorkService", "SCAN_RESULTS_AVAILABLE_ACTION");
//				if (m_scanResultHolderListener != null) {
//					m_scanResultHolderListener.onScanResultListener(m_wm
//							.getScanResults());
//				}
//			}
//		}
//	}
}
