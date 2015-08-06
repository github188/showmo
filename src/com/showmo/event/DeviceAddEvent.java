package com.showmo.event;


import com.showmo.deviceManage.Device;
import com.showmo.eventBus.Event;

public class DeviceAddEvent extends Event {
	public Device device;
	public DeviceAddEvent(Device dev){
		device=dev;
	}
}
