package com.showmo;


import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoApplication;
import com.showmo.base.ShowmoSystem;
import com.showmo.eventBus.Event;
import com.showmo.eventBus.EventBus;
import com.showmo.network.NetworkHelper;
import com.showmo.network.ResponseInfo;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

public class BaseFragment extends Fragment implements View.OnClickListener{
	protected BaseActivity m_activity;
	protected ShowmoApplication showmoApp;
	protected ShowmoSystem showmoSystem;
	protected static NetworkHelper mNetHelper;
	protected ResponseInfo info; // 请求反馈的包装类
	
	public BaseFragment() {
		// TODO Auto-generated constructor stub
		// LogUtils.v("fragment", "BaseFragment");
		showmoApp=ShowmoApplication.getInstance();
		showmoSystem=ShowmoSystem.getInstance();
		if (mNetHelper == null) {
			mNetHelper = NetworkHelper.getInstance();
		}
	}

	private long getErrorCode() {
		if (showmoSystem == null) {
			showmoSystem = ShowmoSystem.getInstance();
		}
		long errorCode = showmoSystem.getLastErrorCode();
		Log.e("NetworkHelper", "errorCode-->" + errorCode);
		return errorCode;
	}
	protected void LogUntreatedError(int errorCode) {
		showToastShort(R.string.operate_err);
		Log.e("BaseFragment", "未处理的错误代码-->" + errorCode);
	}
	protected void showToastShort(int msg) {
		Toast.makeText(m_activity, msg, Toast.LENGTH_SHORT).show();
	}
	protected void showToastShort(String msg) {
		Toast.makeText(m_activity, msg, Toast.LENGTH_SHORT).show();
	}
	

	@Override
	public void onAttach(Activity activity){
		super.onAttach(activity);
		m_activity=(BaseActivity)activity;
		//LogUtils.v("fragment", "onAttach");
	}
	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		//LogUtils.v("fragment", "onCreate");
	}
	@Override 
	public void onActivityCreated(Bundle savedInstaceState){
		super.onActivityCreated(savedInstaceState);
		//LogUtils.v("fragment", "onActivityCreated");
		initView();
	}
	protected void initView(){

	}
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
	}
	protected void eventBusPost(Event event){
		EventBus.getDefault().post(event);
	}
	protected View findViewById(int resid){
		return getActivity().findViewById(resid);
	}
	protected View findAndSetClick(int resid){
		View view=getActivity().findViewById(resid);
		view.setOnClickListener(this);
		return view;
	}
	@Override
	public void onStart(){
		super.onStart();
		//LogUtils.v("fragment", "onStart");
	}
	@Override
	public void onResume(){
		super.onResume();
		//LogUtils.v("fragment", "onResume");
	}
	@Override
	public void onPause(){
		super.onPause();
		//LogUtils.v("fragment", "onPause");
	}
	@Override
	public void onSaveInstanceState(Bundle savedInstanceState){
		super.onSaveInstanceState(savedInstanceState);
		//LogUtils.v("fragment", "onSaveInstanceState");
	}
	@Override
	public void onStop(){
		super.onStop();
		//LogUtils.v("fragment", "onStop");
	}
	@Override
	public void onDestroyView(){
		super.onDestroyView();
		//LogUtils.v("fragment", "onDestroyView");
	}
	@Override 
	public void onDestroy(){
		super.onDestroy();
		//LogUtils.v("fragment", "onDestroy");
	}
	@Override
	public void onDetach() {
		// TODO Auto-generated method stub
		super.onDetach();
		//LogUtils.v("fragment", "onDetach");
	}
}
