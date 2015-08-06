package com.showmo.activity.login;

import com.showmo.base.BaseActivity;
import com.showmo.network.NetWorkErrorCode;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.model.ShowmoAccount;
import com.showmo.userManage.IUserObject;
import com.showmo.userManage.User;
import com.showmo.util.AESUtil;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;
import com.showmo.widget.PasswordText;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.CompoundButton.OnCheckedChangeListener;

import com.showmo.R;

public class ResetPswHaveLoginActivity extends BaseActivity{
	
	private EditText etAccount;
	private PasswordText etPsw;
	private PasswordText etPswRe;
	
	private PasswordText etPswOld;
	private String account;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_reset_psw_have_login);
		account = showmoSystem.getCurUser().getUserName() ;
		initView();
		initNetwork();
	}
	private void initView() {
		setBarTitle(R.string.reset_psw);
		findViewAndSet(R.id.btn_bar_back);
		
		etAccount=(EditText)findViewById(R.id.et_reset_account);
		etPsw = (PasswordText)findViewById(R.id.et_reset_psw);
		etPswRe   =(PasswordText)findViewById(R.id.et_reset_psw_re);
		etPswOld = (PasswordText)findViewById(R.id.et_reset_input_old_psw);
 
		findViewAndSet(R.id.btn_reset_mod);
		
		etAccount.setText(account);
	 
		( (CheckBox) findViewById(R.id.cb_see_psw)).setOnCheckedChangeListener(
				new OnCheckedChangeListener() {
					@Override
					public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
						etPsw.setPswVisible(isChecked);
						etPswRe.setPswVisible(isChecked);
						etPswOld.setPswVisible(isChecked);
					}
				});
//		etPswRe.setOnKeyListener(new OnKeyListener() {
//
//			@Override
//			public boolean onKey(View v, int keyCode, KeyEvent event) {
//				Log.e("out", "keyCode-->"+keyCode);
//				switch (keyCode) {
//				case KeyEvent.KEYCODE_ENTER:
//					if(checkInputContent()){
//						netTaskModifyPsw();
//					}
//					break;
//				default:
//					break;
//				}
//				return false;
//			}
//		});

	}

	@Override
	protected void onClick(int viewId) {
		switch (viewId) {
		case R.id.btn_reset_mod:
			if(checkInputContent()){

				netTaskModifyPsw();
			}
			break;
 
		case R.id.btn_bar_back:
			onBackPressed();
			break;

		default:
			break;
		}

	}
	private void netTaskModifyPsw(){
		mNetHelper.newNetTask(new RequestCallBack() {
			@Override
			public void onPrepare() {
				// TODO Auto-generated method stub
				super.onPrepare();
				showLoadingDialog();
			}
			@Override
			public ResponseInfo doInBackground() {
				
				String pswNew = etPsw.getText().toString();
				IUserObject userObj=new User(account, etPswOld.getText().toString(), false, null);
				return getResponseInfo(mShowmoSys.modifyPsw(userObj, pswNew ));
			}
			
			@Override
			public void onSuccess(ResponseInfo info) {
				Intent intent = new Intent();
				intent.putExtra(INTENT_KEY_STRING, etAccount.getText().toString());
				
				String pswAes = AESUtil.parseByte2HexStr(AESUtil.encrypt("_", AESUtil.KEY_AES));
				
				DaoFactory.getUserDao(ResetPswHaveLoginActivity.this).updateAccount(
						new ShowmoAccount(account,pswAes, 0, false));
				setResult(Activity.RESULT_OK ,intent);
				showmoSystem.unregisterXgPush();
				SharedPreferences mSp=getSharedPreferences(LoginActivity.SHAREDPERENCES_NAME, MODE_PRIVATE);
				mSp.edit().putBoolean(LoginActivity.SP_KEY_AUTO_LOGIN, false).commit();
				ToastUtil.toastShort(getContext(), R.string.modify_psw_success);
				finish();
			}
			
			@Override
			public void onFailure(ResponseInfo info) {
				int errorCode = (int) info.getErrorCode();
				if(handleNetConnectionError(errorCode)){
					return ;
				}
//				int	CSP_ERROR_USER_NAME_NOTEXIST = 102;
//				int	CSP_ERROR_USER_KEY_WRONG = 104 ;
				switch (errorCode) {
				case NetWorkErrorCode.CSP_ERROR_USER_NAME_NOTEXIST:
					showToastShort(R.string.user_not_exist);
					break;
				case NetWorkErrorCode.CSP_ERROR_USER_KEY_WRONG:
					showToastShort(R.string.user_psw_error);
					break;

				default:
					LogUntreatedError(errorCode);
					break;
				}
				
				
			}
			@Override
			public void onFinally() {
				// TODO Auto-generated method stub
				super.onFinally();
				closeLoadingDialog();
			}
		});
		
		
		
	}
	
	
	/**
	 * 检验用户输入内容
	 * @return 
	 */
	private boolean checkInputContent() {
 
		String account =  etAccount.getText().toString().trim();
		String pswNew = etPsw.getText().toString().trim() ;
		String pswNewRe = etPswRe.getText().toString().trim();
		String pswOld = etPswOld.getText().toString().trim();
		
		if(	 !StringUtil.checkPhoneNumber(account )  &&
			 !StringUtil.checkEmail(account)	){
			showToastShort(getString(R.string.account_format_error));
			return false ;
		}
		if(  !StringUtil.checkPsw(pswNew)  || !StringUtil.checkPsw(pswOld)  ){
			showToastShort(getString(R.string.psw_format_error));
			return false ;
		}
		if(  !StringUtil.checkPswRe(pswNew, pswNewRe)  ){
			showToastShort(getString(R.string.psw_inconsistent));
			return false ;
		}
		if(  StringUtil.checkPswRe(pswNew, pswOld)  ){
			showToastShort(getString(R.string.modify_psw_old_not_eq_new));
			return false ;
		}
		
		return true;
		
		
	}
	
	
}
