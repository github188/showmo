package com.showmo.activity.login;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.CAMERA_ALARM_TYPE;
import ipc365.app.showmo.jni.JniDataDef.SDK_APPLY_ACCOUNTINFO;

import java.util.List;

import com.showmo.R;
import com.showmo.activity.register.RegisterActivity;
import com.showmo.base.AlreadyLoginException;
import com.showmo.base.BaseActivity;
import com.showmo.base.NotInitException;
import com.showmo.base.ShowmoApplication;
import com.showmo.base.ShowmoSystem;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.network.NetWorkErrorCode;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.dao.AccountDao;
import com.showmo.ormlite.model.ShowmoAccount;
import com.showmo.userManage.User;
import com.showmo.util.AESUtil;
import com.showmo.util.LogUtils;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;
import com.showmo.widget.PasswordText;
import com.showmo.MainActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Rect;
import android.os.Bundle;
import android.text.Editable;
import android.text.Selection;
import android.text.Spannable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.CompoundButton.OnCheckedChangeListener;

/**
 * 登陆界面
 * 功能：手动登陆，自动登陆，记住账号，账号自动补全，密码显示与隐藏。
 * 
 * 
 * @author ljy
 *
 */
public class LoginActivity extends BaseActivity  {
	private static final String SHOWMO_GUEST_PSW  = "123456" ; 
	private String showmo_account;

	public static int ACCOUNT_STATE_DEFALUT = 0 ;

	public static String SP_KEY_LAST_LOGIN_ACCOUNT = "SP_KEY_LAST_LOGIN_ACCOUNT";

	public static String SP_KEY_AUTO_LOGIN = "SP_KEY_AUTO_LOGIN"; 

	private ScrollView scrollView;

	private RelativeLayout relativeLayout;

	private PasswordText etPsw;

	private AutoCompleteTextView etAccount;

	private AccountDao  accountDao ;

	private List<ShowmoAccount> listAccount ;

	private boolean isAutoLogin;

	private String[] userNameDatas;//用户名数组

	private boolean needClearPsw ;

	private CheckBox mCbSeePsw;

	private boolean flagFirst=true;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//LogOut("onCreate");
		setContentView(R.layout.activity_login);
		accountDao = (AccountDao) getDao(ShowmoAccount.class);
		initView();
		initNetwork();
		initLoginAuto();
		dbTaskGetAllAcount();
		showmoApp =ShowmoApplication.getInstance();
		showmoApp.addActivity(this);
		//	testMethod();

	}

	//	private void testMethod() {
	//		etAccount.setText("15068749510");
	//		etPsw.setText("123456");
	//	}

