package com.showmo.playHelper;

public interface OnPlaybackListener {
	/*
	 * 回放开启结束
	 */
	void onPlaybackOver(boolean bres,int errorCode);
	
	void onPlaybackPre();   
	/*
	 * 回放结束
	 */
	void onPlaybackCompleted();
	void onStopPlayback(boolean bres,int errCode);
}
