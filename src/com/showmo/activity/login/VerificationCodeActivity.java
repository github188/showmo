package com.showmo.activity.login;

import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoSystem;
import com.showmo.network.NetWorkErrorCode;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.timer.TimerService;
import com.showmo.userManage.IUserObject;
import com.showmo.userManage.User;
import com.showmo.util.StringUtil;
import com.showmo.widget.PasswordText;

import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.CompoundButton.OnCheckedChangeListener;

import com.showmo.R;

/**
 * 验证码页面
 *根据getUserType 判断账号类型 给出不同的提示内容
 *账号类型不同，但是登陆使用的接口是相同的
 * 当注册成功或者重置密码成功后，会跳转登陆页面并自动填充注册账号
 * 
 * @author ljy
 *
 */
public class VerificationCodeActivity extends BaseActivity{
	
	private PasswordText etPswRe;
	private PasswordText etPsw;

	private IUserObject mUserObj ;
	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_verification_code);
		mUserObj = (IUserObject) getIntent().getExtras().get(INTENT_KEY_OBJECT);
		initView();
		initNetwork();
		
		
	}



	private void initView() {
		etPsw = (PasswordText)findViewById(R.id.et_register_psw);
		etPswRe   =(PasswordText)findViewById(R.id.et_register_psw_re);
		
		( (CheckBox) findViewById(R.id.cb_see_psw)).setOnCheckedChangeListener(
				new OnCheckedChangeListener() {
					@Override
					public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
						etPsw.setPswVisible(isChecked);
						etPswRe.setPswVisible(isChecked);
					}
				});
	/*	etPsw.setOnKeyListener(new OnKeyListener() {

			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event) {
				switch (keyCode) {
				case KeyEvent.KEYCODE_ENTER:
					
					if(isAgreeChecked){
						if(checkInputContent()){//检验用户输入内容是否合法
							netTaskVerifyUserExisted(etAccount.getText().toString().trim(), etPsw.getText().toString().trim());
							
						}
					}
					
					break;

				default:
					break;
				}
				return false;
			}
		});
*/
		if(mUserObj.getActionForVeri() == User.ACTION_VERI_RESET_PSW){
			setBarTitle(R.string.reset_psw);
		}else{
			setBarTitle(R.string.register);
		}
		findViewAndSet(R.id.btn_veri_comfirm);
		findViewAndSet(R.id.btn_bar_back);


	}

	@Override
	protected void onClick(int viewId) {
		switch (viewId) {
		case R.id.btn_bar_back:
			onBackPressed();
			break;
		case R.id.btn_veri_comfirm:		
			if (checkInputContent()) {
				if(mUserObj.getActionForVeri() == User.ACTION_VERI_RESET_PSW){
					netTaskResetPsw();
				}else{
					netTaskRegister();
				}	
			}else {
				
			}
				
			break;
		default:
			break;
		}

	}

	private void netTaskResetPsw(){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public void onPrepare() {
				showLoadingDialog();
			}

			@Override
			public ResponseInfo doInBackground() {
				mUserObj.setPsw(etPsw.getText().toString().trim());
				return getResponseInfo(mShowmoSys.resetPsw(mUserObj, mUserObj.getverifyCode(), mUserObj.getPsw()));
			}

			@Override
			public void onFailure(ResponseInfo info) {
				showToastShort(R.string.register_fail);
				closeLoadingDialog();
				/*int errorCode = (int) info.getErrorCode();
				if(handleNetConnectionError(errorCode)){
					return ;
				}
				switch (errorCode) {
				case NetWorkErrorCode.CSP_ERROR_VERIFYCODE_WRONG:
					showToastShort(R.string.verification_code_error);
					break;

				default:
					break;
				}*/

			}

			@Override
			public void onSuccess(ResponseInfo info) {
				showToastShort(R.string.reset_psw_success);
				
				
				
				
				resetpwdgo2loginActivity();
			}
		});
	}

	private void netTaskRegister(){
		mNetHelper.newNetTask(new RequestCallBack() {
			@Override
			public void onPrepare() {
				showLoadingDialog();
			}

			@Override
			public ResponseInfo doInBackground() {
				mUserObj.setPsw(etPsw.getText().toString().trim());
				return getResponseInfo(mShowmoSys.userRegister(mUserObj, mUserObj.getverifyCode(),ShowmoSystem.SHOWMO_USER));
			}
			
			@Override
			public void onSuccess(ResponseInfo info) {
				showToastShort(R.string.register_success);
				registergo2loginActivity();
			}
			
			@Override
			public void onFailure(ResponseInfo info) {
				showToastShort(R.string.register_fail);
				closeLoadingDialog();
				/*int errorCode = (int) info.getErrorCode();
				if(handleNetConnectionError(errorCode)){
					return ;
				}
				switch (errorCode) {
				case NetWorkErrorCode.CSP_ERROR_VERIFYCODE_WRONG:
					showToastShort(R.string.verification_code_error);
					break;

				default:
					break;
				}*/
			}
		});
	}
	
	private void resetpwdgo2loginActivity(){
		//成功之后 自动填充账号
		Intent in = new Intent(VerificationCodeActivity.this, LoginActivity.class);
		in.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP );
		in.putExtra(INTENT_KEY_STRING, mUserObj.getUserName());
		in.putExtra(INTENT_KEY_VERI_TO_LOGIN, "success");
		startActivity(in);
		finish();
		slideInFromLeft();
	}
	private void registergo2loginActivity(){
		//成功之后 自动填充账号
		Intent in = new Intent(VerificationCodeActivity.this, LoginActivity.class);
		in.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP );
		in.putExtra(INTENT_KEY_STRING, mUserObj.getUserName());
		in.putExtra(INTENT_KEY_VERI_TO_LOGIN, "success");
		startActivity(in);
		finish();
		slideInFromLeft();
	}
	@Override
	protected void onStop() {	
		super.onStop();
	}

@Override
protected void onDestroy() {	
	super.onDestroy();
}

private boolean checkInputContent() {
	String psw = etPsw.getText().toString().trim() ;
	String pswRe = etPswRe.getText().toString().trim();
	if(  !StringUtil.checkPsw(psw)  ){
		showToastShort(getString(R.string.psw_format_error));
		return false ;
	}else if(  !StringUtil.checkPswRe(psw, pswRe)  ){
		showToastShort(getString(R.string.psw_inconsistent));
		return false ;
	}

	return true;


}

}
