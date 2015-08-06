package com.showmo.widget;

import com.showmo.R;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;

import android.content.Context;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.View;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.LinearLayout;

public class NetLoadingView extends LinearLayout{
	private View leftPoint;
	private View centerPoint;
	private View rightPoint;
	private Handler mHandler=new Handler();
	public NetLoadingView(Context context) {
		this(context,null);
		// TODO Auto-generated constructor stub
	}
	public NetLoadingView(Context context,AttributeSet attrs){
		this(context,attrs,0);
	}
	public NetLoadingView(Context context,AttributeSet attrs,int style){
		super(context,attrs,style);
		this.init();
	}	
	private void init(){
		this.setGravity(Gravity.CENTER);
		
		leftPoint=new View(getContext());
		leftPoint.setBackgroundResource(R.drawable.red_point);
		
		centerPoint=new View(getContext());
		centerPoint.setBackgroundResource(R.drawable.orange_point);
		
		rightPoint=new View(getContext());
		rightPoint.setBackgroundResource(R.drawable.blue_point);
		
		LinearLayout.LayoutParams params=new LayoutParams(HexTrans.dip2px(getContext(), 10), 
				HexTrans.dip2px(getContext(), 10));
		params.setMargins(5, 0, 5, 0);
		this.addView(leftPoint, params);
		this.addView(centerPoint, params);
		this.addView(rightPoint, params);
//		mHandler.postDelayed(new Runnable() {
//			
//			@Override
//			public void run() {
//				// TODO Auto-generated method stub
//				startAnim();
//			}
//		}, 200);
		startAnim();
	}
	
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		int wmode=MeasureSpec.getMode(widthMeasureSpec);
		int hmode=MeasureSpec.getMode(heightMeasureSpec);
		int wsize=MeasureSpec.getSize(widthMeasureSpec);
		int hsize=MeasureSpec.getSize(heightMeasureSpec);
		switch (wmode) {
		case MeasureSpec.AT_MOST:
			//LogUtils.v("measure", "NetLoadingView MeasureSpec.AT_MOST");
			wsize = HexTrans.dip2px(getContext(), 100);
			break;
		case MeasureSpec.EXACTLY:
			//LogUtils.v("measure", "NetLoadingView MeasureSpec.EXACTLY");
			break;
		default:
			break;
		}
		switch (hmode) {
		case MeasureSpec.AT_MOST:
			//LogUtils.v("measure", "NetLoadingView MeasureSpec.AT_MOST");
			hsize = HexTrans.dip2px(getContext(), 40);
			break;
		case MeasureSpec.EXACTLY:
			LogUtils.v("measure", "NetLoadingView MeasureSpec.EXACTLY");
			break;
		default:
			break;
		}
		setMeasuredDimension(wsize, hsize);
	}
	
	public void startAnim(){
		TranslateAnimation animation_left=new TranslateAnimation(
				Animation.RELATIVE_TO_PARENT,0.0f, 
				Animation.RELATIVE_TO_PARENT,0.2f,
				Animation.RELATIVE_TO_PARENT,0.0f, 
				Animation.RELATIVE_TO_PARENT,0.0f);
//		TranslateAnimation animation_left=new TranslateAnimation(
//				0f,  
//				rightPoint.getX()-leftPoint.getX(), 
//				0.0f, 
//				0.0f);
		LogUtils.v("anim", "animation_left "+leftPoint.getX() + "rightPoint get"+rightPoint.getX());
		animation_left.setInterpolator(new AccelerateDecelerateInterpolator());
		animation_left.setDuration(500);
		animation_left.setRepeatCount(Animation.INFINITE);
		animation_left.setRepeatMode(Animation.REVERSE);
		
		TranslateAnimation animation_right=new TranslateAnimation(
				Animation.RELATIVE_TO_PARENT,0f,  
				Animation.RELATIVE_TO_PARENT,-0.2f, 
				Animation.RELATIVE_TO_PARENT,0.0f, 
				Animation.RELATIVE_TO_PARENT,0.0f);
		

//		TranslateAnimation animation_right=new TranslateAnimation(
//				0f,  
//				leftPoint.getX()-rightPoint.getX(), 
//				0.0f, 
//				0.0f);
		animation_right.setInterpolator(new AccelerateDecelerateInterpolator());
		animation_right.setDuration(500);
		animation_right.setRepeatCount(Animation.INFINITE);
		animation_right.setRepeatMode(Animation.REVERSE);
		
		leftPoint.startAnimation(animation_left);
		rightPoint.startAnimation(animation_right);
	}
}
