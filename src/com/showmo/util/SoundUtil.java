package com.showmo.util;

import java.util.HashMap;

import com.showmo.R;
import com.showmo.base.ShowmoApplication;

import android.R.integer;
import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;

public class SoundUtil {
	
	private static SoundUtil m_soundUtil=new SoundUtil();
	public static SoundUtil getInstance(){
		return m_soundUtil;
	}
	public static final int Intercom_begin=1; 
	public static final int Intercom_end=2; 
	
	private SoundPool sPool;
	private Context mContext;
	private AudioManager aManager;
	private HashMap<Integer, Integer> msoundMap;
	private SoundUtil(){
		msoundMap=new HashMap<Integer, Integer>();
		mContext=ShowmoApplication.getInstance();
		sPool=new SoundPool(2, AudioManager.STREAM_ALARM, 5);
		msoundMap.put(Intercom_begin, sPool.load(mContext, R.raw.intercom_begin, 1)) ;
		msoundMap.put(Intercom_end, sPool.load(mContext, R.raw.intercom_end, 1)) ;
		aManager=(AudioManager)mContext.getSystemService(Context.AUDIO_SERVICE);
	}
	private int playSound(int soundKey){
		return sPool.play(msoundMap.get(Integer.valueOf(soundKey)), 1, 1, 1, 0, 1);
	}
	
	public int playIntercomBegin(){
		return playSound(Intercom_begin);
	}
	public int playIntercomEnd(){
		return playSound(Intercom_end);
	}
	
}
