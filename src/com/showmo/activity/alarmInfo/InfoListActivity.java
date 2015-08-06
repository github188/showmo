package com.showmo.activity.alarmInfo;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import com.showmo.R;
import com.showmo.activity.alarmInfo.InfoListAdapter.onBtnImgDownloadClickListener;
import com.showmo.alarmManage.Alarm;
import com.showmo.base.BaseActivity;
import com.showmo.network.RequestCallBack;
import com.showmo.network.ResponseInfo;
import com.showmo.ormlite.dao.IAlarmDao;
import com.showmo.util.AnimUtil;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;
import com.showmo.util.StringUtil;
import com.showmo.util.TimeUtil;
import com.showmo.util.ToastUtil;
import com.showmo.widget.AlarmFilterPop;
import com.showmo.widget.AlarmFilterPop.onFilterSelectedLisenter;
import com.showmo.widget.swipemenulistview.SwipeMenu;
import com.showmo.widget.swipemenulistview.SwipeMenuCreator;
import com.showmo.widget.swipemenulistview.SwipeMenuItem;
import com.showmo.widget.swipemenulistview.SwipeMenuListView;
import com.showmo.widget.swipemenulistview.SwipeMenuListView.IXListViewListener;
import com.showmo.widget.swipemenulistview.SwipeMenuListView.OnMenuItemClickListener;

import ipc365.app.showmo.jni.JniDataDef;
import ipc365.app.showmo.jni.JniDataDef.MSGBroadcastActions;
import ipc365.app.showmo.jni.JniDataDef.alarm_data_download;
import ipc365.app.showmo.jni.JniDataDef.alarm_data_download_progress;
import ipc365.app.showmo.jni.JniDataDef.alarm_data_state;
import android.R.integer;
import android.R.string;
import android.app.ActionBar.LayoutParams;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Environment;
import android.provider.CalendarContract.Instances;
import android.support.v4.util.TimeUtils;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.PopupWindow;
import android.widget.PopupWindow.OnDismissListener;
import android.widget.TextView;



public class InfoListActivity extends BaseActivity
{
	//每次查询限制的条数
	private static final int SQL_LIMIT_DEF = 20 ;

	private SwipeMenuListView mLvAlarmInfo;

	private List<Alarm> mAlarmList ;

	private InfoListAdapter mAdapter;

	private AlarmFilterPop mFilterAlarmPop ;

	private Button mBtnFilter;

	private IAlarmDao mAlarmDao;

	private int mDeviceId ;

	private long mEndTimeBuf ; //每次筛选，结果中最旧的一条的时间
	//筛选条件 
	private long mBeginTime ;

	private long mEndTime ;

	private int mAlarmType ;
	//筛选条件 end	

	private onFilterSelectedLisenter mOnFilterSelectedLisenter ;

	private TextView mTvNoAlarmHint ;

	private NewAlarmReceiver mNewAlarmReciver;

	private Button mBtnNewAlarmHint;

	private AlarmImgDownloadReceiver mAlarmImgDownloadReceiver;
	
	private PopupWindow filterpopupWindow,alarmpopupwindow;
	private View filterPopupWindowView,alarmPopupWindowView;
	 private static Context mContext;

	private Button btnfilter;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		mContext = this;
		setContentView(R.layout.activity_alarm_info_list);
		mAlarmList=new ArrayList<Alarm>();
		initFilterPopupWindow();
		initAlarmPopupWindow();
		initView();
		initListView();
		initNetwork();
		initFilterAlarmPop();
		initReciver();
		mAlarmDao =  (IAlarmDao)getDao(Alarm.class);
		//		dbTaskInsert();

