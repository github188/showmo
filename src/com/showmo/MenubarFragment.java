package com.showmo;

import java.io.File;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import javax.security.auth.PrivateCredentialPermission;

import com.showmo.R;
import com.showmo.activity.alarmInfo.DeviceListActivity;
import com.showmo.activity.deviceManage.DeviceManageActivity;
import com.showmo.activity.deviceManage.DeviceManageTabHostActivity;
import com.showmo.activity.login.LoginActivity;
import com.showmo.activity.login.ResetPswHaveLoginActivity;
import com.showmo.activity.more.ActivityAbout;
import com.showmo.activity.more.ActivityMore;
import com.showmo.activity.purchase.PurchasePageActivity;
import com.showmo.activity.safe.ActivitySafeCenter;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoSystem;
import com.showmo.event.PlaybackEvent;
import com.showmo.preference.userSet.UserSetPreferenceActivity;
import com.showmo.safe.Safe;
import com.showmo.service.PwMsgCallbackDealService;
import com.showmo.service.PwSendExceptionMailService;
import com.showmo.userManage.User;
import com.showmo.util.AnimUtil;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;
import com.showmo.util.MailUtils;
import com.showmo.util.PwNetWorkHelper;
import com.showmo.util.SoundUtil;
import com.showmo.util.ZoomPic;
import com.showmo.widget.AbstractTipButton;
import com.showmo.widget.PwTipButton;
import com.showmo.widget.PwTipImgButton;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.MSGBroadcastActions;
import ipc365.app.showmo.jni.JniDataDef.User_2_mgr_disconn_device;
import android.R.bool;
import android.R.integer;
import android.R.interpolator;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.res.AssetManager;
import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.Path;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.media.AudioManager;
import android.media.SoundPool;
import android.net.wifi.WifiInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.support.v4.widget.SimpleCursorAdapter.ViewBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewParent;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.PopupMenu;
import android.widget.PopupWindow;
import android.widget.RelativeLayout;

public class MenubarFragment extends BaseFragment {
	private ImageButton m_userBtn;
	private AbstractTipButton m_managerMenuBtn;
	private AbstractTipButton m_alarmMenuBtn;
	private AbstractTipButton m_managerBtn;
	private ImageButton m_shareBtn;
	private List<AbstractTipButton> mManagersTipList;
	private Button m_moreBtn;
	
	private ImageButton m_purchaseBtn;
	private PopupWindow m_userWindow;
	private PopupWindow m_managerWindow;
	private MENU_STATE m_curMenuState = MENU_STATE.NONE;
	private MenuClickListener m_menuClickListener;
	private MenuItemClickListener m_menuItemClickListener;
	private OnMenuBarOperationListener m_perationListener;
	private BroadcastReceiver m_NewChangeMsgBroadcastRec;//收到告警信息或者升级信息之后加红点的广播
	//private SoundUtil sUtil;

	public enum MENU_STATE {
		NONE, USER_MENU, MANAGER_MENU, ALARM
	}
	public MenubarFragment(){
		super();
		m_userWindow = null;
		m_managerWindow = null;
		m_perationListener=null;
	}
	public interface OnMenuBarOperationListener{
		void systemExit();
		void exitCurAccount();
	}
	public void setOnMenuBarOperationListener(OnMenuBarOperationListener listener){
		m_perationListener=listener;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		LogUtils.v("fragment", "onCreateView");
		//sUtil=SoundUtil.getInstance();
		return inflater.inflate(R.layout.fragment_mainpage_menubar, container,
				false);
	}
	@Override
	public void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		registerRecievers();
	}
	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		unregisterRecievers();
	}
	private void registerRecievers(){
		IntentFilter filter=new IntentFilter();
		filter.addAction(MSGBroadcastActions.UPDATE_MOBILE_INVITE_INFO);
		filter.addAction(MSGBroadcastActions.CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG);
		m_NewChangeMsgBroadcastRec=new BroadcastReceiver(){
			public void onReceive(Context context, Intent intent) {
				if(intent.getAction()==MSGBroadcastActions.UPDATE_MOBILE_INVITE_INFO){
					//addNewFlagPointToView(m_managerMenuBtn);
					m_managerMenuBtn.showTipImg();
					m_managerBtn.showTipImg();
				}else if(intent.getAction()==MSGBroadcastActions.CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG){
					//addNewFlagPointToView(m_alarmMenuBtn);
					m_alarmMenuBtn.showTipImg();
				}
			};
		};
		m_activity.registerReceiver(m_NewChangeMsgBroadcastRec, filter);

	}

	private void unregisterRecievers(){
		m_activity.unregisterReceiver(m_NewChangeMsgBroadcastRec);
	}

