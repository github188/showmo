package com.showmo.event;

import com.showmo.eventBus.Event;

public class PlaybackEvent extends Event {
	public boolean isOpen;
	public PlaybackEvent(boolean bopen){
		isOpen=bopen;
	}
	
}
