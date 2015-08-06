package com.showmo;

import java.util.ArrayList;
import java.util.List;

import com.showmo.base.ShowmoSystem;
import com.showmo.dataDef.PWPlayBackVideoFrame;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.DeviceUseUtils;
import com.showmo.event.TimelineShowEvent;
import com.showmo.eventBus.EventBus;
import com.showmo.playHelper.IDevicePlayer;
import com.showmo.playHelper.PlayHelper;
import com.showmo.util.LogUtils;
import com.showmo.widget.timeline.Timeline;

import ipc365.app.showmo.jni.JniClient;
import ipc365.app.showmo.jni.JniDataDef;
import ipc365.app.showmo.jni.JniDataDef.SDK_REMOTE_FILE;
import android.app.Fragment;
import android.os.Bundle;
import android.text.format.Time;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class TimelineFragment extends BaseFragment {
	private Timeline m_timeline;
	private IDevicePlayer m_playHelper;
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		LogUtils.v("fragment", "onCreateView");
		return inflater.inflate(R.layout.fragment_timeline, container,false);
	}
	public void reset(){
		if(m_timeline!=null){
			m_timeline.reset();
		}
	}
	
	public void onEventMainThread(TimelineShowEvent ev){
		m_timeline.update();
	}
	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		EventBus.getDefault().unregister(this);
	}
	@Override
	public void onActivityCreated(Bundle savedInstaceState) {
		// TODO Auto-generated method stub
		
		super.onActivityCreated(savedInstaceState);
		EventBus.getDefault().register(this);
		m_playHelper=ShowmoSystem.getInstance().getPlayer();
		m_timeline = (Timeline) m_activity.findViewById(R.id.timeline);
		if (savedInstaceState!=null) {
			LogUtils.v("timeline", "11111");
		}
		m_timeline.setOnPlaybackListener(new Timeline.OnPlaybackListener() {
			@Override
			public List<SDK_REMOTE_FILE> onSearchPlayBackFileList(
					Time beginTime, Time endTime) {
				// TODO Auto-generated method stub
				LogUtils.v("timeline", beginTime.format2445()+"  "+endTime.format2445());
				List<SDK_REMOTE_FILE> list=null;
				Device dev=m_playHelper.getmCurDeviceInfo();
				if(dev!=null){
					DeviceUseUtils userUtil=new DeviceUseUtils(dev);
					list=userUtil.getRemoteFiles(beginTime, endTime);
					SDK_REMOTE_FILE file=new SDK_REMOTE_FILE();
//					file.startTime=new Time();
//					file.startTime.year=2015;
//					file.startTime.month=4;
//					file.startTime.monthDay=29;
//					file.startTime.hour=7;
//					file.startTime.minute=0;
//					file.startTime.second=0;
//					
//					file.endTime=new Time();
//					file.endTime.year=2015;
//					file.endTime.month=4;
//					file.endTime.monthDay=29;
//					file.endTime.hour=9;
//					file.endTime.minute=0;
//					file.endTime.second=0;
					
//					list=new ArrayList<JniDataDef.SDK_REMOTE_FILE>();
//					list.add(file);
//					LogUtils.v("playback","begintime:"+file.startTime.format2445()+"end:"+file.endTime.format2445());
				}
				return list;
			}

			@Override
			public boolean onPlayback(SDK_REMOTE_FILE playbackVideoFrame,int pos) {
				// TODO Auto-generated method stub
				//JniClient.PW_NET_PlayBack(nCamera_id, lpPlayBackFile, fDownLoadDataCallBack, dwDataUser)
				m_playHelper.playback(playbackVideoFrame,(MainActivity)m_activity,pos);
				return true;
			}
		});
		
//		testbtn = (Button) findViewById(R.id.test_btn);
//		testbtn.setOnClickListener();
	}
}
