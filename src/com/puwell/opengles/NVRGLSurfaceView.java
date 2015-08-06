package com.puwell.opengles;
import java.util.Timer;
import java.util.TimerTask;

import com.showmo.util.LogUtils;

import ipc365.app.showmo.jni.JniClient;
import android.graphics.Bitmap;
import android.graphics.SurfaceTexture;
import android.support.v4.view.GestureDetectorCompat;
import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.DragEvent;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnTouchListener;


public class NVRGLSurfaceView extends GLSurfaceView implements OnTouchListener {
	public static final String TAG = "NVRGLSurfaceView";
	public int nFingerMaxCount = 2;
	public final Flinger[] m_clFingerList = new Flinger[nFingerMaxCount];
	NVRRender mRenderer;
	int camIndex;
	public static final int AFTEREXPANDPLAYER= 1;
	public static final int EXPANDPLAYER= 2;
	
	public static final int RENDER_H264 = 1;
	public static final int RENDER_BITMAP = 2;
	
	public static final int VIEW_NO_OPERATE = 1;
//	int mPlayer = MainActivity.PLAYER_REAL;
	long lastTime = 0;
	long lastTime1 = 0;
		
	Handler handler = new Handler() {  
        public void handleMessage(Message msg) {  
        	if(msg.what == VIEW_NO_OPERATE)	 {
        		setRenderMode(RENDERMODE_WHEN_DIRTY);
        	}
            //super.handleMessage(msg);  
        };  
    };  
    public void getThumbnail(ICaptureCallback cb){
    	mRenderer.setOnThumbnailListener(cb);
    	mRenderer.setNeedThumbnail(true);
    }
    public void cancelThumbnail(){
    	mRenderer.setNeedCapture(false);
    	mRenderer.setFrameAvailable(false);
    }
    public void capture(ICaptureCallback cb ){
    	mRenderer.setOnCaptureListener(cb);
    	if(!mRenderer.isNeedCapture()){
    		mRenderer.setNeedCapture(true);
    	}else{
    		cb.onProcess();
    	}
    }
    Timer timer = new Timer();  
    TimerTask task = new TimerTask() {  
        @Override  
        public void run() {  
            // 需要做的事:发送消息   
        	if((SystemClock.elapsedRealtime()-lastTime)>2000) {
        		 Message message = new Message();  
                 message.what = VIEW_NO_OPERATE;  
                 handler.sendMessage(message); 
        	} 
        }  
    };  
    
    
    
	public NVRGLSurfaceView(Context context, AttributeSet attrs) {
		super(context, attrs);
		setEGLContextClientVersion(2);
		timer.schedule(task, 2000, 2000); // 1s后执行task,经过1s再次执行   
		detector=new GestureDetectorCompat(getContext(), new GlSimpleGestureLis());
	}
	public Surface getSurface(){
		return mRenderer.getSurface();
	}
	
	public Bitmap getCaptureBitmap(){
		return mRenderer.captureBitmap;
	}
	
	public void init(int Index, int type) {
		int i;
		camIndex = Index;
		if(type == RENDER_H264) {
			mRenderer = new NVRRenderH264(camIndex,1);
		} else {
			mRenderer = new NVRRenderBitmap(camIndex,1);
		}
		setRenderer(mRenderer);
		setRenderMode(RENDERMODE_WHEN_DIRTY);
		for (i = 0; i < nFingerMaxCount; i++) {
			m_clFingerList[i] = new Flinger();
		}
	}
	
	public void InputData(Bitmap data) {
		mRenderer.InputData(data);
	}
	
	public void Render() {
		requestRender();
	}
	
	public void setVisibility(int visibility)
	{
		if(visibility == View.VISIBLE) {
			JniClient.native_mpgl_Stop();	//放在这里保证下次绘制的时候是初始状态
			mRenderer.PrepareDrawable();
		}
//		} else if(visibility == View.INVISIBLE) {
//			JniClient.native_mpgl_Stop();
//		}
		super.setVisibility(visibility);	//必须放在后面，不然会先绘制一次，导致绘制出上一次的图
	}
	
