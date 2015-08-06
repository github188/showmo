package com.showmo.playHelper;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef.FrameSize;

import java.nio.ByteBuffer;
import java.util.Vector;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.http.util.ByteArrayBuffer;

//import com.example.decodetest.DecodeThread;
//import com.example.decodetest.DecodeThread.DataFrame;








import com.showmo.util.LogUtils;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.media.MediaCodec.BufferInfo;
import android.util.Log;
import android.view.Surface;

public class DecodeThread{
	private static final String TAG = "DecodeTest";
	Surface m_surface;
	private int STREAM_TYPE_IFRAME = 0;
	private MediaCodec decoder = null;
	private int m_iLastWid = 0;
	private int m_iLastHei = 0;
	private Lock m_decodeLock;
	public class DataFrame{
		public byte[] byteArr;
		public long  size;
	}
	public Vector<DataFrame> frameVec;
	public boolean m_bStop;
	private boolean m_bPause=false;
	
	public DecodeThread(Surface surface) {
		m_surface = surface;
		frameVec=new Vector<DecodeThread.DataFrame>();
		frameVec.clear();
		m_bStop=false;
		m_decodeLock=new ReentrantLock();
	}
	public void setStop(boolean flag){
		m_bStop=flag;
	}
	public void setPause(boolean pause){
		this.m_bPause=pause;
	}
	public boolean getPause(){
		return this.m_bPause;
	}
	
	public boolean getH264Nal(byte[] pBuf, int nSize, ByteArrayBuffer pSPS, ByteArrayBuffer pPPS)
	{
	    int nSPSIndex = -1, nPPSIndex = -1, nSEIIndex = -1, nIFrameIndex = -1;
	    int i=0, lCheckLen;

	    lCheckLen = (nSize-5)<200?(nSize-5):200;

	
	    int lTemp, lVal, lTT;
	    for (; i<lCheckLen; i++)
	    {
	        lTemp = pBuf[i];	lVal = lTemp;
	        lTemp = pBuf[i+1];	lVal += lTemp<<8;
	        lTemp = pBuf[i+2];	lVal += lTemp<<16;
	        lTemp = pBuf[i+3];	lVal += lTemp<<24;
	        lTT = pBuf[i+4];

	        if(lVal==0x01000000 && 0x67 == lTT)
	        {
	            nSPSIndex = i;
	        }

	        if(lVal==0x01000000 && 0x68 == lTT)
	        {
	            nPPSIndex = i;
	        }

	        if(lVal==0x01000000 && 0x06 == lTT)   //第二个SEI会把第一个的索引信息覆盖
	        {
	            nSEIIndex = i;
	        }
	        if(lVal==0x01000000 && 0x65 == lTT)   
	        {
	        	nIFrameIndex = i;
	        }
	    }
	   
	    if(nSPSIndex == -1 || nPPSIndex == -1 || nSEIIndex == -1 || nIFrameIndex == -1)
	        return false;

//	    pSPS = new ByteArrayBuffer(nPPSIndex-nSPSIndex);
//	    pPPS = new ByteArrayBuffer(nSEIIndex-nPPSIndex);
	    for(i=0; i<(nPPSIndex-nSPSIndex); i++) {
	    	// LogUtils.v("index","over "+pBuf[nSPSIndex+i]);
	    	pSPS.append(pBuf[nSPSIndex+i]);
	    }
	    //LogUtils.v("index","over1 ");
	    for(i=0; i<(nIFrameIndex-nPPSIndex); i++) {
	    	//LogUtils.v("index","over__ "+pBuf[nPPSIndex+i]);
	    	pPPS.append(pBuf[nPPSIndex+i]);
	    }
	   // LogUtils.v("index","over2 ");
	    //LogUtils.v(TAG, "pSPS:"+pSPS.length()+" pPPS:"+pPPS.length());
	    return true;
	}



	int getH264IFrameIndex(byte[] pBuf, int nSize)
	{
	    int nIFrameIndex = -1;
	    int i=0, lCheckLen;

	    lCheckLen = (nSize-5)<200?(nSize-5):200;

	    int lTemp, lVal, lTT;
	    for (; i<lCheckLen; i++)
	    {
	        lTemp = pBuf[i];	lVal = lTemp;
	        lTemp = pBuf[i+1];	lVal += lTemp<<8;
	        lTemp = pBuf[i+2];	lVal += lTemp<<16;
	        lTemp = pBuf[i+3];	lVal += lTemp<<24;
	        lTT = pBuf[i+4];

	        if(lVal==0x01000000 && 0x65 == lTT)
	        {
	            nIFrameIndex = i;
	            break;
	        }
	    }

	    return nIFrameIndex;
	}

