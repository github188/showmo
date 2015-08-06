package com.puwell.opengles;

import ipc365.app.showmo.jni.JniClient;

import com.showmo.VideoMenubarFragment;
import com.showmo.util.LogUtils;
import com.showmo.util.PwTimer;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

import javax.mail.internet.NewsAddress;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.R.integer;
import android.graphics.Bitmap;
import android.graphics.SurfaceTexture;
import android.graphics.Bitmap.CompressFormat;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.os.AsyncTask;
import android.os.Environment;
import android.os.SystemClock;
import android.util.Log;
import android.view.Surface;
import android.view.TextureView.SurfaceTextureListener;

public class NVRRender implements GLSurfaceView.Renderer{
	private static final int FLOAT_SIZE_BYTES = 4;
	private static final int VERTICES_DATA_STRIDE_BYTES = 3 * FLOAT_SIZE_BYTES;
	private static final int TEXTURES_DATA_STRIDE_BYTES = 2 * FLOAT_SIZE_BYTES;
	private static final int VERTICES_DATA_POS_OFFSET = 0;
	private static final int PATTERN_CREATE = 1;// 第一次调用的时候传入
	private static final int PATTERN_CHANGE = 2;// 窗口发生变化的时候传入
	private static final int PATTERN_NOCHANGE = 3;// 窗口没有任何变化的时候传入
	
	
	private int m_nScreenWidth = 0;
	private int m_nScreenHeight = 0;
	public NVRPattern pattern = null;
	private boolean m_bGetVertex = false;

	private float[] mMVPMatrix = new float[16];
	private float[] mProjMatrix = new float[16];
	private float[] mMMatrix = new float[16];
	private float[] m_fCamMatrix = new float[16]; // Matrix of camera
	private float[] m_fWorldMatrix = new float[16]; // Matrix of world change
	private float[] m_fGroupMatrix = new float[16]; // Matrix of group change
	private int m_nVerSize = 0;
	private int m_nCoorSize = 0;
	private FloatBuffer m_cVerList;
	private FloatBuffer m_cVerOriList;
	private FloatBuffer m_cCoorList;
	private FloatBuffer m_cVerBoardList;
	private FloatBuffer m_cCoorBoardList;
	
	private static final String TAG = "MyRenderer";
	
	public int mProgram = 0;
	public int mTextureID = -1;
	public int muVer2OriHandle = -1;
	public int muMVPMatrixHandle = -1;
	public int maPositionHandle = -1;
	public int maPositionOriHandle = -1;
	public int maTextureHandle = -1;
	public int camIndex;
	public SurfaceTexture mSurface;
	
	public boolean m_isSurfaceUpdate = false;
	public static final int GL_TEXTURE_EXTERNAL_OES = 0x8D65;
	public Bitmap m_bitmap;
	public Bitmap captureBitmap;
	private boolean bNeedCapture=false;
	private boolean bFrameAvailable=false;
	private boolean bNeedThumbnail=false;
	
	private ICaptureCallback mCaptureListener=null;
	private ICaptureCallback mThumbnailListener=null;
	private Object mCaptureLock=new Object();

	
	public boolean isNeedCapture() {
		return bNeedCapture;
	}
	protected void setFrameAvailable(boolean bavailable){
		synchronized(mCaptureLock){
			bFrameAvailable=bavailable;
		}
	}
	
	private PwTimer mCaptureTimer=new PwTimer(false) {
		
		@Override
		public void doInTask() {
			// TODO Auto-generated method stub
			if (isNeedCapture()) {
				LogUtils.e("capture", "TimerTask "+SystemClock.elapsedRealtime()+" hascode "+hashCode());
				synchronized(mCaptureLock){
					if(mCaptureListener!=null){
						LogUtils.e("capture","onFailured");
						mCaptureListener.onFailured();
						setOnCaptureListener(null);
					}
				}
				setNeedCapture(false);
				LogUtils.e("capture","TimerTask over");
			}
		}
	};
	public synchronized void setNeedCapture(boolean bNeedCapture) {
		this.bNeedCapture = bNeedCapture;
		LogUtils.e("capture", "setNeedCapture "+bNeedCapture);
		if(bNeedCapture){
			mCaptureTimer.start(2000, false);
			//pt.start(20, false);
		}
	}
	public void setOnCaptureListener(ICaptureCallback cb){
		synchronized(mCaptureLock){
			mCaptureListener=cb;
		}
	}
	public synchronized void setNeedThumbnail(boolean bNeedThumbnail){
		this.bNeedThumbnail=bNeedThumbnail;
	}
	public void setOnThumbnailListener(ICaptureCallback cb){
		synchronized(mCaptureLock){
			mThumbnailListener=cb;
		}
	}
	
	
	public NVRRender() {
	}
	
