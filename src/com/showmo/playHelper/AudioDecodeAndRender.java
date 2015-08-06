package com.showmo.playHelper;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;

import com.showmo.util.LogUtils;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.AsyncTask;
import android.os.SystemClock;
import android.util.Log;
import cn.sharesdk.framework.statistics.NewAppReceiver;

public class AudioDecodeAndRender {
	private static AudioTrack m_trackplayer;
	
	
	private static short[] A2l = {
	    -5504, -5248, -6016, -5760, -4480, -4224, -4992, -4736,
	    -7552, -7296, -8064, -7808, -6528, -6272, -7040, -6784,
	    -2752, -2624, -3008, -2880, -2240, -2112, -2496, -2368,
	    -3776, -3648, -4032, -3904, -3264, -3136, -3520, -3392,
	    -22016,-20992,-24064,-23040,-17920,-16896,-19968,-18944,
	    -30208,-29184,-32256,-31232,-26112,-25088,-28160,-27136,
	    -11008,-10496,-12032,-11520, -8960, -8448, -9984, -9472,
	    -15104,-14592,-16128,-15616,-13056,-12544,-14080,-13568,
	    -344,  -328,  -376,  -360,  -280,  -264,  -312,  -296,
	    -472,  -456,  -504,  -488,  -408,  -392,  -440,  -424,
	    -88,   -72,  -120,  -104,   -24,    -8,   -56,   -40,
	    -216,  -200,  -248,  -232,  -152,  -136,  -184,  -168,
	    -1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
	    -1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696,
	    -688,  -656,  -752,  -720,  -560,  -528,  -624,  -592,
	    -944,  -912, -1008,  -976,  -816,  -784,  -880,  -848,
	    5504,  5248,  6016,  5760,  4480,  4224,  4992,  4736,
	    7552,  7296,  8064,  7808,  6528,  6272,  7040,  6784,
	    2752,  2624,  3008,  2880,  2240,  2112,  2496,  2368,
	    3776,  3648,  4032,  3904,  3264,  3136,  3520,  3392,
	    22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
	    30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136,
	    11008, 10496, 12032, 11520,  8960,  8448,  9984,  9472,
	    15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568,
	    344,   328,   376,   360,   280,   264,   312,   296,
	    472,   456,   504,   488,   408,   392,   440,   424,
	    88,    72,   120,   104,    24,     8,    56,    40,
	    216,   200,   248,   232,   152,   136,   184,   168,
	    1376,  1312,  1504,  1440,  1120,  1056,  1248,  1184,
	    1888,  1824,  2016,  1952,  1632,  1568,  1760,  1696,
	    688,   656,   752,   720,   560,   528,   624,   592,
	    944,   912,  1008,   976,   816,   784,   880,   848,
	};

	private static int g711a_Decode(byte[] src, byte[] dst) {
	  int[] temp = new int[src.length];		
	   for(int i=0; i<src.length; i++)
	   {
		   temp[i]=(A2l[(src[i]&0xff)]&0xffff);	//转换成无符号
	   }

	   for(int j=0; j<temp.length; j++) {
		   dst[j*2] = (byte) (temp[j]&0xff);
		   dst[j*2+1] = (byte) (temp[j]>>8);
	   }
	   return 1;
	}
	
	public static void RenderInit() {
		int bufsize = AudioTrack.getMinBufferSize(8000,AudioFormat.CHANNEL_OUT_MONO,AudioFormat.ENCODING_PCM_16BIT);

		//注意，按照数字音频的知识，这个算出来的是一秒钟buffer的大小。
		//创建AudioTrack
		m_trackplayer = new AudioTrack(AudioManager.STREAM_MUSIC, 8000, AudioFormat.CHANNEL_OUT_MONO,
				AudioFormat.ENCODING_PCM_16BIT, bufsize, AudioTrack.MODE_STREAM);
		
	}
	
	public static void RenderUninit() {
		m_trackplayer.release();//释放底层资源。
	}
	
	public static void RenderStart() {
		m_trackplayer.play() ;
		mDecodeThread=new RenderThread();
		//AsyncTask.execute(mDecodeThread);
		mDecodeThread.start();
		mCurVideoFrameNum=0;
	}
	
