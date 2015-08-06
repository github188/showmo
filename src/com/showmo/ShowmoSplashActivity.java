package com.showmo;

import java.util.List;

import com.showmo.activity.login.LoginActivity;
import com.showmo.base.BaseActivity;
import com.showmo.base.NotInitException;
import com.showmo.base.ShowmoSystem;
import com.showmo.eventBus.util.AsyncExecutor;
import com.showmo.ormlite.dao.AccountDao;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.model.ShowmoAccount;
import com.showmo.userManage.User;
import com.showmo.util.AESUtil;
import com.showmo.util.LogUtils;
import com.showmo.util.PwNetWorkHelper;
import com.showmo.util.StringUtil;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.view.View;

public class ShowmoSplashActivity extends BaseActivity {
	private AccountDao mAccountDao;
	private SharedPreferences mSp;
	private Handler mHandler=new Handler();
	private PwInfoDialog mNoNetDialog;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_splash);
		initView();		
	}
	private boolean binit=false;
	@Override
	protected synchronized void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		if(!PwNetWorkHelper.getInstance().getNetConnectState()){
			mNoNetDialog.show();
		}else{
			if(!binit){
				binit=true;
				AsyncTask.execute(new Runnable() {
					
					@Override
					public void run() {
						// TODO Auto-generated method stub
						init();
					}
				});	
			}
		}
	}
	private void initView(){
		mNoNetDialog=new PwInfoDialog(this);
		mNoNetDialog.setContentText(R.string.app_need_work_with_net);
		mNoNetDialog.setCancelable(false);
		mNoNetDialog.removeCancelBtn();
		mNoNetDialog.setOkBtnTextAndListener(null, new OnOkClickListener() {
			@Override
			public void onClick() {
				// TODO Auto-generated method stub
				if(PwNetWorkHelper.getInstance().getNetConnectState()){
					AsyncTask.execute(new Runnable() {
						
						@Override
						public void run() {
							// TODO Auto-generated method stub
							init();
						}
					});	
				}else{
					finish();
					System.exit(0);	
				}
			}
		});
	}
	private void init(){
		mAccountDao=DaoFactory.getUserDao(this);
		mSp=getSharedPreferences(LoginActivity.SHAREDPERENCES_NAME, MODE_PRIVATE);
		boolean bAutoLogin=mSp.getBoolean(LoginActivity.SP_KEY_AUTO_LOGIN, false);
		String username = mSp.getString(LoginActivity.SP_KEY_LAST_LOGIN_ACCOUNT, "");
		if(!bAutoLogin || !StringUtil.isNotEmpty(username) || (mAccountDao==null)){
			LogUtils.e("splash", "!bAutoLogin || username==null || (mAccountDao==null)");
			startLoginActivityDelay(2000);
			return;
		}
		List<ShowmoAccount> res = mAccountDao.queryByUserName(username);
		String psw=null;
		if(res != null){
			String pswNotAes = res.get(0).getPsssword();
			byte[] pswbyte = AESUtil.decrypt(AESUtil.parseHexStr2Byte(pswNotAes), AESUtil.KEY_AES);
			if(pswbyte==null){
				psw=null;
			}else {
				psw=new String(pswbyte);
			}
		}
		if(psw==null){
			LogUtils.fi(LogUtils.LogAppFile, "splash activity psw==null");
			startLoginActivityDelay(2000);
			return;
		}
		LogUtils.fi(LogUtils.LogAppFile, "splash activity AutoLogin");
		AutoLogin(username,psw);
	}
	private void AutoLogin(final String userName,final String psw){
		AsyncTask.execute(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				long loginBeforeTime=SystemClock.elapsedRealtime();
				boolean blogin=login(userName,psw);
//				if(!blogin){
//					blogin=login(userName,psw);
//					if(!blogin){
//						blogin=login(userName,psw);
//					}
//				}
				long loginEndTime=SystemClock.elapsedRealtime();
				LogUtils.fi(LogUtils.LogAppFile, "splash activity login use time---->"+(loginEndTime-loginBeforeTime)+"ms");
				int delayms=0;
				if(loginEndTime-loginBeforeTime <3000){
					delayms=3000-(int)(loginEndTime-loginBeforeTime);
				}
				if(!blogin){
					LogUtils.fi(LogUtils.LogAppFile, "splash activity login err ");
					mSp.edit().putBoolean(LoginActivity.SP_KEY_AUTO_LOGIN, false).commit();
					startLoginActivityDelay(delayms);
				}else{
					startMainActivityDelay(delayms);
				}
			}
		});
	}
	private boolean login(String userName,String psw){
		boolean bLogin=false;
		try {
			bLogin=showmoSystem.userLogin(userName, psw, ShowmoSystem.SHOWMO_USER,false);
		} catch (NotInitException e) {
			// TODO: handle exception
			e.printStackTrace();
			showmoSystem.init(showmoApp);
			return false;
		}finally{
			return bLogin;
		}
	}
	
	private void startLoginActivityDelay(int delayms){
		mHandler.postDelayed(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				slideInFromRight(LoginActivity.class);
				finish();
			}
		},delayms);
	}
	private void startMainActivityDelay(int delayms){
		mHandler.postDelayed(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				slideInFromRight(MainActivity.class);
				finish();
			}
		},delayms);
	}
}
