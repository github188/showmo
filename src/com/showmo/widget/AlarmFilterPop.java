package com.showmo.widget;
 

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import com.showmo.R;
import com.showmo.activity.alarmInfo.CameraAlarmType;
import com.showmo.base.ShowmoApplication;
import com.showmo.util.LogUtils;
import com.showmo.util.TimeUtil;
import com.showmo.widget.slidedatetimepicker.SlideDateTimeListener;
import com.showmo.widget.slidedatetimepicker.SlideDateTimePicker;

import android.R.integer;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;  
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;  
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.LayoutInflater;  
import android.view.View;  
import android.view.View.OnClickListener;  
import android.view.ViewGroup.LayoutParams;  
import android.widget.Button;
import android.widget.PopupWindow;  
import android.widget.TextView;
import android.widget.Toast;
  
@SuppressLint("InflateParams")
public class AlarmFilterPop extends PopupWindow 
	implements OnClickListener{  
	
	private static final String MIN_DATE = "2010/1/1 00:00" ;

	private static final String MAX_DATE = "2100/12/31 23:59" ;
	
	public static final String  DATE_FORMAT_DEF="yyyy/MM/dd HH:mm";
	
	private SimpleDateFormat mSimpleDateFormat ;
	
	//默认的时间间隔    单位小时
	private static final int DATE_INTERVAL = 2 ;
  
    private View mPopWindow;
    
    private TextView mTvBeginTime ;
    
    private TextView mTvEndTime;
    
    private TextView mTvType ;
    
	private SlideDateTimeListener mSlideDateTimeListener ;

	private SlideDateTimePicker mSlideDateTimePicker ;
	
	//private Date mBeginDate;

	//private Date mEndDate;
	
	private Date mTempBeginDate ;
	
	private Date mTempEndDate ;
	
	private Calendar mCalendar;
    
	//private int mAlarmType ;

	private FragmentActivity activity ;
	
	private onFilterSelectedLisenter mOnFilterSelectedLisenter ;

	private Button mBtnOk;

	private Button mBtnCancel;
	
	private boolean isSelectBeginTime ;
	
	private AlarmFilterCondition mAlarmFilterCondition;
	public class AlarmFilterCondition{
		public long begintime;
		public long endtime;
		public int alarmType;
	}
	
    public AlarmFilterPop(FragmentActivity activity,onFilterSelectedLisenter listener) {  
        super(activity);  
        this.activity = activity ;
        this.mOnFilterSelectedLisenter = listener ;
        LayoutInflater inflater = (LayoutInflater) activity  
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);  
        mPopWindow = inflater.inflate(R.layout.widget_filter_window, null);  
        
        //点击空白处 隐藏
        mPopWindow.findViewById(R.id.tv_filter_blank).setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				dismiss();
			}
		});
        mSimpleDateFormat = new SimpleDateFormat(DATE_FORMAT_DEF);
        
        mTvBeginTime = (TextView) mPopWindow.findViewById(R.id.tv_filter_begin_time);
        mTvBeginTime.setOnClickListener(this);
        mTvEndTime = (TextView) mPopWindow.findViewById(R.id.tv_filter_end_time);
        mTvEndTime.setOnClickListener(this);
        mTvType = (TextView) mPopWindow.findViewById(R.id.tv_filter_type);
        mTvType.setOnClickListener(this);
        mBtnOk = (Button)mPopWindow.findViewById(R.id.btn_filter_ok);
        mBtnOk.setOnClickListener(this);
        mBtnCancel = (Button)mPopWindow.findViewById(R.id.btn_filter_cancel);
        mBtnCancel.setOnClickListener(this);
        mAlarmFilterCondition=new AlarmFilterCondition();
        //设置内容
        setContentView(mPopWindow);  
        //设置SelectPicPopupWindow弹出窗体的宽  
        setWidth(LayoutParams.MATCH_PARENT);  
        //设置SelectPicPopupWindow弹出窗体的高  
        setHeight(LayoutParams.MATCH_PARENT);  
        //设置SelectPicPopupWindow弹出窗体可点击  
        setFocusable(true);  
        //设置SelectPicPopupWindow弹出窗体动画效果  
