package com.showmo.event;

import com.showmo.eventBus.Event;

import android.R.bool;

public class SoundSwitchEvent extends Event{
	private boolean mSwitch=false;
	public SoundSwitchEvent(boolean switchState){
		mSwitch=switchState;
	}
	public boolean getState(){
		return mSwitch;
	}
}
