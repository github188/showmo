package com.showmo.widget.timeline;

import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;

import java.util.List;

import android.text.format.Time;

public interface ITimelineDial {
	public interface SearchStateListener{
		void searchBegin();
		void searchSuccess();
		void searchFailured();
	}
	public interface ItimeLineCursorYchangedListener {
		public void onYchanged(float curY);
	}
	public interface IPlaybackFileSearchListener {
		public List<SDK_REMOTE_FILE> onGetPlaybackFileList(Time beginTime,
				Time endTime);
	}
	
	public void setSearchStateListener(SearchStateListener lis);
	
	public void setCursorYchanged(ItimeLineCursorYchangedListener listener);
	
	public void setPlaybackFileSearchListener(IPlaybackFileSearchListener listener);
	
	public float getXByTime(Time time);
	
	public Time getTimeByCursorX(float curx);
	
	public SDK_REMOTE_FILE getFileByCursorX(float curx);
	
	public void updateCurTimeByCurx(float curx);
	
	public int getPosByCurx(SDK_REMOTE_FILE file,float curx);
	
	public void setInitTime(float initX, Time initTime);
	
	public void startSearchFile(int delayMsTime);
	
	public Time getCurTime();
	
	public void reset();
	
	public void update();
}
