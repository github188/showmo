package com.showmo;

import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.base.ShowmoSystem;
import com.showmo.commonPagerAdapter.CommonPagerAdapter;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.event.AckCaptureEvent;
import com.showmo.event.AckRecordEvent;
import com.showmo.event.CaptureEvent;
import com.showmo.event.RecordEvent;
import com.showmo.event.TimelineShowEvent;
import com.showmo.eventBus.Event;
import com.showmo.eventBus.EventBus;
import com.showmo.playHelper.IDevicePlayer;
import com.showmo.playHelper.PlayHelper;
import com.showmo.util.LogUtils;
import com.showmo.widget.PwCtrlLightView2.onColorChangeListener;

import android.R.integer;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.GestureDetector;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.AutoCompleteTextView.Validator;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.ToggleButton;

public class ExpandFragment extends BaseFragment implements onColorChangeListener{
	private ViewPager m_viewPager;
	private List<View> mListView;
	//private List<View> mCursorList;
	private CommonPagerAdapter m_pagerAdapter;
	private GestureDetector mDetector;
	//private PwCtrlLightView2 mCtrlLight;
	private ImageButton mInterComBtn;
	private ImageButton mCaptureBtn;
	private ImageButton mRecordBtn;
	
	private TextView mRecordTipTextView;
	private TextView mCaptureTipTextView;
	private TextView mIntercomTipTextView;
	
	private boolean bRecordOpen=false;
	//private SeekBar seekBar;
	
