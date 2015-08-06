package ipc365.app.showmo.jni;

import java.util.ArrayList;
import java.util.List;

import android.R.integer;

import com.puwell.opengles.Flinger;
//import com.puwell.opengles.NVRRender;


import ipc365.app.showmo.jni.JniDataDef.*;

public class JniClient {
	public static native boolean PW_NET_Init();

	// get last error
	public static native long PW_NET_GetLastError();

	// get VerifyCode
	public static native boolean PW_NET_GetVerifyCode(String in_userName);

	// get app version
	public static native boolean PW_ENT_GetAppVersion(int nDeviceType,
			QueryAppVersionRet out_version); // 0 ipc365; 1 showmo

	// add terminal information
	public static native boolean PW_NET_AddTerminal(
			TerminalDeviceAddReq in_terminal);

	// reset password by old password
	public static native boolean PW_NET_ResetPassword(
			ResetPasswordReq lpByOldPass);

	// reset password by verifycode
	public static native boolean PW_NET_ResetPasswordEx(
			ResetPasswordByVerifyReq lpByVerify);

	// ap get device online state
	public static native long PW_NET_GetDeviceid(String strUuid); // error <= 0

	// device bilnd or not
	public static native BindStateInfo PW_NET_BindState(String strUuid); 
	
	// 0:验证码正确 1：验证码不存在  2：验证码不存在  -1：失败
	public static native long   PW_NET_CheckVerifyCode(String strUserName,String strVerifyCode);///

	// add device
	public static native ClientDeviceAddRet PW_NET_AddDevice(
			ClientDeviceAddReq in_lpAddDevice);

	// repeat add device
	public static native ClientDeviceAddRet PW_NET_AddDeviceEx(
			ClientDeviceAddReq in_lpAddDevice);

	// delete device
	public static native boolean PW_NET_DeleteDevice(String strUuid);

	// account
	public static native long PW_NET_VerityAccount(String strUser); // 0 not
																	// exist 1
																	// exist -1
																	// failure
	// apply test account
	public static native SDK_APPLY_ACCOUNTINFO PW_NET_ApplyTestAccount(int appType);

	// delete apply test account
	public static native boolean PW_NET_DeleteApplyAccount(
			SDK_APPLY_ACCOUNTINFO lpAccount);

	// sign up server
	public static native boolean PW_NET_SignUp(ClientRegister2Login lpRegister);

	// manager server
	public static native boolean PW_NET_Mgr_SignIn();

	// disconnect manager server
	public static native boolean PW_NET_Mgr_DisConnect(int nCamera_id);

	// login loginserver
	public static native boolean PW_NET_Login(ClienteLoginReq lpLogin);

	// logout
	public static native boolean PW_NET_Logout();

	// mgr state
	// public static native boolean PW_NET_MgrState(); // 0 outline 1 online

	// get device state
	public static native boolean PW_NET_OnLineState(int nCamera_id); // false err 1
																	// true 
	// get localplay verifycode

	public static native boolean PW_NET_GetLocalVerity(int nCamera_id);

	// change network
	// public static native boolean PW_NET_SetNetWork(int nType);// 0 public 1
	// local
	// video realplay
	public static native int PW_NET_StartRealPlay(int nCamera_id,
			OnRealdataCallBackListener cbRealData, long dwUser);//-1 

	// set stream type
	public static native boolean PW_NET_SetStreamType(int nCamera_id,
			int nSteramType);// 0 main 1 sub 2jpeg

	// get encode info
	public static native boolean PW_NET_Panoinfo(PanoInfo lpInfo);

	//
	// // set msg data callback
	public static native boolean PW_NET_SetMsgDataCallBack(
			OnMsgDataCallBackWraper cbMsgData, long dwUser);

	//
	// // start receive alarm
	public static native boolean PW_NET_SetAlarmState(int nCamera_id,
			int nalarmtype, boolean state);

	//
	// // get alarm state
	
	public static native long PW_NET_GetAlarmState(int nCamera_id,
			int nalarmtype); // 0 
	//
	// // get alarm pic

	public static native long PW_NET_GetAlarmPic(String AlarmPicPath,int recordId,int nCamera_id,
			int nalarmtype, int nalarmcode, int nccid, int nstarttime,
			int nendtime); // -1 failure 0 no pic 1 succcess
	//
	// // release buf

	public static native void PW_NET_ReleaseBuf(byte[] strBuf);

	//
	// // stop realplay
	public static native boolean PW_NET_StopRealPlay(int nCamera_id);

	//
	// // sound
	public static native boolean PW_NET_Audio(int nCamera_id, boolean state);

	//
	// // get device list
	public static native ArrayList<ClientDeviceQueryRet> PW_NET_GetDeviceList();// client_device_query_ret
																				// *lpDeviceList);
	//
	// // print log

	public static native boolean PW_NET_Log(boolean bPrint);

	//

	//
	// // dubug test
	// public static native boolean PW_NET_SetDebugCallBack(errordatacallback
	// func);
	//
	// // upgrade
	public static native boolean PW_NET_Upgrade(int nCamera_id,
			String strVersion);

	//
	// // light
	public static native boolean PW_NET_BrightCtrl(int nCamera_id,
			int nBrightness,int nCtrl);//ctrl ...LightCtrl.close LightCtrl.open LightCtrl.set

