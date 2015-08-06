package com.showmo.event;

import com.showmo.eventBus.Event;

public class InitLightEvent extends Event{
	private int light=0;
	public InitLightEvent(int value){
		light=value;
	}
	public int getValue(){
		return light;
	}
}
