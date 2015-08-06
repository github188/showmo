package com.showmo.widget;

import com.showmo.R;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.util.TypedValue;
import android.view.MotionEvent;
import android.view.View;

public class PwRoundVolumControlView extends View {
	/**
	 * 第一圈的颜色
	 */
	private int mFirstColor;

	/**
	 * 第二圈的颜色
	 */
	private int mSecondColor;
	/**
	 * 圈的宽度
	 */
	private int mRingWidth;
	
	private int mRadius;
	/**
	 * 画笔
	 */
	private Paint mPaint;
	
	private Paint mBackPaint;
	/**
	 * 当前进度
	 */
	private int mCurrentCount = 0;

	/**
	 * 中间的图片
	 */
	private Bitmap mImage;
	/**
	 * 每个块块间的间隙
	 */
	private int mSplitSize;
	/**
	 * 个数
	 */
	
	private int mBackgroundColor;
	private int mCount;

	private Rect mRect;
	private RectF mRectF;
	public PwRoundVolumControlView(Context context, AttributeSet attrs)
	{
		this(context, attrs, 0);
	}

	public PwRoundVolumControlView(Context context)
	{
		this(context, null);
	}

	/**
	 * 必要的初始化，获得一些自定义的值
	 * 
	 * @param context
	 * @param attrs
	 * @param defStyle
	 */
	public PwRoundVolumControlView(Context context, AttributeSet attrs, int defStyle)
	{
		super(context, attrs, defStyle);
		init();
		TypedArray a = context.getTheme().obtainStyledAttributes(attrs, R.styleable.PwRoundVolumControlView, defStyle, 0);
		getDisAttrs(a);
	}
	private void getDisAttrs(TypedArray a){
		int n = a.getIndexCount();

		for (int i = 0; i < n; i++)
		{
			int attr = a.getIndex(i);
			switch (attr)
			{
			case R.styleable.PwRoundVolumControlView_VolumForeColor:
				mFirstColor = a.getColor(attr, Color.GREEN);
				break;
			case R.styleable.PwRoundVolumControlView_VolumBgColor:
				mSecondColor = a.getColor(attr, Color.CYAN);
				break;
			case R.styleable.PwRoundVolumControlView_centerIcon:
				mImage = BitmapFactory.decodeResource(getResources(), a.getResourceId(attr, 0));
				break;
				
			case R.styleable.PwRoundVolumControlView_radius:
				mRadius = a.getDimensionPixelSize(attr, 0);
				break;
			case R.styleable.PwRoundVolumControlView_ringWidth:
				mRingWidth= a.getDimensionPixelSize(attr, (int) TypedValue.applyDimension(
						TypedValue.COMPLEX_UNIT_PX, 20, getResources().getDisplayMetrics()));
				break;
			case R.styleable.PwRoundVolumControlView_VolumLevelCount:
				mCount = a.getInt(attr, 20);// 默认20
				break;
			case R.styleable.PwRoundVolumControlView_splitSize:
				mSplitSize = a.getInt(attr, 20);
				break;
			case R.styleable.PwRoundVolumControlView_backcolor:
				mBackgroundColor = a.getColor(attr,0x5fffffff);
				break;
			}
		}
		a.recycle();
		
	}
	private void init(){
		mPaint = new Paint();
		mRect = new Rect();
		mBackPaint=new Paint();
		mRectF=new RectF();
	}
	@Override
	protected void onDraw(Canvas canvas)
	{
		mPaint.setAntiAlias(true); // 消除锯齿
		mPaint.setStrokeWidth(mRingWidth); // 设置圆环的宽度
		mPaint.setStrokeCap(Paint.Cap.ROUND); // 定义线段断电形状为圆头
		mPaint.setAntiAlias(true); // 消除锯齿
		mPaint.setStyle(Paint.Style.STROKE); // 设置空心
		int centre = getWidth() / 2; // 获取圆心的x坐标
		if(mRadius==0){
			mRadius=centre - mRingWidth / 2;
		}
		
		mBackPaint.setStyle(Paint.Style.FILL);
		mBackPaint.setColor(mBackgroundColor);
		mBackPaint.setAntiAlias(true); 
		mRectF.left=0;
		mRectF.top=0;
		mRectF.bottom=getHeight();
		mRectF.right=getWidth();
		
		canvas.drawRoundRect(mRectF, 14.0f, 14.0f, mBackPaint);
		/**
		 * 画块块去
		 */
		drawOval(canvas, centre, mRadius);

		/**
		 * 计算内切正方形的位置
		 */
		int innerRadius = mRadius - mRingWidth / 2;// 获得内圆的半径
		/**
		 * 内切正方形的距离顶部 = mCircleWidth + relRadius - √2 / 2
		 */
		double innerRecWidHalf=Math.sqrt(2) * 1.0f / 2 * innerRadius;
		mRect.left = (int) (centre - innerRecWidHalf);
		/**
		 * 内切正方形的距离左边 = mCircleWidth + relRadius - √2 / 2
		 */
		mRect.top = (int) (centre - innerRecWidHalf);
		mRect.bottom = (int) (mRect.left + innerRecWidHalf*2);
		mRect.right = (int) (mRect.left + innerRecWidHalf*2);

		/**
		 * 如果图片比较小，那么根据图片的尺寸放置到正中心
		 */
		if (mImage.getWidth() < Math.sqrt(2) * innerRadius)
		{
			mRect.left = (int) (mRect.left + innerRecWidHalf - mImage.getWidth() * 1.0f / 2);
			mRect.top = (int) (mRect.top + innerRecWidHalf - mImage.getHeight() * 1.0f / 2);
			mRect.right = (int) (mRect.left + mImage.getWidth());
			mRect.bottom = (int) (mRect.top + mImage.getHeight());

		}
		// 绘图
		canvas.drawBitmap(mImage, null, mRect, mPaint);
	}

