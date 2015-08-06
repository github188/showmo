package com.showmo.event;

import com.showmo.eventBus.Event;

public class AckCaptureEvent extends Event{
	public static final int captureOver=0;
	public static final int SaveBegin=1;
	public static final int SaveOver=2;
	private int captureStep=SaveBegin;
	private boolean bResult=false;
	public AckCaptureEvent(int step,boolean bres){
		captureStep=step;
		bResult=bres;
	}
	public int getStep(){
		return captureStep;
	}
	public boolean getResult(){
		return bResult;
	}
}
