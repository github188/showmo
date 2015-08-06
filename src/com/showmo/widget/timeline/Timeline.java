package com.showmo.widget.timeline;

import java.security.PublicKey;
import java.util.ArrayList;
import java.util.List;

import com.showmo.dataDef.PWDeviceInfo;
import com.showmo.dataDef.PWPlayBackVideoFrame;
import com.showmo.util.DisViewUtil;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;
import com.showmo.widget.timeline.ITimelineDial.SearchStateListener;
import com.showmo.R;

import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;
import android.R.integer;
import android.content.Context;
import android.os.Handler;
import android.text.format.Time;
import android.util.AttributeSet;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnPreDrawListener;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.view.View.OnTouchListener;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.RotateAnimation;

public class Timeline extends RelativeLayout {
	private ImageView m_cursor;
	private ImageView m_searchStateImg;
	private ProgressBar m_searchProgressBar;
	
	private ITimelineDial m_timelineDial;
	private Context m_context;
	private int m_cursorY;
	private float m_cursorLeftbeforeMove;
	private float m_MotionXPreviouMove;
	private  int MIN_MOVE_DISTANCE=8;
	private static final int CURSOR_ID = 12121;
	private GestureDetector m_cursorGestureDetector;
	private OnPlaybackListener m_playbackListener;
	private ViewTreeObserver mTreeObserver;
	private TlPreDrawListener m_tlPreDrawListener;
	public interface OnPlaybackListener {
		boolean onPlayback(SDK_REMOTE_FILE playbackVideoFrame,int pos);

		public List<SDK_REMOTE_FILE> onSearchPlayBackFileList(Time beginTime,
				Time endTime);
	}

