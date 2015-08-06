package com.showmo.util;

import javax.mail.internet.NewsAddress;

import com.showmo.base.ShowmoApplication;

import android.R.integer;
import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.widget.Toast;

public class ToastUtil {
	private static final int TOAST_SHORT_STR = 1;
	private static final int TOAST_LONG_STR = 2;
	private static final int TOAST_SHORT_ID = 3;
	private static final int TOAST_LONG_ID = 4;
	public static final long mainThreadid=Looper.getMainLooper().getThread().getId();
	public static Toast myToast=null;
	
	public static Handler m_Handler = new Handler(Looper.getMainLooper()) {
		public void handleMessage(android.os.Message msg) {
			if(!(msg.obj instanceof ToastStruc)){
				return;
			}
			createToast(null);
			ToastStruc info=(ToastStruc)msg.obj;
			switch (msg.what) {
			case TOAST_SHORT_STR:
				toastUseSingleToast(info.msg, Toast.LENGTH_SHORT);
				break;
			case TOAST_LONG_STR:
				toastUseSingleToast(info.msg, Toast.LENGTH_LONG);
				break;
			case TOAST_SHORT_ID:
				toastUseSingleToast(info.msgId, Toast.LENGTH_SHORT);
				break;
			case TOAST_LONG_ID:
				toastUseSingleToast(info.msgId, Toast.LENGTH_LONG);
				break;
			default:
				break;
			}
		};
	};

	private static void createToast(Context context){
		if(myToast==null){
			myToast=Toast.makeText(ShowmoApplication.getInstance(), "", Toast.LENGTH_SHORT);
		}
	}
	private static void toastUseSingleToast(int textid,int duration){
		myToast.setDuration(duration);
		myToast.setText(textid);
		myToast.show();
	}
	private static void toastUseSingleToast(String text,int duration){
		myToast.setDuration(duration);
		myToast.setText(text);
		myToast.show();
	}
	public static void toastShort(Context context, String str) {
		
		if(Thread.currentThread().getId() ==mainThreadid){
			createToast(context);
			toastUseSingleToast(str, Toast.LENGTH_SHORT);
		}else{
			toastShortFromThread(context, str);
		}
	}

	public static void toastShort(Context context, int strId) {
		
		if(Thread.currentThread().getId() ==mainThreadid){
			createToast(context);
			toastUseSingleToast(strId, Toast.LENGTH_SHORT);
		}else{
			toastShortFromThread(context, strId);
		}
	}

	public static void toastLong(Context context, String str) {
		
		if(Thread.currentThread().getId() ==mainThreadid){
			createToast(context);
			toastUseSingleToast(str, Toast.LENGTH_LONG);
		}else{
			toastLongFromThread(context,str);
		}
	}

	public static void toastLong(Context context, int strId) {
		
		if(Thread.currentThread().getId() ==mainThreadid){
			createToast(context);
			toastUseSingleToast(strId, Toast.LENGTH_LONG);
		}else{
			toastLongFromThread(context,strId);
		}
	}

	private static void toastShortFromThread(Context context, String str) {
		Message msgMessage = m_Handler.obtainMessage();
		msgMessage.obj = new ToastStruc(context, str, 0);
		msgMessage.what = TOAST_SHORT_STR;
		m_Handler.sendMessage(msgMessage);
	}
	private static void toastShortFromThread(Context context, int strId) {
		Message msgMessage = m_Handler.obtainMessage();
		msgMessage.obj = new ToastStruc(context, "", strId);
		msgMessage.what = TOAST_SHORT_ID;
		m_Handler.sendMessage(msgMessage);
	}
	private static void toastLongFromThread(Context context, String strId) {
		Message msgMessage = m_Handler.obtainMessage();
		msgMessage.obj = new ToastStruc(context, strId,0);
		msgMessage.what = TOAST_LONG_STR;
		m_Handler.sendMessage(msgMessage);
	}
	private static void toastLongFromThread(Context context, int strId) {
		Message msgMessage = m_Handler.obtainMessage();
		msgMessage.obj = new ToastStruc(context, "", strId);
		msgMessage.what = TOAST_LONG_ID;
		m_Handler.sendMessage(msgMessage);
	}
	private static class ToastStruc {
		Context context;
		String msg;
		int msgId;

		public ToastStruc(Context context, String msg, int msgId) {
			super();
			this.context = context;
			this.msg = msg;
			this.msgId = msgId;
		}
	}
}