	@Override
	public void onPause() {
		LogUtils.v("NVRGLSurfaceView", "NVRGLSurfaceView onPause");
		setRenderMode(RENDERMODE_WHEN_DIRTY);
		super.onPause();
	}

	@Override
	public void onResume() {
		LogUtils.v("NVRGLSurfaceView", "NVRGLSurfaceView onResume");
		setRenderMode(RENDERMODE_WHEN_DIRTY);
		super.onResume();
		
	}
	
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		//LogUtil.D(TAG, "surfaceCreated");
		super.surfaceCreated(holder);
		//JniNative.InitOpenGL(camIndex, mPlayer);
		//JniClient.native_init_opengl(camIndex);
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		//LogUtil.D(TAG, "surfaceDestroyed");
		super.surfaceDestroyed(holder);
		//JniNative.UninitOpenGL(camIndex, mPlayer);
		//mRenderer.ReleaseCodec();
	}
	
//	public void RegistCustemorTouchListener(TouchEventListener listener){
//		mFullTouchEventListener = listener;
//	}
	
	private GlClickListener m_GlClickListener=null;
	public void setGlClickListener(GlClickListener lis){
		m_GlClickListener=lis;
		
	}
	public interface GlClickListener{
		void OnClick();
		void onDoubleClick();
	}
	private GestureDetectorCompat detector;
	
	private class GlSimpleGestureLis extends GestureDetector.SimpleOnGestureListener{
		@Override
		public boolean onDoubleTap(MotionEvent e) {
			// TODO Auto-generated method stub

			if(m_GlClickListener!=null){
				m_GlClickListener.onDoubleClick();
			}
			return super.onDoubleTap(e);
		}
	}
	private float downPosX;
	private float downPosY;
	private final static float MinPosThresold=15;
	@Override
	public boolean onTouchEvent(final MotionEvent event) {
		//performClick();
		detector.onTouchEvent(event);
		super.onTouchEvent(event);
		int width = this.getWidth();
		int height = this.getHeight();
		int pointCount = event.getPointerCount();// 触控点的个数；
		int action = (event.getAction() & MotionEvent.ACTION_MASK);// 多指和单指合并;
		boolean bAction = false;
		pointCount = Math.min(pointCount, nFingerMaxCount);
		switch (action) {
		case MotionEvent.ACTION_DOWN:
			bAction = true;
			downPosX=event.getX();
			downPosY=event.getY();
			break;
		case MotionEvent.ACTION_UP:
		//	LogUtils.e("touch", "ACTION_UP");
			if(Math.abs(event.getX()-downPosX)<=MinPosThresold && Math.abs(event.getY()-downPosY)<=MinPosThresold){
				if(m_GlClickListener!=null){
				//	LogUtils.e("touch", "m_GlClickListener.OnClick");
					m_GlClickListener.OnClick();
				}
			}
			if(SystemClock.elapsedRealtime()-lastTime1<300){
				//Log.e(TAG, "MainActivity.width:"+MainActivity.width);
//				if(width<MainActivity.width){
//					//while (NVRRenderer.isUpdateTexImage) {
//						Intent intent = new Intent(getContext(), FullScreenActivity.class);
//						Bundle bundle = new Bundle();
//						bundle.putInt("flag", EXPANDPLAYER);
//						bundle.putInt("Index", camIndex);
//						//Log.e(TAG, ""+camIndex);
//						bundle.putInt("Player", MainActivity.PLAYER_REAL);
//						//Log.e(TAG, ""+MainActivity.PLAYER_REAL);
//						intent.putExtras(bundle);
//						getContext().startActivity(intent);
//					//}
//				}else{
////					Intent intent = new Intent(getContext(), MainActivity.class);
////					getContext().startActivity(intent);
//					if(mFullTouchEventListener != null){
//						mFullTouchEventListener.OnTouchEvent(NormalPlayerFragment.GESTURE_DOUBLE_TOUCH, 0, 0, null);
//					}
//				}	
			}
			lastTime1 = SystemClock.elapsedRealtime();
			bAction = false;
			break;
		case MotionEvent.ACTION_MOVE:
			bAction = true;
			break;
		case MotionEvent.ACTION_POINTER_DOWN:
			bAction = true;
			break;
		case MotionEvent.ACTION_POINTER_UP:

			bAction = true;
			break;
		default:
			bAction = false;
			break;
		}
		int[] xList = new int[pointCount];
		int[] yList = new int[pointCount];
		int[] heightList = new int[pointCount];
		int[] widthList = new int[pointCount];
		
		if (bAction) {
//			Log.e(TAG, "action:  " + action + "  pointCount:  " + pointCount);
			xList = new int[pointCount];
			yList = new int[pointCount];
			heightList = new int[pointCount];
			widthList = new int[pointCount];

			for (int i = 0; i < pointCount; i++) {
				float x = event.getX(i);
				float y = event.getY(i);
				xList[i] = (int) x;
				yList[i] = (int) y;
				heightList[i] = width;
				widthList[i] = height;
			}
		} else
			pointCount = 0;

	//	Log.e(TAG, "##### end #####");

		if (SystemClock.elapsedRealtime() - lastTime > 10 || 0 == pointCount ) {
			// JniNative.Instance().native_mpgl_PatternCtrlFingerDown(camIndex,
			// xList, yList, widthList, heightList, pointCount, 0);
			//JniNative.PatternCtrlFingerDown(camIndex, xList, yList, widthList, heightList, pointCount, 0);
			
//			for (int i = 0; i < xList.length; i++) {
//				LogUtils.e("ogl", "FingerDown: "+"index:"+i+"x: "+xList.);
//			}
			setRenderMode(RENDERMODE_CONTINUOUSLY);
			JniClient.native_mpgl_PatternCtrlFingerDown(camIndex, xList, yList, widthList, heightList, pointCount, 0);
			lastTime = SystemClock.elapsedRealtime();
			// if (pointCount == 2)
			// Log.e(TAG, "PointCount " + pointCount + ": "
			// + m_clFlingerList[0].x + "," + m_clFlingerList[0].y
			// + "," + m_clFlingerList[1].x + ","
			// + m_clFlingerList[1].y);
			// else if (pointCount == 1)
			// Log.e(TAG, "PointCount: " + pointCount + ": "
			// + m_clFlingerList[0].x + "," + m_clFlingerList[0].y);
			// else if (pointCount == 0)
			// Log.e(TAG, "PointCount " + pointCount);
		}
		// JniNative.Instance().native_mpgl_PatternCtrlFingerDown( camIndex,
		// m_clFlingerList, pointCount, nTempTime );
		return true;
	}

	public SurfaceTexture getSurfaceTexture() {
		return mRenderer.getSurfaceTexture();
	}

	@Override
	public boolean onDragEvent(DragEvent event) {
		// TODO Auto-generated method stub
		return super.onDragEvent(event);
	}

	@Override
	public boolean onKeyMultiple(int keyCode, int repeatCount, KeyEvent event) {
		// TODO Auto-generated method stub
		return super.onKeyMultiple(keyCode, repeatCount, event);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean performClick() {
		// TODO Auto-generated method stub
		return super.performClick();
	}

	@Override
	public boolean onTouch(final View v, final MotionEvent event) {
		//v.performClick();
		//LogUtil.D(TAG, "onTouch");
		queueEvent(new Runnable() {
			@Override
			public void run() {
				int pointCount = event.getPointerCount();// 触控点的个数；
				int action = (event.getAction());
				switch (action) {
				case MotionEvent.ACTION_DOWN:
					
					break;
				}
			}
		});

		return true;
	}

	public void ClearSurfaceView(){
		//((NVRRenderer) mRenderer).ReleaseCodec();
	}
	
}
