package com.showmo;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.Thread.UncaughtExceptionHandler;
import java.lang.reflect.UndeclaredThrowableException;
import java.security.PublicKey;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

import com.showmo.userManage.IUserObject;
import com.showmo.userManage.User;
import com.showmo.util.AESUtil;
import com.showmo.util.AppStateCheck;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;
import com.showmo.util.MailUtils;
import com.showmo.util.MediaScanUtils;
import com.showmo.util.PathUtils;
import com.showmo.util.PwTimer;
import com.showmo.util.SoundUtil;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;
import com.showmo.util.VibrateUtil;
import com.showmo.widget.HorizontalListView;
import com.showmo.widget.MyGLSurfaceView;
import com.showmo.widget.NetLoadingView;
import com.showmo.widget.PwRoundVolumControlView;
import com.showmo.widget.PwSoundView;
import com.showmo.widget.VerticalSeekBar;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnCancelClickListener;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;
import com.showmo.widget.timeline.Timeline;

import java.util.List;

import javax.mail.internet.NewsAddress;

import rx.Observable;
import rx.Observable.OnSubscribe;
import rx.Scheduler;
import rx.Subscriber;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.functions.Func1;
import rx.internal.schedulers.ScheduledAction;
import rx.schedulers.Schedulers;

import com.puwell.opengles.ICaptureCallback;
import com.puwell.opengles.NVRGLSurfaceView;

import android.R.anim;
import android.R.bool;
import android.R.integer;
import android.R.interpolator;
import android.animation.Animator;
import android.animation.Animator.AnimatorListener;
import android.animation.AnimatorInflater;
import android.animation.AnimatorListenerAdapter;
import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.animation.ValueAnimator.AnimatorUpdateListener;
import android.app.ActionBar.LayoutParams;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.Paint;
import android.graphics.drawable.AnimationDrawable;
import android.graphics.drawable.GradientDrawable;
import android.media.AudioRecord;
import android.media.ThumbnailUtils;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.preference.PreferenceManager;
import android.provider.MediaStore.Audio.Media;
import android.renderscript.ScriptIntrinsicHistogram;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.widget.SimpleCursorAdapter.ViewBinder;
import android.text.Html;
import android.text.format.Time;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.View.OnKeyListener;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.AnimationUtils;
import android.view.animation.RotateAnimation;
import android.view.animation.ScaleAnimation;
import android.view.animation.TranslateAnimation;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.Gallery;
import android.widget.GridView;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.showmo.ExpandFragment.IntercomEvent;
import com.showmo.VideoMenubarFragment.OnVideoBtnClickListener;
import com.showmo.VideoMenubarFragment.OnVideoBtnClickListener.RecordCallback;
import com.showmo.activity.login.LoginActivity;
import com.showmo.activity.more.ActivityAppUpdate;
import com.showmo.activity.more.ActivityMore;
import com.showmo.base.*;
import com.showmo.commonAdapter.PWDevAdapter;
import com.showmo.dataDef.PWDeviceInfo;
import com.showmo.dataDef.PWPlayBackVideoFrame;
import com.showmo.deviceManage.Device;
import com.showmo.event.AckCaptureEvent;
import com.showmo.event.AckRecordEvent;
import com.showmo.event.CaptureEvent;
import com.showmo.event.DeviceAddEvent;
import com.showmo.event.ExpandEvent;
import com.showmo.event.InitLightEvent;
import com.showmo.event.MainPageMultiAreaDisplayEvent;
import com.showmo.event.PlayDisconnectEvent;
import com.showmo.event.PlaybackCompleteEvent;
import com.showmo.event.RecordEvent;
import com.showmo.event.SoundSwitchEvent;
import com.showmo.event.StopPlaybackEvent;
import com.showmo.event.TimelineShowEvent;
import com.showmo.eventBus.Event;
import com.showmo.eventBus.EventBus;
import com.showmo.eventBus.EventReciever;
import com.showmo.eventBus.util.AsyncExecutor;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef;
import ipc365.app.showmo.jni.JniDataDef.MSGBroadcastActions;
import ipc365.app.showmo.jni.JniDataDef.QueryAppVersionRet;
import ipc365.app.showmo.jni.JniDataDef.SDK_APPLY_ACCOUNTINFO;
import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;
import ipc365.app.showmo.jni.JniDataDef.SDK_SEARCH;
import ipc365.app.showmo.jni.JniDataDef.User_2_mgr_disconn_device;

import com.showmo.network.NetworkHelper;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.DatabaseHelper;
import com.showmo.ormlite.dao.DaoFactory;
import com.showmo.ormlite.dao.IDeviceDao;
import com.showmo.playHelper.AudioDecodeAndRender;
import com.showmo.playHelper.DecodeThread;
import com.showmo.playHelper.IDevicePlayer;
import com.showmo.playHelper.IDevicePlayer.EnumStreamType;
import com.showmo.playHelper.IDevicePlayer.PLAYER_STATUS;
import com.showmo.playHelper.IntercomApiWraper;
import com.showmo.playHelper.NoDeviceIsPlayingException;
import com.showmo.playHelper.OnPlaybackListener;
import com.showmo.playHelper.OnRealplayListener;
import com.showmo.playHelper.OnStopRealplayListener;
import com.showmo.playHelper.IntercomApiWraper.IntercomCallback;
import com.showmo.playHelper.IntercomApiWraper.IntercomErrThrowable;
import com.showmo.playHelper.PlayHelper;
import com.showmo.playHelper.RealplayOutParams;
import com.showmo.rxcallback.RxCallback;
import com.tencent.android.tpush.XGPushManager;
import com.tencent.android.tpush.service.t;

