
package com.showmo.activity.deviceManage;


import java.util.List;

import com.showmo.R;
import com.showmo.deviceManage.Device.AlarmSwitch;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.TextView;



public class AlarmTypeAdapter extends BaseAdapter {


	private List<AlarmSwitch> mAlarmTypeList;

	private Activity context;
	
	
	public AlarmTypeAdapter(Activity context) {
		super();
		this.context = context;
	}
	
	public AlarmTypeAdapter(List<AlarmSwitch> AlarmTypeList, Activity context) {
		super();
		this.mAlarmTypeList = AlarmTypeList;
		this.context = context;
	}
	
	@Override
	public int getCount() {
		return mAlarmTypeList == null ? 0 : mAlarmTypeList.size();
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

	@SuppressLint("InflateParams")
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {

		if (convertView == null) {
			convertView = LayoutInflater.from(context).inflate(R.layout.item_alarm_switch,
					null);

			mViewHolder = new ViewHolder();
			mViewHolder.mAlarmTypeName = (TextView) convertView.findViewById(R.id.tv_alarm_type);
			mViewHolder.mSwitch = (CheckBox)convertView.findViewById(R.id.check_alarm_switch);
			convertView.setTag(mViewHolder);
		} else {
			mViewHolder = (ViewHolder)convertView.getTag();
		}
		if(mAlarmTypeList == null){
			return convertView ;
		}
		
		AlarmSwitch alarmSwitch = mAlarmTypeList.get(position) ;
		if(alarmSwitch == null){
			return convertView;
		}
		
		int  resid = alarmSwitch.alarmNameResId;
		if (resid !=  -1 ) {
			mViewHolder.mAlarmTypeName.setText(resid);
		} else {
			mViewHolder.mAlarmTypeName.setText("");
		}
		mViewHolder.mSwitch.setChecked(alarmSwitch.value);
		
		return convertView;
	}

	class ViewHolder {

		private TextView mAlarmTypeName;
		
		private CheckBox mSwitch ;

	}
 

	public void setAlarmTypeList(List<AlarmSwitch> AlarmTypeList) {
		this.mAlarmTypeList = AlarmTypeList;
	}
 



}
