package com.showmo.activity.more;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.network.NetworkHelper;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.userManage.User;
import com.showmo.util.LogUtils;
import com.showmo.util.ToastUtil;

import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.TextView;
import android.widget.Toast;
import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.QueryAppVersionRet;

public class ActivityMore extends BaseActivity implements OnClickListener {
	private NetworkHelper m_WorkHelpler;
	private String curVersion;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_more);
		findView();
	
	}

	private void findView() {
		setBarTitleWithBackFunc(R.string.more_title);
		findViewAndSet(R.id.goto_update);
		findViewAndSet(R.id.goto_problem);
		findViewAndSet(R.id.goto_about);
		//findViewAndSet(R.id.goto_app_qr);
		try {
			PackageManager pm=getPackageManager();
			PackageInfo info=pm.getPackageInfo(this.getPackageName(), 0);
			curVersion=info.versionName;
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		((TextView)findViewById(R.id.cur_version)).setText(curVersion);
		m_WorkHelpler = NetworkHelper.getInstance();
		
	}
	private class CheckUpdateCallback extends RequestCallBack{

		@Override
		public ResponseInfo doInBackground() {
			// TODO Auto-generated method stub
			QueryAppVersionRet verion=new QueryAppVersionRet();
			boolean bres=JniClient.PW_ENT_GetAppVersion(1, verion);
			boolean haveNew=false;
			if(!bres){
				LogUtils.e("err", "app auto upgrade check failured:"+JniClient.PW_NET_GetLastError());
			}else {
				haveNew=showmoApp.checkAppNewVersion(verion.version);
			}
			ResponseInfo resInfo=new ResponseInfo();
			resInfo.boolValue=haveNew;
			resInfo.setObj(verion);;
			if(!bres){
				resInfo.setErrorCode(JniClient.PW_NET_GetLastError());
			}
			resInfo.setIsSuccess(bres);
			return resInfo;
		}
 
		@Override
		public void onSuccess(ResponseInfo info) {
			// TODO Auto-generated method stub
			super.onSuccess(info);
			closeLoadingDialog();
			if(info.isSuccess())
			{
				LogUtils.v("get", "get version "+((QueryAppVersionRet)info.getObj()).version);
				LogUtils.v("get", "get version "+((QueryAppVersionRet)info.getObj()).feature);
				if(info.boolValue){
					Intent updateIntent=new Intent(ActivityMore.this,ActivityAppUpdate.class);
					updateIntent.putExtra(ActivityAppUpdate.NewVersionKey, ((QueryAppVersionRet)info.getObj()).version);
					updateIntent.putExtra(ActivityAppUpdate.NewFeatureKey, ((QueryAppVersionRet)info.getObj()).feature);
					startActivity(updateIntent);
					slideInFromRight();
				}else {
					ToastUtil.toastShort(ActivityMore.this, R.string.more_no_newverison);
				}
			}
		}
		@Override
		public void onFailure(ResponseInfo info) {
			// TODO Auto-generated method stub
			closeLoadingDialog();
			super.onFailure(info);
			handleNetConnectionError((int)info.getErrorCode());
		}

		@Override
		public void onPrepare() {
			// TODO Auto-generated method stub
			showLoadingDialog();
			super.onPrepare();
		}
		
	}
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		super.onClick(v);
		switch (v.getId()) {
		case R.id.goto_update:
			//move2activity();
			m_WorkHelpler.newNetTask(new CheckUpdateCallback());
			break;
		case R.id.goto_problem:
			slideInFromRight(ActivityProblemFeedback.class);
			break;
		case R.id.goto_about:
			slideInFromRight(ActivityAbout.class);
			break;
		/*case R.id.goto_app_qr:
			slideInFromRight(ActivityAppQrDownload.class);
			break;*/
		default:
			break;
		}

	}
}