		Intent in = getIntent();
		if(in != null){
			mDeviceId = in.getIntExtra(INTENT_KEY_INT, -1);
			//			dbTaskQueryAll();
			dbTaskQueryByTime();
		}

	}

	

	private void initAlarmPopupWindow(){
		alarmPopupWindowView = LayoutInflater.from(mContext).inflate(R.layout.alarm_havenew, null);
		TextView alarm_havenewTextView = (TextView)alarmPopupWindowView.findViewById(R.id.havenewalarm);
		alarm_havenewTextView.setOnClickListener(this);
		alarmpopupwindow=new PopupWindow(alarmPopupWindowView,LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT);
		alarmpopupwindow.setFocusable(true);
		alarmpopupwindow.setOutsideTouchable(true);
		alarmpopupwindow.setBackgroundDrawable(getResources().getDrawable(R.drawable.manage_filteralarm));
		alarmpopupwindow.update();
	    //popupWindow调用dismiss时触发，设置了setOutsideTouchable(true)，点击view之外/按键back的地方也会触发
		alarmpopupwindow.setOnDismissListener(new OnDismissListener() {
	      
	      @Override
	      public void onDismiss() {
	        // TODO Auto-generated method stub
//					showToast("关闭popupwindow");
	      }
	    });
	}
		
	
	
	  private void showAlarmPopupWindow(){
		    	alarmpopupwindow.showAtLocation(mBtnFilter, Gravity.BOTTOM, 0, 0);
		    
		  }

	
	private void initFilterPopupWindow() {
		filterPopupWindowView = LayoutInflater.from(mContext).inflate(R.layout.alarm_filter, null);
	    TextView filter_bytime = (TextView) filterPopupWindowView.findViewById(R.id.filter_bytime);
	    filter_bytime.setOnClickListener(this);
	    //初始化popupwindow，绑定显示view，设置该view的宽度/高度
		filterpopupWindow = new PopupWindow(filterPopupWindowView,LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT);
		filterpopupWindow.setFocusable(true);
		filterpopupWindow.setOutsideTouchable(true);
	    // 这个是为了点击“返回Back”也能使其消失，并且并不会影响你的背景；使用该方法点击窗体之外，才可关闭窗体
		filterpopupWindow.setBackgroundDrawable(getResources().getDrawable(R.drawable.manage_alarm));
	    //Background不能设置为null，dismiss会失效
//			popupWindow.setBackgroundDrawable(null);
	    //设置渐入、渐出动画效果
//			popupWindow.setAnimationStyle(R.style.popupwindow);
		filterpopupWindow.update();
	    //popupWindow调用dismiss时触发，设置了setOutsideTouchable(true)，点击view之外/按键back的地方也会触发
		filterpopupWindow.setOnDismissListener(new OnDismissListener() {
	      
	      @Override
	      public void onDismiss() {
	        // TODO Auto-generated method stub
//					showToast("关闭popupwindow");
	      }
	    });

		
	}
	  private void showFilterPopupWindow(){

		    if(!filterpopupWindow.isShowing()){
		    	filterpopupWindow.showAsDropDown(mBtnFilter, mBtnFilter.getLayoutParams().width/2, 0);
		    }else{
		    	filterpopupWindow.dismiss();
		    }
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



	private void initView() {
		setBarTitle(R.string.alarm_info);
		findViewAndSet(R.id.btn_bar_back);
		mBtnFilter = (Button)findViewAndSet(R.id.btn_ainfo_filter);
		mTvNoAlarmHint = (TextView)findViewAndSet(R.id.tv_ainfo_no_alarm_hint);
		/*mBtnNewAlarmHint = (Button)findViewAndSet(R.id.btn_ainfo_new_alarm_hint);*/
	}
	
		
	
	private void initFilterAlarmPop(){
		mOnFilterSelectedLisenter = new onFilterSelectedLisenter() {
			@Override
			public void onBtnOkClick(long beginTime, long endTime, int alarmType) {
				mBeginTime = beginTime ;
				mEndTime = endTime ;
				//origianl
				//				mAlarmType = alarmType ;
				//for test 
				mAlarmType = 0 ;
				mLvAlarmInfo.scrollTo(0, 0);
				//dbTaskQueryByTime();
				slideInFromRight(FilterResultActivity.class, mBeginTime, mEndTime, mDeviceId);
				InfoListActivity.this.finish();
			}
		};
		mFilterAlarmPop = new AlarmFilterPop(this, mOnFilterSelectedLisenter);
		mBeginTime = mFilterAlarmPop.getmAlarmFilterCondition().begintime;
		mEndTime = mFilterAlarmPop.getmAlarmFilterCondition().endtime;
		mAlarmType = mFilterAlarmPop.getmAlarmFilterCondition().alarmType;
	}

	private boolean isDownloadingImg=false;
	
	//初始化listview
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
				// create "open" item
//				SwipeMenuItem openItem = new SwipeMenuItem(
//						getApplicationContext());
//				// set item background
//				openItem.setBackground(new ColorDrawable(Color.rgb(0xC9, 0xC9,
//						0xCE)));
//				// set item width
//				openItem.setWidth(HexTrans.dip2px(InfoListActivity.this,  90));
//				// set item title
//				openItem.setTitle("Open");
//				// set item title fontsize
//				openItem.setTitleSize(18);
//				// set item title font color
//				openItem.setTitleColor(Color.WHITE);
//				// add to menu
//				menu.addMenuItem(openItem);

				// create "delete" item
				SwipeMenuItem deleteItem = new SwipeMenuItem(
						getApplicationContext());
				// set item background
				deleteItem.setBackground(new ColorDrawable(Color.rgb(0xF9,
						0x3F, 0x25)));
				// set item width
				deleteItem.setWidth(HexTrans.dip2px(InfoListActivity.this, 90));
				// set a icon
				deleteItem.setIcon(R.drawable.ic_delete);
				// add to menu
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



		//		mLvAlarmInfo.setOnItemClickListener(new OnItemClickListener() {
		//
		//			@Override
		//			public void onItemClick(AdapterView<?> parent, View view,
		//					int position, long id) {
		//				Button btnImg = (Button) view.findViewById(R.id.btn_alist_download);
		//				btnImg
		//				
		//				
		//				Alarm alarm = mAlarmList.get(position);
		//				 
		//				
		//				
		//			}
		//		});


		mLvAlarmInfo.setPullLoadEnable(false);
		mLvAlarmInfo.setXListViewListener(new IXListViewListener() {

			@Override
			public void onRefresh() {

			}

			@Override
			public void onLoadMore() {
				//Log.e("InfoListActivity", "onLoadMore");
				dbTaskQueryForMore();
			}
		});


	}




	@Override
	protected void onClick(int viewId) {
		switch (viewId) {

		case R.id.btn_bar_back:
			onBackPressed();
			break;
		case R.id.btn_ainfo_filter:
			showFilterPopupWindow();
			break;
		case R.id.filter_bytime:
			showFilterAlarmPop();
			filterpopupWindow.dismiss();
			break;
		case R.id.havenewalarm:
			alarmpopupwindow.dismiss();

			dbTaskQueryByTime();
		default:
			break;
		}


	}



	private void showFilterAlarmPop(){

		if (mFilterAlarmPop == null) {

			mFilterAlarmPop = new AlarmFilterPop(this, mOnFilterSelectedLisenter);

		}
		mFilterAlarmPop.showAsDropDown(mBtnFilter, 0, 0);

	}



	private void dbTaskQueryByTime(){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public ResponseInfo doInBackground() {

				List<Alarm> list = mAlarmDao.queryLastestItems(mDeviceId, SQL_LIMIT_DEF);
				if(list == null || list.size() == 0){
					return getResponseInfo(false) ;
				}
				mEndTimeBuf = list.get(list.size()-1).getEndTime()-1;

				mAlarmList = list ;

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

	//	private void dbTaskQueryAll(){
	//		mNetHelper.newNetTask(new RequestCallBack() {
	//
	//			@Override
	//			public ResponseInfo doInBackground() {
	//				List<Alarm> data = mAlarmDao.queryAllByDeviceId(mDeviceId);
	//				for (int i = 0; i < data.size(); i++) {
	//					Log.e("out", data.get(i).toString());
	//				}
	//				return null;
	//			}
	//		});
	//
	//	}


	private void dbTaskQueryForMore(){
		mNetHelper.newNetTask(new RequestCallBack() {

			@Override
			public ResponseInfo doInBackground() {

				int size= mAlarmList.size();
				mBeginTime=mAlarmList.get(size-1).getBeginTime();
				List<Alarm> list = mAlarmDao.queryLastestItems(mDeviceId,mBeginTime, SQL_LIMIT_DEF);
				if(list == null || list.size() == 0){
					return getResponseInfo(false);
				}
				mEndTimeBuf = list.get(list.size()-1).getEndTime()-1;
				ResponseInfo info=getResponseInfo(true);
				info.setObj(list);
//				Log.e("InfoListActivity","mEndTimeBuf-->"+ mEndTimeBuf);
//
//				mAlarmList.addAll(list);
//				//mAdapter.notifyDataSetChanged();
				return info;
			}

			@Override
			public synchronized void onSuccess(ResponseInfo info) {
				mAlarmList.addAll((List<Alarm>)info.getObj());
				LogUtils.e("shutdown", "onSuccessonSuccessonSuccessonSuccess");
				mAdapter.notifyDataSetChanged();
				
			}

			@Override
			public void onFailure(ResponseInfo info) {
				LogUtils.e("shutdown", "onFailureonFailureonFailureonFailure");
				//showToastShort(R.string.no_more_alarm);
				ToastUtil.toastShort(InfoListActivity.this, R.string.no_more_alarm);
				mLvAlarmInfo.setPullLoadEnable(false);
			}

			@Override
			public void onFinally() {
				LogUtils.e("shutdown", "onFinallyonFinallyonFinallyonFinallyonFinallyonFinally");
				mLvAlarmInfo.stopLoadMore();
			}


		});
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

	private synchronized void netTaskDownloadImg(final Alarm alarm,final Button btn){
		if(isDownloadingImg){
			ToastUtil.toastShort(InfoListActivity.this, "已有图片下载任务在进行中！");
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
				boolean bres=false;
				long iParam=0;//0图片不存在，2 下载时TF卡被拔出，3告警时没有TF卡
				if(alarm.getChannelNo() == Alarm.ChNoTFCard){//告警发生时没有插TF卡
					bres=false;
					iParam=3;
				}else {
					long ires=mShowmoSys.getAlarmPicture(alarmImgPath,alarm);
					if(ires==1){
						bres=true;
					}else{
						iParam=ires;
					}
				}
				ResponseInfo info=getResponseInfo(bres);
				info.setDateLong(iParam);
				return info;
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
				ToastUtil.toastShort(InfoListActivity.this, R.string.downloading);
			}

			@Override
			public void onFailure(ResponseInfo info) {
				
				isDownloadingImg=false;
				btn.setClickable(true);
				alarm.setmImgDownloading(false);
				mAdapter.notifyDataSetChanged();
				if(info.getDateLong() == 3){
					ToastUtil.toastShort(InfoListActivity.this, R.string.sorry_for_no_tf_when_alarm);
					return;
				}else if (info.getDateLong() ==2) {
					ToastUtil.toastShort(InfoListActivity.this, R.string.sorry_for_no_tf_when_download);
					return;
				}else if(info.getDateLong() ==0){
					ToastUtil.toastShort(InfoListActivity.this, R.string.sorry_for_no_alarm_pic);
					return;
				}
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
				ToastUtil.toastShort(InfoListActivity.this, R.string.alarm_pic_rec_suc);
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
						ToastUtil.toastShort(InfoListActivity.this, R.string.alarm_pic_rec_fai);
					}
				}
			}

		}
	}

	/**
	 * 当新的告警信息来到时，会收到广播
	 * @author Administrator
	 *
	 */
	private class NewAlarmReceiver extends BroadcastReceiver{

		@Override
		public void onReceive(Context context, Intent intent) {
			if(intent  == null){
				return ;
			}

			Alarm alarm =    (Alarm) intent.getExtras().getSerializable(JniDataDef.MSGBroadcastActions.DataKey);
			Log.e("contet", ""+alarm.getBeginTime());
			if(alarm == null){
				return ;
			}
			if(mDeviceId == alarm.getDeviceId()){ //判断是否属于该设备
				showAlarmPopupWindow();
			}
		}

	}
	private void unregisterBroadcastReceiver(){
		unregisterReceiver(mNewAlarmReciver);
		unregisterReceiver(mAlarmImgDownloadReceiver);
	}


	@Override
	protected void onDestroy() {
		unregisterBroadcastReceiver();
		super.onDestroy();
	}
	@Override
	public void onBackPressed() {

		setResult(RESULT_OK );
		super.onBackPressed();
	}
}
