package ipc365.app.showmo.jni;

import com.showmo.util.LogUtils;

import android.R.integer;
import android.test.AndroidTestCase;
import ipc365.app.showmo.jni.JniDataDef.BindStateInfo;
import ipc365.app.showmo.jni.JniDataDef.SDK_WIFI_VALUE;
import junit.framework.Assert;

public class Test extends AndroidTestCase {
	public void testBindState(){
		BindStateInfo info= JniClient.PW_NET_BindState("DC07C1F52B21");
		LogUtils.e("info", "cur user "+info.deviceCurUser+" state "+info.state);
	}
	public void testVerifyCodeExist(){
		long lret=JniClient.PW_NET_CheckVerifyCode("18257168402","802474");
		LogUtils.e("info", "PW_NET_CheckVerifyCode ret "+lret+" err "+JniClient.PW_NET_GetLastError());
	}
	public void testWifiInfo(){
		int cameraid=(int)JniClient.PW_NET_GetDeviceid("DC07C1F52B21");
		assertTrue(cameraid>0);
		SDK_WIFI_VALUE info=JniClient.PW_NET_GetWifiValue(cameraid);
		assertNotNull(info);
		LogUtils.e("test", info.toString());
	}
	public void testIpcVolum(){
//		int cameraid=(int)JniClient.PW_NET_GetDeviceid("DC07C1F52B21");
//		assertTrue(cameraid>0);
//		SDK_VOLUME_VALUE infoValue=JniClient.PW_NET_GetVolume(cameraid);
//		assertNotNull(infoValue);
//		LogUtils.e("test","options:"+infoValue.options+"control:"+infoValue.control
//				+"values:"+infoValue.values+"volume:"+infoValue.volume);
//		boolean bres=JniClient.PW_NET_SetVolume(cameraid, 
//				new SDK_VOLUME_VALUE(VolumeType.STREAM_TYPE,1, 30, false));
//		assertTrue(bres);
	}
}
