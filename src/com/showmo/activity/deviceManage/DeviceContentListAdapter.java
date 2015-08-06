package com.showmo.activity.deviceManage;

import java.util.List;

import com.showmo.R;
import com.showmo.commonAdapter.AdapterViewHolder;
import com.showmo.commonAdapter.CommonAdapter;
import com.showmo.deviceManage.Device;

import android.content.Context;

public class DeviceContentListAdapter extends CommonAdapter<Device> {
	public DeviceContentListAdapter(Context context,List<Device> mDevices){
		super(context, mDevices, R.layout.fragment_device_manager_item);
	}
	@Override
	public void displayViewLayout(AdapterViewHolder viewHolder, Device data) {
		// TODO Auto-generated method stub
		
	}
	
}
