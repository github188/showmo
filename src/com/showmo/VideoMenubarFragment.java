package com.showmo;

import javax.mail.internet.NewsAddress;

import com.showmo.base.ShowmoSystem;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.event.AckRecordEvent;
import com.showmo.event.CaptureEvent;
import com.showmo.event.ExpandEvent;
import com.showmo.event.InitLightEvent;
import com.showmo.event.MainPageMultiAreaDisplayEvent;
import com.showmo.event.PlaybackEvent;
import com.showmo.event.RecordEvent;
import com.showmo.event.SoundSwitchEvent;
import com.showmo.eventBus.EventBus;
import com.showmo.playHelper.IDevicePlayer;
import com.showmo.playHelper.IDevicePlayer.EnumStreamType;
import com.showmo.playHelper.PlayHelper;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;
import com.showmo.util.ZoomPic;

import android.R.bool;
import android.R.integer;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.v4.widget.SimpleCursorAdapter.ViewBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.MeasureSpec;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.PopupWindow;
import android.widget.SeekBar;
import android.widget.PopupWindow.OnDismissListener;
import android.widget.SeekBar.OnSeekBarChangeListener;

public class VideoMenubarFragment extends BaseFragment {
	private ImageButton m_stopBtn;
	private ImageButton m_timelineBtn;
	private ImageButton m_streamBtn;
	//private ImageButton m_recordBtn;
	//private ImageButton m_captureBtn;
	private ImageButton mCtrlLightBtn;
	private ImageButton m_expandBtn;
	private Button m_qualityBtn;
	private Button m_fluencyBtn;
	private Button m_adapterBtn;
	private ImageButton mSoundBtn;
	private SeekBar mCtrlLightSeekBar;
	private VideoCtrlMenuClickListener m_ctrlMenuListener;
	private StreamTypeItemClickListener m_streamItemListener;
	private PopupWindow m_streamMenu;
	private PopupWindow mCtrlLightWindow;
	private View m_streamView;
	private OnVideoBtnClickListener m_btnListener;
	private IDevicePlayer m_PlayHelper;
	private boolean bRuning=false;
	private boolean bExpandBtn_ON=true;
	private boolean bRecord_ON=false;

	public static boolean isCapture = false;

	public VideoMenubarFragment(){
		super();
		m_streamMenu=null;
		m_btnListener=null;
		m_PlayHelper=ShowmoSystem.getInstance().getPlayer();
	}
	public interface OnVideoBtnClickListener{

