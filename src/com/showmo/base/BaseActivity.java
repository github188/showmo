package com.showmo.base;

import java.io.Serializable;
import java.sql.SQLException;

import com.j256.ormlite.dao.Dao;
import com.showmo.R;
import com.showmo.network.NetWorkErrorCode;
import com.showmo.network.NetworkHelper;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.util.AnimUtil;
import com.showmo.util.AppStateCheck;
import com.showmo.util.LogUtils;
import com.showmo.util.ToastUtil;
import com.showmo.widget.CustomDialog;
import com.showmo.widget.LoadingDialog;
import com.showmo.widget.CustomDialog.onCustomDialogClickListener;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnCancelClickListener;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class BaseActivity extends FragmentActivity implements OnClickListener {

	public static final String INTENT_ACTION_BACKGROUND="appbackground";
	
	public static final String INTENT_KEY_BOOLEAN = "INTENT_KEY_BOOLEAN";

	public static final String INTENT_KEY_STRING = "INTENT_KEY_STRING";
	
	public static final String INTENT_KEY_STRINGONE = "INTENT_KEY_STRINGONE";
	
	public static final String INTENT_KEY_STRINGTWO = "INTENT_KEY_STRINGTWO";
	
	public static final String INTENT_KEY_STRINGS = "INTENT_KEY_STRINGS";
	
	public static final String INTENT_KEY_STRINGSS = "INTENT_KEY_STRINGSS";

	public static final String INTENT_KEY_OBJECT = "INTENT_KEY_OBJECT";

	public static final String INTENT_KEY_INT = "INTENT_KEY_INT";

	public static final String TIME_CHANGED_ACTION = "TIME_CHANGED_ACTION";

	public static final String TIME_CONTROL_ACTION = "TIME_CONTROL_ACTION";

	public static final String SHAREDPERENCES_NAME = "SHAREDPERENCES_NAME";
	
	public static final String SHAREDPERENCES_LASTPSW = "SHAREDPERENCES_LASTPSW";
	
	public static final String SP_KEY_NOMORE_UPGRADE_APP="UPGRADE_APP";
	
	public static final String INTENT_KEY_VERI_TO_LOGIN="INTENT_KEY_FLAG";
	
	public static final String SP_KEY_XG_PUSH="xg_push_switch";
	public static final String SP_KEY_REG_ACCOUNT="xg_push_account";
	
	public static final String SP_KEY_VIDEO_SOUND="SP_KEY_VIDEO_SOUND";
	
	protected ShowmoApplication showmoApp;

	protected ShowmoSystem showmoSystem;
	
	public BaseActivity() {
		// TODO Auto-generated constructor stub
		showmoApp=ShowmoApplication.getInstance();
		showmoSystem=ShowmoSystem.getInstance();
	}
	
	// SharedPreferences存储 start
	protected SharedPreferences showmoSp;
	
	public SharedPreferences getCommonSharedPreferences(){
		return getSharedPreferences(SHAREDPERENCES_NAME, Context.MODE_PRIVATE);
	}
	
	protected void saveInSharedPreferences(String key, String value) {
		if (showmoSp == null) {
			showmoSp = getSharedPreferences(SHAREDPERENCES_NAME, MODE_PRIVATE);
		}
		Editor editor = showmoSp.edit();
		editor.putString(key, value);
		editor.commit();
	}
	public void saveBooleanInSharedPreferences(String key, boolean value) {
		if (showmoSp == null) {
			showmoSp = getSharedPreferences(SHAREDPERENCES_NAME, MODE_PRIVATE);
		}
		Editor editor = showmoSp.edit();
		editor.putBoolean(key, value);
		editor.commit();
	}

	public boolean getBooleanFromSharedPreferences(String key,boolean def) {
		if (showmoSp == null) {
			showmoSp = getSharedPreferences(SHAREDPERENCES_NAME, MODE_PRIVATE);
		}
		return showmoSp.getBoolean(key, def);
	}

	// SharedPreferences存储 end

	// 键盘管理
	protected void hideSoftInputMethod() {
		InputMethodManager inputMethodManager = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
		View view = this.getCurrentFocus();
		if (view != null) {
			inputMethodManager.hideSoftInputFromWindow(view.getWindowToken(),
					InputMethodManager.HIDE_NOT_ALWAYS);
		}
	}

	protected Context getContext(){
		return this;
	}
	protected void showSoftInputMethod(View view) {
		InputMethodManager inputMethodManager = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
		// 接受软键盘输入的编辑文本或其它视图
		inputMethodManager.showSoftInput(view, InputMethodManager.SHOW_FORCED);

	}

	// 键盘管理

	// 过程动画与页面跳转 start

	public void slideInFromRight() {
		AnimUtil.slideInFromRight(this);
	}

	public void slideInFromRight(Class activityClass) {
		move2activity(activityClass);
		slideInFromRight();
	}

	public void slideInFromRight(Class activityClass, boolean flag) {
		move2activity(activityClass, flag);
		slideInFromRight();
	}

	public void slideInFromRight(Class activityClass, String msg) {
		move2activity(activityClass, msg);
		slideInFromRight();
	}
	public void slideInFromRight(Class activityClass, int msg1, String msg2) {
		move2activity(activityClass, msg1,msg2);
		slideInFromRight();
	}

	public void slideInFromRight(Class activityClass, int msg) {
		move2activity(activityClass, msg);
		slideInFromRight();
	}
	
	public void slideInFromRight(Class activityClass, long msg1,long msg2,int msg3) {
		move2activity(activityClass, msg1,msg2,msg3);
		slideInFromRight();
	}

	public void slideInFromRight(Class activityClass, Serializable Seri) {
		move2activity(activityClass, Seri);
		slideInFromRight();
	}

	public void slideInFromLeft() {
		AnimUtil.slideInFromLeft(this);
	}

	public void slideInFromLeft(Class activityClass) {
		move2activity(activityClass);
		slideInFromLeft();
	}

	public void slideInFromLeft(Class activityClass, boolean flag) {
		move2activity(activityClass, flag);
		slideInFromLeft();
	}
	
	public void slideInFromLeft(Class activityClass, int msg) {
		move2activity(activityClass, msg);
		slideInFromLeft();
	}

	public void slideInFromLeft(Class activityClass, String msg) {
		move2activity(activityClass, msg);
		slideInFromLeft();
	}
	
	
	public void move2activity(Class activityClass) {
		Intent intent = new Intent(this, activityClass);
		startActivity(intent);
	}

	public void move2activity(Class activityClass, long msg1,long msg2,int msg3) {
		Intent intent = new Intent(this, activityClass);
		intent.putExtra(INTENT_KEY_STRING, msg1);
		intent.putExtra(INTENT_KEY_STRINGS, msg2);
		intent.putExtra(INTENT_KEY_STRINGSS, msg3);
		startActivity(intent);
	}
	
	public void move2activity(Class activityClass, String msg) {
		Intent intent = new Intent(this, activityClass);
		intent.putExtra(INTENT_KEY_STRING, msg);
		startActivity(intent);
	}
	public void move2activity(Class activityClass, int msg1, String msg2) {
		Intent intent = new Intent(this, activityClass);
		intent.putExtra(INTENT_KEY_STRINGONE, msg1);
		intent.putExtra(INTENT_KEY_STRINGTWO, msg2);
		startActivity(intent);
	}
	public void move2activity(Class activityClass, boolean flag) {
		Intent intent = new Intent(this, activityClass);
		intent.putExtra(INTENT_KEY_BOOLEAN, flag);
		startActivity(intent);
	}

	public void move2activity(Class activityClass, int msg) {
		Intent intent = new Intent(this, activityClass);
		intent.putExtra(INTENT_KEY_INT, msg);
		startActivity(intent);
	}

	public void move2activity(Class activityClass, Serializable seri) {
		Intent intent = new Intent(this, activityClass);
		Bundle extras = new Bundle();
		extras.putSerializable(INTENT_KEY_OBJECT, seri);
		intent.putExtras(extras);
		startActivity(intent);
	}

	// 过程动画与页面跳转 end

	// 消息提示 start

	protected void showToastShort(String msg) {
		Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
	}

	protected void showToastShort(int msg) {
		Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
	}

	protected void showToastLong(String msg) {
		Toast.makeText(this, msg, Toast.LENGTH_LONG).show();
	}

	// 消息提示 end

	// 窗口提示 start

	protected LoadingDialog mLoadimgDialog;

	public void showLoadingDialog() {
		if (mLoadimgDialog == null) {
			mLoadimgDialog = new LoadingDialog(this, R.style.style_load_dialog);
		}
		mLoadimgDialog.setHint(R.string.loading);
		mLoadimgDialog.setCancelable(false); // 不可以取消
		mLoadimgDialog.show();
	}
	public void showLoadingDialog(int strId) {
		if (mLoadimgDialog == null) {
			mLoadimgDialog = new LoadingDialog(this, R.style.style_load_dialog);
		}
		mLoadimgDialog.setHint(strId);
		mLoadimgDialog.setCancelable(false); // 不可以取消
		mLoadimgDialog.show();
	}
	public void disableLoadingDialogCancel(){
		if(mLoadimgDialog !=null){
			mLoadimgDialog.disableCancelBtn();
		}
	}
	public void closeLoadingDialog() {
		if (mLoadimgDialog != null) {
			mLoadimgDialog.dismiss();
			mLoadimgDialog.disableCancelBtn();
		}
	}
	public void showLoadingDialogCancelAble(View.OnClickListener lis){
		if (mLoadimgDialog == null) {
			mLoadimgDialog = new LoadingDialog(this, R.style.style_load_dialog);
		}
		mLoadimgDialog.setHint(R.string.loading);
		mLoadimgDialog.setCancelable(false); // 不可以取消
		mLoadimgDialog.enableCancelBtn(lis);
		mLoadimgDialog.show();
	}

	protected PwInfoDialog buildCustomDialog(int titleStrid, int msgStrid,
			String okText,String cancelText , 
			 OnOkClickListener okListener, 
			OnCancelClickListener cancelListener) {

		PwInfoDialog pwInfoDialog = new PwInfoDialog(this);
		
		pwInfoDialog.setContentText(msgStrid) 
		 						.setDialogTitle(titleStrid)
								.setCancelBtnTextAndListener(cancelText, cancelListener)
								.setOkBtnTextAndListener(okText, okListener);
		
		return pwInfoDialog ;
		
	}
	
	protected PwInfoDialog buildCustomDialog(int titleStrid ,
			String okText,String cancelText , 
			 OnOkClickListener okListener, 
			OnCancelClickListener cancelListener) {

		PwInfoDialog pwInfoDialog = new PwInfoDialog(this);
		
		 
		pwInfoDialog	.setDialogTitle(titleStrid)
								.setCancelBtnTextAndListener(cancelText, cancelListener)
								.setOkBtnTextAndListener(okText, okListener);
		
		return pwInfoDialog ;
		
	}
	
	 
	

	// 窗口提示 end

	// 快速初始化 start

	/**
	 * 布局文件包含titlebar时 快速设置标题
	 * 
	 * @param titleId
	 */
	protected void setBarTitle(int stringId) {
		((TextView) findViewById(R.id.tv_bar_title)).setText(stringId);
		findViewAndSet(R.id.btn_bar_back);
	}

	protected void setBarTitle(String title) {
		((TextView) findViewById(R.id.tv_bar_title)).setText(title);
		findViewAndSet(R.id.btn_bar_back);

	}

	protected void setBarTitleWithBackFunc(int stringId) {
		TextView view=((TextView) findViewById(R.id.tv_bar_title));
		view.setText(stringId);
		findViewById(R.id.btn_bar_back).setOnClickListener(
				new OnClickListener() {

					@Override
					public void onClick(View v) {
						finish();
						slideInFromLeft();
					}
				});
	}
	public Button setBarTitleWithRightBtn(int titleStringId) {
		TextView tv = ((TextView) findViewById(R.id.tv_bar_title));
		tv.setText(titleStringId);
		findViewById(R.id.btn_bar_back).setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				finish();
				slideInFromLeft();
			}
		});
		Button btn = (Button) findViewById(R.id.btn_common_title_next);
		btn.setVisibility(View.VISIBLE);
		btn.setOnClickListener(this);
		return btn;
	}

	public Button setBarTitleWithRightBtn(int titleStringId,int rightBtnTextId) {
		TextView tv = ((TextView) findViewById(R.id.tv_bar_title));
		tv.setText(titleStringId);
		findViewById(R.id.btn_bar_back).setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				finish();
				slideInFromLeft();
			}
		});
		Button btn = (Button) findViewById(R.id.btn_common_title_next);
		btn.setText(rightBtnTextId);
		btn.setVisibility(View.VISIBLE);
		btn.setOnClickListener(this);
		return btn;
	}

	/**
	 * findView 并且设置监听
	 * 
	 * @param viewId
	 * @return
	 */
	protected View findViewAndSet(int viewId) {
		View view = findViewById(viewId);
		view.setOnClickListener(this);
		return view;
	}

	@Override
	public void onBackPressed() {
		finish();
		slideInFromLeft();
	}

	@Override
	public void onClick(View v) {
		int viewId = v.getId();
		onClick(viewId);
	}

	protected void onClick(int viewId) {

	}

	// 快速初始化 end

	// 网络请求 start

	protected static ShowmoSystem mShowmoSys;

	protected static NetworkHelper mNetHelper;

	protected ResponseInfo info; // 请求反馈的包装类

	protected ResponseInfo getResponseInfo() {
		if (info == null) {
			info = new ResponseInfo();
		}
		return info;
	}

	protected ResponseInfo getResponseInfo(boolean isSuccess) {
		if (info == null) {
			info = new ResponseInfo();
		}
		info.setIsSuccess(isSuccess);
		if (!isSuccess) {
			info.setErrorCode(getErrorCode());
		}
		return info;
	}
	protected ResponseInfo getResponseInfoCheckVerifyCode(long dateLong) {
		/*if (info == null) {
			info = new ResponseInfo();
		}
		info.setDateLong(dateLong);
		if (dateLong == 0) {
			info.setErrorType(0);;
		} else if (dateLong==1||dateLong==2) {
			info.setErrorType(1);
		}
		else {
			info.setErrorType(-1);
			info.setErrorCode(getErrorCode());
		}
		return info;*/
		if (info == null) {
			info = new ResponseInfo();
		}
		info.setDateLong(dateLong);
		if (dateLong == 0) {
			info.setIsSuccess(true);
		} else {
			info.setIsSuccess(false);
			info.setErrorCode(getErrorCode());
		}
		return info;
	}
	protected ResponseInfo getResponseInfo(long dateLong) {
		if (info == null) {
			info = new ResponseInfo();
		}
		info.setDateLong(dateLong);
		if (dateLong != -1) {
			info.setIsSuccess(true);
		} else {
			info.setIsSuccess(false);
			info.setErrorCode(getErrorCode());
		}
		return info;
	}

	private long getErrorCode() {
		if (mShowmoSys == null) {
			mShowmoSys = ShowmoSystem.getInstance();
		}
		long errorCode = mShowmoSys.getLastErrorCode();
		Log.e("NetworkHelper", "errorCode-->" + errorCode);
		return errorCode;
	}

	/**
	 * 需要进行网络请求时 在onCreate中调用此方法
	 */
	protected void initNetwork() {

		if (mShowmoSys == null) {
			mShowmoSys = ShowmoSystem.getInstance();
		}
		if (mNetHelper == null) {
			mNetHelper = NetworkHelper.getInstance();
		}
	}

	/**
	 * 统一处理网络连接错误 在RequestCallback 的onFailure() 调用此方法
	 * 
	 * @param errorCode
	 * @return true代表是网络连接错误 false反之并且要自己处理errorCode。
	 */
	public boolean handleNetConnectionError(int errorCode) {
		boolean res = false;
		switch (errorCode) {
		case NetWorkErrorCode.CSP_ERROR_SOCKET_CONNECT:
		case NetWorkErrorCode.CSP_ERROR_SOCKET_CREATE:
		case NetWorkErrorCode.CSP_ERROR_SOCKET_BIND:
		case NetWorkErrorCode.CSP_ERROR_SOCKET_SEND:
		case NetWorkErrorCode.CSP_ERROR_LOGIN_CONN:
			ToastUtil.toastShort(this,R.string.communication_failed);
			res = true;
			break;
		case NetWorkErrorCode.CSP_ERROR_MGR_NOLINK:
			ToastUtil.toastShort(this,R.string.mgr_not_connect);
			res = true;
			break;
		case NetWorkErrorCode.CSP_ERROR_VIDEO_NOLINK:
		case NetWorkErrorCode.CSP_ERROR_IPC_NOLINK:
			ToastUtil.toastShort(this,R.string.dev_not_connect);
			break;
		case NetWorkErrorCode.CSP_ERROR_TIMEOUT:
			ToastUtil.toastShort(this,R.string.communication_timeout);
			break;
		
		default:
//			showToastShort(R.string.operate_err);
			break;
		}

		return res;
	}

	/**
	 * 打印未处理的错误
	 * 
	 * @param errorCode
	 */
	protected void LogUntreatedError(int errorCode) {
		showToastShort(R.string.operate_err);
		Log.e("BaseActivity", "未处理的错误代码-->" + errorCode);
	}

	// 网络请求 end

	// 数据库 start

	protected static DatabaseHelper mDbHelper;

	/**
	 * 获得DatabaseHelper对象
	 * 
	 * @param daoClass
	 * @return 相应的dao对象
	 */
	@SuppressWarnings("rawtypes")
	protected Dao getDao(Class daoClass) {
		Dao dao = null;
		if (mDbHelper == null) {
			mDbHelper = DatabaseHelper.getHelper(this);
		}
		try {
			dao = mDbHelper.getDao(daoClass);
		} catch (SQLException e) {
			e.printStackTrace();
			return null;
		}
		return dao;

	}

	private void onBackground(){
		LogUtils.e("background", "app current is in background");
		Intent intent=new Intent(INTENT_ACTION_BACKGROUND);
		sendBroadcast(intent);
	}
	private static int Handle_Background=0;
	private Handler mHandler=new Handler(){
		public void handleMessage(android.os.Message msg) {
			if(msg.what == Handle_Background){
				boolean isBackground=AppStateCheck.isBackground(BaseActivity.this);
				LogUtils.e("background", "baseActivity onstop isBackground:"+isBackground);
				if(isBackground){
					BaseActivity.this.onBackground();
				}
			}
		};
	};
	
	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		mHandler.removeMessages(Handle_Background);
		mHandler.sendEmptyMessageDelayed(Handle_Background, 1000);
	}

	public boolean sorryForExperience(){
		if(showmoSystem.getCurUser().isExperience()){
			ToastUtil.toastShort(this, R.string.sorry_for_experience);
			return true;
		}
		return false;
	}
	// 数据库 end

}