	public void stop_decode()
	{
		m_decodeLock.lock();
		if(decoder != null) {
			decoder.flush();
			decoder.stop();
			decoder.release();
			decoder = null;
		}
		m_decodeLock.unlock();
		m_iLastWid = 0;
		m_iLastHei = 0;
	}
	
	
	public void decode(byte[] pSrc, int iCount, int iStreamType)
	{
		long timeoutUs = 100000;
		int byteCount = 0;
		boolean bStop = false;
		if(m_bPause){
			return;
		}
		if(iStreamType == STREAM_TYPE_IFRAME) {
			FrameSize size = JniClient.PW_NET_GetFrameSize(pSrc, iCount);
			if((m_iLastWid!=0) && (m_iLastHei!=0) && ((m_iLastWid!=size.nWidth) || (m_iLastHei!=size.nHeight))) {
				stop_decode();
			}
		}
		
		if(decoder==null && iStreamType == STREAM_TYPE_IFRAME) {
			ByteArrayBuffer sps = null,pps=null;
	        sps = new ByteArrayBuffer(50);
	        pps = new ByteArrayBuffer(50);
	        getH264Nal(pSrc, iCount, sps, pps);
	        
	        FrameSize size = JniClient.PW_NET_GetFrameSize(pSrc, iCount);
	        m_iLastWid = size.nWidth;
	        m_iLastHei = size.nHeight;
	       // LogUtils.v(TAG, "java PW_NET_GetFrameSize"+size.nWidth);
	        //解析宽高
			MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", size.nWidth, size.nHeight);
			//LogUtils.v(TAG, "##################decode 0");
			mediaFormat.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE, size.nWidth * size.nHeight);
			mediaFormat.setByteBuffer("csd-0", ByteBuffer.wrap(sps.buffer()));
			mediaFormat.setByteBuffer("csd-1", ByteBuffer.wrap(pps.buffer()));
			//LogUtils.v(TAG, "##################decode 0-1");
			if(mediaFormat == null){
				LogUtils.v(TAG, "createVideoFormat fail!");
				return;
			}
			//LogUtils.v(TAG, "##################decode 0-2");
			String mime = mediaFormat.getString(MediaFormat.KEY_MIME);
			//LogUtils.v(TAG, "##################decode 0-3"+mime);
			try {
				decoder = MediaCodec.createDecoderByType(mime);
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
			
			if (decoder == null) {
				Log.e(TAG, "createDecoderByType fail!");
				return;
			}

			//LogUtils.v(TAG, "##################decode!");
			decoder.configure(mediaFormat, m_surface, null, 0);
			decoder.start();
		}
		
		if(decoder == null)
			return;
		
		//LogUtils.v(TAG, "##################decode 1");
		BufferInfo info = new BufferInfo();
		if(iStreamType == STREAM_TYPE_IFRAME) {
			FrameSize size = JniClient.PW_NET_GetFrameSize(pSrc, iCount);
			//Log.v(TAG, "decoder "+decoder+" "+size.nWidth+" "+size.nHeight);
		}
		if(!m_decodeLock.tryLock()){
			return;
		}
		ByteBuffer[] inputBuffers = decoder.getInputBuffers();
		ByteBuffer[] outputBuffers = decoder.getOutputBuffers();
//		for (;;) {
			int inputBufferIndex = decoder.dequeueInputBuffer(timeoutUs);
		//	LogUtils.v(TAG, "##################decode 2 "+inputBufferIndex);
			if (inputBufferIndex >= 0) {
//				if(bStop){
//					//LogUtils.v(TAG, "queueInputBuffer BUFFER_FLAG_END_OF_STREAM");
//					decoder.queueInputBuffer(inputBufferIndex, 0, 0, 0, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
//				}else
				{
				    // fill inputBuffers[inputBufferIndex] with valid data
					ByteBuffer buffer = inputBuffers[inputBufferIndex];
					int iRealSize = 0;
					if(iStreamType == STREAM_TYPE_IFRAME) {
						int iIFrameIndex = getH264IFrameIndex(pSrc, iCount);
						iRealSize = iCount-iIFrameIndex;
						buffer.put(pSrc, iIFrameIndex, iRealSize);
					} else {
						iRealSize = iCount;
						buffer.put(pSrc, 0, iRealSize);
					}
					
					decoder.queueInputBuffer(inputBufferIndex, 0, iRealSize, 0, 0);
					bStop = true;
				}
			}	 
		//	LogUtils.v(TAG, "##################decode 3 ");
			int outputBufferIndex = decoder.dequeueOutputBuffer(info, timeoutUs);
		//	LogUtils.v(TAG, "##################decode 4 "+outputBufferIndex);
			if (outputBufferIndex >= 0) {
				// outputBuffer is ready to be processed or rendered.
				decoder.releaseOutputBuffer(outputBufferIndex, true);
				if ((info.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
					LogUtils.v(TAG, "OutputBuffer BUFFER_FLAG_END_OF_STREAM is over");
				}
			} else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
				LogUtils.v(TAG, "INFO_OUTPUT_BUFFERS_CHANGED");
				outputBuffers = decoder.getOutputBuffers();
			} else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
				// Subsequent data will conform to new format.
				LogUtils.v(TAG, "INFO_OUTPUT_FORMAT_CHANGED!");
				MediaFormat format = decoder.getOutputFormat();
			} else if (outputBufferIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {
				LogUtils.v(TAG, "dequeueOutputBuffer timed out AGAIN_LATER!");
			}
	//		LogUtils.v(TAG, "##################decode 5 ");
	//	}
			m_decodeLock.unlock();
	}
	public void addVideoFrame(byte[] data,long size){
		synchronized (this) {
			LogUtils.v("decodethread", "addVideoFrame");
			DataFrame frame=new DataFrame();
			frame.byteArr=data;
			frame.size=size;
			frameVec.addElement(frame);;
		}
		
	}
	public DataFrame getFrame(){
		synchronized (this) {
			DataFrame frame=null;
			if(!frameVec.isEmpty()){
				frame=frameVec.get(0);
				frameVec.remove(0);
			}
			return frame;
		}
	}
	
}
