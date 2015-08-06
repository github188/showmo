
package com.showmo.activity.alarmInfo;


import java.util.List;

import com.showmo.R;
import com.showmo.deviceManage.Device;
import com.showmo.util.StringUtil;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class DeviceListAdapter extends BaseAdapter {


	private List<Device> mDeviceList;

	private Activity context;

	public DeviceListAdapter(List<Device> DeviceList, Activity context) {
		super();
		this.mDeviceList = DeviceList;
		this.context = context;
	}

	@Override
	public int getCount() {
		return mDeviceList == null ? 0 : mDeviceList.size();
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
			convertView = LayoutInflater.from(context).inflate(R.layout.item_alarm_info_devices,
					null);

			mViewHolder = new ViewHolder();

			mViewHolder.mDeviceName = (TextView)convertView.findViewById(R.id.tv_alarm_info_name);
			mViewHolder.mDeviceImg = (ImageView)convertView.findViewById(R.id.img_alarm_info_pic);
			mViewHolder.mPoint = (ImageView) convertView.findViewById(R.id.img_alarm_info_point);
			mViewHolder.mMessage = (TextView) convertView.findViewById(R.id.tv_alarm_info_message);
			convertView.setTag(mViewHolder);
		} else {
			mViewHolder = (ViewHolder)convertView.getTag();
		}
		if(mDeviceList == null){
			return convertView ;
		}
		Device device = mDeviceList.get(position);
		if (device != null) {
			mViewHolder.mDeviceName.setText(device.getmDeviceName() );
			if(device.getHaveNewAlarmInfo()){
				mViewHolder.mMessage.setText(R.string.have_new_alarm  );
				mViewHolder.mPoint.setVisibility(View.VISIBLE);
			}else{
				mViewHolder.mMessage.setText(R.string.no_new_alarm  );
				mViewHolder.mPoint.setVisibility(View.INVISIBLE);
			}
		
			String imgPath = device.getmTinyImgFilePath() ;
			if(StringUtil.isNotEmpty(imgPath)){
				Bitmap deviceImg = BitmapFactory.decodeFile(device.getmTinyImgFilePath());
				mViewHolder.mDeviceImg.setImageDrawable(new BitmapDrawable(context.getResources(),deviceImg));
			}else{
				mViewHolder.mDeviceImg.setImageDrawable(context.getResources().getDrawable(R.drawable.dev_init));
			}


		} else {
			mViewHolder.mDeviceName.setText("");
		}
		
		
		return convertView;
	}
	
	
 
	
	class ViewHolder {
		
		private TextView mDeviceName;

		private TextView mMessage;
		
		private ImageView mPoint ;
		
		private ImageView mDeviceImg ;

	}

	public List<Device> getDeviceList() {
		return mDeviceList;
	}

	public void setDeviceList(List<Device> DeviceList) {
		this.mDeviceList = DeviceList;
	}
	



}
