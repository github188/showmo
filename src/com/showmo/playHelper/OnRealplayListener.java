package com.showmo.playHelper;

public interface OnRealplayListener {
	/*
	 * 播放时的状态回调
	 */
	void onRealplayStateListener(RealplayOutParams para);

	/*
	 * 播放结果回调
	 */
	void onRealplayResultListener(RealplayOutParams para);
	
	/*
	 * 播放前回调
	 */
	void onRealplayBeforeListener();
}
