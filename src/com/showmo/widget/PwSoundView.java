package com.showmo.widget;

import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.util.LogUtils;

import android.R.integer;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnPreDrawListener;
import android.view.animation.Animation;
import android.view.animation.ScaleAnimation;
import android.widget.LinearLayout;

public class PwSoundView extends LinearLayout{

	private int mColor=Color.argb(255, 255, 87, 34);
	private int mItemWid=20;
	private int mSound=0;
	private int mMax=100;
	private int mMin=0;
	private boolean bInit=false;
	public PwSoundView(Context context){
		this(context, null);
	}
	public PwSoundView(Context context,AttributeSet attrs){
		this(context, attrs, 0);
	}
	public PwSoundView(Context context,AttributeSet attrs,int style){
		
		super(context, attrs, style);
		TypedArray tArray=context.obtainStyledAttributes(attrs, R.styleable.PwSoundView);
		mColor=tArray.getColor(R.styleable.PwSoundView_itemColor, Color.argb(255, 255, 87, 34));
		mMax=tArray.getInt(R.styleable.PwSoundView_maxValue, 100);
		mMin=tArray.getInt(R.styleable.PwSoundView_minValue, 0);
		ViewTreeObserver vb= this.getViewTreeObserver();
		mChildList=new ArrayList<View>();
		vb.addOnPreDrawListener(new OnPreDrawListener() {
			
			@Override
			public boolean onPreDraw() {
				// TODO Auto-generated method stub
				if(getMeasuredWidth()>0){
					if(!bInit){
						bInit=true;
						init();
					}
				}
				return true;
			}
		});
	}
	private class PwSoundItemView extends View{
		public PwSoundItemView(Context context){
			this(context, null);
		}
		public PwSoundItemView(Context context,AttributeSet attrs){
			this(context, attrs, 0);
		}
		public PwSoundItemView(Context context,AttributeSet attrs,int style){
			super(context, attrs, style);
		}
		int mColor=Color.argb(255, 255, 87, 34);
		public void setmColor(int mColor) {
			this.mColor = mColor;
			postInvalidate();
		}
		@Override
		protected void onDraw(Canvas canvas) {
			// TODO Auto-generated method stub
			super.onDraw(canvas);
			Paint mPaint=new Paint(Paint.ANTI_ALIAS_FLAG);
			mPaint.setColor(mColor);
			mPaint.setStyle(Paint.Style.FILL);
			
			int centerX=getMeasuredWidth()/2;
			int centerY=getMeasuredHeight()/2;
			
			int radios=(getMeasuredWidth()>getMeasuredHeight()?getMeasuredHeight():getMeasuredWidth())/2;
			//LogUtils.e("display", "centerX centerY radios "+centerX+" "+centerY+" "+radios);
			canvas.drawArc(new RectF(centerX-radios,centerY-radios,centerX+radios,centerY+radios)
					, 0, 360, true, mPaint);
			//canvas.drawCircle(centerX, centerY, radios, mPaint);
			canvas.drawRoundRect(new RectF(0,0,getMeasuredWidth(),getMeasuredHeight()), 
					radios, radios, mPaint);
		}
	}
	List<View> mChildList;
	private void init(){
		this.setOrientation(LinearLayout.HORIZONTAL);
		this.setGravity(Gravity.CENTER);
		int wid=getMeasuredWidth();
		mItemCount=wid/35;
		LayoutParams lParams=new LayoutParams(15, 20, 1);
		lParams.leftMargin=10;
		lParams.rightMargin=10;
		setWeightSum(wid);
		//LogUtils.e("dis", "wid "+wid+"count "+mItemCount);
		
		for (int i = 0; i < mItemCount; i++) {
			PwSoundItemView view=new PwSoundItemView(getContext());
			view.setmColor(mColor);
			//view.setBackgroundColor(Color.BLUE);
			mChildList.add(view);
			this.addView(view, lParams);
		}
	}
	Handler mHandler=new Handler();
	private int mItemCount=0;
	public void setVolum(final int volum) {
		// TODO Auto-generated method stub
		mHandler.post(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				int mVolum=volum;
				if(volum >mMax){
					mVolum=mMax;
				}
				if(volum<mMin){
					mVolum=mMin;
				}
				
				int count=(int)((mVolum-mMin)*1.0f/(mMax-mMin)*mItemCount/2);
				float centerIndex= (mChildList.size()+1)/2.0f;
			//	LogUtils.e("dis", "volum"+mVolum+"count "+count+" centerIndex "+centerIndex+" ("+(int)(centerIndex-count)+", "+(int)(centerIndex+count));
				for (int i = (int)(centerIndex-count); i < (int)(centerIndex+count); i++) {
					/*
					 * 范围在1-5倍之间变化
					 */
					float hscale=(Math.abs(i-centerIndex  +1) /Math.abs(mChildList.size() - centerIndex+1)) *5+1;
					//LogUtils.e("start", "hscale "+hscale+"centerIndex "+centerIndex);
					mChildList.get(i).setScaleX(1.1f);
					mChildList.get(i).setScaleY(hscale);
					ScaleAnimation sa=new ScaleAnimation(1.0f, 1.0f/1.1f, 
							1.0f,1.0f /hscale, 
							Animation.RELATIVE_TO_SELF, 0.5f, 
							Animation.RELATIVE_TO_SELF, 0.5f);
					sa.setFillAfter(true);
					sa.setDuration(500);
					
					mChildList.get(i).startAnimation(sa);
				}
			}
		});
		
	}
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		int widmode=MeasureSpec.getMode(widthMeasureSpec);
		int widSize=MeasureSpec.getSize(widthMeasureSpec);
		switch (widmode) {
		case MeasureSpec.AT_MOST:
			widSize=200;
			break;
		case MeasureSpec.UNSPECIFIED:
			break;
		case MeasureSpec.EXACTLY:
			break;
		default:
			break;
		}
		int heimode=MeasureSpec.getMode(widthMeasureSpec);
		int heiSize=MeasureSpec.getSize(widthMeasureSpec);
		switch (heimode) {
		case MeasureSpec.AT_MOST:
			heiSize=100;
			break;
		case MeasureSpec.UNSPECIFIED:
			break;
		case MeasureSpec.EXACTLY:
			break;
		default:
			break;
		}
		measureChildren(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(widSize, heiSize);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);

	}
}