	public SurfaceTexture getSurfaceTexture() {
		return null;
	}
	
	public Surface getSurface(){
		return null;
	}
	
	public Bitmap getCaptureBitmap(){
		return captureBitmap;
	}
	
	public void InputData(Bitmap data) {
	}
	
	public void checkGlError(String op) {
		int error;
		while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
		///	Log.e(TAG, op + ": glError " + error);
			throw new RuntimeException(op + ": glError " + error);
		}
	}
	
	public boolean _gl_MoveCamera(CameraPosture cCamPosture) {
		boolean bRes = false;
		float fScale, fWid_to_Hei, fScaleJudge;
		double dRadius;

		if (m_nScreenWidth <= 0 || m_nScreenHeight <= 0)
			return bRes;

		fWid_to_Hei = 1.0f * m_nScreenHeight / m_nScreenWidth;

		dRadius = cCamPosture.fViewAngle * 0.5 * 0.01745329; // 0.01745329 //
																// Degree to PI

		fScale = (float) (cCamPosture.fMinDistance * Math.tan(dRadius));
		fScaleJudge = fScale * fWid_to_Hei;

		GLES20.glViewport(0, 0, m_nScreenWidth, m_nScreenHeight); // 设置视口
		Matrix.frustumM(mProjMatrix, 0, -fScale, fScale, -fScaleJudge, fScaleJudge, cCamPosture.fMinDistance, cCamPosture.fMaxDistance);

		Matrix.setIdentityM(m_fCamMatrix, 0);
		Matrix.rotateM(m_fCamMatrix, 0, cCamPosture.fTilt, 1.0f, 0.0f, 0.0f);// 绕x轴旋转
		Matrix.rotateM(m_fCamMatrix, 0, cCamPosture.fPan, 0.0f, 0.0f, 1.0f);// 绕z轴旋转
		Matrix.rotateM(m_fCamMatrix, 0, cCamPosture.fRotate, 0.0f, 1.0f, 0.0f);// 绕y轴旋转
		Matrix.translateM(m_fCamMatrix, 0, cCamPosture.fPosiX, cCamPosture.fPosiY, cCamPosture.fPosiZ);// 反向平移

		bRes = true;
		return bRes;
	}

	public boolean _gl_MoveWorld(Posture cWorldState) {
		boolean bRes = false;
		Matrix.setIdentityM(m_fWorldMatrix, 0);
		if (false != cWorldState.bNeedRotate || false != cWorldState.bNeedTranslate) {
			if (false != cWorldState.bNeedTranslate)
				Matrix.translateM(m_fWorldMatrix, 0, cWorldState.fTranslate_X, cWorldState.fTranslate_Y, cWorldState.fTranslate_Z);// 反向平移

			if (false != cWorldState.bNeedRotate) {
				Matrix.rotateM(m_fWorldMatrix, 0, cWorldState.fRotate_X, 1.0f, 0.0f, 0.0f);// 绕x轴旋转
				Matrix.rotateM(m_fWorldMatrix, 0, cWorldState.fRotate_Y, 0.0f, 1.0f, 0.0f);// 绕y轴旋转
				Matrix.rotateM(m_fWorldMatrix, 0, cWorldState.fRotate_Z, 0.0f, 0.0f, 1.0f);// 绕z轴旋转
			}
			if (false != cWorldState.bNeedScale)
				Matrix.scaleM(m_fWorldMatrix, 0, cWorldState.fScale_X, cWorldState.fScale_Y, cWorldState.fScale_Z);

			bRes = true;
		}
		return bRes;
	}

	public boolean _gl_MoveGroup(Posture cGroupState) {
		boolean bRes = false;
		Matrix.setIdentityM(m_fGroupMatrix, 0);
		if (false != cGroupState.bNeedRotate || false != cGroupState.bNeedTranslate) {
			if (false != cGroupState.bNeedTranslate)
				Matrix.translateM(m_fGroupMatrix, 0, cGroupState.fTranslate_X, cGroupState.fTranslate_Y, cGroupState.fTranslate[2]);// 反向平移
			if (false != cGroupState.bNeedRotate) {
				Matrix.rotateM(m_fGroupMatrix, 0, cGroupState.fRotate_X, 1.0f, 0.0f, 0.0f);// 绕x轴旋转
				Matrix.rotateM(m_fGroupMatrix, 0, cGroupState.fRotate_Y, 0.0f, 1.0f, 0.0f);// 绕y轴旋转
				Matrix.rotateM(m_fGroupMatrix, 0, cGroupState.fRotate_Z, 0.0f, 0.0f, 1.0f);// 绕z轴旋转
			}
			if (false != cGroupState.bNeedScale)
				Matrix.scaleM(m_fGroupMatrix, 0, cGroupState.fScale_X, cGroupState.fScale_Y, cGroupState.fScale_Z);
			bRes = true;
		}
		return bRes;
	}
	
	public boolean _gl_Move_2(NVRPattern cPattern) {
		boolean bRes = false;
		int i;
		for (i = 9; i >0 ; i--) {
			if( i== cPattern.m_plBoardRotatePri_3[0] )
				Matrix.rotateM(m_fWorldMatrix, 0, cPattern.m_fBoardRotate_3[0], 1.0f, 0.0f, 0.0f);
			if( i== cPattern.m_plBoardRotatePri_3[1] )
				Matrix.rotateM(m_fWorldMatrix, 0, cPattern.m_fBoardRotate_3[1], 0.0f, 1.0f, 0.0f);
			if( i== cPattern.m_plBoardRotatePri_3[2] )
				Matrix.rotateM(m_fWorldMatrix, 0, cPattern.m_fBoardRotate_3[2], 0.0f, 0.0f, 1.0f);
			
			if( i== cPattern.m_plBoardTransPri_3[0] )
				Matrix.translateM(m_fWorldMatrix, 0, cPattern.m_fBoardTrans_3[0], 0.0f, 0.0f);
			if( i== cPattern.m_plBoardTransPri_3[1] )
				Matrix.translateM(m_fWorldMatrix, 0, 0.0f, cPattern.m_fBoardTrans_3[1], 0.0f);
			if( i== cPattern.m_plBoardTransPri_3[2] )
				Matrix.translateM(m_fWorldMatrix, 0, 0.0f, 0.0f, cPattern.m_fBoardTrans_3[2]);
		
			if( i== cPattern.m_plBoardScalePri_3[0] )
				Matrix.translateM(m_fWorldMatrix, 0, cPattern.m_fBoardScale_3[0], 0.0f, 0.0f);
			if( i== cPattern.m_plBoardScalePri_3[1] )
				Matrix.translateM(m_fWorldMatrix, 0, 0.0f, cPattern.m_fBoardScale_3[1], 0.0f);
			if( i== cPattern.m_plBoardScalePri_3[2] )
				Matrix.translateM(m_fWorldMatrix, 0, 0.0f, 0.0f, cPattern.m_fBoardScale_3[2]);
		}
		bRes = true;

		return bRes;
	}

	void _gl_DrawBoard(NVRPattern cPattern) {
		int nMemberNo, nDrawType, nSize;
		boolean bDrawTexture = false;
		boolean bCullFace = false;
		float fScale;
		int nMemberOffset, nVertexOffset;
		// -- Group have texture --
		// if( strlen( pstPatternGroup->stCfgTexture.pstrTextureName ) != 0 )
		bDrawTexture = true;

	

		GLES20.glVertexAttribPointer(maPositionHandle, 3, GLES20.GL_FLOAT, false, VERTICES_DATA_STRIDE_BYTES, m_cVerBoardList);
		checkGlError("glVertexAttribPointer maPosition");
		GLES20.glEnableVertexAttribArray(maPositionHandle);
		checkGlError("glEnableVertexAttribArray maPositionHandle");

		GLES20.glVertexAttribPointer(maPositionOriHandle, 3, GLES20.GL_FLOAT, false, VERTICES_DATA_STRIDE_BYTES, m_cVerBoardList);
		checkGlError("glVertexAttribPointer maPositionOri");
		GLES20.glEnableVertexAttribArray(maPositionOriHandle);
		checkGlError("glEnableVertexAttribArray maPositionOriHandle");

		GLES20.glVertexAttribPointer(maTextureHandle, 2, GLES20.GL_FLOAT, false, TEXTURES_DATA_STRIDE_BYTES, m_cCoorBoardList);
		checkGlError("glVertexAttribPointer maTextureHandle");
		GLES20.glEnableVertexAttribArray(maTextureHandle);
		checkGlError("glEnableVertexAttribArray maTextureHandle");
		
		Matrix.setIdentityM(m_fWorldMatrix, 0);
		Matrix.setIdentityM(mMMatrix, 0);
		_gl_Move_2( cPattern );
		// Matrix.multiplyMM( mMMatrix, 0, m_fGroupMatrix, 0, mMMatrix, 0 );
		Matrix.multiplyMM(mMMatrix, 0, m_fWorldMatrix, 0, mMMatrix, 0);
		Matrix.multiplyMM(mMMatrix, 0, m_fCamMatrix, 0, mMMatrix, 0);		
		Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMMatrix, 0);

		GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);
		GLES20.glUniform1f(muVer2OriHandle, 1.0f);
		if(m_isSurfaceUpdate) {
			GLES20.glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureID);
		} else {
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureID);
		}
	
		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 6);

	}
	
	void _gl_DrawGroup(NVRPattern cPattern, int nGroupNo) {
		int nMemberNo, nDrawType, nSize;
		boolean bDrawTexture = false;
		boolean bCullFace = false;
		float fScale;
		int nMemberOffset, nVertexOffset;
		// -- Group have texture --
		// if( strlen( pstPatternGroup->stCfgTexture.pstrTextureName ) != 0 )
		bDrawTexture = true;

		nMemberOffset = cPattern.m_nGroupMemOffSetList[nGroupNo];

		GLES20.glVertexAttribPointer(maPositionHandle, 3, GLES20.GL_FLOAT, false, VERTICES_DATA_STRIDE_BYTES, m_cVerList);
		checkGlError("glVertexAttribPointer maPosition");
		GLES20.glEnableVertexAttribArray(maPositionHandle);
		checkGlError("glEnableVertexAttribArray maPositionHandle");

		GLES20.glVertexAttribPointer(maPositionOriHandle, 3, GLES20.GL_FLOAT, false, VERTICES_DATA_STRIDE_BYTES, m_cVerOriList);
		checkGlError("glVertexAttribPointer maPositionOri");
		GLES20.glEnableVertexAttribArray(maPositionOriHandle);
		checkGlError("glEnableVertexAttribArray maPositionOriHandle");

		GLES20.glVertexAttribPointer(maTextureHandle, 2, GLES20.GL_FLOAT, false, TEXTURES_DATA_STRIDE_BYTES, m_cCoorList);
		checkGlError("glVertexAttribPointer maTextureHandle");
		GLES20.glEnableVertexAttribArray(maTextureHandle);
		checkGlError("glEnableVertexAttribArray maTextureHandle");

		nSize = cPattern.m_nVertexTotal;

		Matrix.setIdentityM(mMMatrix, 0);
		// Matrix.multiplyMM( mMMatrix, 0, m_fGroupMatrix, 0, mMMatrix, 0 );
		Matrix.multiplyMM(mMMatrix, 0, m_fWorldMatrix, 0, mMMatrix, 0);
		Matrix.multiplyMM(mMMatrix, 0, m_fCamMatrix, 0, mMMatrix, 0);		
		Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMMatrix, 0);

		GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);

		fScale = cPattern.m_fGroupVer2OriList[nGroupNo];
		//Log.i(TAG, "fScale:"+fScale);
		GLES20.glUniform1f(muVer2OriHandle, fScale);

		if(m_isSurfaceUpdate) {
			GLES20.glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureID);
		} else {
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureID);
		}
	
		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, nSize);

	}
	
	void _gl_Draw(NVRPattern cPattern) {
		int nGroupNo, nVexSize, nTexSize;
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
//		if(m_isSurfaceUpdate)
			GLES20.glClearColor(0, 0, 0, 1);
//		else
//			GLES20.glClearColor(cPattern.m_pfBackColor[0], cPattern.m_pfBackColor[1], cPattern.m_pfBackColor[2], cPattern.m_pfBackColor[3]);

		//GLES20.glClearColor(0, 0, 0, 1);
		
		
		GLES20.glUseProgram(mProgram);
		checkGlError("glUseProgram");
		GLES20.glDisable(GLES20.GL_CULL_FACE);
		
		// -- Move camera --
		_gl_MoveCamera(cPattern.m_clCamState);

		// -- Need to move whole world --
		_gl_MoveWorld(cPattern.m_clWorldPosture);

		
		for (nGroupNo = 0; nGroupNo < cPattern.m_nGroupCount; nGroupNo++) {
			// _gl_MoveGroup( pattern.m_pclGroupPostureList[nGroupNo] );
			
			if( cPattern.m_plGroupPriorityList[nGroupNo] > 0 ) {
				_gl_DrawGroup(pattern, nGroupNo); // nGLTextureID 在此APP中无用
			}
				
		}
		m_cVerBoardList.put(cPattern.m_pfBoardVertexList_3, 0, 6 * 3).position(0);				
		m_cCoorBoardList.put(cPattern.m_pfBoardTexCordList_2, 0, 6 * 2).position(0);
		 _gl_DrawBoard( cPattern );
	}
	
	@Override
	public void onDrawFrame(GL10 gl) {
		// TODO Auto-generated method stub
//		if(!m_bDrawable)
//			return;
		
		int ret, type;

		if (false == m_bGetVertex) {
			type = PATTERN_CREATE;
			//ret = JniNative.GetPatternWithTime(PATTERN_CREATE, this, camIndex, 0, m_nScreenHeight, m_nScreenWidth);
			ret =JniClient.native_mpgl_GetPatternWithTime(PATTERN_CREATE, this, camIndex, 0, m_nScreenHeight, m_nScreenWidth);
			if (1 == ret) {
				m_bGetVertex = true;
			}
		} else {
			type = PATTERN_CHANGE;
			//ret = JniNative.GetPatternWithTime(PATTERN_CHANGE, this, camIndex, 0, m_nScreenHeight, m_nScreenWidth);
			ret =JniClient.native_mpgl_GetPatternWithTime(PATTERN_CHANGE, this, camIndex, 0, m_nScreenHeight, m_nScreenWidth);
		}
		if(ret==0){
			return;
		}

		if(m_isSurfaceUpdate)
			mSurface.updateTexImage();
		else {
			//实际加载纹理
			if(m_bitmap != null) {
				GLUtils.texImage2D (
						GLES20.GL_TEXTURE_2D,   //纹理类型，在OpenGL ES中必须为GL10.GL_TEXTURE_2D
						0, 					  //纹理的层次，0表示基本图像层，可以理解为直接贴图
						m_bitmap, 			  	  //纹理图像
						0					  //纹理边框尺寸
				);
			}	
		}

		if (pattern != null) {
			if (PATTERN_CREATE == type) {
				int nSize;
				m_nVerSize = pattern.m_pfVertexList_3.length;
				m_nCoorSize = pattern.m_pfTexCordList_2.length;
				m_cVerList = ByteBuffer.allocateDirect(m_nVerSize * FLOAT_SIZE_BYTES).order(ByteOrder.nativeOrder()).asFloatBuffer();
				m_cVerOriList = ByteBuffer.allocateDirect(m_nVerSize * FLOAT_SIZE_BYTES).order(ByteOrder.nativeOrder()).asFloatBuffer();
				m_cCoorList = ByteBuffer.allocateDirect(m_nCoorSize * FLOAT_SIZE_BYTES).order(ByteOrder.nativeOrder()).asFloatBuffer();
				m_cVerBoardList = ByteBuffer.allocateDirect(18 * FLOAT_SIZE_BYTES).order(ByteOrder.nativeOrder()).asFloatBuffer();
				m_cCoorBoardList = ByteBuffer.allocateDirect(12 * FLOAT_SIZE_BYTES).order(ByteOrder.nativeOrder()).asFloatBuffer();
				nSize = pattern.m_nVertexTotal;
				m_cVerList.put(pattern.m_pfVertexList_3, 0, nSize * 3).position(0);
				m_cVerOriList.put(pattern.m_pfVertexOriList_3, 0, nSize * 3).position(0);
				m_cCoorList.put(pattern.m_pfTexCordList_2, 0, nSize * 2).position(0);
			}

			_gl_Draw(pattern);
			
			if(isNeedCapture()){
				LogUtils.e("capture", "ondrawframe is needcapture");
				synchronized(mCaptureLock){
					if(mCaptureListener!=null){
						long capbegin= SystemClock.elapsedRealtime();
						captureBitmap=SavePixels(0,0,m_nScreenWidth,m_nScreenHeight, gl);
						LogUtils.e("capture", "SavePixels usetime:"+(SystemClock.elapsedRealtime()-capbegin));
						if(captureBitmap!=null){
							long capoversuc= SystemClock.elapsedRealtime();
							mCaptureListener.onSuccess(captureBitmap);
							LogUtils.e("capture", "SavePixels usetime:"+(SystemClock.elapsedRealtime()-capoversuc));
						}else {
							LogUtils.i("capture", "onFailured captureBitmap");
							mCaptureListener.onFailured();
						};
						mCaptureTimer.stopIfStarted();
						setNeedCapture(false);
					}
				}
			}
			if(bNeedThumbnail && bFrameAvailable){
				LogUtils.i("capture", "SavePixels");
				captureBitmap=SavePixels(0,0,m_nScreenWidth,m_nScreenHeight, gl);
				if(mThumbnailListener!=null){
					mThumbnailListener.onSuccess(captureBitmap);
					setNeedThumbnail(false);
					setFrameAvailable(false);
				}
			}
			
			
			
			//LogUtils.e("capture", "onDrawFrame "+isNeedCapture()+" hascode "+hashCode());
			
			//return;
		}
	
	}

	@Override 
	public void onSurfaceChanged(GL10 arg0, int width, int height) {
		// TODO Auto-generated method stub
		m_nScreenWidth = width;
		m_nScreenHeight = height;
	}

	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
		// TODO Auto-generated method stub
		//Log.v(tag, msg);
		
	}
	
	public static Bitmap SavePixels(int x, int y, int w, int h, GL10 gl) {
		int b[] = new int[w * h];
		int bt[] = new int[w * h];
		IntBuffer ib = IntBuffer.wrap(b);
		ib.position(0);
		//GLES20.glReadPixels(x, y, w, h, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, ib);
		gl.glReadPixels(x, y, w, h, GL10.GL_RGBA, GL10.GL_UNSIGNED_BYTE, ib);

		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				int pix = b[i * w + j];
				int pb = (pix >> 16) & 0xff;
				int pr = (pix << 16) & 0x00ff0000;
				int pix1 = (pix & 0xff00ff00) | pr | pb;
				bt[(h - i - 1) * w + j] = pix1;
			}
		}
		Bitmap sb = Bitmap.createBitmap(bt, w, h, Bitmap.Config.ARGB_8888);
		return sb;
	}

	public int loadShader(int shaderType, String source) {
		int shader = GLES20.glCreateShader(shaderType);
		if (shader != 0) {
			GLES20.glShaderSource(shader, source);
			GLES20.glCompileShader(shader);
			int[] compiled = new int[1];
			GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
			if (compiled[0] == 0) {
			//	Log.e(TAG, "Could not compile shader " + shaderType + ":");
			//	Log.e(TAG, GLES20.glGetShaderInfoLog(shader));
				GLES20.glDeleteShader(shader);
				shader = 0;
			}
		}
		return shader;
	}

	public int createProgram(String vertexSource, String fragmentSource) {
		int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
		if (vertexShader == 0) {
			return 0;
		}
		int pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
		if (pixelShader == 0) {
			return 0;
		}

		int program = GLES20.glCreateProgram();
		if (program != 0) {
			GLES20.glAttachShader(program, vertexShader);
			checkGlError("glAttachShader");
			GLES20.glAttachShader(program, pixelShader);
			checkGlError("glAttachShader");
			GLES20.glLinkProgram(program);
			int[] linkStatus = new int[1];
			GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
			if (linkStatus[0] != GLES20.GL_TRUE) {
			//	Log.e(TAG, "Could not link program: ");
				//Log.e(TAG, GLES20.glGetProgramInfoLog(program));
				GLES20.glDeleteProgram(program);
				program = 0;
			}
		}
		return program;
	}
	
	public void PrepareDrawable()
	{
		
	}
}
