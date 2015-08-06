package com.showmo.activity.deviceManage;

import java.util.ArrayList;
import java.util.List;

import com.showmo.BaseFragment;
import com.showmo.R;
import com.showmo.deviceManage.Device;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

public class DeviceListFragment extends BaseFragment {
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		//super.onCreateView(inflater, container, savedInstanceState);
		View view=inflater.inflate(R.layout.fragment_device_manager_list_content, container,false);
		return view;
	}
	@Override
	public void onActivityCreated(Bundle savedInstaceState) {
		// TODO Auto-generated method stub
		super.onActivityCreated(savedInstaceState);
		this.init();
	}
	private ListView mListView;
	public void init(){
		mListView=(ListView)m_activity.findViewById(R.id.device_manager_content_list);
		List<Device> devices=new ArrayList<Device>();
		devices.add(new Device());
		devices.add(new Device());
		devices.add(new Device());
		devices.add(new Device());
		devices.add(new Device());
		mListView.setAdapter(new DeviceContentListAdapter(m_activity, devices));
	}
}
