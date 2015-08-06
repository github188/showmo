package com.puwell.receiver;

import java.sql.SQLException;
import java.util.List;

import org.json.JSONException;
import org.json.JSONObject;

import android.R.string;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.hardware.Camera.Face;
import android.util.Log;
import android.widget.Toast;

import com.j256.ormlite.dao.Dao;
import com.showmo.activity.addDevice.AddDeviceConfigFailuredActivity;
import com.showmo.activity.alarmInfo.DeviceListActivity;
import com.showmo.activity.deviceManage.AlarmSwitchActivity;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoApplication;
import com.showmo.base.ShowmoSystem;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.dao.AccountDao;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.model.ShowmoAccount;
import com.showmo.util.AESUtil;
import com.tencent.android.tpush.XGPushBaseReceiver;
import com.tencent.android.tpush.XGPushClickedResult;
import com.tencent.android.tpush.XGPushRegisterResult;
import com.tencent.android.tpush.XGPushShowedResult;
import com.tencent.android.tpush.XGPushTextMessage;
import com.tencent.android.tpush.XGPushManager;

public class MessageReceiver extends XGPushBaseReceiver {
	
	private AccountDao  accountDao ;

	@Override
	public void onDeleteTagResult(Context arg0, int arg1, String arg2) {
		// TODO Auto-generated method stub
		Log.e("xgnotification", "onDeleteTagResult");
	}

	@Override
	public void onNotifactionClickedResult(Context context,
			XGPushClickedResult arg1) {
		// TODO Auto-generated method stub
		
		Log.e("xgnotification", "onNotifactionClickedResult");
		
		/* Intent intent = new Intent();
	     intent.setClass(context.getApplicationContext(), AddDeviceConfigFailuredActivity.class);
	     intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	     context.getApplicationContext().startActivity(intent);*/

		Intent intent=new Intent(context,DeviceListActivity.class);
		context.startActivity(intent);
//		SharedPreferences preferences = context.getSharedPreferences(BaseActivity.SHAREDPERENCES_NAME, Context.MODE_PRIVATE);
//		String regAccount=preferences.getString(BaseActivity.SP_KEY_REG_ACCOUNT, "");
//		Log.e("xgnotification", "onNotifactionClickedResult:: "+regAccount);
//		if(regAccount.equals("")){
//			return;
//		}
//		accountDao = DaoFactory.getUserDao(context);
//		List<ShowmoAccount> res =  accountDao.queryByUserName(regAccount);
//		if(res != null){
//			Log.e("xgnotification", "onNotifactionClickedResult:: "+res.get(0));
//		}
		
	}
	@Override
	public void onNotifactionShowedResult(Context context, XGPushShowedResult arg1) {//获取被展示的通知
		// TODO Auto-generated method stub
		Log.e("xgnotification", "onNotifactionShowedResult");
		SharedPreferences sPreferences= context.getSharedPreferences(BaseActivity.SHAREDPERENCES_NAME, Context.MODE_PRIVATE);
		final String regAccount=sPreferences.getString(BaseActivity.SP_KEY_REG_ACCOUNT, "");
		if(regAccount.equals("")){
			return;
		}
		XGNotification notific = new XGNotification();
		notific.setContent(arg1.getContent());
		//String string=notific.getContent();
		
		
	}

	@Override
	public void onRegisterResult(Context context, int errorCode,
			XGPushRegisterResult message) {//注册的回调
		// TODO Auto-generated method stub
		Log.e("xgnotification", "onRegisterResult");
	
		
		if (context == null || message == null) {
			return;
		}
		String text = "";
		if (errorCode == XGPushBaseReceiver.SUCCESS) {
			text = message + "注册成功";
			// 在这里拿token
			String token = message.getToken();
			Log.e("xgnotification", "onRegisterResult token::"+token);
		} else {
			text = message + "注册失败，错误码：" + errorCode;
		}
	}

	@Override
	public void onSetTagResult(Context arg0, int arg1, String arg2) {
		// TODO Auto-generated method stub
		Log.e("xgnotification", "onSetTagResult");
	}

	@Override
	public void onTextMessage(Context arg0, XGPushTextMessage message) {//推送的消息
		// TODO Auto-generated method stub
		Log.e("xgnotification", "onTextMessage");
		
		String text = "收到消息:" + message.toString();
		// 获取自定义key-value
		String customContent = message.getCustomContent();
		if (customContent != null && customContent.length() != 0) {
			try {
				JSONObject obj = new JSONObject(customContent);
				// key1为前台配置的key
				if (!obj.isNull("key")) {
					String value = obj.getString("key");
					Log.d("onTextMessage", "get custom value:" + value);
				}
				// ...
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
		// APP自主处理消息的过程...
		Log.d("xgnotification", text);
	}

	@Override
	public void onUnregisterResult(Context arg0, int arg1) {
		// TODO Auto-generated method stub
		Log.e("xgnotification", "onUnregisterResult");
	}
	
	
}