//        this.setAnimationStyle(R.style.AnimBottom);  
        //实例化一个ColorDrawable颜色为半透明  
        ColorDrawable dw = new ColorDrawable(0xb0000000);  
        //设置SelectPicPopupWindow弹出窗体的背景  
        this.setBackgroundDrawable(dw);  
        
        resetFilterAll();
        
    }
    
    public void resetFilterAll(){
    	resetFilterDate();
    	resetFilterAlarmType();
    }
    
	//重置筛选的时间条件
	public void resetFilterDate(){
		mCalendar = Calendar.getInstance();
		 mTempEndDate= mCalendar.getTime();
			 mAlarmFilterCondition.endtime=getTime_t(mTempEndDate);
				mCalendar.add(Calendar.HOUR_OF_DAY, -DATE_INTERVAL);
				 mTempBeginDate	 = mCalendar.getTime();
				 mAlarmFilterCondition.begintime=getTime_t(mTempBeginDate);
	 }
	
	//重置筛选的告警类型条件
	public void resetFilterAlarmType(){
		mAlarmFilterCondition.alarmType = CameraAlarmType.ALARM_TYPE_DETECTION_MOTION ;
	}
    
    
    private void showDateTimePicker( ) {

		if(mSlideDateTimePicker == null){

			mSlideDateTimeListener = new SlideDateTimeListener() {

				@Override
				public void onDateTimeSet(Date date) {
					if(isSelectBeginTime){
						mTempBeginDate = date ;
						mTvBeginTime.setText(mSimpleDateFormat.format(mTempBeginDate.getTime()));
					}else{
						mTempEndDate = date ;
						mTvEndTime.setText(mSimpleDateFormat.format(mTempEndDate.getTime()));
					}
				}
			};
			
				try {
					mSlideDateTimePicker = new SlideDateTimePicker.Builder(activity.getSupportFragmentManager())
					.setListener(mSlideDateTimeListener)
					// .setInitialDate(new Date())
					.setMinDate(mSimpleDateFormat.parse(MIN_DATE))
					.setMaxDate(mSimpleDateFormat.parse(MAX_DATE))
					.setIs24HourTime(true)
					//.setTheme(SlideDateTimePicker.HOLO_DARK)
					.setIndicatorColor(Color.parseColor("#e85a05"))
					.build();
				} catch (ParseException e) {
					e.printStackTrace();
				}
				
		}
		if(isSelectBeginTime){
			mSlideDateTimePicker.setInitialDate(mTempBeginDate);
		}else{
			mSlideDateTimePicker.setInitialDate(mTempEndDate);
		}

		mSlideDateTimePicker.show();
	}
    
    private long getTime_t(Date date){
    	Calendar canl=Calendar.getInstance();
		int timezoneOffsetSec=canl.get(Calendar.ZONE_OFFSET)+canl.get(Calendar.DST_OFFSET);
		timezoneOffsetSec/=1000;
		return date.getTime()/1000+timezoneOffsetSec;
    }
    @Override
	public void onClick(View v) {
    	switch (v.getId()) {
		case R.id.btn_filter_ok:
			if(mOnFilterSelectedLisenter != null){
				mAlarmFilterCondition.begintime= getTime_t(mTempBeginDate);
				mAlarmFilterCondition.endtime = getTime_t(mTempEndDate); 
				
				if(mAlarmFilterCondition.begintime  > mAlarmFilterCondition.endtime ){
					Toast.makeText(activity, activity.getString(R.string.begintime_bigger_than_endtime) , Toast.LENGTH_SHORT).show();
					break ;
				}
				
				LogUtils.e("AlarmFilterPop", "mBeginDate-->"+mTempBeginDate);
				LogUtils.e("AlarmFilterPop", "beginTime-->"+mAlarmFilterCondition.begintime);		
				
				LogUtils.e("AlarmFilterPop", "mEndDate-->"+mTempEndDate);
				LogUtils.e("AlarmFilterPop", "endTime--->"+mAlarmFilterCondition.endtime);
				LogUtils.e("AlarmFilterPop", "mAlarmType-->"+mAlarmFilterCondition.alarmType);
				
				
				mOnFilterSelectedLisenter.onBtnOkClick(mAlarmFilterCondition.begintime, mAlarmFilterCondition.endtime, mAlarmFilterCondition.alarmType);
			}
			dismiss();
			break;
		case R.id.btn_filter_cancel:

			dismiss();
			break;
		case R.id.tv_filter_begin_time:
			isSelectBeginTime = true ;
			showDateTimePicker( );
			break;
		case R.id.tv_filter_end_time:
			isSelectBeginTime = false ;
			showDateTimePicker( );
			break;
		case R.id.tv_filter_type:
			break;
			

		default:
			break;
		}
		
		
		
	}  
    
 
    public interface onFilterSelectedLisenter {
    	void onBtnOkClick(long beginTime,long endTime,int alarmType );
    		
    }


	public onFilterSelectedLisenter getOnFilterSelectedLisenter() {
		return mOnFilterSelectedLisenter;
	}

	public void setOnFilterSelectedLisenter(
			onFilterSelectedLisenter mOnFilterSelectedLisenter) {
		this.mOnFilterSelectedLisenter = mOnFilterSelectedLisenter;
	}
	
//	public Date getBeginDate() {
//		return mBeginDate;
//	}
//
//	public void setBeginDate(Date mBeginDate) {
//		this.mBeginDate = mBeginDate;
//	}
//
//	public Date getEndDate() {
//		return mEndDate;
//	}
//
//	public void setEndDate(Date mEndDate) {
//		this.mEndDate = mEndDate;
//	}
//
//	public int getAlarmType() {
//		return mAlarmType;
//	}
//
//	public void setAlarmType(int mAlarmType) {
//		this.mAlarmType = mAlarmType;
//	}
    
    public AlarmFilterCondition getmAlarmFilterCondition() {
		return mAlarmFilterCondition;
	}

	public void setmAlarmFilterCondition(AlarmFilterCondition mAlarmFilterCondition) {
		this.mAlarmFilterCondition = mAlarmFilterCondition;
	}

	@Override
    public void showAsDropDown(View anchor) {
    	setViewContent();
    	super.showAsDropDown(anchor);
    }
    
    @Override
    public void showAsDropDown(View anchor, int xoff, int yoff) {
    	setViewContent();
    	super.showAsDropDown(anchor, xoff, yoff);
    }
    
    @Override
    public void showAtLocation(View parent, int gravity, int x, int y) {
    	setViewContent();
    	super.showAtLocation(parent, gravity, x, y);
    }
    
    private void setViewContent(){
    	
    	mTvBeginTime.setText(TimeUtil.format(mAlarmFilterCondition.begintime));
    	mTvEndTime.setText(TimeUtil.format(mAlarmFilterCondition.endtime));
    }
    

  
}  