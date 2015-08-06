package com.puwell.opengles;

import ipc365.app.showmo.jni.JniClient;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.R.integer;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.SurfaceTexture;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.os.SystemClock;
import android.util.Log;
import android.view.Surface;

public class NVRRenderBitmap extends NVRRender {
	private final String mVertexShader = "uniform mat4 uMVPMatrix;\n" +
										 "uniform float ufVer2Ori;\n" + 
										 "attribute vec3 aPosition;\n" +
										 "attribute vec3 aPositionOri;\n"+
										 "attribute vec2 aTextureCoord;\n" + 
										 "varying vec2 vTextureCoord;\n" +
										 "void main() {\n" +
										 "  vec3 temp1;\n" +
										 "  vec3 temp2;\n" +
										 "  vec3 temp3;\n"+
										 "  temp1 = aPosition * (1.0-ufVer2Ori);\n" +
										 "  temp2 = aPositionOri * ufVer2Ori;\n" +
										 "  temp3 = temp2 + temp1;\n" + 
										 "  gl_Position = uMVPMatrix * vec4(temp3, 1.0);\n"+
										 "  vTextureCoord = aTextureCoord;\n" +
										 "}\n";

	private final String mFragmentShader = "#extension GL_OES_EGL_image_external : require\n" + 
										   "precision mediump float;\n" + 
										   "varying vec2 vTextureCoord;\n"+
										   "uniform sampler2D sTexture;\n"+
										   //"uniform samplerExternalOES sTexture;\n" + 
										   "void main() {\n" +
										   "  gl_FragColor = texture2D(sTexture, vTextureCoord);\n" +
										   "}\n";
	
	private Surface surface;
	private static final String TAG = "RendererBitmap";
	public boolean m_bDrawable = false;
	
	public void PrepareDrawable()
	{
		m_bDrawable = true;
	}
	
	public void InputData(Bitmap data) {
		m_bitmap = data;
		if(m_bDrawable) {
			Log.v("onFrameAvailable", "onFrameAvailable bitmap");
			JniClient.native_mpgl_Start();
			m_bDrawable = false;
		}
	}
	
	public NVRRenderBitmap(int camIndex, int player) {
		this.camIndex = camIndex;
		m_isSurfaceUpdate = false;
	}

	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
	//	Log.v(TAG, "onSurfaceCreated");
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
		GLES20.glClearColor(0f, 0f, 0f, 1.0f);
		/* Set up shaders and handles to their variables */
		mProgram = createProgram(mVertexShader, mFragmentShader);
		if (mProgram == 0) {
			return;
		}

		maPositionHandle = GLES20.glGetAttribLocation(mProgram, "aPosition");
		checkGlError("glGetAttribLocation aPosition");
		if (maPositionHandle == -1) {
			throw new RuntimeException("Could not get attrib location for aPosition");
		}

		maPositionOriHandle = GLES20.glGetAttribLocation(mProgram, "aPositionOri");
		checkGlError("glGetAttribLocation aPositionOri");
		if (maPositionOriHandle == -1) {
			throw new RuntimeException("Could not get attrib location for aPositionOri");
		}

		maTextureHandle = GLES20.glGetAttribLocation(mProgram, "aTextureCoord");
		checkGlError("glGetAttribLocation aTextureCoord");
		if (maTextureHandle == -1) {
			throw new RuntimeException("Could not get attrib location for aTextureCoord");
		}

		muMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");
		checkGlError("glGetUniformLocation uMVPMatrix");
		if (muMVPMatrixHandle == -1) {
			throw new RuntimeException("Could not get attrib location for uMVPMatrix");
		}

		muVer2OriHandle = GLES20.glGetUniformLocation(mProgram, "ufVer2Ori");
		checkGlError("glGetUniformLocation ufVer2Ori");
		if (muVer2OriHandle == -1) {
			throw new RuntimeException("Could not get attrib location for ufVer2Ori");
		}
		
		//…˙≥…Œ∆¿ÌID
		if (mTextureID < 0) {
			int[] textures = new int[1];
			GLES20.glGenTextures(1, textures, 0);
			mTextureID = textures[0];
		}

		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureID);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER,GLES20.GL_NEAREST);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,GLES20.GL_TEXTURE_MAG_FILTER,GLES20.GL_LINEAR);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,GLES20.GL_CLAMP_TO_EDGE);
			
	}
}