public class MainActivity extends BaseActivity implements
JniDataDef.OnRealdataCallBackListener, OnRealplayListener,
OnStopRealplayListener, OnPlaybackListener {
	private NVRGLSurfaceView m_glSurfaceView;
	private NVRGLSurfaceView m_glSurfaceViewBitmap;
	private FrameLayout m_videoLayout;
	private TextView m_statusText;
	private TextView m_lanText;
	private TextView m_RxText;
	private DecodeThread decodeThread = null;
	private MenubarFragment m_menubarFragment;
	private DeviceListFragment m_devicelistFragment;
	private VideoMenubarFragment m_videoMenubarFragment;
	private ExpandFragment m_ExpandFragment;
	private TimelineFragment m_timelineFragment;
	private PlaybackCtrlFragment mPlaybackCtrlFragment;

	private SoundUtil soundUtil;
	private VibrateUtil vibUtil;
	private final static int ExpandAreaH=ShowmoApplication.getInstance().getResources().getDimensionPixelSize(R.dimen.dimen_device_list_layout_h);
	private final static int VideoMenuH=ShowmoApplication.getInstance().getResources().getDimensionPixelSize(R.dimen.dimen_device_menu_layout_h);

	private BroadcastReceiver m_DeviceChangeReceiver;
	private BroadcastReceiver m_ScreenStateChangeReceiver;
	private PwSoundView m_intercomVolum;
	private NetLoadingView  m_intercomNetLoadingView;
	private RelativeLayout m_intercomLayout;
	private RelativeLayout m_intercomerrlayout;

	private static final String FmTagDevice = "device";
	private static final String FmTagTimeline = "timeline";
	private static final String FmTagMenubar = "menubar";
	private static final String FmTagVideoMenubar = "videomenubar";
	private static final String FmTagPlaybackCtrlBar="PlaybackCtrl";
	private static final String FmTagExpand = "expand";

	private static final int LandVideoMenuHideTime = 3000;

	private Timer m_exitTimer;
	private FragmentManager fm;
	private FrameLayout m_container_multi_compound;
	private FrameLayout m_container_menubar;
	private LinearLayout m_layout_video_menu;
	private LinearLayout m_layout_status_bar;

	private IDeviceDao m_DeviceDao;
	private MHandler m_Handler = new MHandler();
	private boolean m_exitTimerRunFlag;
	private ShowmoSystem m_showmoSys;
	private IDevicePlayer mPlayHelper;
	private PwTimer m_videoMenuBarHideTimer;
	private RTTimer mRxTimer;
	private PwTimer mCheckUpgradeAppTimer;

	private PWDevAdapter m_deviceAdapter = null;

	private TextView m_getDeviceErrTip;
	private ProgressBar m_getDeviceIndicator;
	private PwEventReciever m_EventReciever;

	private String m_strRecordPath = null;
	public IDevicePlayer m_PlayHelper;
	public final static int REQUEST_LOGIN = 1000;
	public final static int REQUEST_RESETPSW = 1001;
	private final static int ASYNTOAST = 1;
	private final static int ASYNSTATUSBARSET = 2;
	private final static int ASYNLANSET = 3;
	private final static int UPGRADEAPP = 4;
	private SharedPreferences mSp;

	private class MHandler extends Handler {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case ASYNTOAST:
				Toast.makeText(MainActivity.this, (String) msg.obj,
						Toast.LENGTH_SHORT).show();
				break;
			case ASYNSTATUSBARSET:
				LogUtils.v("stat", (String) msg.obj);
				m_statusText.setText((String) msg.obj);
				break;
			case ASYNLANSET:
				m_lanText.setText((String) msg.obj);
				break;
			case UPGRADEAPP:
				showUpgradeAppDialog((QueryAppVersionRet)msg.obj);
				break;
			default:
				break;
			}
		}
	}
	String streampath;
	File streamFile;
	FileOutputStream streamOut;
	private EnumExpandComType m_curExpandComType = EnumExpandComType.DeviceState;
	public enum EnumExpandComType {
		DeviceState, TimelineState, NoneState, ExpandState;
	}
	public enum EnumVideoCtrlType {
		realplay,playback;
	}

	public class RTTimer extends PwTimer {
		private long Rx = 0;

		public synchronized long getRx() {
			return Rx;
		}

		public synchronized void setRx(long rx) {
			Rx = rx;
		}

		public void clearRx() {
			Rx = 0;
			doInTask();
		}

		public RTTimer() {
			super(false);
		}

		public void stop() {
			super.stopIfStarted();
			Rx = 0;
			doInTask();
		}

		@Override
		public void doInTask() {
			// TODO Auto-generated method stub
			long period=(long)(getRepeatTime() / 1000.0);
			m_RxText.setText((long)(Rx / 1024.0f/period) + "Kb/s");
			setRx(0);
		}
	}
	PwInfoDialog mUpgradeQueryDialog;
	public void showUpgradeAppDialog(final QueryAppVersionRet info){
		if(mUpgradeQueryDialog == null){
			mUpgradeQueryDialog=new PwInfoDialog(this);
			mUpgradeQueryDialog.removeCancelBtn();
			mUpgradeQueryDialog.setContentText(R.string.upgrade_tip);
			mUpgradeQueryDialog.setOkBtnTextAndListener(R.string.upgrade_app_nomore, new OnOkClickListener() {
				@Override
				public void onClick() {
					// TODO Auto-generated method stub
					saveBooleanInSharedPreferences(SP_KEY_NOMORE_UPGRADE_APP, true);
				}
			});
			mUpgradeQueryDialog.addBtn(R.string.upgrade_app_yes, new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					mUpgradeQueryDialog.dismiss();
					Intent updateIntent=new Intent(MainActivity.this,ActivityAppUpdate.class);
					updateIntent.putExtra(ActivityAppUpdate.NewVersionKey, info.version);
					updateIntent.putExtra(ActivityAppUpdate.NewFeatureKey, info.feature);
					startActivity(updateIntent);
					slideInFromRight();
//					
//					Intent updateIntent=new Intent(Intent.ACTION_VIEW);
//					String str="market://details?id="+getPackageName();
//					Log.v("market", str);
//					updateIntent.setData(Uri.parse(str));
//					if(!AppStateCheck.isActivityExist(getContext(), updateIntent)){
//						ToastUtil.toastShort(getContext(), R.string.can_not_goto_target_activity);
//						return;
//					}
//					startActivity(updateIntent);
//					slideInFromRight();
				}
			});
		}
		try {
			mUpgradeQueryDialog.show();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}

	}

	private boolean isMainThread(){
		return (Thread.currentThread().getId()==getMainLooper().getThread().getId());
	}
	public MainActivity() {
		ShowmoApplication.getInstance().addActivity(this);
		//LogUtils.v("fragment", "MainActivity");
		this.init();
		m_videoMenuBarHideTimer = new PwTimer(false) {

			@Override
			public void doInTask() {
				// TODO Auto-generated method stub
				if (!m_videoMenubarFragment.isbRuning()) {
					setVideoMenuBarVisible(false);
				} else {
					this.start(LandVideoMenuHideTime);
				}
			}
		};
		mCheckUpgradeAppTimer=new UpGradeAppTimer();
		mCheckUpgradeAppTimer.start(10000, false);
		m_PlayHelper=showmoSystem.getPlayer();
	}
	public class UpGradeAppTimer extends PwTimer{
		public UpGradeAppTimer(){
			super(true);
		}
		@Override
		public void doInTask() {
			// TODO Auto-generated method stub
			//LogUtils.e("upgrade", "in upgrade app timer");
			if(getBooleanFromSharedPreferences(SP_KEY_NOMORE_UPGRADE_APP,false)){
				return;
			}
			QueryAppVersionRet verion=new QueryAppVersionRet();
			boolean bres=JniClient.PW_ENT_GetAppVersion(1, verion);

			if(!bres){
				LogUtils.e("err", "app auto upgrade check failured:"+JniClient.PW_NET_GetLastError());
				return;
			}
			if(showmoApp.checkAppNewVersion(verion.version)){
				Message msg=m_Handler.obtainMessage(UPGRADEAPP);
				msg.obj = verion;
				m_Handler.sendMessage(msg);
			}
		}
	}
	public class MyExitTimerTask extends TimerTask {
		public void run() {
			MainActivity.this.m_exitTimerRunFlag = false;
		}
	}

	@Override
	public boolean onKeyUp(int keyCode, android.view.KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			if (m_exitTimerRunFlag) {
				//				if(m_PlayHelper.getmCurDeviceInfo()!=null){
				//					m_PlayHelper.stopIfPlaying();
				//				}
				showmoApp.exit();
			}
			String str = getResources().getString(R.string.exit_warn);
			Toast t = Toast.makeText(this, str, Toast.LENGTH_SHORT);
			t.show();
			m_exitTimerRunFlag = true;
			m_exitTimer.schedule(new MyExitTimerTask(), 2500);
		}
		return true;
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Configuration confi = this.getResources().getConfiguration();
		setContentView(R.layout.activity_main);
		findView();
		if (confi.orientation == Configuration.ORIENTATION_PORTRAIT) {
			// m_ExpandComType = EnumExpandComType.DeviceState;
			initFragment();
		} else if (confi.orientation == Configuration.ORIENTATION_LANDSCAPE) {
			// m_ExpandComType = EnumExpandComType.NoneState;
			initFragment();
			onLandScreenLayoutChange();
		}
		try {
			m_DeviceDao = (IDeviceDao) DatabaseHelper.getHelper(this).getDao(
					Device.class);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		registerReceivers();
		IntercomApiWraper.init();
		AudioDecodeAndRender.RenderInit();
		registerXgPush();
		//		m_Handler.postDelayed(new Runnable() {
		//			@Override
		//			public void run() {
		//				// TODO Auto-generated method stub
		//				if(showmoSystem.getCurUser()!=null) {
		//					NetworkHelper.getInstance().newNetTask(new PwUpdateDeviceAsynTaskCallback());
		//				}
		//			}
		//		}, 200);
		//SDK_APPLY_ACCOUNTINFO INFO= JniClient.PW_NET_ApplyTestAccount(ShowmoSystem.SHOWMO_USER);

		//LogUtils.e("test", "SDK_APPLY_ACCOUNTINFO "+INFO.user_name+" "+INFO.user_type);
	}
	private void registerXgPush(){
		SharedPreferences sPreferences=PreferenceManager.getDefaultSharedPreferences(this);
		boolean bPushSwitch=sPreferences.getBoolean(BaseActivity.SP_KEY_XG_PUSH,true);
		if(bPushSwitch){
			String curRegAccount=sPreferences.getString(BaseActivity.SP_KEY_REG_ACCOUNT, "");
			String curAccount=showmoSystem.getCurUser().getUserName();
			if(!curRegAccount.equals(curAccount)){		
				if(!curRegAccount.equals("")){
					showmoSystem.unregisterXgPush();
				}
				showmoSystem.registerCurUserXgPush();
			}
		}
	}
	/*
	 * 实时播放状态监听
	 * @see com.showmo.playHelper.PlayHelper.OnRealplayTaskExecListener#onRealplayTaskExecStateListener(com.showmo.playHelper.RealplayOutParams)
	 */
	@Override
	public void onRealplayStateListener(RealplayOutParams para) {
		//		LogUtils.v("realplay",
		//				"ExecState CurOrder:" + para.getCurOrder() + " Res:  " + para.isExecRes()
		//				+ " Errcode: " + para.getErrcode() + " Thread Id:"
		//				+ Thread.currentThread().getId());
		String str = new String();
		switch (para.getCurOrder()) {
		case MGRSIGNIN:
			if (para.isExecRes()) {
				str = getString(R.string.connect_server_success);
			} else {
				str = getString(R.string.connect_server_fai);
			}
			break;
		case MGRRECONNECT:
			str = getString(R.string.mgr_reconnect);
			break;
		case GETDEVIP:
			if (para.isExecRes()) {
				str = getString(R.string.get_dev_ip_success);
			} else {
				str = getString(R.string.get_dev_ip_fai);
			}
			break;
		case REALPLAY:
			if (para.isExecRes()) {
				str = getString(R.string.realplay_success);
				if (m_lanText != null) {
					m_lanText.setVisibility(View.VISIBLE);
					m_lanText.setText((para.isArg2() ? "LAN" : "WAN"));
				}
			} else {
				if(para.getErrcode()!=2005){
					if(!isContinuePlaying){
						str = getString(R.string.realplay_fai)+"(错误代码:"+para.getErrcode()+")";
					}
				}else{
					str = getString(R.string.device_not_online);
				}
			}
			break;

		default:
			break;
		}
		postMsgToUI(ASYNSTATUSBARSET, str, 0, 0);
	}

	/*
	 * 回放结束监听
	 * @see com.showmo.playHelper.PlayHelper.OnPlaybackListener#onPlaybackOver(boolean, int)
	 */
	@Override
	public void onPlaybackOver(boolean bres, int err) {
		LogUtils.e("playbackres", "onPlaybackOver " + bres);
		isContinuePlaying=false;
		if (!bres) {
			postMsgToUI(ASYNSTATUSBARSET, getString(R.string.play_back_fai)+"(错误代码:"+err+")", 0, 0);
			handleNetConnectionError(err);
		} else {
			playbackedDevice();
			postMsgToUI(ASYNSTATUSBARSET, getString(R.string.play_back_success), 0, 0);
			showView(true,m_layout_video_menu, VideoMenuH,null);
			toExpandFragment(FmTagTimeline);
			replaceVideoMenuContainer(EnumVideoCtrlType.playback);
			m_glSurfaceView.setVisibility(View.VISIBLE);
			m_videoLayout.postInvalidate();
			if(isBackground){
				stopWhenBackground();
			}
		}
		closeLoadingDialog();
	}

	@Override
	public void onPlaybackPre() {
		LogUtils.v("playback", "onPlaybackPre ");
		showLoadingDialogCancelAble(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				v.setVisibility(View.GONE);
				mLoadimgDialog.setHint(R.string.canceling);
				m_PlayHelper.cancelPlayback();
			}
		});
	}

	@Override
	public void onPlaybackCompleted() {
		// TODO Auto-generated method stub
		onstopRealplayRes(true, 0);
		postMsgToUI(ASYNSTATUSBARSET, R.string.playback_complete, 0, 0);
	}

	/*
	 * 播放成功  ，显示操作条
	 */
	@Override
	public void onRealplayResultListener(final RealplayOutParams para) {
		//		LogUtils.v("realplay",
		//				"Result CurOrder:" + para.getCurOrder() + " Res: " + para.isExecRes() + " Errcode: "
		//						+ para.getErrcode() + " ThreadId "
		//						+ Thread.currentThread().getId());
		isContinuePlaying=false;
		if (!para.isExecRes()) {
			handleNetConnectionError(para.getErrcode());
		} else {
			//LogUtils.e("devicelist", "onRealplayResultListener");
			if(mPlayHelper.isPublic()){
				m_lanText.setText("WAN");
			}else{
				m_lanText.setText("LAN");
			}
			mRxTimer.start(3000, true);

			showView(true,m_layout_video_menu, VideoMenuH,null);
			Configuration confi = getResources().getConfiguration();
			if (confi.orientation == Configuration.ORIENTATION_LANDSCAPE){
				m_videoMenuBarHideTimer.start(LandVideoMenuHideTime);
			}
			showView(true,m_container_multi_compound, ExpandAreaH,null);
			toExpandFragment(FmTagExpand);
			EventBus.getDefault().post(new MainPageMultiAreaDisplayEvent(true));
			replaceVideoMenuContainer(EnumVideoCtrlType.realplay);

			try {
				m_PlayHelper.setStreamType(EnumStreamType.ADAPTER);
				mSp=getSharedPreferences(SHAREDPERENCES_NAME,MODE_PRIVATE);
				mSp.edit().putString(IDevicePlayer.SP_STREAM_KEY, IDevicePlayer.SP_STREAM_ADAPTER).commit();
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
			m_glSurfaceViewBitmap.setVisibility(View.INVISIBLE);
			m_glSurfaceView.setVisibility(View.VISIBLE);
			m_videoLayout.postInvalidate();
			AudioDecodeAndRender.RenderStart();
			AudioDecodeAndRender.RenderPause(!getBooleanFromSharedPreferences(SP_KEY_VIDEO_SOUND, true));
			setNeedThumbnail(true);
			initLight();
			if(isBackground){
				stopWhenBackground();
			}
		}
		closeLoadingDialog();
	}

	/*
	 * 停止实时/回放播放成功，隐藏设备列表，调出操作条，
	 */
	@Override
	public void onstopRealplayRes(boolean res, int errCode) {// 主线�?
		LogUtils.e("playbackres", "onstopRealplayRes ");
		String str;
		IntercomApiWraper.endIntercomIfOPen(new IntercomStopCb());
		if (res) {
			//m_timelineFragment.reset();
			StopedDevice();
			str = getResources().getString(R.string.stop_play_over);
			//			hideView(m_layout_video_menu);
			//			showView(m_container_multi_compound, 100);
			toExpandFragment(FmTagDevice);
			hideView(true,m_layout_video_menu,null);
			showView(true,m_container_multi_compound, ExpandAreaH,null);


			if (decodeThread != null)
				decodeThread.stop_decode();
			m_glSurfaceViewBitmap.setVisibility(View.INVISIBLE);
			m_glSurfaceView.setVisibility(View.INVISIBLE);
			m_videoLayout.postInvalidate();
			m_glSurfaceView.cancelThumbnail();
			clearFrameCount();
			setNeedThumbnail(false);
			m_glSurfaceView.cancelThumbnail();
			postMsgToUI(ASYNSTATUSBARSET, str, 0, 0);
			EventBus.getDefault().post(new AckCaptureEvent(AckCaptureEvent.captureOver,false));
		} else {
			str = getResources().getString(R.string.stop_play_err);
			handleNetConnectionError(errCode);
		}
	}
	@Override
	public void onStopPlayback(boolean bres,int errCode){
		onstopRealplayRes(bres,errCode);
		if(!isBackground){
			if(!m_PlayHelper.playbeforeRealplayDevice()){
				ToastUtil.toastShort(this, R.string.can_not_realplay_before);
			}
		}
	}
	private boolean isPlayDisconnected=false;
	private boolean isContinuePlaying=true;
	@Override
	public void onRealplayBeforeListener() {
		//		LogUtils.v("realplay", "onRealplayBeforeListener threadid:"
		//				+ Thread.currentThread().getId());
		mRxTimer.clearRx();
		showLoadingDialogCancelAble(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				isContinuePlaying=false;
				v.setVisibility(View.GONE);
				mLoadimgDialog.setHint(R.string.canceling);
				m_PlayHelper.cancelRealplay();
			}
		});
		if(isPlayDisconnected){
			mLoadimgDialog.setHint(R.string.play_disconnect);
			isPlayDisconnected=false;
		}
	}

	/*
	 * 播放状�?�，启动流量计算 无播放状态停止流量计算，同时去掉LAN WAN标志�?
	 */
	private void StopedDevice(){
		mRxTimer.stop();

		if (m_lanText != null) {
			m_Handler.post(new Runnable() {
				@Override
				public void run() {
					// TODO Auto-generated method stub
					m_lanText.setVisibility(View.GONE);
				}
			});
		}
	}
	private void playbackedDevice(){
		m_lanText.setVisibility(View.VISIBLE);
		mRxTimer.start(3000, true);
	}

	//	@Override
	//	public void onStatusChanged(PLAYER_STATUS stat) {// 主线程可能是非主线程
	//		// TODO Auto-generated method stub
	//		if (stat == PLAYER_STATUS.NOPLAY) {
	//			
	//		} else if(stat == PLAYER_STATUS.PLAYBACKING){
	//			if (m_lanText != null) {
	//				m_Handler.post(new Runnable() {
	//					@Override
	//					public void run() {
	//						// TODO Auto-generated method stub
	//						m_lanText.setVisibility(View.VISIBLE);
	//						mRxTimer.start(1000, true);
	//					}
	//				});
	//			}
	//		}else {
	//			mRxTimer.start(1000, true);
	//		}
	//	}
	private void initLight(){
		Observable.create(new Observable.OnSubscribe<Integer>() {
			@Override
			public void call(Subscriber<? super Integer> t) {
				// TODO Auto-generated method stub
				int value=JniClient.PW_NET_Brightness(m_PlayHelper.getmCurDeviceInfo().getmCameraId());
				LogUtils.v("light", "PW_NET_Brightness  "+value);
				if(value<0){
					EventBus.getDefault().post(new InitLightEvent(0));
				}else{
					if(value>=0 && value<=100){
						EventBus.getDefault().post(new InitLightEvent(value));
					}
				}
			}
		}).subscribeOn(Schedulers.io())
		.subscribe();
	}
	//	private void slideOutViewToBottom(View v) {
	//		//Animation anim=AnimationUtils.loadAnimation(this, R.anim.anim_slide_out_to_bottom);
	//		//anim.setFillAfter(true);
	//		//v.startAnimation(anim);
	//		v.setVisibility(View.GONE);
	//	}
	//	private void slideInViewFromBottom(View v){
	////		Animation anim=AnimationUtils.loadAnimation(this, R.anim.anim_slide_in_from_bottom);
	////		anim.setFillAfter(true);
	////		v.startAnimation(anim);
	//		v.setVisibility(View.VISIBLE);
	//	}
	private void getThumbnail(final Device dev){//获取微缩图
		Observable.create(new Observable.OnSubscribe<Bitmap>(){
			@Override
			public void call(final Subscriber<? super Bitmap> t) {
				// TODO Auto-generated method stub
				m_glSurfaceView.getThumbnail(new ICaptureCallback() {
					@Override
					public void onSuccess(Bitmap bmp) {
						// TODO Auto-generated method stub
						//LogUtils.e("thumbnail", "onSuccess");
						t.onNext(bmp);
					}
					@Override
					public void onFailured() {
						// TODO Auto-generated method stub
						//LogUtils.e("thumbnail", "onFailured");
						t.onError(new Exception("capture err"));
					}
					@Override
					public void onProcess() {
						// TODO Auto-generated method stub

					}
				});

			}
		}).subscribeOn(Schedulers.immediate())
		.flatMap(new Func1<Bitmap, Observable<String>>() {
			@Override
			public Observable<String> call(final Bitmap bitmap) {
				// TODO Auto-generated method stub
				
				return Observable.create(new OnSubscribe<String>() {
					@Override
					public void call(Subscriber<? super String> t) {
						// TODO Auto-generated method stub
						Bitmap thumbnailbmp=ThumbnailUtils.extractThumbnail(bitmap, 100, 100);
						File bmpfilename= new File(PathUtils.getThumbnailDataPath()+"/"+dev.getmCameraId()+".png");
						LogUtils.e("thumbnail","getThumbnailSuccess: file  "+bmpfilename.getAbsolutePath() );
						FileOutputStream os=null;
						try {
							os=new FileOutputStream(bmpfilename);
						} catch (Exception e) {
							// TODO: handle exception
							e.printStackTrace();
						}
						thumbnailbmp.compress(CompressFormat.PNG, 0, os);
						t.onNext(bmpfilename.getAbsolutePath());
					}
				});
			}
		}).observeOn(AndroidSchedulers.mainThread())
		.subscribe(new Subscriber<String>() {
			@Override
			public void onNext(String filename) {
				// TODO Auto-generated method stub
				for(Device device:showmoSystem.getCurUser().getDevices()){
					if(device.getmCameraId() == dev.getmCameraId()){
						device.setmTinyImgFilePath(filename);
						m_devicelistFragment.getAdapter().notifyDataSetChanged();
						DaoFactory.getDeviceDao(MainActivity.this).updateDevice(device);
					}
				}
			}
			@Override
			public void onError(Throwable e) {
				// TODO Auto-generated method stub
				e.printStackTrace();
			}
			@Override
			public void onCompleted() {}
		});
	}
	class ViewLayoutParamWrapper {
		private View v;

		public ViewLayoutParamWrapper(View v) {
			this.v = v;
		}

		public void setHeight(int height) {
			ViewGroup.LayoutParams param=v.getLayoutParams();
			param.height = height;
			v.requestLayout();
		}

		public int getHeight() {
			return v.getLayoutParams().height;
		}
	}
	private void hideView(boolean banim,final View v,final AnimCallback animcb) {
		if(!banim){
			ViewGroup.LayoutParams para= v.getLayoutParams();
			para.height=0;
			v.setLayoutParams(para);
			return;
		}

		LogUtils.e("hide", "hideView hideView hideView");
		ValueAnimator animator= ValueAnimator.ofFloat(v.getLayoutParams().height,0).setDuration(200);
		animator.start();
		animator.addUpdateListener(new AnimatorUpdateListener() {

			@Override
			public void onAnimationUpdate(ValueAnimator animation) {
				// TODO Auto-generated method stub
				float value=((Float)animation.getAnimatedValue()).floatValue();
				//Log.e("anim", "hideView onAnimationUpdate "+value);
				v.getLayoutParams().height=(int)value;
				v.requestLayout();
			}
		});
		animator.addListener(new PwAnimatorLis(animcb));
		//		new ViewLayoutParamWrapper(v).setHeight(0);
		//		ObjectAnimator animator= ObjectAnimator.ofInt(new ViewLayoutParamWrapper(v), "height",
		//				0).setDuration(500);
		//		animator.start();
		//		animator.addListener(new PwAnimatorLis(v));
	}

	public void showExpandArea(boolean bAnim, final View expandBtn){
		showView(bAnim,m_container_multi_compound, ExpandAreaH,new AnimCallback() {

			@Override
			public void end() {
				// TODO Auto-generated method stub
				if(expandBtn != null)
					expandBtn.setClickable(false);
			}

			@Override
			public void begin() {
				// TODO Auto-generated method stub
				if(expandBtn != null)
					expandBtn.setClickable(true);
			}
		});
		EventBus.getDefault().post(new MainPageMultiAreaDisplayEvent(true));
	}
	public void hideExpandArea(boolean bAnim,final View expandBtn){
		hideView(bAnim,m_container_multi_compound,new AnimCallback() {

			@Override
			public void end() {
				// TODO Auto-generated method stub
				if(expandBtn != null)
					expandBtn.setClickable(false);
			}

			@Override
			public void begin() {
				// TODO Auto-generated method stub
				if(expandBtn != null)
					expandBtn.setClickable(true);
			}
		});
		EventBus.getDefault().post(new MainPageMultiAreaDisplayEvent(false));
	}
	public interface AnimCallback
	{
		void begin();
		void end();
	}
	private void showView(boolean banim,final View v, int targetHdip,final AnimCallback animcb) {
		if(!banim){
			ViewGroup.LayoutParams para= v.getLayoutParams();
			para.height=targetHdip;
			v.setLayoutParams(para);
			return;
		}
		ValueAnimator animator= ValueAnimator.ofFloat(0,targetHdip).setDuration(200);
		animator.start();
		animator.addUpdateListener(new AnimatorUpdateListener() {

			@Override
			public void onAnimationUpdate(ValueAnimator animation) {
				// TODO Auto-generated method stub
				float value=((Float)animation.getAnimatedValue()).floatValue();
				v.getLayoutParams().height=(int)value;
				v.requestLayout();
			}
		});

		//		ObjectAnimator animator	= ObjectAnimator.ofInt(new ViewLayoutParamWrapper(v), "height",
		//						targetHdip).setDuration(500);
		//		animator.start();
		animator.addListener(new PwAnimatorLis(animcb));		
	}
	public class PwAnimatorLis extends AnimatorListenerAdapter{
		private AnimCallback cb;
		PwAnimatorLis(AnimCallback animcb){
			this.cb=animcb;
		}
		@Override
		public void onAnimationStart(Animator animation) {
			// TODO Auto-generated method stub
			//LogUtils.e("anim", "onAnimationStart");
			super.onAnimationStart(animation);
			if(cb!=null)
				cb.begin();
		}
		@Override
		public void onAnimationEnd(Animator animation) {
			// TODO Auto-generated method stub
			//LogUtils.e("anim", "onAnimationEnd");
			super.onAnimationEnd(animation);
			if(cb!=null)
				cb.end();
		}
	}
	private void hideOrShowView(final View v, int h) {
		ViewGroup.LayoutParams para = v.getLayoutParams();
		if (para.height > 0) {
			//				animator=ObjectAnimator
			//						.ofInt(new ViewLayoutParamWrapper(v), "height", 0)
			//						.setDuration(500);
			new ViewLayoutParamWrapper(v).setHeight(0);
		} else {
			//				animator=ObjectAnimator
			//						.ofInt(new ViewLayoutParamWrapper(v), "height",
			//								HexTrans.dip2px(this, h))
			//						.setDuration(500);
			new ViewLayoutParamWrapper(v).setHeight(HexTrans.dip2px(this, h));
		}
		//			animator.start();
		//			animator.addListener(new PwAnimatorLis(v));

		//		}
	}

	private boolean bNeedThumbnail=false;
	public synchronized void setNeedThumbnail(boolean bNeedThumbnail) {
		this.bNeedThumbnail = bNeedThumbnail;
	}

	private int IFrameCount = 0;
	private synchronized void addIFrame(){
		IFrameCount++;
	}
	private synchronized void clearFrameCount(){
		IFrameCount=0;
	}
	@Override
	public int onDataCallBack(byte[] pBuffer, long lStreamType, long lFrameNum,
			long lbufsize, long dwUser) {
		if (decodeThread == null) {
			Surface sf = m_glSurfaceView.getSurface();
			decodeThread = new DecodeThread(sf);
		}
		mRxTimer.setRx(mRxTimer.getRx() + lbufsize);
		if(lStreamType == 3) {
			//LogUtils.e("frameindex", "audio "+lFrameNum+" size "+lbufsize);
			SystemClock.elapsedRealtime();
			long timebefore=SystemClock.elapsedRealtime();
			AudioDecodeAndRender.RenderInputData(lFrameNum,pBuffer);
			//LogUtils.e("audioSpent", "input data use time-->"+(SystemClock.elapsedRealtime()-timebefore));
		}
		if (lStreamType == 0 || lStreamType == 1){
			//LogUtils.e("frameindex", "vedio "+lFrameNum);
			AudioDecodeAndRender.updateCurVideoFrameNum(lFrameNum);
			m_PlayHelper.streamFlowAdd(lbufsize);
			if(lStreamType == 0){
				addIFrame();
			}
			
			decodeThread.decode(pBuffer, pBuffer.length, (int) lStreamType);
			m_glSurfaceView.Render();
			
			if(lStreamType == 0 && IFrameCount == 2){
				if(bNeedThumbnail){
					LogUtils.i("thumnail", "bNeedThumbnail");
					FileOutputStream oStream=null;
//					try {
//						oStream=new FileOutputStream(new File(PathUtils.getExternalDiretory()+"/iframe.h264"),true);
//						oStream.write(pBuffer);
//						oStream.close();
//					} catch (Exception e) {
//						// TODO: handle exception
//						e.printStackTrace();
//					}
					m_Handler.postDelayed(new Runnable() {

						@Override
						public void run() {
							// TODO Auto-generated method stub
							getThumbnail(m_PlayHelper.getmCurDeviceInfo());
						}
					}, 400);
					setNeedThumbnail(false);
				}
			}
		}
		return 1;
	}

	private void findView() {
		// m_glSurfaceView = (MyGLSurfaceView) findViewById(R.id.video_surface);
		m_container_multi_compound = (FrameLayout) findViewById(R.id.container_device_timeline);
		m_container_menubar = (FrameLayout) findViewById(R.id.container_menu_bar);
		m_layout_video_menu = (LinearLayout) findViewById(R.id.video_menu_layout);
		m_layout_status_bar = (LinearLayout) findViewById(R.id.status_bar);
		m_getDeviceErrTip=(TextView)findViewAndSet(R.id.load_device_list_err_btn);
		m_getDeviceErrTip.setText(getString(R.string.get_device_err_tip));
		m_getDeviceErrTip.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
		m_getDeviceIndicator=(ProgressBar)findViewById(R.id.load_device_list_indicator);
		m_statusText = (TextView) findViewById(R.id.status_text);
		m_lanText = (TextView) findViewById(R.id.lan_state);
		m_RxText = (TextView) findViewById(R.id.download_text);
		if (m_glSurfaceView == null) {
			m_glSurfaceView = new NVRGLSurfaceView(this,null);
			// m_glSurfaceView.setOnTouchListener(new GlTouchListener());
			m_glSurfaceView.setGlClickListener(new GlClickListener());
		}
		if (m_glSurfaceViewBitmap == null) {
			m_glSurfaceViewBitmap = new NVRGLSurfaceView(this,null);
			m_glSurfaceViewBitmap.setGlClickListener(new GlClickListener());
			//m_glSurfaceView.setOn

		}

		m_videoLayout = (FrameLayout) findViewById(R.id.video_play_area);
		FrameLayout.LayoutParams lp = new FrameLayout.LayoutParams(
				FrameLayout.LayoutParams.MATCH_PARENT,
				FrameLayout.LayoutParams.MATCH_PARENT);
		m_videoLayout.addView(m_glSurfaceView, lp);
		m_videoLayout.addView(m_glSurfaceViewBitmap, lp);
		m_glSurfaceView.init(0,NVRGLSurfaceView.RENDER_H264);
		m_glSurfaceViewBitmap.init(0, NVRGLSurfaceView.RENDER_BITMAP);
		m_glSurfaceViewBitmap.setVisibility(View.INVISIBLE);
		m_glSurfaceView.setVisibility(View.VISIBLE);
		m_videoLayout.postInvalidate();

		m_intercomVolum=(PwSoundView)findViewById(R.id.intercom_round_volum);
		m_intercomNetLoadingView=(NetLoadingView)findViewById(R.id.intercom_net_loading);
		m_intercomLayout=(RelativeLayout)findViewById(R.id.intercom_layout);
		m_intercomerrlayout=(RelativeLayout)findViewById(R.id.intercom_net_err);
		showExpandArea(true,null);
		hideView(false,m_layout_video_menu,null);

	}

	private void showIntercomVolum(){
		m_intercomLayout.setVisibility(View.VISIBLE);
		m_intercomVolum.setVisibility(View.VISIBLE);
		m_intercomNetLoadingView.setVisibility(View.GONE);
		m_intercomerrlayout.setVisibility(View.GONE);
	}
	private void showIntercomLoading(){
		m_intercomLayout.setVisibility(View.VISIBLE);
		m_intercomVolum.setVisibility(View.GONE);
		m_intercomNetLoadingView.setVisibility(View.VISIBLE);
		m_intercomerrlayout.setVisibility(View.GONE);
	}
	private void showIntercomErr(){
		m_intercomLayout.setVisibility(View.VISIBLE);
		m_intercomVolum.setVisibility(View.GONE);
		m_intercomNetLoadingView.setVisibility(View.GONE);
		m_intercomerrlayout.setVisibility(View.VISIBLE);
	}

	public class GlClickListener implements NVRGLSurfaceView.GlClickListener {
		@Override
		public void OnClick() {
			// TODO Auto-generated method stub
		//	LogUtils.e("menu", "gl click0");
			Configuration confi = getResources().getConfiguration();
			if (confi.orientation == Configuration.ORIENTATION_LANDSCAPE) {
		//		LogUtils.e("menu", "gl click1");
				if (m_curExpandComType != EnumExpandComType.DeviceState) {
		//			LogUtils.e("menu", "gl click2");
					setVideoMenuBarVisible(true);
					m_videoMenuBarHideTimer.start(LandVideoMenuHideTime);
				}
			}
		}
		@Override
		public void onDoubleClick() {
			// TODO Auto-generated method stub
			LogUtils.e("gl", "onDoubleClick");
			if(mPlayHelper.getmCurDeviceInfo()!=null){
				JniClient.native_mpgl_reset();
			}
		}
	}
	public void postMsgToUI(int what, Object str, int arg1, int arg2) {
		Message msg = Message.obtain();
		msg.what = what;
		msg.obj = str;
		msg.arg1 = arg1;
		msg.arg2 = arg2;
		m_Handler.sendMessage(msg);
	}

	private void replaceVideoMenuContainer(EnumVideoCtrlType toType){
		FragmentTransaction transa = fm.beginTransaction();
		transa.addToBackStack(null);
		if(toType ==EnumVideoCtrlType.realplay ){
			transa.replace(R.id.container_video_menubar,
					m_videoMenubarFragment, FmTagVideoMenubar);
		}else if (toType == EnumVideoCtrlType.playback) {
			transa.replace(R.id.container_video_menubar, mPlaybackCtrlFragment,
					FmTagPlaybackCtrlBar);
		}

		transa.commit();
	}

	private void addFragment(Fragment fragment, int containerViewId,
			String tag, boolean visible) {
		// ft.setCustomAnimations(R.anim.anim_fragment_slide_in_from_bottom,
		// R.anim.anim_fragment_slide_out_to_bottom);
		FragmentTransaction ft=fm.beginTransaction();
		Fragment objRef = fm.findFragmentByTag(tag);
		if (objRef == null) {
			objRef = fragment;
			ft.add(containerViewId, objRef, tag).commit();
		} 
		if(!visible){
			ft.hide(objRef);
		}else{
			ft.show(objRef);
		}
	}

	private Fragment mCurFragment=null;
	private void toExpandFragment(String tag){
		FragmentTransaction fTransaction=fm.beginTransaction();
		Fragment toFragment=fm.findFragmentByTag(tag);
		if(tag.equals(FmTagDevice)){
			m_curExpandComType = EnumExpandComType.DeviceState;
		}else if(tag.equals(FmTagExpand)){
			m_curExpandComType = EnumExpandComType.ExpandState;
		}else if(tag.equals(FmTagTimeline)){
			m_curExpandComType = EnumExpandComType.TimelineState;
		}
		if(toFragment==null){
			if(tag.equals(FmTagDevice)){
				toFragment=m_devicelistFragment;
			}else if(tag.equals(FmTagExpand)){
				toFragment=m_ExpandFragment;
			}else if(tag.equals(FmTagTimeline)){
				toFragment=m_timelineFragment;
			}
			if(mCurFragment != null){
				fTransaction.hide(mCurFragment);
			}
			mCurFragment = toFragment;
			fTransaction.add(R.id.container_device_timeline,toFragment,tag);
			fTransaction.commit();
		}else{
			if(mCurFragment == toFragment){
				return;
			}else{
				fTransaction.hide(mCurFragment);
				fTransaction.show(toFragment);
				fTransaction.commitAllowingStateLoss();
				mCurFragment=toFragment;
			}
		}
	}

	private void initFragment() {
		toExpandFragment(FmTagTimeline);
		toExpandFragment(FmTagExpand);
		toExpandFragment(FmTagDevice);
		addFragment(m_menubarFragment,R.id.container_menu_bar, FmTagMenubar, true);
		addFragment(m_videoMenubarFragment, R.id.container_video_menubar,FmTagVideoMenubar, true);
		//		addFragment(m_timelineFragment, R.id.container_device_timeline, FmTagTimeline, false);
		//		addFragment(m_ExpandFragment, R.id.container_device_timeline, FmTagExpand, true);
	}

	private void init() {
		m_exitTimer = new Timer();
		m_exitTimerRunFlag = false;
		fm = this.getSupportFragmentManager();

		m_showmoSys = ShowmoSystem.getInstance();
		mPlayHelper = m_showmoSys.getPlayer();
		mPlayHelper.setOnRealplayListener(this);
		mPlayHelper.setOnStopPlayListener(this);
		mPlayHelper.setOnPlaybackListener(this);
		mRxTimer = new RTTimer();

		m_devicelistFragment = new DeviceListFragment();
		m_menubarFragment = new MenubarFragment();
		m_videoMenubarFragment = new VideoMenubarFragment();
		mPlaybackCtrlFragment = new PlaybackCtrlFragment();
		m_timelineFragment = new TimelineFragment();
		m_ExpandFragment = new ExpandFragment();
		m_videoMenubarFragment
		.setOnVideoBtnClickListener(new MyVideoBtnClickListener());
		m_menubarFragment
		.setOnMenuBarOperationListener(new MyMenubarOperateListener());
		showmoSystem.getCurUser().sortDevice();
		m_deviceAdapter = new PWDevAdapter(MainActivity.this, showmoSystem.getCurUser().getDevices(),
				R.layout.device_list_item);
		m_Handler.postDelayed(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				m_devicelistFragment.setAdapter(m_deviceAdapter);
			}
		},1000);


		soundUtil=SoundUtil.getInstance();
		vibUtil=VibrateUtil.getInstance();
	}

	private void onIntercomEvent(IntercomEvent ev){
		LogUtils.v("EventReciever", "IntercomEventReciever ");
		//super.onEventMainThread(ev);
		if(ev instanceof IntercomEvent){
			IntercomEvent iev=(IntercomEvent)ev;
			LogUtils.v("eventbus", "get A IntercomEvent from event bus "+iev.mIntercom);
			if(iev.mIntercom){
				m_intercomVolum.setVisibility(View.VISIBLE);
				Device curDev=m_PlayHelper.getmCurDeviceInfo();
				if(curDev == null){
					ToastUtil.toastShort(MainActivity.this, R.string.no_device_playing);
					return;
				}
				AudioDecodeAndRender.RenderPause(true);
				showIntercomLoading();
				IntercomApiWraper.beginIntercom(curDev.getmCameraId(), new IntercomCallback() {

					@Override
					public void onVolumChanged(int sucinfo){
						// TODO Auto-generated method stub
						//m_intercomVolum.setVolumLevel(sucinfo);
						m_intercomVolum.setVolum(sucinfo);
					}
					@Override
					public void onCompleted() {
						// TODO Auto-generated method stub
						IntercomApiWraper.closeSound();
						showIntercomVolum();
						//soundUtil.playIntercomBegin();
						vibUtil.vib(50);
						m_Handler.postDelayed(new Runnable() {

							@Override
							public void run() {
								// TODO Auto-generated method stub
								IntercomApiWraper.openSound();
							}
						}, 500);
					}
					@Override
					public void onError(Throwable result) {
						// TODO Auto-generated method stub
						showIntercomErr();
						if(getBooleanFromSharedPreferences(SP_KEY_VIDEO_SOUND, true)){
							AudioDecodeAndRender.RenderPause(false);
						}
						IntercomErrThrowable info=(IntercomErrThrowable)result;
						LogUtils.e("intercom", "intercom err "+info.errtype);
						switch (info.errtype) {
						case IntercomApiWraper.API_ERR:
							handleNetConnectionError(info.netErrCode);
							break;
						case IntercomApiWraper.DEV_STATE_ON:
							ToastUtil.toastShort(MainActivity.this, R.string.device_is_already_open);
							break;
						case IntercomApiWraper.DEV_OPEN_ERR:
							ToastUtil.toastShort(MainActivity.this, R.string.device_intercom_err);
							break;
						default:
							break;
						}
					}
				});
			}else{
				//m_intercomVolum.setVisibility(View.INVISIBLE);
				m_intercomLayout.setVisibility(View.GONE);
				IntercomApiWraper.endIntercomIfOPen(new IntercomStopCb());
			}
		}
	}
	private void onCaptureEvent(){
		if(m_glSurfaceViewBitmap.getVisibility()==View.VISIBLE){
			m_glSurfaceViewBitmap.capture(captureCallback);
		}else{
			m_glSurfaceView.capture(captureCallback);
		}
	}
	private void onRecordEvent(RecordEvent ev){
		Log.v("onRecordeBtnClick", "isRecord "+ev.getEnable());
		if(!ev.getEnable()) {
			boolean bres = JniClient.PW_NET_StopRecord();
			if(bres){
				MediaScanUtils.MediaScan(MainActivity.this, m_strRecordPath);
				String str=getString(R.string.device_record_suc);
				ToastUtil.toastShort(MainActivity.this, str);
			}else{
				ToastUtil.toastShort(MainActivity.this, R.string.device_stop_record_err);
			}
			EventBus.getDefault().post(new AckRecordEvent(AckRecordEvent.closed, bres));    
		} else {
			String path;
			path=PathUtils.getExternalDiretory();
			if(path!=null){
				path+="/ShowMo/Record";
			}else{
				path=PathUtils.getDataDiretory()+"/ShowMo/Record";
			}
			if(!PathUtils.createFileIfNotExist(path)){
				ToastUtil.toastShort(MainActivity.this, R.string.device_record_err);
				EventBus.getDefault().post(new AckRecordEvent(AckRecordEvent.opened, false));  
				return;
			}
			SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss", Locale.US);
			String filename = sdf.format(new Date())+".avi";
			m_strRecordPath = path+"/"+filename;	
			Log.v("onRecord", "m_strRecordPath "+m_strRecordPath);
			boolean bres = JniClient.PW_NET_Record(m_strRecordPath);
			if(!bres)
				ToastUtil.toastShort(MainActivity.this, R.string.device_record_err);
			EventBus.getDefault().post(new AckRecordEvent(AckRecordEvent.opened, bres));  
		}
	}
	public class PwEventReciever{
		//		public void onEventMainThread(DeviceAddEvent ev){
		//			LogUtils.v("EventReciever", "DeviceAddEvent onEventMainThread "+m_devicelistFragment.getAdapter().getCount());
		//			m_devicelistFragment.getAdapter().notifyDataSetChanged();
		//		}
		public synchronized void onEventMainThread(PlayDisconnectEvent ev){
			LogUtils.i("realplay", "PlayDisconnectEvent");
			isPlayDisconnected=true;
			isContinuePlaying=true;
			if(m_PlayHelper.getStatus() == PLAYER_STATUS.NOPLAY){
				return;
			}else if (m_PlayHelper.getStatus() == PLAYER_STATUS.PLAYBACKING) {
				postMsgToUI(ASYNSTATUSBARSET, getString(R.string.play_back_disconnect), 0, 0);
				//m_PlayHelper.playBeforePlaybackDevcice();
			}else if (m_PlayHelper.getStatus() ==PLAYER_STATUS.REALPLAYING ) {
				m_PlayHelper.playbeforeRealplayDevice();
			}
		}
		public void onEventMainThread(SoundSwitchEvent switchstate){
			AudioDecodeAndRender.RenderPause(!switchstate.getState());
			
		}
		public void onEventMainThread(RecordEvent ev){
			onRecordEvent(ev);
		}
		public void onEventAsync(CaptureEvent ev){
			onCaptureEvent();
		}
		public void onEventMainThread(IntercomEvent ev) {
			// TODO Auto-generated method stub
			onIntercomEvent(ev);
		}
		public void onEventMainThread(StopPlaybackEvent ev){
			AudioDecodeAndRender.RenderStop();
			mPlayHelper.stop();
		}
		public void onEventMainThread(ExpandEvent ev){
			if(ev.getEnable()){
				showExpandArea(true,m_container_multi_compound);
			}else{
				hideExpandArea(true,m_container_multi_compound);
			}
		}
	}
	public class IntercomStopCb extends RxCallback<Void>{
		@Override
		public void onNext(Void t) {
			// TODO Auto-generated method stub
			super.onNext(t);
			soundUtil.playIntercomEnd();
			if(getBooleanFromSharedPreferences(SP_KEY_VIDEO_SOUND, true)){
				AudioDecodeAndRender.RenderPause(false);
			}
		}
	}

	private void SaveCaptureAsync(final Bitmap bmp){
		Observable.create(new OnSubscribe<Void>() {
			@Override
			public void call(Subscriber<? super Void> t) {//保存截图
				// TODO Auto-generated method stub
				//LogUtils.e("capture", "SaveCaptureAsync ");
				long savetimeBegin=SystemClock.elapsedRealtime();
				String path = null;
				if(Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())){
					path=Environment.getExternalStorageDirectory().getPath()+"/ShowMo/Capture";
				} else {
					path=MainActivity.this.getFilesDir().getPath()+"/ShowMo/Record";
				}
				File file = new File(path);
				if (!file.exists()) {
					file.mkdirs();
				}
				SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss", Locale.US);
				File imageFile = new File(file, sdf.format(new Date())+ ".png");
				FileOutputStream fos;
				try {
					fos = new FileOutputStream(imageFile);
					bmp.compress(CompressFormat.PNG, 100, fos);
					MediaScanUtils.MediaScan(MainActivity.this, imageFile.getAbsolutePath());
					fos.flush();
					fos.close();
				} catch (FileNotFoundException e) {
					e.printStackTrace();
					ToastUtil.toastShort(MainActivity.this, getString(R.string.capture_over_saving_fai));
					EventBus.getDefault().post(new AckCaptureEvent(AckCaptureEvent.SaveOver, false));
					return;
				}catch (IOException e) {
					e.printStackTrace();
					ToastUtil.toastShort(MainActivity.this, getString(R.string.capture_over_saving_fai));
					EventBus.getDefault().post(new AckCaptureEvent(AckCaptureEvent.SaveOver, false));
					return;
				}
				//LogUtils.e("capture", "save img usetime:"+(SystemClock.elapsedRealtime() - savetimeBegin));
				ToastUtil.toastShort(MainActivity.this, getString(R.string.device_capture_suc));
				EventBus.getDefault().post(new AckCaptureEvent(AckCaptureEvent.SaveOver, true));
			}
		}).subscribeOn(Schedulers.io()).subscribe();
	}
	public class PwCaptureCallback implements ICaptureCallback{
		@Override
		public void onSuccess(Bitmap bmp) {
			// TODO Auto-generated method stub
			//LogUtils.e("capture", "onsuccess");
			ToastUtil.toastShort(MainActivity.this, getString(R.string.capture_over_saving));
			EventBus.getDefault().post(new AckCaptureEvent(AckCaptureEvent.SaveBegin, true));
			SaveCaptureAsync(bmp);
			//ToastUtil.toastShort(MainActivity.this, getString(R.string.device_capture_suc));
		}
		@Override
		public void onFailured() {
		///	LogUtils.e("capture", "onFailured");
			// TODO Auto-generated method stub
			ToastUtil.toastShort(MainActivity.this, R.string.device_capture_fai);
			EventBus.getDefault().post(new AckCaptureEvent(AckCaptureEvent.captureOver,false));
			LogUtils.e("capture", "onFailured over");
		}
		@Override
		public void onProcess() {
		//	LogUtils.e("capture", "onProcess");
			// TODO Auto-generated method stub
			ToastUtil.toastShort(MainActivity.this, R.string.capture_info);
			EventBus.getDefault().post(new AckCaptureEvent(AckCaptureEvent.captureOver,false));
		}
	}
	private PwCaptureCallback captureCallback=new PwCaptureCallback();
	public class MyVideoBtnClickListener implements
	VideoMenubarFragment.OnVideoBtnClickListener {

		@Override
		public void onStopPlayBtnClick() {
			AudioDecodeAndRender.RenderStop();
			mPlayHelper.stop();
		}
		@Override
		public boolean onQualityBtnClick() {
			// TODO Auto-generated method stub
			int iRes=-1;
			try {
				iRes=m_PlayHelper.setStreamType(EnumStreamType.QUALITY);
			} catch (NoDeviceIsPlayingException e) {
				// TODO: handle exception
				ToastUtil.toastShort(MainActivity.this, R.string.no_device_playing);
				return false;
			}
			if(iRes>=0){
				//				if(iRes==PlayHelper.STREAM_TYPE_JPEG){
				//					m_glSurfaceViewBitmap.setVisibility(View.VISIBLE);
				//					m_glSurfaceView.setVisibility(View.INVISIBLE);
				//					m_videoLayout.postInvalidate();
				//				}else {
				//					m_glSurfaceViewBitmap.setVisibility(View.INVISIBLE);
				//					m_glSurfaceView.setVisibility(View.VISIBLE);
				//					m_videoLayout.postInvalidate();
				//
				mSp=getSharedPreferences(SHAREDPERENCES_NAME, Context.MODE_PRIVATE);
				mSp.edit().putString(IDevicePlayer.SP_STREAM_KEY, IDevicePlayer.SP_STREAM_QUALITY).commit();
				ToastUtil.toastShort(MainActivity.this, R.string.quality_mode);
			}else{
				ToastUtil.toastShort(MainActivity.this, R.string.stream_switch_err);
			}
			return true;
		}
		@Override
		public boolean onFluencyBtnClick() {
			// TODO Auto-generated method stub
			int ires=-1;
			try {
				ires=m_PlayHelper.setStreamType(EnumStreamType.FLUENCY);
			} catch (NoDeviceIsPlayingException e) {
				// TODO: handle exception
				ToastUtil.toastShort(MainActivity.this, R.string.no_device_playing);
				return false;
			}	
			if(ires>=0){
				//				m_glSurfaceViewBitmap.setVisibility(View.INVISIBLE);
				//				m_glSurfaceView.setVisibility(View.VISIBLE);
				//				m_videoLayout.postInvalidate();
				mSp=getSharedPreferences(SHAREDPERENCES_NAME, Context.MODE_PRIVATE);
				mSp.edit().putString(IDevicePlayer.SP_STREAM_KEY, IDevicePlayer.SP_STREAM_FLUENCY).commit();
				ToastUtil.toastShort(MainActivity.this, R.string.fluency_mode);
			}

			return true;
		}
		@Override
		public boolean onAdapterBtnClick() {
			// TODO Auto-generated method stub
			try {
				m_PlayHelper.setStreamType(EnumStreamType.ADAPTER);
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
			mSp=getSharedPreferences(SHAREDPERENCES_NAME, Context.MODE_PRIVATE);
			mSp.edit().putString(IDevicePlayer.SP_STREAM_KEY, IDevicePlayer.SP_STREAM_ADAPTER).commit();
			ToastUtil.toastShort(MainActivity.this, R.string.adapter_mode);
			return true;
		}
		@Override
		public void onCaptureBtnClick() {
			// TODO Auto-generated method stub


		}

		@Override
		public void onRecordeBtnClick(boolean isRecord,final RecordCallback callback){
			// TODO Auto-generated method stub

		}

		@Override
		public void onExpandBtnClick(boolean isExpand) {
			// TODO Auto-generated method stub
			//hideOrShowView(m_container_multi_compound, 100);
			//slideInViewFromBottom(m_container_multi_compound);
			if(isExpand){
				//showView(m_container_multi_compound, ExpandAreaH);
				showExpandArea(true,m_container_multi_compound);
			}else{
				hideExpandArea(true,m_container_multi_compound);
				//hideView(m_container_multi_compound);
			}
		}

		@Override
		public void onTimelineBtnClick() {

			if (!(mCurFragment instanceof TimelineFragment)) {
				toExpandFragment(FmTagTimeline);
				EventBus.getDefault().post(new TimelineShowEvent());
			} else {
				toExpandFragment(FmTagExpand);
			}
			//showView(m_container_multi_compound, ExpandAreaH);
			showExpandArea(false,m_container_multi_compound);
		}
	}

	private void setVideoMenuBarVisible(boolean visible) {
		ValueAnimator vanim;
		ViewGroup.LayoutParams lp = m_layout_video_menu.getLayoutParams();
		if (visible) {
			if (lp.height > 0) {
				return;
			}
			vanim = ValueAnimator.ofFloat(0,
					HexTrans.dip2px(MainActivity.this, 51)).setDuration(500);
		} else {
			if (lp.height == 0) {
				return;
			}
			vanim = ValueAnimator.ofFloat(
					HexTrans.dip2px(MainActivity.this, 51), 0).setDuration(500);
		}
		vanim.start();
		vanim.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
			@Override
			public void onAnimationUpdate(ValueAnimator animation) {
				// TODO Auto-generated method stub
				float var = (Float) animation.getAnimatedValue();
				ViewGroup.LayoutParams lp = m_layout_video_menu
						.getLayoutParams();
				lp.height = (int) var;
				m_layout_video_menu.setLayoutParams(lp);
			}
		});

	}

	public class MyMenubarOperateListener implements
	MenubarFragment.OnMenuBarOperationListener {
		public void systemExit() {
			ShowmoApplication.getInstance().exit();
		}

		public void exitCurAccount() {
			mCheckUpgradeAppTimer.stopIfStarted();
			mRxTimer.stopIfStarted();
			m_videoMenuBarHideTimer.stopIfStarted();
			logoutCurAccount();
		}

	}

	private void logoutCurAccount() {
		NetworkHelper.getInstance().newNetTask(new RequestCallBack() {

			@Override
			public ResponseInfo doInBackground() {
				// TODO Auto-generated method stub
				//LogUtils.e("logout", "logout cur account");
				showmoSystem.unregisterXgPush();
				if(m_PlayHelper.getmCurDeviceInfo()!=null){
					m_PlayHelper.stop();
					m_PlayHelper.setOnStopPlayListener(null);
				}
				String username=m_showmoSys.getCurUser().getUserName();
				showmoSystem.userLogout();
				//long timebf=SystemClock.elapsedRealtime();
				//m_PlayHelper.clear();
				//LogUtils.e("clear", "clear use time:"+(SystemClock.elapsedRealtime()-timebf));
				ResponseInfo res=new ResponseInfo();
				res.setIsSuccess(true);
				res.setObj(username);
				return res;
			}
			@Override
			public void onSuccess(ResponseInfo info) {
				closeLoadingDialog();
				Intent intent = new Intent(MainActivity.this, LoginActivity.class);
				//intent.putExtra(INTENT_KEY_STRING,(String)info.getObj());
				//intent.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
				startActivity(intent);
				slideInFromRight();
				SharedPreferences mSp=getSharedPreferences(LoginActivity.SHAREDPERENCES_NAME, MODE_PRIVATE);
				mSp.edit().putBoolean(LoginActivity.SP_KEY_AUTO_LOGIN, false).commit();
				//m_PlayHelper.clear();
				//LogUtils.e("logout", "logout cur account");
				finish();
			}

			@Override
			public void onPrepare() {
				// TODO Auto-generated method stub
				super.onPrepare();
				showLoadingDialog(R.string.exiting);
			}
		});
	}
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		// TODO Auto-generated method stub
		super.onSaveInstanceState(outState);
	}

	@Override
	public void onDestroy() {

		unRegisterReceivers();
		IntercomApiWraper.uninit();
		AudioDecodeAndRender.RenderUninit();
		super.onDestroy();
	}

	private void onPortraitScreenLayoutChange() {
		m_videoMenuBarHideTimer.stopIfStarted();
		m_container_menubar.setVisibility(View.VISIBLE);
		m_layout_status_bar.setVisibility(View.VISIBLE);
		RelativeLayout.LayoutParams lp;// = (RelativeLayout.LayoutParams)
		//showView(m_container_multi_compound, ExpandAreaH);
		showExpandArea(true,m_container_multi_compound);
		//m_container_multi_compound.setScaleX(1f);
		if (m_curExpandComType != EnumExpandComType.DeviceState) {
			//showView(m_layout_video_menu, 51);
			showView(true,m_layout_video_menu, VideoMenuH,null);
		}
		lp = new RelativeLayout.LayoutParams(
				RelativeLayout.LayoutParams.MATCH_PARENT,
				RelativeLayout.LayoutParams.MATCH_PARENT);
		lp.addRule(RelativeLayout.ABOVE, R.id.video_menu_layout);
		lp.addRule(RelativeLayout.BELOW, R.id.status_bar);
		m_videoLayout.setLayoutParams(lp);
	}

	private void onLandScreenLayoutChange() {
		m_container_menubar.setVisibility(View.GONE);
		m_layout_status_bar.setVisibility(View.GONE);
		RelativeLayout.LayoutParams lp;
		if (m_curExpandComType != EnumExpandComType.DeviceState) {// 播放状�?�，隐藏多碎片区�?,启动隐藏定时�?
			//			lp = (RelativeLayout.LayoutParams) m_container_multi_compound
			//					.getLayoutParams();
			//			lp.height = 0;
			//			m_container_multi_compound.setLayoutParams(lp);
			hideExpandArea(true,m_container_multi_compound);
			m_videoMenuBarHideTimer.start(LandVideoMenuHideTime);
		}
		m_container_multi_compound.bringToFront();
		// lp = (RelativeLayout.LayoutParams) m_layout_video_menu
		// .getLayoutParams();
		// lp.height = HexTrans.dip2px(this, 51);
		// m_layout_video_menu.setLayoutParams(lp);
		m_layout_video_menu.bringToFront();

		lp = new RelativeLayout.LayoutParams(
				RelativeLayout.LayoutParams.MATCH_PARENT,
				RelativeLayout.LayoutParams.MATCH_PARENT);
		lp.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
		m_videoLayout.setLayoutParams(lp);
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
		Configuration confi = this.getResources().getConfiguration();

		if (confi.orientation == Configuration.ORIENTATION_PORTRAIT) {
			onPortraitScreenLayoutChange();
		} else if (confi.orientation == Configuration.ORIENTATION_LANDSCAPE) {
			LogUtils.v("fragment", "onConfigurationChanged ORIENTATION_LANDSCAPE");
			// if (m_ExpandComType != ExpandComType.DeviceState) {
			// m_ExpandComType = ExpandComType.NoneState;
			// }
			//requestWindowFeature(Window.FEATURE_NO_TITLE);
			onLandScreenLayoutChange();

		}
		m_videoMenubarFragment.dismissAllMenu();
		m_menubarFragment.dismissAllMenu();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		//User curUser = m_showmoSys.getCurUser();
		//NetworkHelper m_WorkHelpler = NetworkHelper.getInstance();
		isBackground=false;
		if(getBooleanFromSharedPreferences(SP_KEY_VIDEO_SOUND, true)){
			AudioDecodeAndRender.RenderPause(false);
		}
		if(decodeThread!=null)
			decodeThread.setPause(false);
		if (m_showmoSys.getCurUser() == null) {
			Toast.makeText(this, "请先登录系统", Toast.LENGTH_SHORT).show();
			Intent intent = new Intent(this, LoginActivity.class);
			//intent.setFlags(Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT);
			startActivity(intent);
			slideInFromRight();
			finish();
		} 

		//		showView(m_container_multi_compound, ExpandAreaH);
		//		hideView(m_layout_video_menu);
	}
	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		if(decodeThread!=null){
			decodeThread.setPause(true);
		}
		AudioDecodeAndRender.RenderPause(true);
		super.onStop();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);

		switch (requestCode) {
		case REQUEST_LOGIN:
			if (resultCode == Activity.RESULT_OK) {

			}
			break;
		case REQUEST_RESETPSW:
			if (resultCode == Activity.RESULT_OK) {
				//				Intent intent = new Intent(MainActivity.this, LoginActivity.class);
				////				intent.putExtra(INTENT_KEY_STRING, data.getStringExtra(INTENT_KEY_STRING));
				////				intent.setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
				//				startActivity(intent);
				//				m_showmoSys.userLogout();
				//				finish();
				//				slideInFromRight();
				logoutCurAccount();

			}


			break;
		default:
			break;
		}
	}

	private void registerReceivers() {
		m_EventReciever=new PwEventReciever();
		EventBus.getDefault().register(m_EventReciever);
		IntentFilter deviceChangeFilter = new IntentFilter();
		m_DeviceChangeReceiver = new DeviceChangeReceiver();
		deviceChangeFilter
		.addAction(MSGBroadcastActions.CLIENT_MGR_CAMERA_ONLINE_STATE_CHANGE_MSG);
		deviceChangeFilter.addAction(User.DEVICE_ADD_ACTION);
		deviceChangeFilter.addAction(User.DEVICE_REMOVE_ACTION);
		deviceChangeFilter.addAction(Device.DEVICE_RENAME_ACTION);
		deviceChangeFilter.addAction(JniDataDef.MSGBroadcastActions.UPDATE_SUCCESS_MOBILE_INVITE_ACK);
		deviceChangeFilter.addAction(JniDataDef.MSGBroadcastActions.UPDATE_FAILE_MOBILE_INVITE_ACK);
		registerReceiver(m_DeviceChangeReceiver, deviceChangeFilter);

		IntentFilter Screenfilter=new IntentFilter();
		Screenfilter.addAction(Intent.ACTION_SCREEN_ON);
		Screenfilter.addAction(Intent.ACTION_SCREEN_OFF);
		Screenfilter.addAction(INTENT_ACTION_BACKGROUND);
		m_ScreenStateChangeReceiver=new ScreenStateChangeReceiver();
		registerReceiver(m_ScreenStateChangeReceiver,Screenfilter);

	}

	private void unRegisterReceivers() {
		EventBus.getDefault().unregister(m_EventReciever);
		unregisterReceiver(m_DeviceChangeReceiver);
		unregisterReceiver(m_ScreenStateChangeReceiver);
	}

	private void notifyDeviceListChanged() {
		PWDevAdapter adapter = m_devicelistFragment.getAdapter();
		if (adapter != null) {
			adapter.notifyDataSetChanged();
		}
	}

	private class DeviceChangeReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
			//			User_2_mgr_disconn_device info=(User_2_mgr_disconn_device)intent.getSerializableExtra(MSGBroadcastActions.DataKey);
			//			LogUtils.e("onReceive", info.toString());
			if (intent.getAction() == MSGBroadcastActions.CLIENT_MGR_CAMERA_ONLINE_STATE_CHANGE_MSG) {
				notifyDeviceListChanged();
			} else if (intent.getAction() == User.DEVICE_ADD_ACTION) {
				notifyDeviceListChanged();
			} else if (intent.getAction() == User.DEVICE_REMOVE_ACTION) {
				LogUtils.e("remove", "mainactivity intent.getAction() == User.DEVICE_REMOVE_ACTION");
				notifyDeviceListChanged();
			} else if (intent.getAction() == Device.DEVICE_RENAME_ACTION) {
				notifyDeviceListChanged();
			} else if(intent.getAction() ==JniDataDef.MSGBroadcastActions.UPDATE_SUCCESS_MOBILE_INVITE_ACK){
				JniDataDef.SDK_CAMERA_UPDATE  data = (JniDataDef.SDK_CAMERA_UPDATE) intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
				for (Device dev:showmoSystem.getCurUser().getDevices()) {
					if(dev.getmCameraId()==data.cameraid){
						showToastShort( getString(R.string.upgrade_success,dev.getmDeviceName()));
						return;
					}
				}	
			}else if(intent.getAction() ==JniDataDef.MSGBroadcastActions.UPDATE_FAILE_MOBILE_INVITE_ACK){
				JniDataDef.SDK_CAMERA_UPDATE  data = (JniDataDef.SDK_CAMERA_UPDATE) intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
				for (Device dev:showmoSystem.getCurUser().getDevices()) {
					if(dev.getmCameraId()==data.cameraid){
						showToastShort( getString(R.string.upgrade_failure,dev.getmDeviceName()));
						return;
					}
				}
			}
		}
	}
	private class ScreenStateChangeReceiver extends BroadcastReceiver{
		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
			if(intent.getAction().equals(Intent.ACTION_SCREEN_OFF)){
				LogUtils.e("screen", "ACTION_SCREEN_OFF");
				stopWhenBackground();
			}else if(intent.getAction().equals(Intent.ACTION_SCREEN_ON)){
				LogUtils.e("screen", "ACTION_SCREEN_ON");

			}else if(intent.getAction().equals(INTENT_ACTION_BACKGROUND)){
				LogUtils.e("screen", "INTENT_ACTION_BACKGROUND");
				stopWhenBackground();
			}
		}
	}
	private boolean isBackground=false;
	private void stopWhenBackground(){
		isBackground=true;
		SharedPreferences pref=PreferenceManager.getDefaultSharedPreferences(this);
		boolean bbgstg=pref.getBoolean("screen_off_strategy", true);
		if(bbgstg){
			if(mPlayHelper.getmCurDeviceInfo()!=null){
				AudioDecodeAndRender.RenderStop();
				mPlayHelper.stop();
			}
		}
	}
	@Override
	protected void onClick(int viewId) {
		// TODO Auto-generated method stub
		super.onClick(viewId);
		//		if(viewId==R.id.load_device_list_err_btn){
		//			m_getDeviceErrTip.setVisibility(View.INVISIBLE);
		//			NetworkHelper m_WorkHelpler = NetworkHelper.getInstance();
		//			m_WorkHelpler.newNetTask(new PwUpdateDeviceAsynTaskCallback());
		//		}
	}
}
