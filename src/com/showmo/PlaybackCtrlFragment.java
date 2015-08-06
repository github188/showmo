package com.showmo;

import com.showmo.event.CaptureEvent;
import com.showmo.event.ExpandEvent;
import com.showmo.event.MainPageMultiAreaDisplayEvent;
import com.showmo.event.StopPlaybackEvent;
import com.showmo.eventBus.EventBus;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.Switch;

public class PlaybackCtrlFragment extends BaseFragment {
	private ImageButton mCaptureBtn;
	private ImageButton mStopBtn;
	private ImageButton mExpandBtn;
	private boolean mExpand=true;
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreateView(inflater, container, savedInstanceState);
		return inflater.inflate(R.layout.fragment_playback_ctrl_menu, container,false);
	}
	@Override
	protected void initView() {
		// TODO Auto-generated method stub
		super.initView();
		mCaptureBtn=(ImageButton)findAndSetClick(R.id.playback_ctrl_capture);
		mStopBtn=(ImageButton)findAndSetClick(R.id.playback_ctrl_stop);
		mExpandBtn=(ImageButton)findAndSetClick(R.id.playback_ctrl_expand);
	}
	public void onEventMainThread(MainPageMultiAreaDisplayEvent ev){
		if(ev.bShow){
			setExpandBtn(true);
		}else{
			setExpandBtn(false);
		}
	}
	public void setExpandBtn(boolean bexpand){
		mExpand=bexpand;
		if(bexpand){
			Drawable dr=getResources().getDrawable(R.drawable.menu_arrow_down);
			mExpandBtn.setImageDrawable(dr);
		}else{
			Drawable dr=getResources().getDrawable(R.drawable.menu_arrow_up);
			mExpandBtn.setImageDrawable(dr);
		}
	}
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		super.onClick(v);
		switch (v.getId()) {
		case R.id.playback_ctrl_capture:
			eventBusPost(new CaptureEvent());
			break;
		case R.id.playback_ctrl_stop:
			eventBusPost(new StopPlaybackEvent());
			break;
		case R.id.playback_ctrl_expand:
			eventBusPost(new ExpandEvent(!mExpand));
			break;
		default:
			break;
		}
	}
	@Override
	public void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		EventBus.getDefault().register(this);
	}
	@Override
	public void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		EventBus.getDefault().unregister(this);
	}
}
