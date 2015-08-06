package com.showmo.widget;

import com.showmo.R;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.os.Looper;
import android.util.AttributeSet;
import android.util.Log;
import android.util.TypedValue;
import android.view.MotionEvent;
import android.view.View;

public class PwCtrlLightView2 extends View {
public static int COLOR_UPDATE_INTERVAL = 100; //按下后颜色更新的时间间隔(ms)
	
	public static float COLOR_UPDATE_STEP = 0.035f ;  //按下后颜色更新的步数
	
	public static float COLOR_UPDATE_MAX = 4f ; //按下后颜色对比度最大值
	
	public static float COLOR_UPDATE_MIN = 0.5f ;  //按下后颜色对比度最小值 

	public static float COLOR_OFFSET_DEF = 1.2f ; //默认的颜色对比度
	
	//计算公式（COLOR_OFFSET_DEF- COLOR_UPDATE_MIN）/COLOR_UPDATE_STEP
	public static int STEP_COUNT_DEF = 20 ; //默认的步数 
	
	
//	private int stepCount ;

	private Handler mHandler ;

	private UpdateColorRunnable mUpdateColorRunnable ;

	private float mColorOffsetNow   ; //色彩偏移量

	private boolean isColorLight;  //颜色是否变亮

	private boolean mColorUpdateFlag ;

	private float[] mColorMatrix   ;

	private  onColorChangeListener mOnColorChangeListener ;
	private int mLumilance;
	private boolean mInitColorOffset;
	
	private Drawable m_Drawable;
	private Bitmap m_Bitmap;
	
