package com.showmo.event;

import com.showmo.eventBus.Event;

public class MainPageMultiAreaDisplayEvent extends Event{
	public boolean bShow;
	public MainPageMultiAreaDisplayEvent(boolean bShow){
		this.bShow=bShow;
	}
}