	public static void RenderStop() {
		m_trackplayer.stop();//停止播放	
		mDataQueue.clear();
		if(mDecodeThread!=null){
			mDecodeThread.exit(true);
		}
	}
	public static void RenderPause(boolean bPause){
		if(mDecodeThread!=null){
			mDecodeThread.pause(bPause);
		}
		bAcceptSoundData=!bPause;
	}
	private static boolean bAcceptSoundData=true;
	/*
	 *缓冲区，最大为125的容量
	 */
	public static class AudioBlockData{
		long FrameNum;
		byte[] data;
		public AudioBlockData(long FrameNum,byte[] data){
			this.FrameNum = FrameNum;
			this.data = data;
		}
	}
	private static BlockingQueue<AudioBlockData> mDataQueue=new ArrayBlockingQueue<AudioBlockData>(125);
	public static int AttenuateTh=30;
	private static RenderThread mDecodeThread;
	public static void RenderInputData(long frameNum,byte[] src) {
		if(bAcceptSoundData){
			try {
				if(!mDataQueue.offer(new AudioBlockData(frameNum,src),3,TimeUnit.MILLISECONDS)){
					//LogUtils.e("frameindex", "offer data timeout "+frameNum+" size "+mDataQueue.size());
				}
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
			}
		}
	}
	
	public static long mCurVideoFrameNum=0;
	public static Object objAudioWait=new Object();
	public static void updateCurVideoFrameNum(long framenum){
		mCurVideoFrameNum=framenum;
		synchronized (objAudioWait) {
			objAudioWait.notifyAll();
		}
	}
	private static class RenderThread extends Thread{
		private  boolean bPause=false;
		private  boolean bExit=false;
		public   void pause(boolean pause){
			bPause=pause;
		}
		public   void exit(boolean bexit){
			bExit=bexit;
		}
		@Override
		public void run() {
			// TODO Auto-generated method stub
			long beforeTime=SystemClock.elapsedRealtime();
			long totalsize=0;
			while(!bExit){
				if(!bPause){
//				float db=(float)countDb(src);
//				float volu=AttenuateTh/db;
//				if(volu>1.0f){
//					volu=1.0f;
//				}
//				//LogUtils.e("dbctrl", " volu "+volu+" db "+db);
//				m_trackplayer.setStereoVolume(volu, volu);
					AudioBlockData data=null;
					try {
						data=mDataQueue.poll(100,TimeUnit.MILLISECONDS);
						if(data==null){
							//LogUtils.e("frameindex", "poll data timeout ");
							continue;
						}
						while(data.FrameNum > mCurVideoFrameNum){
							synchronized (objAudioWait) {
								try {
									objAudioWait.wait();
								} catch (Exception e) {
									// TODO: handle exception
									e.printStackTrace();
								}
							}
						}
						if(mDataQueue.size()>=8){
							continue;
						}
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
					}
					if(data!=null){
						byte[] dst = new byte[data.data.length*2];
						totalsize += dst.length;
						LogUtils.e("frameindex", "decode audio "+data.FrameNum);
						if(SystemClock.elapsedRealtime() - beforeTime >=1000){
							LogUtils.e("frameindex", "decode audio "+data.FrameNum+" "+" totalsize "+totalsize);
							totalsize=0;
							beforeTime = SystemClock.elapsedRealtime(); 
						}
						g711a_Decode(data.data, dst);
						m_trackplayer.write(dst, 0, dst.length) ;//往track中写数据
					}
				}else{
					try {
						Thread.sleep(20);
					} catch (Exception e) {
						// TODO: handle exception
						e.printStackTrace();
					}
				}
			}
			super.run();
		}
	}
	
	private static double countDb(byte[] data){
		float BASE=32768f;
	    float maxAmplitude = 0;
	     
//	    int v=0;
//	    for (int i = 0; i < data.length; i++) {
//			v += data[i] * data[i];
//		}
//		int db = -90;
//		if (v != 0) {
//			db = (int) (20 * Math.log10(Math.sqrt(v/data.length) / 32768f));
//		}
	    
	    for (int i = 0; i < data.length; i++) 
	    {
	        maxAmplitude += data[i] * data[i];
	    }
	    maxAmplitude=(float)Math.sqrt(maxAmplitude/data.length);
	    float ratio=maxAmplitude / BASE;
	    float db =0;
	    if(ratio>0)
	    {
	        db = (float) (20 * Math.log10(ratio))+100;
	    }
	    m_db=db;
	    return db;
	}
	private static double m_db=0;
	public static double getDb(){
		return m_db;
	}
}
