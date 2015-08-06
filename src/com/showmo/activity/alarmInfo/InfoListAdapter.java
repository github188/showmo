
package com.showmo.activity.alarmInfo;

import java.util.List;

import com.showmo.R;
import com.showmo.alarmManage.Alarm;
import com.showmo.base.BaseActivity;
import com.showmo.util.LogUtils;
import com.showmo.util.StringUtil;
import com.showmo.util.TimeUtil;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

public class InfoListAdapter extends BaseAdapter {


	private List<Alarm> mAlarmList;

	private BaseActivity context;

	public InfoListAdapter(List<Alarm> AlarmList, BaseActivity context) {
		super();
		this.mAlarmList = AlarmList;
		this.context = context;
	}

	@Override
	public int getCount() {
		return mAlarmList == null ? 0 : mAlarmList.size();
	}

	@Override
	public Object getItem(int position) {
		return null;
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	private ViewHolder mViewHolder;
	

	@SuppressLint({ "InflateParams", "SimpleDateFormat" })
	@Override
	public View getView(final int position, View convertView, ViewGroup parent) {
		int i=position;
		if (convertView == null) {
			convertView = LayoutInflater.from(context).inflate(R.layout.item_alarm_info_list,
					null);

			mViewHolder = new ViewHolder();

			//mViewHolder.mRecordId = (TextView)convertView.findViewById(R.id.tv_alist_recordid);
			mViewHolder.mTime = (TextView) convertView.findViewById(R.id.tv_alist_time);
			mViewHolder.mType = (TextView)convertView.findViewById(R.id.tv_alist_type);
			//mViewHolder.mIndex = (TextView)convertView.findViewById(R.id.tv_alist_index);
			mViewHolder.mBtnImg = (Button) convertView.findViewById(R.id.btn_alist_download);
			mViewHolder.mDownloadProgressBar=(ProgressBar)convertView.findViewById(R.id.alarm_img_download_progress);
			mViewHolder.mDownloadImg=(ImageView)convertView.findViewById(R.id.alarm_downloadImg);
			convertView.setTag(mViewHolder);
		} else {
			mViewHolder = (ViewHolder)convertView.getTag();
		}
		if(mAlarmList == null){
			return convertView ;
		}
		Alarm alarm = mAlarmList.get(position);
		
		if (alarm != null) {
			//mViewHolder.mIndex.setText( (position + 1 )+".");
			
			//mViewHolder.mRecordId.setText(alarm.getRecordId()+"" );
			
			mViewHolder.mTime.setText(	TimeUtil.format(alarm.getEndTime())  );
			
			mViewHolder.mType.setText(Alarm.getAlarmTypeName(alarm.getAlarmType()) );
			
			mViewHolder.mBtnImg.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					mOnBtnImgDownloadClickListener.onBtnImgDownloadClick( (Button)v , position );
				}
			});
			//mViewHolder.mBtnImg.setTag(mAlarmList.get(position));
			
			if(alarm.getmImgDownloading()){
				mViewHolder.mBtnImg.setText(R.string.downloading);
				mViewHolder.mBtnImg.setClickable(false);
				mViewHolder.mBtnImg.setVisibility(View.GONE);
				mViewHolder.mDownloadProgressBar.setVisibility(View.VISIBLE);
				mViewHolder.mDownloadImg.setVisibility(View.GONE);
				mViewHolder.mDownloadProgressBar.setProgress(alarm.getmImgDownloadPos());
				LogUtils.e("alarm", "set progress pos "+alarm.getmImgDownloadPos()+" recordid "+alarm.getRecordId());
				
			}else{
	//			LogUtils.v("alarm", "adapter "+alarm.getRecordId()+"  "+alarm.getmImgPath());
				mViewHolder.mBtnImg.setVisibility(View.VISIBLE);
				mViewHolder.mDownloadProgressBar.setVisibility(View.GONE);
				if(!StringUtil.isNotEmpty(alarm.getmImgPath())){
					mViewHolder.mBtnImg.setClickable(true);
					mViewHolder.mDownloadImg.setVisibility(View.VISIBLE);
					mViewHolder.mBtnImg.setText("        ");
				}else{
					mViewHolder.mBtnImg.setClickable(true);
					mViewHolder.mDownloadImg.setVisibility(View.GONE);
					mViewHolder.mBtnImg.setText(R.string.look_img);
				}
			}
			
			
			
		} else {
			//mViewHolder.mRecordId.setText("" );
			mViewHolder.mTime.setText("");
			mViewHolder.mType.setText("");
		}
		
		
		return convertView;
	}
 
	class ViewHolder {
		
		private TextView mIndex ;
		
		private TextView mRecordId;

		private TextView mTime;
		
		private TextView mType ;
		
		private Button mBtnImg ;
		private ImageView mDownloadImg;
		private ProgressBar mDownloadProgressBar;
	}

	public List<Alarm> getmAlarmList() {
		return mAlarmList;
	}

	public void setmAlarmList(List<Alarm> mAlarmList) {
		
		this.mAlarmList = mAlarmList;
	}
	
	private onBtnImgDownloadClickListener mOnBtnImgDownloadClickListener ;
	
    public void setOnBtnImgDownloadClickListener( onBtnImgDownloadClickListener listener){
    	mOnBtnImgDownloadClickListener = listener ;
    }
	
    public interface onBtnImgDownloadClickListener{
    	void onBtnImgDownloadClick(Button btn,int position);
    }



}
