package com.showmo.widget.timeline;

import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;

import java.lang.reflect.Array;
import java.net.PasswordAuthentication;
import java.sql.SQLNonTransientConnectionException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

import com.showmo.dataDef.PWPlayBackVideoFrame;
import com.showmo.util.LogUtils;
import com.showmo.widget.timeline.PWGestureDetector.PWPinchEvent;
import com.showmo.R;

import android.R.integer;
import android.R.interpolator;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.Path;
import android.graphics.Point;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.Layout.Alignment;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.text.format.Time;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.GestureDetector;
import android.view.View;
import android.view.MotionEvent;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnPreDrawListener;

public class TimelineDial extends View implements
		PWGestureDetector.PwOnGestureListener,ITimelineDial{

	private Paint m_axisPaint_hor;
	private Paint m_axisPaint_ver;
	private Paint m_spanTimeBlockPaint;
	private Paint m_curTimeTextPaint;
	private Paint m_spanTextPaint;
	private Paint m_videoFramePaint;
	private Paint m_timelineBgPaint;
	private TextPaint m_textPaint;
	private Path m_leftSpanTimeBlockPath;
	private Path m_rightSpanTimeBlockPath;
	private Time m_initRefTime;
	private float m_initRefX;// 参照X单位是px
	private Time m_leftTime;
	private float m_leftX;
	private Time m_rightTime;
	private float m_rightX;
	private Time m_CurTime;
	private float m_CurX;
	
	private float m_totalSpanMsec;// 最大1天 最小2分钟
	private final static long MAX_TOTALSPAN = 24 * 3600 * 1000;
	private final static long MIN_TOTALSPAN = 2 * 60 * 1000;
	private final static int SEARCHTIMEDELAY=1000;
	private float m_pixPerMsec;
	private float m_dpToPxscale;
	private Context m_context;

	private float m_cursor_x;
	private float m_cursor_y;
	private float m_axis_height;

	private boolean m_bInitDraw;

	private final static long INIT_PAINT_TYPE = 3600000 * 12;
	private final static long MIN_HOUR_PAINT_TYPE = 3600000 * 3;
	private final static int HOUR_SCALE_SIZE = 32;
	private final static int HOUR_TENM_SCALE_SIZE = 24;

	private enum SCALES_PAINT_TYPE {
		HOUR, // 只显示小时刻度
		HOUR_TENM, // 显示小时刻度以及十分钟刻度
		HOR_TENM_M, HOR_TENM_M_TENSEC;
	}

	private SCALES_PAINT_TYPE m_scalePaintType;

	private float m_fullHourArr[];// 小时刻度数组
	private float m_fullTenMinArr[];// 十分钟刻度数组，全轴时间小于三个小时的时候出现

	private PWGestureDetector m_gestureDetect;

	private List<SDK_REMOTE_FILE> m_playBackFileList;
	private List<FileFrameRect> m_playbackFileRectList;
	private class FileFrameRect{
		float beginx;
		float endx;
		FileFrameRect(float bx,float ex){
			beginx=bx;
			endx=ex;
		}
	}
	private ItimeLineCursorYchangedListener m_cursorYChangedListener;
	private SearchTask m_searchTask;
	private Timer m_searchTimer;
	private static final Object taskWatchObj = new Object();
	private final static int MSG_FLING = 1;
	private final static int MSG_SEARCHOVER = 2;
	private final static int MSG_SEARCH_BEGIN=3;
	private final static int MSG_SEARCH_SUC=4;
	private final static int MSG_SEARCH_FAI=5;
	
	private final static int FLING_TIMER_MSEC_INTERVAL = 50;
	private final static int MIN_FLING_VELOCITY = 100;
	private final static int MIN_FLING_DISTANCE = 100;
	private final static int MIN_PINCH_DISTANCE = 15;
	private final static float MIN_SCROLL_DISTANCE = 0.8f;

	private float prePinchPoint1X;
	private float prePinchPoint2X;
	private float pinchBeginPoint1x;
	private float pinchBeginPoint2x;
	private float pxPerMsecBeforePinch;

	private int m_touchFingCount;
	IPlaybackFileSearchListener m_searchListener;
	private MyFlingThread flingThread;

	private Handler m_handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_FLING:
				// LogUtils.v("onFling", "fling in handler "+msg.arg1);
				m_leftTime.set(m_leftTime.toMillis(false) + msg.arg1);
				m_rightTime.set(m_rightTime.toMillis(false) + msg.arg1);
				
				postCaldulate();
				break;
			case MSG_SEARCHOVER:
				postCaldulate();
				break;
			case MSG_SEARCH_BEGIN:
				if(mSearchStateListener!=null){
					mSearchStateListener.searchBegin();
				}
				break;
			case MSG_SEARCH_SUC:
				if(mSearchStateListener!=null){
					mSearchStateListener.searchSuccess();
				}
				break;
			case MSG_SEARCH_FAI:
				if(mSearchStateListener!=null){
					mSearchStateListener.searchFailured();
				}
				break;
			default:
				break;
			}
		}
	};
	public void reset(){
		if(m_playBackFileList!=null){
			m_playBackFileList.clear();
		}
		if(m_playbackFileRectList!=null){
			m_playbackFileRectList.clear();
		}
		//postCaldulate();
	}
	public void update(){
		reset();
		startSearchFile(200);
	}
	private class MyFlingThread extends Thread {
		private float flingVelocity;
		private boolean m_bStop;

		public MyFlingThread(String name) {
			// TODO Auto-generated constructor stub
			super(name);
			flingVelocity = -1;
			m_bStop = false;
		}

		public synchronized void setStop(boolean flag) {
			m_bStop = flag;
		}

		public synchronized void setVelocity(float ve) {
			flingVelocity = ve;
		}

		@Override
		public void run() {
			while (!m_bStop) {
				// LogUtils.v("onSearch", "fling velocity "+Math.abs(flingVelocity));
				if (Math.abs(flingVelocity) < MIN_FLING_DISTANCE) {
					TimelineDial.this.startSearchFile(SEARCHTIMEDELAY);
					return;
				}
				float m_increaseX = flingVelocity * FLING_TIMER_MSEC_INTERVAL
						/ 1000;
				int increaseMsec = -(int) (m_increaseX / m_pixPerMsec);
				this.setVelocity(flingVelocity / 1.4f);
				Message msg = new Message();
				msg.what = MSG_FLING;
				msg.arg1 = increaseMsec;
				m_handler.sendMessage(msg);
				try {
					this.sleep(FLING_TIMER_MSEC_INTERVAL);
				} catch (InterruptedException e) {
					// TODO: handle exception
					e.printStackTrace();
				}
			}
		}
	}
	
	private SearchStateListener mSearchStateListener;
	public void setSearchStateListener(SearchStateListener lis){
		mSearchStateListener=lis;
	}
	private class SearchTask extends TimerTask {
		@Override
		public void run() {
			// LogUtils.v("onSearch", "TimerTask");
			if (m_searchListener != null) {
				synchronized (taskWatchObj) {
					m_handler.sendEmptyMessage(MSG_SEARCH_BEGIN);
					m_playBackFileList = m_searchListener
							.onGetPlaybackFileList(m_leftTime, m_rightTime);
					if (m_playBackFileList == null) {
						LogUtils.v("playback", "get file err");
						m_handler.sendEmptyMessage(MSG_SEARCH_FAI);
						return;
					}
					m_handler.sendEmptyMessage(MSG_SEARCH_SUC);
					LogUtils.v("playback", "get" + m_playBackFileList.size()
							+ "files");

//					for (int i = 0; i < m_playBackFileList.size(); i++) {
//						LogUtils.v("onSearch", "get file " + i + " "
//								+ m_playBackFileList.get(i).sFileName);
//					}
					Message msg = new Message();
					msg.what = MSG_SEARCHOVER;
					m_handler.sendMessage(msg);
				}
			}
		}
	}
	public TimelineDial(Context context) {
		super(context);
		m_context = context;
		initTimelineDialView();
	}

	public TimelineDial(Context context, AttributeSet attr) {
		super(context, attr);
		m_context = context;
		initTimelineDialView();
	}

	public TimelineDial(Context context, AttributeSet attr, int defaultStyle) {
		super(context, attr, defaultStyle);
		m_context = context;
		initTimelineDialView();
	}

	public void setCursorYchanged(ItimeLineCursorYchangedListener listener) {
		m_cursorYChangedListener = listener;
	}

	public void setPlaybackFileSearchListener(
			IPlaybackFileSearchListener listener) {
		m_searchListener = listener;
	}

	public void setSpanTimeTextSize(float size) {
		m_spanTextPaint.setTextSize(size);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		int mesurewidth = mesureSize(widthMeasureSpec);
		int mesureheight = mesureSize(heightMeasureSpec);
		this.setMeasuredDimension(mesurewidth, mesureheight);
		 LogUtils.v("onMeasure", "getMeasuredHeight "+this.getMeasuredHeight());
		// LogUtils.v("onMeasure", "getWid "+this.getMeasuredWidth());
		m_cursor_y = this.getMeasuredHeight()
				- (int) m_context.getResources().getDimension(
						R.dimen.dimen_axis_hor_up_y_to_bottom);
		// LogUtils.v("onMeasure", "m_cursor_y " + m_cursor_y);
		if (m_cursorYChangedListener != null) {
			m_cursorYChangedListener.onYchanged(m_cursor_y);
		}
		m_pixPerMsec = (float) this.getMeasuredWidth() / this.m_totalSpanMsec;
		m_leftX = m_context.getResources()
				.getDimension(R.dimen.dimen_left_refx);
		m_rightX = this.getMeasuredWidth()
				- m_context.getResources().getDimension(
						R.dimen.dimen_right_refx);
		m_leftSpanTimeBlockPath.reset();
		m_rightSpanTimeBlockPath.reset();
		//LogUtils.e("timeline", "m_leftX:"+m_leftX+"m_rightX:"+m_rightX);
	}

	private int mesureSize(int measureSpec) {
		int specMode = MeasureSpec.getMode(measureSpec);
		int specSize = MeasureSpec.getSize(measureSpec);
		int result = 100;
		if (specMode == MeasureSpec.AT_MOST) {
			// LogUtils.v("onMeasure", "AT_MOST "+specSize);
			result = specSize;
		} else if (specMode == MeasureSpec.EXACTLY) {
			// LogUtils.v("onMeasure", "EXACTLY "+specSize);
			result = specSize;
		} else if (specMode == MeasureSpec.UNSPECIFIED) {
			// LogUtils.v("onMeasure", "UNSPECIFIED "+specSize);
			result = specSize;
		}
		return result;
	}

	

	public int getPxFromDp(int dp) {
		return (int) (dp * m_dpToPxscale + 0.5f);
	}

	public void setPlaybackfileList(List<SDK_REMOTE_FILE> list) {
		synchronized (taskWatchObj) {
			m_playBackFileList = list;
		}
	}

	protected void initTimelineDialView() {
		m_axisPaint_hor = new Paint(Paint.ANTI_ALIAS_FLAG);
		m_curTimeTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
		m_axisPaint_ver = new Paint(Paint.ANTI_ALIAS_FLAG);
		m_spanTextPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
		m_videoFramePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
		m_timelineBgPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
		m_spanTimeBlockPaint = new Paint(Paint.ANTI_ALIAS_FLAG);

		m_initRefTime = new Time();
		m_initRefTime.setToNow();
		m_initRefX = -1;
		
		m_leftTime = new Time();
		m_rightTime = new Time();
		m_CurTime=new Time();
		m_CurTime.setToNow();
		m_leftSpanTimeBlockPath = new Path();
		m_rightSpanTimeBlockPath = new Path();

		m_dpToPxscale = m_context.getResources().getDisplayMetrics().density;

		float spanSize = m_context.getResources().getDimension(
				R.dimen.dimen_span_time_text);
		int spanColor = m_context.getResources().getColor(
				R.color.color_span_time_text);
		m_spanTextPaint.setTextSize(spanSize);
		m_spanTextPaint.setColor(spanColor);
		m_spanTextPaint.setTextAlign(Paint.Align.CENTER);

		m_textPaint = new TextPaint();
		m_textPaint.setColor(spanColor);
		m_textPaint.setTextAlign(Paint.Align.CENTER);
		m_textPaint.setTextSize(spanSize);
		m_textPaint.setAntiAlias(true);

		m_curTimeTextPaint.setTextSize(m_context.getResources().getDimension(
				R.dimen.dimen_cur_time_text));
		m_curTimeTextPaint.setColor(m_context.getResources().getColor(
				R.color.color_cur_time_text));

		m_axisPaint_hor.setStyle(Paint.Style.STROKE);

		m_axisPaint_hor.setStrokeWidth(m_context.getResources().getDimension(
				R.dimen.dimen_axis_hor_wid));
		int horColor = m_context.getResources().getColor(R.color.color_axis);
		m_axisPaint_hor.setColor(horColor);
		m_spanTimeBlockPaint.setStrokeWidth(2);
		m_spanTimeBlockPaint.setStyle(Paint.Style.FILL);

		m_spanTimeBlockPaint.setColor(horColor);

		m_axisPaint_ver.setStyle(Paint.Style.STROKE);

		m_axisPaint_ver.setStrokeWidth(m_context.getResources().getDimension(
				R.dimen.dimen_axis_ver_wid));
		m_axisPaint_ver.setColor(m_context.getResources().getColor(
				R.color.color_axis));

		m_videoFramePaint.setStyle(Paint.Style.FILL);
		m_videoFramePaint.setColor(m_context.getResources().getColor(
				R.color.color_video_frame));

		m_timelineBgPaint.setStyle(Paint.Style.FILL);
		m_timelineBgPaint.setColor(m_context.getResources().getColor(
				R.color.color_timeline_bg));
		m_axis_height = m_context.getResources().getDimension(
				R.dimen.dimen_axis_hei);

		m_gestureDetect = new PWGestureDetector(m_context, this);

		m_totalSpanMsec = INIT_PAINT_TYPE;

		m_fullHourArr = new float[HOUR_SCALE_SIZE];
		m_fullTenMinArr = new float[HOUR_TENM_SCALE_SIZE];

		m_scalePaintType = SCALES_PAINT_TYPE.HOUR;

		m_bInitDraw = false;
		m_touchFingCount = 0;
		prePinchPoint1X = 0;
		prePinchPoint2X = 0;
		pinchBeginPoint1x = 0;
		pinchBeginPoint2x = 0;
		pxPerMsecBeforePinch = 0;
		m_searchListener = null;
		m_searchTimer = new Timer();
		
//		final ViewTreeObserver vTreeObserver =  this.getViewTreeObserver();
//		vTreeObserver.addOnPreDrawListener(new OnPreDrawListener() {
//			private boolean binit=false;
//			@Override
//			public boolean onPreDraw() {
//				if(binit){
//					return true;
//				}
//				binit=true;
//				startSearchFile(200);
//				//vTreeObserver.removeOnPreDrawListener(this);
//				return true;
//			}
//		});
		
		
		
		m_playbackFileRectList=new ArrayList<TimelineDial.FileFrameRect>();
		//LogUtils.v("timeline", "init");
	}

	public synchronized void startSearchFile(int delayMsTime) {
		if (m_searchTask == null) {
			m_searchTask = new SearchTask();
		} else {
			m_searchTask.cancel();
			m_searchTask = new SearchTask();
		}
		m_searchTimer.schedule(m_searchTask, delayMsTime);
	}

	@Override
	public boolean onTouchEvent(MotionEvent ev) {
		m_gestureDetect.onTouchEvent(ev);
		return true;
	}

	@Override
	public boolean onDown(MotionEvent e) {
		// LogUtils.v("motionDetect", "onDown");
		return false;
	}

	@Override
	public void onShowPress(MotionEvent e) {
		// LogUtils.v("motionDetect", "onShowPress");
	}

	@Override
	public boolean onSingleTapUp(MotionEvent e) {
		// LogUtils.v("motionDetect", "onSingleTapUp");
		return false;
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX,
			float distanceY) {
		// LogUtils.v("test", "increaseMsec " +
		// distanceX+" "+distanceY+" "+Math.sqrt(distanceX*distanceX+distanceY*distanceY));
		if (Math.sqrt(distanceX * distanceX + distanceY * distanceY) < MIN_SCROLL_DISTANCE) {
			return true;
		}

		long increaseMsec = (long) (distanceX / m_pixPerMsec);
		// LogUtils.v("onSearch", "increaseMsec " + increaseMsec);
		m_leftTime.set(m_leftTime.toMillis(false) + increaseMsec);
		m_rightTime.set(m_rightTime.toMillis(false) + increaseMsec);
		postCaldulate();
		return true;
	}

	@Override
	public void onLongPress(MotionEvent e) {
		// LogUtils.v("motionDetect", "onLongPress");
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
			float velocityY) {
		// LogUtils.v("onSearch", "onFling "+Math.abs(e1.getX()-e2.getX()));

		if (Math.abs(e1.getX() - e2.getX()) < this.MIN_FLING_DISTANCE) {
			return true;
		}
		if (flingThread != null && flingThread.isAlive()) {
			// LogUtils.v("onFling", "flingThread!=null && flingThread.isAlive()");
			flingThread.setStop(true);
			try {
				flingThread.join();
				LogUtils.v("onFling", "join over");
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}

		}
		flingThread = new MyFlingThread("fling");
		flingThread.setVelocity(velocityX);
		flingThread.start();
		return true;
	}

	@Override
	public boolean onPinch(PWPinchEvent ev) {
		float curPointDistance1 = (float) Math.sqrt(Math.pow(
				ev.pinchCurPoint1.x - ev.pinchPrePoint1.x, 2.0)
				+ Math.pow(ev.pinchCurPoint1.y - ev.pinchPrePoint1.y, 2));
		float curPointDistance2 = (float) Math.sqrt(Math.pow(
				ev.pinchCurPoint2.x - ev.pinchPrePoint2.x, 2.0)
				+ Math.pow(ev.pinchCurPoint2.y - ev.pinchPrePoint2.y, 2));
		if (curPointDistance1 < MIN_PINCH_DISTANCE
				&& curPointDistance2 < MIN_PINCH_DISTANCE) {
			return false;
		}
		//LogUtils.e("timeline", "onPinch getTimeByCursorX m_curx "+m_CurX);
		Time curTime=this.getTimeByCursorX(m_CurX);
		float curDistance = (float) Math.sqrt(Math.pow(ev.pinchCurPoint1.x
				- ev.pinchCurPoint2.x, 2.0)
				+ Math.pow(ev.pinchCurPoint1.y - ev.pinchCurPoint2.y, 2));
		
		float beginDistance = (float) Math.sqrt(Math.pow(ev.pinchBeginPoint1.x
				- ev.pinchBeginPoint2.x, 2.0)
				+ Math.pow(ev.pinchBeginPoint1.y - ev.pinchBeginPoint2.y, 2));
		
		float pinchRatio = curDistance / beginDistance;
		
		LogUtils.v("onPinch", "pinchRatio " + pinchRatio);
		if (m_totalSpanMsec < MIN_TOTALSPAN && pinchRatio > 1) {
			return false;
		} else if (m_totalSpanMsec > MAX_TOTALSPAN && pinchRatio < 1) {
			return false;
		}
		m_totalSpanMsec = pxPerMsecBeforePinch / pinchRatio;//计算出整个时间轴的时间跨度
		
		// MIN_HOUR_TENM_PAINT_TYPE
		if (m_totalSpanMsec >= MIN_HOUR_PAINT_TYPE) {
			m_scalePaintType = SCALES_PAINT_TYPE.HOUR;
		} else if (m_totalSpanMsec < MIN_HOUR_PAINT_TYPE) {
			m_scalePaintType = SCALES_PAINT_TYPE.HOUR_TENM;
		}
		m_pixPerMsec = getMeasuredWidth() / m_totalSpanMsec;//计算出px/ms
		
		//LogUtils.e("timeline", "onPinch getLeftAndRightTimeByRef m_curx "+m_CurX);
		getLeftAndRightTimeByRef(m_CurX, curTime);
		//getScalesByDestRef(centerX, centerTime);		
		postCaldulate();
		this.startSearchFile(SEARCHTIMEDELAY);
		return true;
	}

	@Override
	public boolean onTouchUp() {
		this.startSearchFile(SEARCHTIMEDELAY);
		return true;
	}

	public float getXByTime(Time time) {
		float x = -1;
		float tmpMsec = time.toMillis(false) - m_leftTime.toMillis(false);
		x = m_leftX + m_pixPerMsec * tmpMsec;
		return x;
	}

	public Time getTimeByCursorX(float curx) {
		Time tmpTime = new Time();
		float tmpMsec = m_leftTime.toMillis(false) - (m_leftX - curx)
				/ m_pixPerMsec;
		tmpTime.set((long) tmpMsec);
		return tmpTime;
	}
	public SDK_REMOTE_FILE getFileByCursorX(float curx){
		Time CurTime=getTimeByCursorX(curx);
		if(m_playBackFileList!=null){
			for (SDK_REMOTE_FILE file : m_playBackFileList) {
				if(file.startTime.before(CurTime) && file.endTime.after(CurTime)){
					return file;
				}
			}
		}
		return null;
	}
	
	public void updateCurTimeByCurx(float curx){
		//m_CurTime=getTimeByCursorX(curx);
		m_CurX=curx;
		postCaldulate();
	}
	public int getPosByCurx(SDK_REMOTE_FILE file,float curx){
		Time CurTime=getTimeByCursorX(curx);
		long curMsec=CurTime.toMillis(false)-file.startTime.toMillis(false);
		return (int)(100*curMsec*1.0/(file.endTime.toMillis(false)-file.startTime.toMillis(false)));
	}
	@Override
	public boolean onPinchBegin() {
		pxPerMsecBeforePinch = m_totalSpanMsec;
		return true;
	}

	@Override
	public boolean onPinchEnd() {
		// LogUtils.v("onSearch ","onDialPinchEnd");
		if (m_searchListener != null) {
			m_playBackFileList = m_searchListener.onGetPlaybackFileList(
					m_leftTime, m_rightTime);
		}
		return true;
	}

	public void setInitTime(float initX, Time initTime) {
		m_initRefTime = initTime;
		m_initRefX = initX;
	}

	private void getLeftAndRightTimeByRef(float refx, Time refTime) {
		float leftMsec = refTime.toMillis(false) - (refx - m_leftX)
				/ m_pixPerMsec;
		m_leftTime.set((long) leftMsec);
		float rightMsec = refTime.toMillis(false) - (refx - m_rightX)
				/ m_pixPerMsec;
		m_rightTime.set((long) rightMsec);

		// LogUtils.v("timeline", "m_leftTime:"+m_leftTime+"m_leftTime:"+m_leftTime);
	}

	private synchronized void getScalesByDestRef(float refX, Time refTime) {
		Arrays.fill(m_fullHourArr, -1);
		Arrays.fill(m_fullTenMinArr, -1);

		if (m_scalePaintType == SCALES_PAINT_TYPE.HOUR) {
			getScales(m_fullHourArr, refX, refTime, 3600000,
					SCALES_PAINT_TYPE.HOUR);
		} else if (m_scalePaintType == SCALES_PAINT_TYPE.HOUR_TENM) {
			getScales(m_fullHourArr, refX, refTime, 3600000,
					SCALES_PAINT_TYPE.HOUR);
			getScales(m_fullTenMinArr, refX, refTime, 600000,
					SCALES_PAINT_TYPE.HOUR_TENM);
		}
	}

	private void getScales(float[] arr, float refX, Time refTime,
			long scaleSpanTime, SCALES_PAINT_TYPE type) {
		long x_msec = refTime.toMillis(false);
		long fullHour_leftFirstGap = x_msec % scaleSpanTime;
		long fullHour_rightFirstGap = scaleSpanTime - x_msec % scaleSpanTime;
		int index = 0;
		arr[index++] = refX + fullHour_rightFirstGap * m_pixPerMsec;
		arr[index++] = refX - fullHour_leftFirstGap * m_pixPerMsec;
		// 左右两边第一个整点刻度
		int max_size = 0;
		if (type == SCALES_PAINT_TYPE.HOUR) {
			max_size = HOUR_SCALE_SIZE;
		} else if (type == SCALES_PAINT_TYPE.HOR_TENM_M) {
			max_size = HOUR_TENM_SCALE_SIZE;
		}
		int i = 1;
		while (true) {
			if (arr[0] > this.getMeasuredWidth()) {
				break;
			}
			float rightFullHourTemp = arr[0] + scaleSpanTime * m_pixPerMsec
					* (i);
			if (rightFullHourTemp > this.getMeasuredWidth()) {
				break;
			}
			arr[index++] = rightFullHourTemp;
			// if(index>=max_size){
			// return;
			// }
			i++;
		}
		i = 1;
		while (true) {
			if (arr[1] < 0) {
				break;
			}
			float leftFullHourTemp = arr[1] - scaleSpanTime * m_pixPerMsec
					* (i);
			if (leftFullHourTemp < 0) {
				break;
			}

			arr[index++] = leftFullHourTemp;
			// if(index>=max_size){
			// return;
			// }
			i++;
		}
	}
	private void postCaldulate(){
		AsyncTask.execute(new CalculateRunnable());
	}
	
	private class CalculateRunnable implements Runnable{
		@Override
		public void run() {
			// TODO Auto-generated method stub
			calculateDrawData();
			postInvalidate();
		}
	}
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		// TODO Auto-generated method stub
		super.onSizeChanged(w, h, oldw, oldh);
		m_CurX=getXByTime(m_CurTime);
		postCaldulate();
	}
	public Time getCurTime(){
		return m_CurTime;
	}
	private void calculateDrawData(){
		//LogUtils.e("timeline", "calculateDrawData m_CurX "+m_CurX);
		m_CurTime=getTimeByCursorX(m_CurX);
		m_playbackFileRectList.clear();
		//LogUtils.e("timeline", "calculateDrawData m_leftX "+m_leftX+" m_leftTime "+m_leftTime);
		getScalesByDestRef(m_leftX, m_leftTime);
		
		//getScalesByDestRef(m_leftX, m_leftTime);
		if (m_playBackFileList != null) {
			for (int i = 0; i < m_playBackFileList.size(); i++) {
				float beginx = this
						.getXByTime(m_playBackFileList.get(i).startTime);
				float endx = this.getXByTime(m_playBackFileList.get(i).endTime);
				m_playbackFileRectList.add(new FileFrameRect(beginx,endx));
			}
		}
		
	}
	
	
	
	public  void setCurrentTimeTextSize(float size) {
		m_curTimeTextPaint.setTextSize(size);
	}

	public void setSpanTimeTextColor(int color) {
		m_spanTextPaint.setColor(color);
		;
	}

	public void setCurrentTimeTextColor(int color) {
		m_curTimeTextPaint.setColor(color);
	}

	public void setAxisColor(int color) {
		m_axisPaint_hor.setColor(color);
		m_axisPaint_ver.setColor(color);
		m_spanTimeBlockPaint.setColor(color);
	}

	public void setVideoFrameColor(int color) {
		m_videoFramePaint.setColor(color);
	}

	public void setAxisVerWidth(int wid) {
		m_axisPaint_ver.setStrokeWidth(wid);
	}

	public void setAxisHorWidth(int wid) {
		m_axisPaint_hor.setStrokeWidth(wid);
	}

	public void setTimelineBgColor(int color) {
		m_timelineBgPaint.setColor(color);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		if (!m_bInitDraw) {
			m_bInitDraw = true;
			if (m_initRefX < 0) {
				m_initRefTime.setToNow();
				m_initRefX = (float) (this.getMeasuredWidth() / 2.0);
			}
			// LogUtils.v("timeline",
			// "m_initRefX:"+m_initRefX+"m_initRefTime: "+m_initRefTime);
			this.getLeftAndRightTimeByRef(m_initRefX, m_initRefTime);
			this.getScalesByDestRef(m_leftX, m_leftTime);
			// LogUtils.v("timeline", "m_leftX:"+m_leftX+"m_rightX:"+m_rightX);
		}
		int m_width = this.getMeasuredWidth();
		int m_height = this.getMeasuredHeight();

		int lineUpY = (int) m_cursor_y;
		int lineBelowY = lineUpY + (int) m_axis_height;
		int blockW = this.getPxFromDp(3);
		if (m_leftSpanTimeBlockPath.isEmpty()) {
			// LogUtils.v("timeline", "m_leftSpanTimeBlockPath.isEmpty " + m_leftX
			// + " " + lineUpY);
			m_leftSpanTimeBlockPath.moveTo(m_leftX, lineUpY - 2);
			m_leftSpanTimeBlockPath.lineTo(m_leftX - blockW,
					(float) (lineUpY - 2 * blockW));
			m_leftSpanTimeBlockPath.lineTo(m_leftX + blockW,
					(float) (lineUpY - 2 * blockW));
			m_leftSpanTimeBlockPath.close();
		}
		if (m_rightSpanTimeBlockPath.isEmpty()) {
			// LogUtils.v("timeline", "m_rightSpanTimeBlockPath.isEmpty " +
			// m_rightX);
			m_rightSpanTimeBlockPath.moveTo(m_rightX, lineUpY - 2);
			m_rightSpanTimeBlockPath.lineTo(m_rightX - blockW,
					(float) (lineUpY - 2 * blockW));
			m_rightSpanTimeBlockPath.lineTo(m_rightX + blockW,
					(float) (lineUpY - 2 * blockW));
			m_rightSpanTimeBlockPath.close();
			// m_rightSpanTimeBlockPath.moveTo(m_rightX, lineUpY);
		}
		//Time centerTime = this.getTimeByCursorX(getMeasuredWidth() / 2);
		String str=getContext().getResources().getString(R.string.string_current_time);
		String centerTimeString = str+m_CurTime.format("%Y-%m-%d\r\n%H:%M:%S");
	
//		Paint centerPaint = new Paint();
//		centerPaint.setColor(spanColor);
//		centerPaint.setTextSize(25);

		String leftTimeString = m_leftTime.format("%Y-%m-%d\r\n%H:%M:%S");
		String rightTimeString = m_rightTime.format("%Y-%m-%d\r\n%H:%M:%S");

		canvas.drawRect(0, 0, m_width, m_height, m_timelineBgPaint);
		float spantexth = m_textPaint.measureText("Yy");
		float spantextw = m_textPaint.measureText(leftTimeString);

		canvas.drawText(centerTimeString, 120, 30, m_textPaint);
		StaticLayout layout = new StaticLayout(leftTimeString, m_textPaint,
				(int) spantextw, Alignment.ALIGN_NORMAL, 1.0f, 0.0f, true);
		canvas.save();
		canvas.translate(m_leftX,
				(float) (lineUpY - 2 * blockW - 2.1 * spantexth));
		layout.draw(canvas);
		canvas.restore();

		layout = new StaticLayout(rightTimeString, m_textPaint,
				(int) spantextw, Alignment.ALIGN_NORMAL, 1.0f, 0.0f, true);
		canvas.save();
		canvas.translate(m_rightX,
				(float) (lineUpY - 2 * blockW - 2.1 * spantexth));
		layout.draw(canvas);
		canvas.restore();

		// canvas.drawText(leftTimeString, m_leftX,
		// (float)(lineUpY-2*blockW-spantexth/2), m_spanTextPaint);
		// canvas.drawText(rightTimeString, m_rightX,
		// (float)(lineUpY-2*blockW-spantexth/2), m_spanTextPaint);

		canvas.drawLine(0, lineUpY, m_width, lineUpY, m_axisPaint_hor);
		canvas.drawLine(0, lineBelowY, m_width, lineBelowY, m_axisPaint_hor);
		canvas.drawPath(m_leftSpanTimeBlockPath, m_spanTimeBlockPaint);
		canvas.drawPath(m_rightSpanTimeBlockPath, m_spanTimeBlockPaint);

		for (int i = 0; i < m_fullHourArr.length; i++) {
			if (m_fullHourArr[i] > 0) {
				canvas.drawLine(m_fullHourArr[i], lineUpY + 1,
						m_fullHourArr[i], lineBelowY - 1, m_axisPaint_ver);
			}
		}
		for (int j = 0; j < m_fullTenMinArr.length; j++) {
			if (m_fullTenMinArr[j] > 0) {
				canvas.drawLine(m_fullTenMinArr[j], lineUpY + 1,
						m_fullTenMinArr[j], lineBelowY - 1 - getPxFromDp(8),
						m_axisPaint_ver);
			}
		}
		List<FileFrameRect> tempList=new ArrayList<TimelineDial.FileFrameRect>();
		tempList.addAll(m_playbackFileRectList);
		for(int i=0;i<tempList.size();i++){
			canvas.drawRect(tempList.get(i).beginx, lineUpY + 1, 
							tempList.get(i).endx, lineBelowY - 1,
							m_videoFramePaint);
		}
	}
}