	public static class IntercomEvent extends Event{
		public boolean mIntercom;
		public IntercomEvent(boolean bIntercom){
			this.mIntercom=bIntercom;	
		}
	}
//	@Override
	public void onStart() {
		// TODO Auto-generated method stub
		LogUtils.e("expand", "onStart");
		super.onStart();
		EventBus.getDefault().register(this);
	}
//	@Override
//	public void onPause() {
//		LogUtils.e("expand", "onPause");
//		// TODO Auto-generated method stub
//		super.onPause();
//	}
//	@Override
//	public void onResume() {
//		// TODO Auto-generated method stub
//		LogUtils.e("expand", "onResume");
//		super.onResume();
//	}
	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		EventBus.getDefault().unregister(this);
	}
	@Override
	public void onActivityCreated(Bundle savedInstaceState) {
		// TODO Auto-generated method stub
		findViewAndSet();
		super.onActivityCreated(savedInstaceState);
		
	}
	
	private void changeCursor(int curpos){
	}
	public void onEventMainThread(AckRecordEvent ev){
		if(ev.getType() == AckRecordEvent.opened){
			if(ev.getResult()){
				bRecordOpen=true;
				mRecordBtn.setImageResource(R.drawable.recording);
				mRecordTipTextView.setText(R.string.recording);
			}
		}
		if(ev.getType() == AckRecordEvent.closed){
			if(ev.getResult()){
				bRecordOpen=false;
				mRecordBtn.setImageResource(R.drawable.record);
				mRecordTipTextView.setText(R.string.record);
			}
		}
	}
	public void onEventMainThread(AckCaptureEvent ev){
		LogUtils.e("capture", "AckCaptureEvent ");
		switch (ev.getStep()) {
		case AckCaptureEvent.captureOver://½ØÍ¼Ê§°Ü
			if(!ev.getResult()){
				mCaptureBtn.setEnabled(true);
				mCaptureTipTextView.setText(R.string.capture);
			}
			break;
		case AckCaptureEvent.SaveBegin:
			//LogUtils.e("capture", "AckCaptureEvent capture_saving");
			mCaptureTipTextView.setText(R.string.capture_saving);
			break;
		case AckCaptureEvent.SaveOver://½ØÍ¼±£´æÍê±Ï
			//LogUtils.e("capture", "AckCaptureEvent SaveOver");
			mCaptureBtn.setEnabled(true);
			mCaptureTipTextView.setText(R.string.capture);
			break;
		default:
			break;
		}	
	}
	private void findViewAndSet(){
		m_viewPager=(ViewPager)m_activity.findViewById(R.id.expand_view_pager);
		mListView=new ArrayList<View>();
		LayoutInflater inflater=LayoutInflater.from(m_activity);
		View m_ctrlLightPage=inflater.inflate(R.layout.fragment_expand_page_ctrl_light, null);
		View m_intercomPage=inflater.inflate(R.layout.fragment_expand_page_intercom, null);
		
		final TextView currentProgress=(TextView) m_ctrlLightPage.findViewById(R.id.currentprogress);
		//seekBar=(SeekBar) m_ctrlLightPage.findViewById(R.id.seekBar);
		
		
		mCaptureBtn=(ImageButton)m_intercomPage.findViewById(R.id.btn_video_capture);
		mCaptureBtn.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				EventBus.getDefault().post(new CaptureEvent());
				mCaptureBtn.setEnabled(false);
				mCaptureTipTextView.setText(R.string.captureing);
			}
		});
		mRecordTipTextView = (TextView)m_intercomPage.findViewById(R.id.record_state_tip);
		mIntercomTipTextView = (TextView)m_intercomPage.findViewById(R.id.intercom_state_tip);
		mCaptureTipTextView = (TextView)m_intercomPage.findViewById(R.id.capture_state_tip);
		
		mRecordBtn=(ImageButton)m_intercomPage.findViewById(R.id.btn_video_record);
		
		mRecordBtn.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				EventBus.getDefault().post(new RecordEvent(!bRecordOpen));
			}
		});
		//mCtrlLight=(PwCtrlLightView2)m_ctrlLightPage.findViewById(R.id.ctrl_light);
		mInterComBtn=(ImageButton)m_intercomPage.findViewById(R.id.btn_intercom);
		//mCtrlLight.setOnColorChangeListener(this);
		mInterComBtn.setOnTouchListener(new View.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				if (m_activity.sorryForExperience()) {
					return false;
				}
				switch (event.getAction()) {
				case MotionEvent.ACTION_DOWN:
					mCaptureBtn.setEnabled(false);
					mRecordBtn.setEnabled(false);
					LogUtils.e("inter", "MotionEvent.ACTION_DOWN");
					EventBus.getDefault().post(new IntercomEvent(true));
					mIntercomTipTextView.setVisibility(View.GONE);
					break;
				case MotionEvent.ACTION_UP:
					mCaptureBtn.setEnabled(true);
					mRecordBtn.setEnabled(true);
					LogUtils.e("inter", "MotionEvent.ACTION_UP");
					EventBus.getDefault().post(new IntercomEvent(false));
					mIntercomTipTextView.setVisibility(View.VISIBLE);
					break;
				case MotionEvent.ACTION_CANCEL:
					mCaptureBtn.setEnabled(true);
					mRecordBtn.setEnabled(true);
					LogUtils.e("inter", "MotionEvent.ACTION_CANCEL");
					EventBus.getDefault().post(new IntercomEvent(false));
					mIntercomTipTextView.setVisibility(View.VISIBLE);
					break;
				default:
					break;
				}
				return false;
			}
		});
		//mListView.add(m_ctrlLightPage);
		mListView.add(m_intercomPage);
		m_pagerAdapter=new CommonPagerAdapter(mListView);
		m_viewPager.setAdapter(m_pagerAdapter);
		m_viewPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
			
			@Override
			public void onPageSelected(int arg0) {
				// TODO Auto-generated method stub
				changeCursor(arg0);
			}
			@Override
			public void onPageScrolled(int arg0, float arg1, int arg2) {
				// TODO Auto-generated method stub	
			}
			@Override
			public void onPageScrollStateChanged(int arg0) {
				// TODO Auto-generated method stub
			}
		});
//		mCursorList=new ArrayList<View>();
//		mCursorList.add(m_activity.findViewById(R.id.ctrl_light_cursor));
//		mCursorList.add(m_activity.findViewById(R.id.intercom_cursor));
//		m_viewPager.setCurrentItem(1);
//		changeCursor(1);
	}
	
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		//inflater.inflate(R.layout.fragment_device_list, container, false);
		return inflater.inflate(R.layout.fragment_compound_expand, container, false);
	}
	
	@Override
	public void onColorChange(int luminance) {
		// TODO Auto-generated method stub
		Log.v("onColorChange", "onColorChange  :: value "+luminance);
		IDevicePlayer playhelper= ShowmoSystem.getInstance().getPlayer();
		Device dev= playhelper.getmCurDeviceInfo();
		if(dev!=null){
			DeviceUseUtils utils=new DeviceUseUtils(dev);
			utils.brightCtrl(luminance);
		}
	}
}
