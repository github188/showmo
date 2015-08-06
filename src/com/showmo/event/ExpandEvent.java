package com.showmo.event;

import com.showmo.eventBus.Event;

public class ExpandEvent extends Event {
	private boolean bEnable=true;
	public ExpandEvent(boolean enable){
		bEnable=enable;
	}
	public boolean getEnable(){
		return bEnable;
	}

}