	private float m_xScale;
	private float m_yScale;
	private Paint m_paint;
	public PwCtrlLightView2(Context context){
		this(context,null);
	}
	public PwCtrlLightView2(Context context,AttributeSet attrs){
		this(context,attrs,0);
	}
	public PwCtrlLightView2(Context context,AttributeSet attrs,int style){
		super(context,attrs,style);
		this.init();
		TypedArray ta=context.getTheme().obtainStyledAttributes(attrs,R.styleable.PwCtrlLightView,style,0);
		getAttrs(ta);
	}
	private void init(){
		new Thread(){
			public void run(){
				Looper.prepare();
				mHandler = new Handler(Looper.myLooper());
				Looper.loop();
			}
		}.start();
		mUpdateColorRunnable = new UpdateColorRunnable();
		mInitColorOffset = true ;
		mColorOffsetNow = COLOR_OFFSET_DEF ;
		m_paint=new Paint();
		m_paint.setAntiAlias(true);
		mLumilance=0;
	}
	
	
	public int getmLumilance() {
		return mLumilance;
	}
	public void setmLumilance(int mLumilance) {
		if(this.mLumilance==mLumilance){
			return;
		}
		this.mLumilance = mLumilance;
		if(mOnColorChangeListener!=null){
			mOnColorChangeListener.onColorChange(this.mLumilance);
		}
	}
	private void getAttrs(TypedArray ta){
		m_Drawable=ta.getDrawable(R.styleable.PwCtrlLightView_src);
		int srcwid=ta.getDimensionPixelSize(R.styleable.PwCtrlLightView_src_wid, m_Drawable.getIntrinsicWidth());
		int srchei=ta.getDimensionPixelSize(R.styleable.PwCtrlLightView_src_hei, m_Drawable.getIntrinsicHeight());
		m_xScale=srcwid * 1.0f / m_Drawable.getIntrinsicWidth();
		m_yScale=srchei * 1.0f / m_Drawable.getIntrinsicHeight();
	}
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
			// TODO Auto-generated method stub
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		int widMode=MeasureSpec.getMode(widthMeasureSpec);
		int widSize=MeasureSpec.getSize(widthMeasureSpec);
		int heiMode=MeasureSpec.getMode(heightMeasureSpec);
		int heiSize=MeasureSpec.getSize(heightMeasureSpec);
		if(widMode==MeasureSpec.AT_MOST){
			int marg=(int)TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 20, getResources().getDisplayMetrics());
			widSize=m_Drawable.getIntrinsicWidth()+marg;
			heiSize=m_Drawable.getIntrinsicHeight()+marg;
		}
		setMeasuredDimension(widSize, heiSize);
	}
	
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		Bitmap bitmap=((BitmapDrawable)m_Drawable).getBitmap();
		Matrix matrix = new Matrix(); 
		matrix.postScale(m_xScale,m_yScale); //长和宽放大缩小的比例
		m_Bitmap=Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
		int left=(int)(getMeasuredWidth()/2.0f-m_Bitmap.getWidth()/2.0f);
		int top=(int)(getMeasuredHeight()/2.0f-m_Bitmap.getHeight()/2.0f);
		
		canvas.drawBitmap(m_Bitmap,left,top,m_paint);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		super.onTouchEvent(event);
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			isColorLight = event.getX() < getWidth()/2 ? true :false ;
			if(mHandler!=null)
				mHandler.post(mUpdateColorRunnable );
			mColorUpdateFlag = true ;
			break;
		case MotionEvent.ACTION_UP:
		//	LogUtils.v("ctrl", "MotionEvent.ACTION_UP");
			mColorUpdateFlag = false ;
			break;
		case MotionEvent.ACTION_CANCEL:
			mColorUpdateFlag = false ;
			break;
		default:
			break;
		}
		return true;
	}

	private class UpdateColorRunnable implements  Runnable{
		@Override
		public void run() {
			if(mColorUpdateFlag){
				boolean canUpdate = false  ;
				canUpdate = isColorLight?mColorOffsetNow<=COLOR_UPDATE_MAX:
					mColorOffsetNow  >= COLOR_UPDATE_MIN;
				
				if(canUpdate){
					int stepCount=getmLumilance();
					if(isColorLight){
						mColorOffsetNow = mColorOffsetNow +COLOR_UPDATE_STEP >COLOR_UPDATE_MAX?
								COLOR_UPDATE_MAX:mColorOffsetNow + COLOR_UPDATE_STEP   ;
						stepCount = stepCount < 100 ? stepCount +1: 100   ;
					}else{
						mColorOffsetNow =  mColorOffsetNow -COLOR_UPDATE_STEP <COLOR_UPDATE_MIN?
								COLOR_UPDATE_MIN:mColorOffsetNow - COLOR_UPDATE_STEP  ;
						stepCount = stepCount > 0 ? stepCount -1 : 0   ;
					}
//					LogUtils.v("ctrl light", "setColorFiltersetColorFiltersetColorFilter "+mColorOffsetNow);
					m_paint.setColorFilter(
							new ColorMatrixColorFilter(getFloatArray2(mColorOffsetNow)));
//					getBackground().setColorFilter(
//							new ColorMatrixColorFilter(getFloatArray2(mColorOffsetNow)));
//					setBackgroundDrawable(getBackground());
					postInvalidate();
					setmLumilance(stepCount);
					if(mHandler!=null)
						mHandler.postDelayed(this, COLOR_UPDATE_INTERVAL);
				}
				
				
			}
		}
	}

	public float[] getFloatArray(float colorOffset){
		if(mColorMatrix == null){
			mColorMatrix = new float[20] ;
			for(int i=0;i<20;i++){
				switch (i) {
				case 0:
				case 6:
				case 12:
				case 18:
					mColorMatrix[i] = 1 ;
					break;
				default:
					mColorMatrix[i]=0;
					break;
				}
			}
		}

		mColorMatrix[4] = colorOffset ;
		mColorMatrix[9] = colorOffset ;
		mColorMatrix[14] = colorOffset ;

		return mColorMatrix;

	}
	
	float lumR = 0.3086f;
	float lumG = 0.6094f;
	float lumB = 0.0820f;
	float lum = 0 ;
	
	public float[] getFloatArray2(float colorOffset){
		if(mColorMatrix == null){
			mColorMatrix = new float[25] ;
			for(int i=0;i<20;i++){
				switch (i) {
				case 0:
				case 6:
				case 12:
				case 18:
				case 24:
					mColorMatrix[i] = 1 ;
					break;
				default:
					mColorMatrix[i]=0;
					break;
				}
			}
		}
		
		//饱和度
//		float s = colorOffset ;
//		float sr = (1-s)*lumR ;
//		float sg = (1-s)*lumG ;
//		float sb = (1-s)*lumB ;
//		
//		mColorMatrix[0] = sr + s;
//		mColorMatrix[1] = sr  ;
//		mColorMatrix[2] = sr  ;
//		
//		mColorMatrix[5] = sg;
//		mColorMatrix[6] = sg+s  ;
//		mColorMatrix[7] = sg  ;
//		
//		mColorMatrix[10] = sb;
//		mColorMatrix[11] = sb  ;
//		mColorMatrix[12] = sb +s  ;
		
//		mColorMatrix[0] = colorOffset ;
//		mColorMatrix[6] = colorOffset ;
//		mColorMatrix[12] = colorOffset ;
		
		//对比度
		float c = colorOffset ;
		float t = (1 - c )/2 ;
		
		mColorMatrix[0] = c ;
		mColorMatrix[6] = c ;
		mColorMatrix[12] =c  ;
		
		mColorMatrix[20] = t ;
		mColorMatrix[21] = t ;
		mColorMatrix[22] = t  ;
		
		
		mColorMatrix[20] += (colorOffset - COLOR_UPDATE_MIN) *30 ;
		mColorMatrix[21] += (colorOffset - COLOR_UPDATE_MIN) *30 ;
		mColorMatrix[22] +=(colorOffset - COLOR_UPDATE_MIN) *30 ;
		
		return mColorMatrix;

	}

	public interface onColorChangeListener{
		void onColorChange(int luminance);
	}

	public onColorChangeListener getOnColorChangeListener() {
		return mOnColorChangeListener;
	}


	public void setOnColorChangeListener(
			onColorChangeListener mOnColorChangeListener) {
		this.mOnColorChangeListener = mOnColorChangeListener;
	}
	/**
	 * 根据参数改变图片颜色
	 * @param step 范围 0 - 100 当>100 作为100处理 当<0 作为0处理
	 */
	public void setColor(int step){
		if(step>100 ){
			step = 100 ;
		}else if(step < 0){
			step = 0 ;
		}
		mLumilance = step ;
		mColorOffsetNow = COLOR_UPDATE_MIN + COLOR_UPDATE_STEP * step ;
		postInvalidate();
	}
}