	/**
	 * 根据参数画出每个小块
	 * 
	 * @param canvas
	 * @param centre
	 * @param radius
	 */
	private void drawOval(Canvas canvas, int centre, int radius)
	{
		/**
		 * 根据需要画的个数以及间隙计算每个块块所占的比例*360
		 */
		float itemSize = (360 * 1.0f - mCount * mSplitSize) / mCount;

		RectF oval = new RectF(centre - radius, centre - radius, centre + radius, centre + radius); // 用于定义的圆弧的形状和大小的界限

		mPaint.setColor(mSecondColor); // 设置圆环的颜色
		for (int i = 0; i < mCount; i++)
		{
			canvas.drawArc(oval, 90+i * (itemSize + mSplitSize), itemSize, false, mPaint); // 根据进度画圆弧
		}

		mPaint.setColor(mFirstColor); // 设置圆环的颜色
		for (int i = 0; i < mCurrentCount; i++)
		{
			canvas.drawArc(oval, 90+i * (itemSize + mSplitSize), itemSize, false, mPaint); // 根据进度画圆弧
		}
		
	}
	
	public int getMaxLevel(){
		return mCount;
	}
	public void setVolumLevel(int level)
	{
		if(level>mCount){
			mCurrentCount=mCount;
		}else if (level<0) {
			mCurrentCount=0;
		}
		mCurrentCount=level;
		postInvalidate();
	}
	
	/**
	 * 当前数量+1
	 */
	public void up()
	{
		setVolumLevel(++mCurrentCount);
	}

	/**
	 * 当前数量-1
	 */
	public void down()
	{
		setVolumLevel(--mCurrentCount);
	}

//	private int xDown, xUp;
//
//	@Override
//	public boolean onTouchEvent(MotionEvent event)
//	{
//
//		switch (event.getAction())
//		{
//		case MotionEvent.ACTION_DOWN:
//			xDown = (int) event.getY();
//			break;
//
//		case MotionEvent.ACTION_UP:
//			xUp = (int) event.getY();
//			if (xUp > xDown+20)// 下滑
//			{
//				down();
//			} else
//			{
//				up();
//			}
//			break;
//		}
//
//		return true;
//	}
}