	//
	// /************************************************************************/
	// /************************************************************************/
	//
	// // remote file
	public static native ArrayList<SDK_REMOTE_FILE> PW_NET_SearchRemoteFile(
			int nCamera_id, SDK_SEARCH lpFindInfo);// SDK_REMOTE_FILE
	//
	// // remote play

	public static native boolean PW_NET_PlayBack(int nCamera_id,
			SDK_REMOTE_FILE lpPlayBackFile,
			OnRealdataCallBackListener fDownLoadDataCallBack, long dwDataUser);
	
	//
	// // remote play pos
	public static native long PW_NET_GetPlayBackPos(int nCamera_id,
			String filename);

	//
	// // remote play control
	public static native boolean PW_NET_PlayBackControl(int nCamera_id,
			int ctrl, int ctrlvalue);

	//
	// // stop remote play
	public static native boolean PW_NET_StopPlayBack(int nCamera_id);

	//
	// // start record
	public static native boolean PW_NET_Record(String filename);
		
	//
	// // stop record
	public static native boolean PW_NET_StopRecord();
		
	//
	// // get device config
	public static native boolean PW_NET_GetDevConfig(long dwCommand,
			String lpInBuffer, long dwInBufferSize, String lpOutBuffer,
			long dwOutBufferSize, int nWaittime);

	//
	// // set device config
	public static native long PW_NET_SetDevConfig(long dwCommand,
			String lpHeadBuffer, long dwHeadLen, String lpInBuffer,
			long dwInBufferSize, int nWaittime);

	public static native FrameSize PW_NET_GetFrameSize(byte[] pBuf, int nSize);


	public static native long PW_NET_GetTalkState(int nCamera_id); // 0 close 1 open -1 failed

	// talk ctrl
	public static native boolean PW_NET_TalkCtrl(int nCamera_id,boolean bstate);// 1 start 0 close

	// send talk data
	public static native boolean PW_NET_SendTalkData(int nCamera_id,byte[] strTalk ,long lTalkSize,String path);
	
	public static native boolean PW_NET_ModifyDevName(int CameraId,String newName);
	
	public static native int PW_NET_Brightness(int cameraId);//小于零表示获取失败
	
	public static native SDK_WIFI_VALUE  PW_NET_GetWifiValue(int nCamerai_d);
	
	// get volume
	public static native int    PW_NET_GetVolume(int nCamerai_d);//获取IPC播报声音大小
	// set volume
	public static native boolean PW_NET_SetVolume(int nCamerai_d,int volume);
	
	public static native List<DeviceVolumState> PW_NET_GetVolumeSwitchs(int nCamerai_d);//获取所有开关状态
	
	public static native boolean PW_NET_OpenVolumeSwitch(int nCamerai_d,int type);
	
	public static native boolean PW_NETCloseVolumeSwitch(int nCamerai_d,int type);
	
	public static native int PW_NET_SetDebugDatacallback(OnDebugDataCallbackListener debugObj);
	
	public static native long PW_PRI_DevLogin(String ip);

	public static native void PW_PRI_DevLogout(long LoginID);

	public static native boolean PW_PRI_DevSetTimezone(long LoginID,
			int timezone);

	public static native boolean PW_PRI_DevSetOSD(long LoginID, String osdName);

	public static native boolean PW_PRI_DevSetWifi(long LoginID,
			PW_PHONE_BL_WIFI_CONFIG config);

	public static native boolean PW_PRI_isDevConnectedInApModel(long LoginID);

	public static native int PW_PRI_GetConnectedRouterStatus(long LoginID);

	public static native boolean PW_PRI_DevSetDHCP(long LoginID, boolean bOpen);

	public static native boolean PW_PRI_DevAlive(long LoginID);

	public static native boolean PW_PRI_DevAPSwap(long LoginID);

	public static native int PW_PRI_GetLastError();

	public static native boolean PW_PRI_BeginCycleBroadcastToDev(Broadcast_wifi_info wifiInfo);
	
	public static native boolean PW_PRI_StopCycleBroadcastToDev();
	
	public static native boolean PW_PRI_SetPauseCycleBroadcastToDev(boolean bPause);
	
	public static native List<String> PW_PRI_SearchDevInLan(int searchSecTime);
	
	public static native User_2_mgr_disconn_device pw_jni_call_test();
	
	
//opengles 
	public static native void native_mpgl_init(String LibPath, String CompPath, String GLContent);
	public static native void native_mpgl_setPpi(float ppi);
	public static native void native_mpgl_setPpiXy(float ppix,float ppiy);
	
	public static native void native_mpgl_PatternCtrlFingerDown(int index, Flinger[] flingerList, int FingerCount, int tickCount);
	public static native void native_mpgl_PatternCtrlFingerDown(int index, int[] x, int[] y, int[] width, int[] height, int count, int tickCount);
	public static native int native_mpgl_GetPatternWithTime(int type, Object renderer, int index, int tickCount, int height, int width);
	
	public static native void native_mpgl_Stop();
	public static native void native_mpgl_Start();
	public static native void native_mpgl_reset();
//	public static native void native_init_opengl(int index);
//	public static native void native_uninit_opengl(int index);
}
