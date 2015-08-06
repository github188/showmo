package com.showmo.activity.alarmInfo;

import ipc365.app.showmo.jni.JniDataDef;
import ipc365.app.showmo.jni.JniDataDef.alarm_data_download;
import ipc365.app.showmo.jni.JniDataDef.alarm_data_download_progress;
import ipc365.app.showmo.jni.JniDataDef.alarm_data_state;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.R.id;
import com.showmo.R.layout;
import com.showmo.R.menu;
import com.showmo.activity.alarmInfo.InfoListAdapter.onBtnImgDownloadClickListener;
import com.showmo.alarmManage.Alarm;
import com.showmo.base.BaseActivity;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.dao.IAlarmDao;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;
import com.showmo.util.StringUtil;
import com.showmo.util.TimeUtil;
import com.showmo.util.ToastUtil;
import com.showmo.widget.swipemenulistview.SwipeMenu;
import com.showmo.widget.swipemenulistview.SwipeMenuCreator;
import com.showmo.widget.swipemenulistview.SwipeMenuItem;
import com.showmo.widget.swipemenulistview.SwipeMenuListView;
import com.showmo.widget.swipemenulistview.SwipeMenuListView.IXListViewListener;
import com.showmo.widget.swipemenulistview.SwipeMenuListView.OnMenuItemClickListener;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class FilterResultActivity extends BaseActivity {
	private SwipeMenuListView mLvAlarmInfo;

	private List<Alarm> mAlarmList ;

	private InfoListAdapter mAdapter;
	
	private long mBeginTime;

	private long mEndTime ;

	private int mAlarmType ;
	
	private int mDeviceId ;
	
	private NewAlarmReceiver mNewAlarmReciver;

	private Button mBtnNewAlarmHint;
	
	private IAlarmDao mAlarmDao;
	
	private boolean isDownloadingImg=false;

	private AlarmImgDownloadReceiver mAlarmImgDownloadReceiver;
	private TextView mTvNoAlarmHint ;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_filter_result);
		Intent intent=getIntent();
		
		if(intent!=null){
			mBeginTime=intent.getLongExtra(INTENT_KEY_STRING, 0);
			mEndTime=intent.getLongExtra(INTENT_KEY_STRINGS, 0);
			mDeviceId=intent.getIntExtra(INTENT_KEY_STRINGSS, 0);
		}
		mAlarmList=new ArrayList<Alarm>();
		initView();
		initListView();
		initNetwork();
		initReciver();
		mAlarmDao =  (IAlarmDao)getDao(Alarm.class);
		dbTaskQueryByTime();
	}

	
	
	
	@Override
	protected void onClick(int viewId) {
		switch (viewId) {

		case R.id.btn_bar_back:					

			FilterResultActivity.this.finish();
			slideInFromLeft(InfoListActivity.class, mDeviceId);
			break;
	
		case R.id.btn_ainfo_new_alarm_hint:
			FilterResultActivity.this.finish();
			slideInFromLeft(InfoListActivity.class, mDeviceId);
			mBtnNewAlarmHint.setVisibility(View.INVISIBLE);
			break;
		default:
			break;
		}


	}

	
	
	
	private void dbTaskQueryByTime(){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public ResponseInfo doInBackground() {
				List<Alarm> list = mAlarmDao.queryLimitByTime(mDeviceId,mBeginTime,mEndTime, 20);
				if(list == null || list.size() == 0){
					return getResponseInfo(false) ;
				}
				mAlarmList = list; 
				return getResponseInfo(true) ;
			}
			@Override
			public void onFailure(ResponseInfo info) {
				if(mAlarmList  != null){
					mAlarmList.clear();
				}
				mAdapter.setmAlarmList(mAlarmList);
				mAdapter.notifyDataSetChanged();
				mLvAlarmInfo.setPullLoadEnable(false);
				mTvNoAlarmHint.setVisibility(View.VISIBLE);
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				mTvNoAlarmHint.setVisibility(View.GONE);
				mLvAlarmInfo.setPullLoadEnable(true);
				mAdapter.setmAlarmList(mAlarmList);
				mAdapter.notifyDataSetChanged();
				mLvAlarmInfo.setSelection(0);
			}



		});
	}
	private void dbTaskQueryForMore(){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public ResponseInfo doInBackground() {
				mEndTime=mAlarmList.get(mAlarmList.size()-1).getBeginTime();
				List<Alarm> list = mAlarmDao.queryLimitByTime(mDeviceId,mBeginTime,mEndTime, 20);
				if(list == null || list.size() == 0){
					return getResponseInfo(false);
				}

				mAlarmList.addAll(list);
				return getResponseInfo(true);
			}

			@Override
			public void onSuccess(ResponseInfo info) {
				mAdapter.notifyDataSetChanged();
			}

			@Override
			public void onFailure(ResponseInfo info) {
				//showToastShort(R.string.no_more_alarm);
				ToastUtil.toastShort(FilterResultActivity.this, R.string.no_more_alarm);
				mLvAlarmInfo.setPullLoadEnable(false);
			}

			@Override
			public void onFinally() {
				mLvAlarmInfo.stopLoadMore();
			}


		});
	}
	private void initView() {
		//setBarTitle(R.string.alarm_info);
		findViewAndSet(R.id.btn_bar_back);
		mTvNoAlarmHint = (TextView)findViewAndSet(R.id.tv_ainfo_no_alarm_hint);

		mBtnNewAlarmHint = (Button)findViewAndSet(R.id.btn_ainfo_new_alarm_hint);
	}
	
	private void initListView(){

		mLvAlarmInfo  = (SwipeMenuListView)findViewById(R.id.lv_ainfo_list);
		mAdapter = new InfoListAdapter(mAlarmList, this);
		mAdapter.setOnBtnImgDownloadClickListener(new onBtnImgDownloadClickListener() {

			@Override
			public void onBtnImgDownloadClick(Button btn, int position) {
				
				String str =  btn.getText().toString();
				Object o=btn.getTag();

				String imgPath=mAlarmList.get(position).getmImgPath();						
				if(StringUtil.isNotEmpty(imgPath)){
					slideInFromRight(InfoImgActivity.class, imgPath);
				}else{
					btn.setClickable(false);
					netTaskDownloadImg(mAlarmList.get(position),btn);
				}
				mAdapter.notifyDataSetChanged();
				LogUtils.e("alarm", "onBtnImgDownloadClick tag err,btn tag not correct alarm info");
				return;

			}
		});

		mLvAlarmInfo.setAdapter(mAdapter);

		// step 1. create a MenuCreator
		SwipeMenuCreator creator = new SwipeMenuCreator() {

			@Override
			public void create(SwipeMenu menu) {

				SwipeMenuItem deleteItem = new SwipeMenuItem(
						getApplicationContext());
				deleteItem.setBackground(new ColorDrawable(Color.rgb(0xF9,
						0x3F, 0x25)));
				deleteItem.setWidth(HexTrans.dip2px(FilterResultActivity.this, 90));
	
				deleteItem.setIcon(R.drawable.ic_delete);

				menu.addMenuItem(deleteItem);
			}
		};


		// set creator
		mLvAlarmInfo.setMenuCreator(creator);

		// step 2. listener item click event
		mLvAlarmInfo.setOnMenuItemClickListener(new OnMenuItemClickListener() {
			@Override
			public boolean onMenuItemClick(int position, SwipeMenu menu, int index) {
				switch (index) {
				case 0:
//					// open
//					break;
//				case 1:
					// delete
					//							delete(item);
					List<Alarm> list = new ArrayList<Alarm>();
					list.add(mAlarmList.get(position) );
					dbTaskRemoveAlarmInfo(list);
					mAlarmList.remove(position);
					mAdapter.notifyDataSetChanged();
					break;
				}
				return false;
			}
		});





		mLvAlarmInfo.setPullLoadEnable(false);
		mLvAlarmInfo.setXListViewListener(new IXListViewListener() {

			@Override
			public void onRefresh() {

			}

			@Override
			public void onLoadMore() {
				dbTaskQueryForMore();
			}
		});
	}

	private void initReciver() {
		mNewAlarmReciver = new NewAlarmReceiver();
		IntentFilter filter1 = new IntentFilter(JniDataDef.MSGBroadcastActions.CAMERA_ALARM_INFO_SERVER_UPLOAD_MSG);

		registerReceiver(mNewAlarmReciver, filter1); 
		
		mAlarmImgDownloadReceiver = new AlarmImgDownloadReceiver();
		IntentFilter filter2 = new IntentFilter(JniDataDef.MSGBroadcastActions.CAMERA_ALARM_DATA_UPLOAD_MSG);  
		filter2.addAction(JniDataDef.MSGBroadcastActions.CAMERA_ALARM_DATA_DOWLOAD_POS);
		filter2.addAction(JniDataDef.MSGBroadcastActions.CAMERA_ALARM_DATA_DOWLOAD_fai_MSG);
		registerReceiver(mAlarmImgDownloadReceiver, filter2);  


	}
	
	
	
	
	private void dbTaskRemoveAlarmInfo(final List<Alarm> data){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public ResponseInfo doInBackground() {

				int  db = mAlarmDao.Remove(  data);

				if(db  == 0 ){
					return getResponseInfo(false);
				}else{
					return getResponseInfo(true);
				}

			}

			@Override
			public void onSuccess(ResponseInfo info) {

			}

		});


	}
	
	private class NewAlarmReceiver extends BroadcastReceiver{

		@Override
		public void onReceive(Context context, Intent intent) {
			if(intent  == null){
				return ;
			}

			Alarm alarm =    (Alarm) intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
			if(alarm == null){
				return ;
			}
			mEndTime=alarm.getEndTime();
			if(mDeviceId == alarm.getDeviceId()){ //判断是否属于该设备
				//				showToastShort(R.string.have_new_alarm);
				mBtnNewAlarmHint.setVisibility(View.VISIBLE);
			}
		}
	}
		private synchronized void netTaskDownloadImg(final Alarm alarm,final Button btn){
			if(isDownloadingImg){
				ToastUtil.toastShort(FilterResultActivity.this, "已有图片下载任务在进行中！");
				return;
			}
			isDownloadingImg=true;
			mNetHelper.newNetTask(new RequestCallBack() {

				@Override
				public ResponseInfo doInBackground() {
					String alarmImgPath=Environment.getExternalStorageDirectory().getAbsolutePath();
					alarmImgPath+="/alarmImg";
					File file=new File(alarmImgPath);
					if(!file.exists()){
						file.mkdir();
					}
					return getResponseInfo(mShowmoSys.getAlarmPicture(alarmImgPath,alarm));
				}
				@Override
				public void onPrepare() {
					// TODO Auto-generated method stub
					alarm.setmImgDownloading(true);
					mAdapter.notifyDataSetChanged();
					super.onPrepare();
				}
				@Override
				public void onSuccess(ResponseInfo info) {
					btn.setText(R.string.downloading);
					//showToastShort(R.string.downloading);
					ToastUtil.toastShort(FilterResultActivity.this, R.string.downloading);
				}

				@Override
				public void onFailure(ResponseInfo info) {
					isDownloadingImg=false;
					btn.setClickable(true);
					alarm.setmImgDownloading(false);
					mAdapter.notifyDataSetChanged();
					int errorCode = (int) info.getErrorCode();
					if(handleNetConnectionError(errorCode)){
						return ;
					}
					switch (errorCode) {

					default:
						//LogUntreatedError(errorCode);
						break;
					}
				}

			});
		}
		private class AlarmImgDownloadReceiver extends BroadcastReceiver {

			@Override
			public void onReceive(Context context, Intent intent) {

				if(intent.getAction().equals(JniDataDef.MSGBroadcastActions.CAMERA_ALARM_DATA_UPLOAD_MSG)){
					alarm_data_state data = (alarm_data_state) intent.getExtras().get(JniDataDef.MSGBroadcastActions.DataKey);
					LogUtils.v("alarm", "AlarmImgDownloadReceiver: recordid "+data.recordId+" cameraId "+data.cameraId);
					ToastUtil.toastShort(FilterResultActivity.this, R.string.alarm_pic_rec_suc);
					if(mDeviceId == data.cameraId){ //判断是否属于该设备
						Alarm alarm =mAlarmDao.queryByRecordId(data.recordId);//(data.alarmCode);
						LogUtils.v("alarm", "AlarmImgDownloadReceiver get: "+alarm.toString());
						//查看是否符合筛选条件
						boolean isInFilter=false;
						for (int i = 0; i < mAlarmList.size(); i++) {
							if(mAlarmList.get(i).getRecordId()==alarm.getRecordId()){
								mAlarmList.get(i).setmImgPath(alarm.getmImgPath());
								mAlarmList.get(i).setmImgDownloading(alarm.getmImgDownloading());
								isDownloadingImg=false;
								isInFilter=true;
								break;
							}
						}
						if(!isInFilter){
							mAlarmList.add(0, alarm);
						}
						mAdapter.notifyDataSetChanged();
						//				if(alarm.getBeginTime() > mBeginTime && alarm.getEndTime() < mEndTime){
						//					mAlarmList.add(alarm);
						//					LogUtils.v("alarm","notifyDataSetChanged");
						//					
						//				}
					}
				}
				else if (intent.getAction().equals(JniDataDef.MSGBroadcastActions.CAMERA_ALARM_DATA_DOWLOAD_POS)) {
					alarm_data_download_progress prog=(alarm_data_download_progress)intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
					LogUtils.e("alarm", "POS ret "+prog.pos);
					for (Alarm alarm : mAlarmList) {
						if(alarm.getRecordId() == prog.recordId){
							alarm.setmImgDownloadPos(prog.pos);
							LogUtils.e("alarm", "pos notify "+alarm.getRecordId()+" pos "+prog.pos);
							mAdapter.notifyDataSetChanged();
						}
					}
				}else if (intent.getAction().equals(JniDataDef.MSGBroadcastActions.CAMERA_ALARM_DATA_DOWLOAD_fai_MSG)) {
					alarm_data_download downloadinfo=(alarm_data_download)intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
					for (Alarm alarm : mAlarmList) {
						if(alarm.getRecordId() == downloadinfo.recordId){
							alarm.setmImgDownloading(false);
							isDownloadingImg=false;
							alarm.setmImgDownloadPos(0);
							mAdapter.notifyDataSetChanged();
							ToastUtil.toastShort(FilterResultActivity.this, R.string.alarm_pic_rec_fai);
						}
					}
				}

			}
		}
	
}