		void onTimelineBtnClick();
		void onStopPlayBtnClick();
		boolean onQualityBtnClick();
		boolean onFluencyBtnClick();
		boolean onAdapterBtnClick();
		void onCaptureBtnClick();
		interface RecordCallback{
			void onRecordListener(boolean bres,boolean isRecord);//结果,停止OR开启
		}
		void onRecordeBtnClick(boolean isRecord,final RecordCallback callback);//开启录像，or停止
		void onExpandBtnClick(boolean isExpand);
	}
	public void setOnVideoBtnClickListener(OnVideoBtnClickListener listener){
		m_btnListener=listener;
	}
	@Override
	public void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		EventBus.getDefault().register(this);
	}
	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		EventBus.getDefault().unregister(this);
		super.onStop();
	}
	public void onEventMainThread(PlaybackEvent ev){
		if(ev.isOpen){
			m_streamBtn.setVisibility(View.GONE);
		}else{
			m_streamBtn.setVisibility(View.VISIBLE);
		}

	}
	public void onEventMainThread(InitLightEvent ev){
		//LogUtils.e("light", "InitLightEvent "+ev.getValue());
		mProgress=-1;
		mCtrlLightSeekBar.setProgress(ev.getValue());
	}
	public void onEventMainThread(MainPageMultiAreaDisplayEvent ev){
		if(ev.bShow){
			setExpandBtn(true);
		}else{
			setExpandBtn(false);
		}
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		LogUtils.v("fragment", "onCreateView");
		return inflater.inflate(R.layout.fragment_mainpage_video_menubar, container, false);
	}
	@Override
	public void onActivityCreated(Bundle savedInstaceState) {
		super.onActivityCreated(savedInstaceState);
		m_stopBtn=(ImageButton)m_activity.findViewById(R.id.btn_video_stop);
		m_timelineBtn=(ImageButton)m_activity.findViewById(R.id.btn_video_timeline);
		m_streamBtn=(ImageButton)m_activity.findViewById(R.id.btn_video_stream);
		mCtrlLightBtn=(ImageButton)m_activity.findViewById(R.id.btn_ctrl_light);
		//m_recordBtn=(ImageButton)m_activity.findViewById(R.id.btn_video_record);
		//m_captureBtn=(ImageButton)m_activity.findViewById(R.id.btn_video_capture);
		m_ctrlMenuListener=new VideoCtrlMenuClickListener();
		m_stopBtn.setOnClickListener((View.OnClickListener)m_ctrlMenuListener);
		m_timelineBtn.setOnClickListener((View.OnClickListener)m_ctrlMenuListener);
		m_streamBtn.setOnClickListener((View.OnClickListener)m_ctrlMenuListener);
		mCtrlLightBtn.setOnClickListener((View.OnClickListener)m_ctrlMenuListener);
		//	m_recordBtn.setOnClickListener((View.OnClickListener)m_ctrlMenuListener);
		//	m_captureBtn.setOnClickListener((View.OnClickListener)m_ctrlMenuListener);

		m_streamItemListener=new StreamTypeItemClickListener();
		m_expandBtn=(ImageButton)m_activity.findViewById(R.id.btn_video_expand_b_layout);
		m_expandBtn.setOnClickListener((View.OnClickListener)m_ctrlMenuListener);
		mSoundBtn=(ImageButton)m_activity.findViewById(R.id.btn_video_sound);
		mSoundBtn.setOnClickListener((View.OnClickListener)m_ctrlMenuListener);
		boolean bSound=m_activity.getBooleanFromSharedPreferences(m_activity.SP_KEY_VIDEO_SOUND, true);
		if(bSound){
			mSoundBtn.setImageResource(R.drawable.sound_on);
		}else{
			mSoundBtn.setImageResource(R.drawable.sound_off);
		}
		
		initPopCtrlLightWindow();
		initPopStreamMenu();

	}
	private int mProgress=-1;
	private void initPopCtrlLightWindow(){
		LayoutInflater inflater=m_activity.getLayoutInflater();
		View ctrlContent=inflater.inflate(R.layout.menu_layout_ctrllight, null);
		mCtrlLightSeekBar = (SeekBar)ctrlContent.findViewById(R.id.ctrl_light_seekBar);
		mCtrlLightSeekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			@Override
			public synchronized void onStopTrackingTouch(SeekBar seekBar) {
				LogUtils.e("light", "onStopTrackingTouch "+seekBar.getProgress());
				if(!ctrlLight(seekBar.getProgress())){
					if(!ctrlLight(seekBar.getProgress())){
						ctrlLight(seekBar.getProgress());
					}
				}
				mProgress = seekBar.getProgress();
			}
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {}
			@Override
			public synchronized void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				// TODO Auto-generated method stub
				//currentProgress.setText("当前亮度："+progress);
				LogUtils.e("light", "onProgressChanged "+progress+" pre "+mProgress);
				if(mProgress == -1){
					mProgress = progress;
				}else{
					if(Math.abs(mProgress - progress) >5){
						ctrlLight(progress);
						mProgress = progress;
					}else {
						return;
					}
				}
			}
		});
		mCtrlLightWindow=new PopupWindow(ctrlContent,HexTrans.dip2px(m_activity, 340),
				HexTrans.dip2px(m_activity, 60),true);
		mCtrlLightWindow.setFocusable(true);
		mCtrlLightWindow.setOutsideTouchable(true);
		Drawable drawable=m_activity.getResources().getDrawable(R.drawable.white_drawable);
		mCtrlLightWindow.setBackgroundDrawable(drawable);
	}
	private boolean ctrlLight(int progress){
		IDevicePlayer playhelper= ShowmoSystem.getInstance().getPlayer();
		Device dev= playhelper.getmCurDeviceInfo();
		boolean bres=true;
		if(dev!=null){
			DeviceUseUtils utils=new DeviceUseUtils(dev);
			LogUtils.e("light", "brightCtrl "+progress);
			bres=utils.brightCtrl(progress);
		}
		return bres;
	}
	private void initPopStreamMenu(){
		LayoutInflater inflater=m_activity.getLayoutInflater();
		m_streamView=inflater.inflate(R.layout.menu_mainpage_fluency, null);
		m_qualityBtn=(Button)m_streamView.findViewById(R.id.menu_item_quality_priority);
		m_fluencyBtn=(Button)m_streamView.findViewById(R.id.menu_item_fluency_priority);
		m_adapterBtn=(Button)m_streamView.findViewById(R.id.menu_item_adapter_priority);
		m_qualityBtn.setOnClickListener((View.OnClickListener)m_streamItemListener);
		m_fluencyBtn.setOnClickListener((View.OnClickListener)m_streamItemListener);
		m_adapterBtn.setOnClickListener((View.OnClickListener)m_streamItemListener);

		m_streamMenu=new PopupWindow(m_streamView,LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT,true);
		Drawable drawable=m_activity.getResources().getDrawable(R.drawable.fluency_menu);
		drawable=ZoomPic.zoomDrawable(drawable, 
				HexTrans.dip2px(m_activity, 100),
				HexTrans.dip2px(m_activity, 70));
		m_streamMenu.setFocusable(true);
		m_streamMenu.setOutsideTouchable(true);
		m_streamMenu.setBackgroundDrawable(drawable);
		m_streamMenu.setOnDismissListener(new OnDismissListener() {
			@Override
			public void onDismiss() {
				// TODO Auto-generated method stub
				setbRuning(false);
			}
		});
	}
	private void showStreamMenu(View v){
		int[] pos=new int[2];
		v.getLocationOnScreen(pos);
		LogUtils.v("menu", "x:"+ pos[0]+"y:"+ pos[1]);
		m_streamView.measure(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		int sreamView_wid=m_streamView.getMeasuredWidth();
		int sreamView_hei=m_streamView.getMeasuredHeight();
		SharedPreferences sPreferences=m_activity.getSharedPreferences(m_activity.SHAREDPERENCES_NAME, Context.MODE_PRIVATE);
		String stream=sPreferences.getString(IDevicePlayer.SP_STREAM_KEY, "");
		Button btn=null;
		Button btnList[]={m_adapterBtn,m_fluencyBtn,m_qualityBtn};
		if(stream.equals(IDevicePlayer.SP_STREAM_ADAPTER)){
			btn = m_adapterBtn;
		}else if(stream.equals(IDevicePlayer.SP_STREAM_FLUENCY)){
			btn = m_fluencyBtn;
		}else if (stream.equals(IDevicePlayer.SP_STREAM_QUALITY)) {
			btn=m_qualityBtn;
		}else {
			btn = null;
		}
		LogUtils.e("stream","cur stream type "+stream);
		if(btn != null){
			for (int i = 0; i < btnList.length; i++) {
				if(btn == btnList[i]){
					btnList[i].setBackgroundResource(R.drawable.btn_menu_fluency_cur);
					btnList[i].setClickable(false);
				}else{
					btnList[i].setBackgroundResource(R.drawable.btn_menu_item_selector);
					btnList[i].setClickable(true);
				}
			}
		}

		LogUtils.v("menu", "sreamView_wid:"+ (sreamView_wid/2-v.getWidth()/2)+"sreamView_hei:"+ sreamView_hei);
		m_streamMenu.showAtLocation(v, Gravity.NO_GRAVITY, pos[0]-HexTrans.dip2px(m_activity, 10), pos[1]-sreamView_hei-HexTrans.dip2px(m_activity, 10));
		setbRuning(true);
	}
	private void showCtrlLight(View v){
		int[] pos=new int[2];
		v.getLocationOnScreen(pos);
		//mCtrlLightWindow.showAsDropDown(v, 0, 0, Gravity.RIGHT|Gravity.TOP);
		mCtrlLightWindow.showAtLocation(v, Gravity.RIGHT|Gravity.TOP, -HexTrans.dip2px(m_activity, 360), 
				pos[1]-HexTrans.dip2px(m_activity, 60));
	}
	public void setExpandBtn(boolean bexpand){
		bExpandBtn_ON=bexpand;
		if(bexpand){
			Drawable dr=getResources().getDrawable(R.drawable.menu_arrow_down);
			m_expandBtn.setImageDrawable(dr);
		}else{
			Drawable dr=getResources().getDrawable(R.drawable.menu_arrow_up);
			m_expandBtn.setImageDrawable(dr);
		}
	}

	public class VideoCtrlMenuClickListener implements View.OnClickListener{
		@Override
		public void onClick(View v){
			switch (v.getId()) {
			case R.id.btn_video_stream:
				showStreamMenu(v);
				break;
			case R.id.btn_ctrl_light:
				if (m_activity.sorryForExperience()) {
					return;
				}
				LogUtils.i("ctrl", "btn_ctrl_light  ");
				showCtrlLight(v);
				break;
			case R.id.btn_video_timeline:
				if (m_activity.sorryForExperience()) {
					return;
				}
				if(m_btnListener!=null){
					m_btnListener.onTimelineBtnClick();
				}
				break;
			case R.id.btn_video_stop:

				if(m_btnListener!=null){
					if(bRecord_ON){
						EventBus.getDefault().post(new RecordEvent(false));
					}
					m_btnListener.onStopPlayBtnClick();
				}
				break;
			case R.id.btn_video_expand_b_layout:
//				if(m_btnListener!=null){
//					m_btnListener.onExpandBtnClick(!bExpandBtn_ON);
//				}
				eventBusPost(new ExpandEvent(!bExpandBtn_ON));
				break;
			case R.id.btn_video_sound:
				boolean bSound=m_activity.getBooleanFromSharedPreferences(m_activity.SP_KEY_VIDEO_SOUND,true);
				eventBusPost(new SoundSwitchEvent(!bSound));
				m_activity.saveBooleanInSharedPreferences(m_activity.SP_KEY_VIDEO_SOUND, !bSound);
				if(!bSound){
					mSoundBtn.setImageResource(R.drawable.sound_on);
				}else{
					mSoundBtn.setImageResource(R.drawable.sound_off);
				}
				
				break;
			default:
				break;
			}

		}
	}
	public class StreamTypeItemClickListener implements View.OnClickListener{
		@Override
		public void onClick(View v){
			if (m_activity.sorryForExperience()) {
				return;
			}
			switch (v.getId()) {
			case R.id.menu_item_quality_priority:
				if(m_btnListener!=null){
					m_btnListener.onQualityBtnClick();
				}
				break;
			case R.id.menu_item_fluency_priority:
				if(m_btnListener!=null){
					m_btnListener.onFluencyBtnClick();
				}
				break;
			case R.id.menu_item_adapter_priority:
				if(m_btnListener!=null){
					m_btnListener.onAdapterBtnClick();
				}
				break;
			default :
				break;
			}
			m_streamMenu.dismiss();
		}
	}
	public boolean isbRuning() {
		return bRuning;
	}
	public void setbRuning(boolean bRuning) {
		this.bRuning = bRuning;
	}
	public void dismissAllMenu(){
		if(m_streamMenu!=null){
			if(m_streamMenu.isShowing())
				m_streamMenu.dismiss();
		}
	}


}


