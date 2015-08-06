package com.showmo.event;

import com.showmo.eventBus.Event;

public class AckRecordEvent extends Event {
	public  final static int opened=0;
	public  final static int closed=1;
	private boolean mResult=false;
	private  int mType=opened;
	public AckRecordEvent(int type,boolean bres){
		mResult=bres;
		mType=type;
	}
	public boolean getResult(){
		return mResult;
	}
	public int getType(){
		return mType;
	}
}