//	private void hideRedPointIfExist(View targetView){
//		Object o=targetView.getTag();
//		if(o!=null && (o instanceof View)){
//			View v=(View)o;
//			v.setVisibility(View.GONE);
//			return;
//		}
//	}
//	/*targetView 的Tag为红点图片的引用
//	 * 如果tag不存在则为其添加红点，已存在则将红点显示
//	 */
//	private void addNewFlagPointToView(View targetView){
//		Object o=targetView.getTag();
//		if(o!=null && (o instanceof View)){
//			View v=(View)o;
//			v.setVisibility(View.VISIBLE);
//			return;
//		}else{
//			ViewParent vp= targetView.getParent();
//			if(vp instanceof RelativeLayout){
//				LogUtils.v("addflag", "vp instanceof RelativeLayout");
//				RelativeLayout rl=(RelativeLayout)vp;
//				ImageView iv=new ImageView(m_activity);
//				iv.setImageResource(R.drawable.ico_dian);
//				RelativeLayout.LayoutParams lp=new RelativeLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
//				lp.addRule(RelativeLayout.ALIGN_RIGHT,targetView.getId());
//				lp.addRule(RelativeLayout.ALIGN_TOP,targetView.getId());
//				lp.topMargin=HexTrans.dip2px(m_activity, 4);
//				lp.rightMargin=HexTrans.dip2px(m_activity, 4);
//				rl.addView(iv,lp);
//				targetView.setTag(iv);
//			}
//			//			ViewGroup vGroup=(ViewGroup)vp;
//			//			ImageView iv=new ImageView(m_activity);
//			//			iv.setImageResource(R.drawable.ico_dian);
//			//			vGroup.addView(iv);
//			////			iv.layout(targetView.getLeft()+targetView.getWidth(), 
//			////					targetView.getTop(), 
//			////					targetView.getLeft()+targetView.getWidth()+iv.getWidth(),
//			////					targetView.getTop()+iv.getHeight());
//			//			iv.scrollTo(targetView.getScrollX()+targetView.getWidth()-10, 
//			//					targetView.getScrollY()+10);
//			//			vGroup.invalidate();
//		}
//	}

	@Override
	public void onActivityCreated(Bundle savedInstaceState) {
		super.onActivityCreated(savedInstaceState);
		
		mManagersTipList=new ArrayList<AbstractTipButton>();
	
		m_shareBtn=(ImageButton)m_activity.findViewById(R.id.share);
		m_shareBtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				startActivity(new Intent(m_activity,ActivityAbout.class));
				AnimUtil.slideInFromRight(m_activity);						
			}
		});
		m_userBtn = (ImageButton) m_activity.findViewById(R.id.menu_user);
		m_managerMenuBtn = (AbstractTipButton) m_activity.findViewById(R.id.menu_manage);
		
		m_alarmMenuBtn = (AbstractTipButton) m_activity.findViewById(R.id.menu_alarm);
		
		m_purchaseBtn = (ImageButton) m_activity.findViewById(R.id.menu_purchase);
		m_menuClickListener = new MenuClickListener();
		m_userBtn
		.setOnClickListener((View.OnClickListener) m_menuClickListener);
		m_managerMenuBtn
		.setOnClickListener((View.OnClickListener) m_menuClickListener);
		m_alarmMenuBtn
		.setOnClickListener((View.OnClickListener) m_menuClickListener);
		if(showmoSystem.getCurUser().isExperience()){
			m_alarmMenuBtn.setVisibility(View.GONE);
		}
		m_purchaseBtn
		.setOnClickListener((View.OnClickListener) m_menuClickListener);
		m_menuItemClickListener = new MenuItemClickListener();
		initUserMenu();
		initDevManager();
	}
	public boolean isAnyManagerTiping(){
		for (AbstractTipButton tipbtn : mManagersTipList) {
			if(tipbtn.isTipShow()){
				return true;
			}
		}
		return false;
	}
	private void initDevManager(){
		LayoutInflater inflater = m_activity.getLayoutInflater();
		View manager_view = inflater.inflate(
				R.layout.menu_mainpage_manager, null);
		m_managerBtn = (AbstractTipButton) manager_view
				.findViewById(R.id.munu_item_device_manager);
		mManagersTipList.add(m_managerBtn);
		/*Button m_safe_center = (Button) manager_view
				.findViewById(R.id.munu_item_safe_center);*/
		Button m_set = (Button) manager_view
				.findViewById(R.id.munu_item_set);
		m_moreBtn = (Button) manager_view
				.findViewById(R.id.munu_item_more);
		m_managerBtn
		.setOnClickListener((View.OnClickListener) m_menuItemClickListener);
	/*	m_safe_center
		.setOnClickListener((View.OnClickListener) m_menuItemClickListener);*/
		m_set.setOnClickListener((View.OnClickListener) m_menuItemClickListener);
		m_moreBtn.setOnClickListener((View.OnClickListener) m_menuItemClickListener);

		m_managerWindow = new PopupWindow(manager_view,
				LayoutParams.WRAP_CONTENT,
				LayoutParams.WRAP_CONTENT, true);
		Drawable manageDrawableBg = m_activity.getResources()
				.getDrawable(R.drawable.manage_menu_bg);
		manageDrawableBg = ZoomPic.zoomDrawable(manageDrawableBg,
				HexTrans.dip2px(m_activity, 100),
				HexTrans.dip2px(m_activity, 130));
		m_managerWindow.setBackgroundDrawable(manageDrawableBg);
		m_managerWindow.setFocusable(true);
		m_managerWindow.setOutsideTouchable(true);

		if(showmoSystem.getCurUser().isExperience()){
			m_managerBtn.setVisibility(View.GONE);
			/*m_safe_center.setVisibility(View.GONE);*/
		}
	}
	
	private void initUserMenu(){
		LayoutInflater inflater = m_activity.getLayoutInflater();
		View menu_view = inflater.inflate(
				R.layout.menu_mainpage_user, null);
		Button m_modifyPsw = (Button) menu_view
				.findViewById(R.id.munu_item_modify_psw);
		Button m_switchUser = (Button) menu_view
				.findViewById(R.id.munu_item_switch_user);
		Button m_exit = (Button) menu_view
				.findViewById(R.id.munu_item_exit);
		m_modifyPsw
		.setOnClickListener((View.OnClickListener) m_menuItemClickListener);
		m_switchUser
		.setOnClickListener((View.OnClickListener) m_menuItemClickListener);
		m_exit.setOnClickListener((View.OnClickListener) m_menuItemClickListener);
		m_userWindow = new PopupWindow(menu_view,
				LayoutParams.WRAP_CONTENT,
				LayoutParams.WRAP_CONTENT, true);
		m_userWindow.setFocusable(true);
		m_userWindow.setOutsideTouchable(true);
		Drawable drawable = m_activity.getResources().getDrawable(
				R.drawable.user_menu_bg);
		drawable = ZoomPic.zoomDrawable(drawable,
				HexTrans.dip2px(m_activity, 100),
				HexTrans.dip2px(m_activity, 100));
		m_userWindow.setBackgroundDrawable(drawable);

		if(showmoSystem.getCurUser().isExperience()){
			m_modifyPsw.setVisibility(View.GONE);
		}
	}

	public class MenuClickListener implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			LogUtils.v("onClick", "MenuClickListener");
			switch (v.getId()) {
			case com.showmo.R.id.menu_user:
				m_userWindow.showAsDropDown(v, 0, 20);

				break;
			case R.id.menu_manage:
				//sUtil.playIntercomEnd();
				m_managerWindow.showAsDropDown(v, -v.getWidth() / 2, 20);

				break;
			case R.id.menu_alarm:
				//sUtil.playIntercomBegin();
				startActivity(new Intent(m_activity,DeviceListActivity.class));
				AnimUtil.slideInFromRight(m_activity);
				m_alarmMenuBtn.hideTipImg();
				break;
			case R.id.menu_purchase:
				startActivity(new Intent(m_activity,PurchasePageActivity.class));
				AnimUtil.slideInFromRight(m_activity);
				break;
			default:
				break;
			}
		}
	}

	public class MenuItemClickListener implements View.OnClickListener {
		@Override
		public void onClick(View v) {
			switch (v.getId()) {
			case R.id.munu_item_modify_psw:
				LogUtils.v("menu", "munu_item_modify_psw");
				Intent intent = new Intent(m_activity,ResetPswHaveLoginActivity.class);
				m_activity.startActivityForResult(intent, MainActivity.REQUEST_RESETPSW);
				m_activity.slideInFromRight( );

				break;
			case R.id.munu_item_switch_user:
				LogUtils.v("menu", "munu_item_switch_user");
				if(m_perationListener!=null){
					m_perationListener.exitCurAccount();
				}
				break;
			case R.id.munu_item_exit:
				LogUtils.v("menu", "munu_item_exit");
				if(m_perationListener!=null){
					m_perationListener.systemExit();
				}
				break;
			case R.id.munu_item_device_manager:
				LogUtils.v("menu", "munu_item_device_manager");
				//startActivity(new Intent(m_activity,DeviceManageActivity.class));
				startActivity(new Intent(m_activity,DeviceManageTabHostActivity.class));
				AnimUtil.slideInFromRight(m_activity);
				if(isAnyManagerTiping()){
					m_managerMenuBtn.hideTipImg();
				}
				m_managerBtn.hideTipImg();
				//hideRedPointIfExist(v);
				//hideRedPointIfExist(m_managerMenuBtn);
				break;
			/*case R.id.munu_item_safe_center:
				m_activity.slideInFromRight(ActivitySafeCenter.class);
				break;*/
			case R.id.munu_item_set:
				m_activity.slideInFromRight(UserSetPreferenceActivity.class);
				break;
			case R.id.munu_item_more:
				//m_activity.slideInFromRight(RecyclerTestActivity.class);
				
				m_activity.slideInFromRight(ActivityMore.class);
				//				new AsyncTask<Void, Void, Void>() {
				//					@Override
				//					protected Void doInBackground(Void... params) {
				//						// TODO Auto-generated method stub
				//						Log.e("send", "MailExceptionRunnable sendMail");
				//						boolean bres=MailUtils.sendMail("debug", "mail test", 
				//								"741355980@qq.com", "Li3885163", "741355980@qq.com", null);
				//						Log.e("send", "MailExceptionRunnable sendMail res "+bres);
				//						return null;
				//					}
				//				}.execute();
				//				Intent serviceIntent=new Intent("ipc365.app.showmo.service.PwSendExceptionMailService.RUNACTOIN");
				//				
				//				serviceIntent.putExtra("data", "this is a string from another process");
				//				m_activity.startService(serviceIntent);
				break;
			default:
				break;
			}
			if(m_userWindow!=null){
				m_userWindow.dismiss();
			}
			if(m_managerWindow!=null){
				m_managerWindow.dismiss();
			}
		}
	}
	public void dismissAllMenu(){
		if(m_managerWindow!=null){
			if(m_managerWindow.isShowing())
				m_managerWindow.dismiss();
		}
		if(m_userWindow!=null){
			if(m_userWindow.isShowing())
				m_userWindow.dismiss();
		}
	}
}
