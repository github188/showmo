package com.showmo.event;

import com.showmo.eventBus.Event;

public class RecordEvent extends Event{
	private boolean mEnable;
	public RecordEvent(boolean bEnable){
		mEnable=bEnable;
	}
	public boolean getEnable(){
		return mEnable;
	}
}