private Button loginBtnButton;
	/**
	 * 初始化view
	 */
	private void initView() {
		relativeLayout=(RelativeLayout) findViewById(R.id.login_img_city_background);
		relativeLayout.getViewTreeObserver().addOnGlobalLayoutListener(new OnGlobalLayoutListener(){

			//当键盘弹出隐藏的时候会 调用此方法。
			@Override
			public void onGlobalLayout() {
				//	                Rect r = new Rect();
				//	                //获取当前界面可视部分
				//	                getWindow().getDecorView().getWindowVisibleDisplayFrame(r);
				//	                //获取屏幕的高度
				//	                int screenHeight =  getWindow().getDecorView().getRootView().getHeight();
				//	                //此处就是用来获取键盘的高度的， 在键盘没有弹出的时候 此高度为0 键盘弹出的时候为一个正数
				//	                int heightDifference = screenHeight - r.bottom;
				//	                if (heightDifference>0) {
				//	                	relativeLayout.setVisibility(View.GONE);
				//					}else {
				//						relativeLayout.setVisibility(View.VISIBLE);
				//					}
			}

		});
		if(getWindow().getAttributes().softInputMode==WindowManager.LayoutParams.SOFT_INPUT_STATE_UNSPECIFIED){
			//隐藏软键盘
			//getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN);
			//隐藏图片
			relativeLayout.setVisibility(View.GONE);
		}




		loginBtnButton=(Button)findViewAndSet(R.id.btn_login_login);
		findViewAndSet(R.id.btn_login_register);
		findViewAndSet(R.id.btn_login_forget_psw);
		findViewAndSet(R.id.btn_login_fast_experience);

		mCbSeePsw = (CheckBox) findViewById(R.id.cb_see_psw) ;
		mCbSeePsw.setOnCheckedChangeListener(
				new OnCheckedChangeListener() {
					@Override
					public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
						etPsw.setPswVisible(isChecked);
					}
				});

		etPsw  = (PasswordText) findViewById(R.id.et_login_password); 

		//		etPsw.setOnKeyListener(new OnKeyListener() {
		//
		//			@Override
		//			public boolean onKey(View v, int keyCode, KeyEvent event) {
		//				if(event.getAction() == KeyEvent.ACTION_UP){
		//					switch (keyCode) {
		//						case KeyEvent.KEYCODE_ENTER:
		//							LogUtils.e("login", "KEYCODE_ENTER login");
		//							if(checkInputContent()){
		//								hideSoftInputMethod();
		//								netTaskLogin(etAccount.getText().toString(),etPsw.getText().toString(),ShowmoSystem.SHOWMO_USER,false);
		//							}
		//							break;
		//						default:
		//							break;
		//					}
		//					return false ;
		//					}
		//					return false;
		//				}
		//		});


		etPsw.addTextChangedListener(new TextWatcher() {
			private String curTypeInCharString="";
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {
				//Log.e("pwd", "1111111111::"+s+"   "+start+"    "+before+"   "+count);
				SharedPreferences mSp=getSharedPreferences(SHAREDPERENCES_LASTPSW, MODE_PRIVATE);
				mSp.edit().putString("psw", s.toString()).commit();
				if(s.length()==1){
					curTypeInCharString=""+s.charAt(0);
				}
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {

			}

			@Override
			public void afterTextChanged(Editable s) {
				if(needClearPsw){
					needClearPsw = false ;
					s.clear();
					s.append(curTypeInCharString);
					if (s instanceof Spannable) {
						Spannable spanText = (Spannable)s;
						Selection.setSelection(spanText, s.length());
					}
					mCbSeePsw.setVisibility(View.VISIBLE);
				}
			}
		});

		etAccount = (AutoCompleteTextView)findViewById(R.id.et_login_account);
		etAccount.setOnKeyListener(new OnKeyListener() {
			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event) {
				// TODO Auto-generated method stub
				if(event.getAction() == KeyEvent.ACTION_UP){
					if(keyCode == KeyEvent.KEYCODE_DEL){
					//	LogUtils.e("login", "keyCode == KeyEvent.KEYCODE_DEL");
					}
				}
				return false;
			}
		});
		etAccount.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				TextView tv = (TextView) view ;
				autoWriteLoginInfo( tv.getText().toString());
				isAutoLogin = false ;
				//重置光标
				Editable text = null ;
				if(etAccount.isFocused()){
					text = etAccount.getText();
				} 
				if (text instanceof Spannable) {
					Spannable spanText = (Spannable)text;
					Selection.setSelection(spanText, text.length());
				}

				hideSoftInputMethod();

			}
		});

		etAccount.addTextChangedListener(new TextWatcher() {

			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count) {

			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {

			}

			@Override
			public void afterTextChanged(Editable s) {
				if(needClearPsw){
					etPsw.setText("");
					needClearPsw = false ;
				}
			}
		});




	}


	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}
	@Override
	public void onClick(View view) {
		int viewId = view.getId();
		switch (viewId) {

		case R.id.btn_login_register:
			//slideInFromRight(RegisterActivity.class );
			User user = new User(User.ACTION_VERI_REGISTER);
			slideInFromRight(RegisterActivity.class,user);
			break;

		case R.id.btn_login_fast_experience:
			netTaskGetAccountValue();

			break;

		case R.id.btn_login_forget_psw:
			//slideInFromRight( ResetPswActivity.class);
			User user1 = new User(User.ACTION_VERI_RESET_PSW);
			slideInFromRight(ResetPswActivity.class,user1);
			break;
		case R.id.btn_login_login:
		//	LogUtils.e("login", "btn_login_login login");
			if(checkInputContent()){
				netTaskLogin(etAccount.getText().toString(),etPsw.getText().toString(),ShowmoSystem.SHOWMO_USER,false);
			}
			break;

		default:
			break;
		}


	}
	private void netTaskGetAccountValue(){
		mNetHelper.newNetTask(new RequestCallBack() {
			@Override
			public void onPrepare() {
				showLoadingDialog();
			}
			@Override
			public ResponseInfo doInBackground() {
				// TODO Auto-generated method stub
				SDK_APPLY_ACCOUNTINFO INFO= JniClient.PW_NET_ApplyTestAccount(ShowmoSystem.SHOWMO_USER);
				ResponseInfo resInfo=new ResponseInfo();
				if (INFO!=null) {				
					resInfo.setIsSuccess(true);
					showmo_account=INFO.user_name;				
				}else {
					resInfo.setIsSuccess(false);
				}									
				return resInfo;
			}
			@Override
			public void onFailure(ResponseInfo info) {
				closeLoadingDialog();
				showToastShort(R.string.get_apply_test_account_failure);
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				netTaskLogin(showmo_account,SHOWMO_GUEST_PSW,ShowmoSystem.SHOWMO_USER,true);
			}
		});

	}

	private void initLoginAuto() {
		if(accountDao == null ){
			Log.e("out", "accountDao == null");
			return ;
		}

		if(showmoSp == null){
			showmoSp = getSharedPreferences(SHAREDPERENCES_NAME, MODE_PRIVATE);
		}
		isAutoLogin = showmoSp.getBoolean(SP_KEY_AUTO_LOGIN, false);

		//获取本地保存的账号
		String userName = showmoSp.getString(SP_KEY_LAST_LOGIN_ACCOUNT, "");
		
		if(!autoWriteLoginInfo(userName)){
			return ;
		}

		if(isAutoLogin){
			netTaskLogin(etAccount.getText().toString(),etPsw.getText().toString(),ShowmoSystem.SHOWMO_USER,false);
		}
	}


	/**
	 * 根据用户名在数据库中查找密码
	 * 并且填写etAccount、etPsw
	 * @param userName
	 * @return false 1、参数为空 2、数据库中找不到 3、密码为空
	 */
	private boolean autoWriteLoginInfo(String userName){
	//	LogUtils.e("autologin", "last one username "+userName);
		if(StringUtil.isNotEmpty(userName)){

			List<ShowmoAccount> res = accountDao.queryByUserName(userName);
			//LogUtils.e("autologin", "query in database: "+res);
			if(res != null){
				etAccount.setText(userName);
				String pswStr = res.get(0).getPsssword();
				byte[] psw = AESUtil.decrypt(AESUtil.parseHexStr2Byte(pswStr), AESUtil.KEY_AES);
				LogUtils.e("autologin", "decrypt psw: "+psw);
				if(psw == null){
					LogOut("psw-->null");
					return false;
				}
				SharedPreferences mSp=getSharedPreferences(LoginActivity.SHAREDPERENCES_LASTPSW, MODE_PRIVATE);
				String sharepsw=mSp.getString("psw", "");
				Intent intent=getIntent();
				if ((!sharepsw.equals(new String(psw)))) {					
					etPsw.setText("");
				}else{
					etPsw.setText(new String(psw));
				}

				if ("success".equals(intent.getStringExtra(INTENT_KEY_VERI_TO_LOGIN)) && flagFirst){			
					etAccount.setText("");
					etPsw.setText("");
					flagFirst=false;
				}

				needClearPsw = true ;
				mCbSeePsw.setVisibility(View.INVISIBLE);
			//	LogUtils.e("autologin", "auto enter over ");
				return true ;
			}else{
				LogOut("res-->null");
			}
		}
		//LogUtils.e("autologin", "auto enter failured ");
		return false ;
	}

	private void LogOut(String str){
		Log.i("LoginActivity", str);
	}


	private boolean isAlreadyLogin=false;
	private synchronized void netTaskLogin( final String account ,final String psw,final int type,final boolean isExperience){
		mNetHelper.newNetTask(new RequestCallBack() {
			@Override
			public void onPrepare() {
				showLoadingDialog();
			}
			@Override
			public ResponseInfo doInBackground() {
				synchronized (this) {
					try {
						String targetAccount="";
						if( StringUtil.checkEmail(account)	){
							targetAccount = StringUtil.email2LowerCase(account) ;
						}else{
							targetAccount=account;
						}	
					//	LogUtils.e("login", "begin to login ");
						boolean bres=false;
						if(!isAlreadyLogin){
							
							bres= mShowmoSys.userLogin(account,psw,type,isExperience);
							if(bres){
								isAlreadyLogin=true;
							//	LogUtils.e("login", "login success ");
								if(!isExperience){
									if(!isAutoLogin){   //非自动登录时 保存一下信息
										//LogUtils.e("autologin", "dbTaskSaveAccount");
										dbTaskSaveAccount();
										isAutoLogin = true ;
									}
								}
							}
						}else{
							bres=true;
						}
						return getResponseInfo(bres);
					} catch (NotInitException e) {
						LogUtils.fe(LogUtils.LogAppFile, "NotInitException catch");
						mShowmoSys.init(LoginActivity.this);
						ToastUtil.toastShort(getContext(), R.string.login_failuer);
						e.printStackTrace();
						return getResponseInfo(false) ;
					}catch (AlreadyLoginException e) {
						// TODO: handle exception
						e.printStackTrace();
						return getResponseInfo(false);
					}
				}
			}
			@Override
			public void onSuccess(ResponseInfo info) {
				LogUtils.fe(LogUtils.LogAppFile, "login success!!!");
				closeLoadingDialog();
				loginBtnButton.setEnabled(false);
				showToastShort(R.string.login_successfully);
				slideInFromRight(MainActivity.class);
				finish();
			}

			@Override
			public void onFailure(ResponseInfo info) {
				LogUtils.fe(LogUtils.LogAppFile, "login failured!!!");
				closeLoadingDialog();
				isAutoLogin = false ;
				int errorCode = (int) info.getErrorCode();
				if(handleNetConnectionError(errorCode)){
					return ;
				}
				switch (errorCode) {
				case NetWorkErrorCode.CSP_ERROR_USER_NAME_NOTEXIST:
					showToastShort(R.string.user_not_exist);
					break;
				case NetWorkErrorCode.CSP_ERROR_USER_KEY_WRONG:
					showToastShort(R.string.user_psw_error);
					break;
				case NetWorkErrorCode.CSP_ERROR_LOGIN_REPEAT:
					showToastShort(R.string.login_repeat);
					break;
				case NetWorkErrorCode.CSP_ERROR_LOGIN_IN_OTHER_INTERMINAL:
					showToastShort(R.string.others_login);
					break;
				default:
					LogUntreatedError(errorCode);
					break;
				}
			}

			@Override
			public void onFinally() {
				
			}
		});
	}
	/**
	 * 用户成功登录后调用此方法 保存账号信息
	 */
	private void saveAccountMsg(){
		String userName = etAccount.getText().toString().trim();
		String psw = etPsw.getText().toString().trim();
		String pswAes = AESUtil.parseByte2HexStr(AESUtil.encrypt(psw, AESUtil.KEY_AES));
		//保存至本地 保存成功登录的用户的信息
		if(accountDao == null ){
			Log.e("out", "accountDao == null");
			return   ;
		}
		boolean havaSameAccount = false ;

		if(listAccount != null){//检验是否已有账号保存在本地

			for (ShowmoAccount showmoAccount : listAccount) {
				
				if(showmoAccount.getUserName().equals(userName)){
					havaSameAccount = true ;
					break ;
				}
			}

		}


		int dbRes = 0 ; 
		if(havaSameAccount){//有同样的账号 则更新数据库中的账号
			dbRes =  accountDao.updateAccount( new ShowmoAccount(
					userName,pswAes,
					ACCOUNT_STATE_DEFALUT, //预留的属性，未使用
					true //true 记住密码
					));
		//	LogUtils.e("autologin", "updateAccount "+dbRes);
		}else{

			dbRes =  accountDao.insertAccount( new ShowmoAccount(
					userName,pswAes,
					ACCOUNT_STATE_DEFALUT, //预留的属性，未使用
					true //true 记住密码
					));
		//	LogUtils.e("autologin", "insertAccount "+dbRes);
		}

		if(dbRes == 0){
			Log.e("autologin", "用户信息未保存成功");
			return ; 
		}

	}

	private void getAllAccount(){
		List<ShowmoAccount> res =  accountDao.queryForAllAccount();
		if(res != null){
			listAccount = res ;
		}
	}

	private void setAdapterForAutoText(){
		if(listAccount != null) {

			userNameDatas = new String[listAccount.size()];
			int index = 0 ;
			for (ShowmoAccount temp : listAccount) {
				userNameDatas[index] = temp.getUserName();
				index ++ ;
			}

			ArrayAdapter<String> adapter = new ArrayAdapter<String>(
					LoginActivity.this, R.layout.single_textview, userNameDatas);  
			etAccount.setAdapter(adapter);

		}
	}

	private void dbTaskSaveAccount(){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public ResponseInfo doInBackground() {
				saveAccountMsg();
				getAllAccount();
				return null;
			}

			@Override
			public void onFinally() {
				setAdapterForAutoText();
			}

		});
	}

	private void dbTaskGetAllAcount(){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public ResponseInfo doInBackground() {
				getAllAccount();
				return null;
			}
			@Override
			public void onFinally() {
				setAdapterForAutoText();
			}


		});
	}


	/**
	 * 检验用户输入内容
	 * @return true 内容通过检验
	 */
	private boolean checkInputContent() {

		String account =  etAccount.getText().toString().trim();
		String psw = etPsw.getText().toString().trim() ;

		if(	 !StringUtil.checkPhoneNumber(account ) &&
				!    StringUtil.checkEmail(account)	    ){
			showToastShort(getString(R.string.account_format_error));
			return false ;
		}else if(  !StringUtil.checkPsw(psw)  ){
			showToastShort(getString(R.string.psw_format_error));
			return false ;
		} 


		return true;

	}

	@Override
	protected void onStop() {
		super.onStop();

		//保存至SP中 用于查看是否要自动登录
		if(showmoSp == null){
			showmoSp = getSharedPreferences(SHAREDPERENCES_NAME, MODE_PRIVATE);
		}
		if(isAutoLogin){
			LogOut("onStop().isAutoLogin-->true");
			Editor editor = showmoSp.edit();
			editor.putString(SP_KEY_LAST_LOGIN_ACCOUNT, etAccount.getText().toString().trim());
			//LogUtils.e("autologin", "activity stop remember user:"+etAccount.getText().toString());
			editor.putBoolean(SP_KEY_AUTO_LOGIN, true);
			editor.commit();
		}else{
			//LogOut("onStop().isAutoLogin-->false");
			Editor editor = showmoSp.edit();
			editor.putBoolean(SP_KEY_AUTO_LOGIN, false);
			editor.commit();
		}


	}


	//	@Override
	//	public boolean onKeyDown(int keyCode, KeyEvent event) {
	//	    if(keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_UP){   
	//	        onBackPressed();
	//	        return false;   
	//	    }
	//	    return super.onKeyDown(keyCode, event);
	//	}

	@Override
	protected void onNewIntent(Intent intent) {
		LogOut("onNewIntent");
		//		String userName = intent.getStringExtra(INTENT_KEY_STRING);
		//		if(StringUtil.isNotEmpty(userName)){
		//			if(SHOWMO_GUEST_ACCOUNT.equals(userName)){ //不显示
		//				return ;
		//			}
		//			etAccount.setText(userName);
		//			etPsw.setText("");
		//			etPsw.requestFocus();
		//		}

		super.onNewIntent(intent);
	}

	@Override
	protected void onRestart() {
		LogOut("onRestart");
		super.onRestart();
		isAutoLogin =false ;
	}

	@Override
	protected void onDestroy() {
		LogOut("onDestroy");
		super.onDestroy();
		//showmoApp.exit();
	}

}
