package com.showmo.event;

import com.showmo.eventBus.Event;

import ipc365.app.showmo.jni.JniDataDef.User_2_mgr_disconn_device;

public class DevOnlineStateEvent extends Event {
	public boolean online;
	public User_2_mgr_disconn_device info;
	
	public DevOnlineStateEvent(boolean online,User_2_mgr_disconn_device devData){
		this.online=online;
		info=devData;
	}
}