	public void setOnPlaybackListener(OnPlaybackListener listener) {
		this.m_playbackListener = listener;
		
	}
	public void reset(){
		m_timelineDial.reset();
	}
	public void update(){
		m_timelineDial.update();
	}
	public Timeline(Context context) {
		super(context);
		m_context = context;
		initTimelineView();
	}
	public Timeline(Context context, AttributeSet attr) {
		super(context, attr);
		m_context = context;
		initTimelineView();
		m_cursorY = 80;

	}
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		// TODO Auto-generated method stub
		super.onSizeChanged(w, h, oldw, oldh);
		float curx=m_timelineDial.getXByTime(m_timelineDial.getCurTime());
		LogUtils.e("sizechange", "curx "+curx);
		changeCursorX((int)curx);
	}
	public Timeline(Context context, AttributeSet attr, int defaultStyle) {
		super(context, attr, defaultStyle);
		m_context = context;
		initTimelineView();
	}
	
	private float mCursorDownX=0;
	private float mCursorUpX=0;
	protected void initTimelineView() {
		//MIN_MOVE_DISTANCE=HexTrans.dip2px(this.getContext(), 15);
		m_cursor = new ImageView(m_context);
		m_cursor.setImageDrawable(getResources().getDrawable(R.drawable.cursor));
		// m_cursor.setBackground(getResources().getDrawable(R.drawable.cursor));
		m_timelineDial = new TimelineDial(m_context);
		m_timelineDial
				.setCursorYchanged(new TimelineDial.ItimeLineCursorYchangedListener() {
					@Override
					public void onYchanged(float curY) {
						// TODO Auto-generated method stub
						//LogUtils.e("timeline", "onYchanged "+curY);
						m_cursorY = (int) curY;
						postInvalidate();
					}
				});
		m_timelineDial
				.setPlaybackFileSearchListener(new TimelineDial.IPlaybackFileSearchListener() {

					@Override
					public List<SDK_REMOTE_FILE> onGetPlaybackFileList(
							Time beginTime, Time endTime) {
						// TODO Auto-generated method stub
						if (m_playbackListener != null) {
							return m_playbackListener.onSearchPlayBackFileList(
									beginTime, endTime);
						}
						return new ArrayList<SDK_REMOTE_FILE>();
					}
				});
		m_playbackListener = null;
		LayoutParams dialLayoutParam = new LayoutParams(
				LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		this.addView((View)m_timelineDial, dialLayoutParam);

		float cursorW = this.getResources()
				.getDimension(R.dimen.dimen_cursor_w);
		float cursorH = this.getResources()
				.getDimension(R.dimen.dimen_cursor_h);
		LayoutParams cursorParam = new LayoutParams((int) cursorW,
				(int) cursorH);
		LogUtils.v("cursor", "m_cursor addToView ");
		this.addView(m_cursor, cursorParam);

		m_cursor.bringToFront();
		m_cursor.setId(CURSOR_ID);
		
		m_cursor.setOnTouchListener(new OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent ev) {
				
				if (v.getId() == CURSOR_ID) {
					int action = ev.getAction();
					switch (action) {
					case MotionEvent.ACTION_MOVE:
						if (Math.abs(ev.getRawX() - m_MotionXPreviouMove) > MIN_MOVE_DISTANCE) {//MIN_MOVE_DISTANCE
							m_MotionXPreviouMove=ev.getRawX();
							int motionAbsolutePointX = (int) ev.getRawX();
							changeCursorX(motionAbsolutePointX);
							
							float curx=v.getX()+v.getWidth()/2;
							m_timelineDial.updateCurTimeByCurx(curx);
						}
						break;
					case MotionEvent.ACTION_DOWN:
						m_MotionXPreviouMove=ev.getRawX();
						mCursorDownX=ev.getRawX();
						break;
					case MotionEvent.ACTION_UP:
						mCursorUpX = ev.getRawX();
						if(Math.abs(mCursorDownX-mCursorUpX) <= 30){
							Log.v("playback", "onclick "+v.getX()+" "+v.getY()+" "+v.getWidth()+" "+v.getHeight());
							float curx=v.getX()+v.getWidth()/2;
							SDK_REMOTE_FILE file=m_timelineDial.getFileByCursorX(curx);
							if(file==null){
								return true;
							}
							int  pos=m_timelineDial.getPosByCurx(file, curx);
							if(file!=null){
								if(m_playbackListener!=null){
									m_playbackListener.onPlayback(file,pos);
								}
							}
						}
						break;
					default:
						break;
					}
				}
				return true;
			}
		});
		m_searchStateImg=new ImageView(getContext());
		m_searchStateImg.setVisibility(View.GONE);
		m_searchStateImg.setBackground(getResources().getDrawable(R.drawable.btn_main_menu_selector));
		m_searchStateImg.setBackgroundResource(R.drawable.research);
				
		m_searchProgressBar=new ProgressBar(getContext());
		m_searchProgressBar.setIndeterminate(true);
		m_searchProgressBar.setIndeterminateDrawable(getResources().getDrawable(R.drawable.one_frame_circle_indicator));
		m_searchProgressBar.setVisibility(View.GONE);
		//m_searchProgressBar.setAnimation(AnimationUtils.loadAnimation(getContext(), R.anim.anim_rotate));
		
		LayoutParams searchLp=new LayoutParams(HexTrans.dip2px(getContext(), 20), 
												HexTrans.dip2px(getContext(), 20));
		searchLp.topMargin=HexTrans.dip2px(getContext(), 10);
		searchLp.rightMargin=HexTrans.dip2px(getContext(), 10);
		searchLp.addRule(RelativeLayout.ALIGN_PARENT_TOP);
		searchLp.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
		addView(m_searchStateImg, searchLp);
		addView(m_searchProgressBar,searchLp);
		m_searchStateImg.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				//m_timelineDial
				LogUtils.e("playback", "research");
				m_timelineDial.startSearchFile(200);
			}
		});
		m_timelineDial.setSearchStateListener(new SearchStateListener() {
			
			@Override
			public void searchSuccess() {
				// TODO Auto-generated method stub
				LogUtils.e("playback", "m_searchStateImg setVisibility GONE");
				//m_searchStateImg.clearAnimation();
				m_searchStateImg.setVisibility(View.GONE);
				m_searchProgressBar.setVisibility(View.GONE);
				//m_searchStateImg.invalidate();
				LogUtils.e("playback", "searchSuccess");
			}
			@Override
			public void searchFailured() {
				// TODO Auto-generated method stub
				LogUtils.e("playback", "searchFailured  m_searchStateImg.setVisibility(View.VISIBLE)");
				m_searchStateImg.setVisibility(View.VISIBLE);
				m_searchProgressBar.setVisibility(View.GONE);
				//m_searchStateImg.invalidate();
				//m_searchStateImg.clearAnimation();
				//m_searchStateImg.setBackgroundResource(R.drawable.research);
				//m_searchStateImg.setClickable(true);
			}
			@Override
			public void searchBegin() {
				// TODO Auto-generated method stub
				LogUtils.e("playback", "searchBegin m_searchStateImg.setVisibility(View.VISIBLE)");
				m_searchStateImg.setVisibility(View.GONE);
				m_searchProgressBar.setVisibility(View.VISIBLE);
				//m_searchStateImg.invalidate();
//				m_searchStateImg.setBackgroundResource(R.drawable.one_frame_circle_indicator);
//				Animation animation=AnimationUtils.loadAnimation(getContext(), R.anim.anim_rotate);
//				RotateAnimation rotateAnimation=new RotateAnimation(
//						0, 360, 
//						Animation.RELATIVE_TO_SELF, 0.5f, 
//						Animation.RELATIVE_TO_SELF, 0.5f);
//				rotateAnimation.setDuration(1000);
//				rotateAnimation.setInterpolator(new DecelerateInterpolator());
//				m_searchStateImg.startAnimation(rotateAnimation);
//				m_searchStateImg.setClickable(false);
			}
		});
		
		
		mTreeObserver =  this.getViewTreeObserver();
		//View.MeasureSpec.makeMeasureSpec(0, mode)
		m_tlPreDrawListener=new TlPreDrawListener();
		mTreeObserver.addOnPreDrawListener(m_tlPreDrawListener);
	}
	protected class TlPreDrawListener implements OnPreDrawListener{
		private boolean binit=false;
		@Override
		public boolean onPreDraw() {
			// TODO Auto-generated method stub
			//LogUtils.e("predraw", "onPreDraw");
			
			if(binit){
				return true;
			}
			if(getMeasuredWidth() <= 0){
				binit=false;
				return true;
			}
			binit=true;
			m_timelineDial.startSearchFile(200);
			//LogUtils.e("timeline1", "getWidth() "+getWidth()+" m "+getMeasuredWidth());
			int curLeft = getMeasuredWidth() / 2 - m_cursor.getMeasuredWidth() / 2;
			int curTop = (int) m_cursorY - m_cursor.getMeasuredHeight() - 10;
			int curRight = curLeft;
			int curBottom = m_cursor.getMeasuredHeight() - (int) m_cursorY + 10;
			
			//LogUtils.e("timeline1", "curLeft "+curLeft+" curTop "+curTop+" curBottom "+curBottom);
			//LogUtils.v("cursorm", "measuread parent wid:"+getWidth()+"hei:"+getHeight());
			//LogUtils.v("cursorm", "measuread  wid: "+m_cursor.getWidth()+"hei "+m_cursor.getHeight());
			RelativeLayout.LayoutParams lParams= (RelativeLayout.LayoutParams)m_cursor.getLayoutParams();
			lParams.setMargins(curLeft, curTop, curRight, curBottom);
			m_cursor.setLayoutParams(lParams);
			
			float curx=lParams.leftMargin+m_cursor.getMeasuredWidth()/2;
			m_timelineDial.updateCurTimeByCurx(curx);
			//LogUtils.v("cursorm", "curx "+curx);
			//LogUtils.e("timeline", "updateCurTimeByCurx curx "+curx);
			//m_timelineDial.updateCurTimeByCurx(curx);
			//vTreeObserver.removeOnPreDrawListener(this);
			return true;
		}
	}
	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		super.onLayout(changed, l, t, r, b);
		//LogUtils.e("timeline", "timelinetimeline onLayout "+getMeasuredWidth());
		//LogUtils.v("cursor", "onLayout"+m_cursor.getLeft()+" "+m_cursor.getRight());
	};

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
	}
	
	private void changeCursorX(int absX){
		int cursorW = m_cursor.getWidth();
		int left = absX - cursorW / 2;
		int right = absX + cursorW / 2;
		right =getWidth()-right;
		if (left < 0) {
			left = 0;
			right = getWidth()-cursorW;
		}
		if (right < 0) {
			right = 0;
			left = getWidth() - cursorW;
		}
		RelativeLayout.LayoutParams lParams= (RelativeLayout.LayoutParams)m_cursor.getLayoutParams();
		lParams.setMargins(left, 0, right, 0);
		m_cursor.setLayoutParams(lParams);	
	}

}
