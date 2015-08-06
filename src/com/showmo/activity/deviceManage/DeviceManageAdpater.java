
package com.showmo.activity.deviceManage;


import ipc365.app.showmo.jni.JniDataDef.CAMERA_ALARM_TYPE;

import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.deviceManage.Device;
import com.showmo.deviceManage.Device.AlarmSwitch;
import com.showmo.util.StringUtil;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;


//DeviceManageActivity
public class DeviceManageAdpater extends BaseAdapter {


	private List<Device> mDeviceList;

	private Activity context;

	private int selectPos = -1 ;


	private ArrayList<View> mConvertViews = new ArrayList<View>();

	/**
	 * item被点击时调用，设置item背景色
	 * @param pos item的position
	 * @param view 被点击的view
	 */
	public void updateColor(int pos,View view){

		selectPos = pos ;
		for(int i=0;i<mConvertViews.size();i++){
			mConvertViews.get(i).findViewById(R.id.lin_dev_container).setBackgroundColor(Color.WHITE);
		}
		view.findViewById(R.id.lin_dev_container).setBackgroundColor(Color.GRAY);
	}
	/**
	 * 将当前页面中的item颜色恢复
	 */
	public void resetColor(){
		selectPos = -1 ;
		for(int i=0;i<mConvertViews.size();i++){
			mConvertViews.get(i).findViewById(R.id.lin_dev_container).setBackgroundColor(Color.WHITE);
		}
	}


	public DeviceManageAdpater(List<Device> DeviceList, Activity context) {
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
			convertView = LayoutInflater.from(context).inflate(R.layout.item_device_manage,
					null);

			mViewHolder = new ViewHolder();

			mViewHolder.mDeviceName = (TextView)convertView.findViewById(R.id.tv_dev_name);
			mViewHolder.mDeviceUuid = (TextView)convertView.findViewById(R.id.tv_dev_uuid);
			mViewHolder.mDeviceImg = (ImageView)convertView.findViewById(R.id.img_dev_pic);
			mViewHolder.mDeviceUpgrade = (TextView)convertView.findViewById(R.id.tv_dev_firmware_update);
			mViewHolder.mDeviceAlarmSwitch = (TextView)convertView.findViewById(R.id.tv_dev_alarmswitch);
			
			
			convertView.setTag(mViewHolder);
			mConvertViews.add(convertView);
		} else {
			mViewHolder = (ViewHolder)convertView.getTag();
		}
		if(mDeviceList == null){
			return convertView ;
		}
		Device device = mDeviceList.get(position);
		if (device != null) {
			mViewHolder.mDeviceName.setText(device.getmDeviceName() );
			mViewHolder.mDeviceUuid.setText(device.getmUuid() );

			
			if(device.ismUpgrading()){
				mViewHolder.mDeviceUpgrade.setText(R.string.is_upgrading);
			}else{
				if(StringUtil.isNotEmpty(device.getmVersion())){
					mViewHolder.mDeviceUpgrade.setText(R.string.firmware_can_upgrade);
				}else{
					mViewHolder.mDeviceUpgrade.setText(R.string.temporarily_not_have_upgrade);
				}
			}
			
			if(device.ismSwitchStateValid()){
				List<AlarmSwitch> alarmSwitchs=device.getmAlarmSwitchs();
				for (int i = 0; i < alarmSwitchs.size(); i++) {
					if(alarmSwitchs.get(i).cameraAlarmType == CAMERA_ALARM_TYPE.ALARM_TYPE_DETECTION_MOTION){
						if(alarmSwitchs.get(i).value){
							mViewHolder.mDeviceAlarmSwitch.setText(R.string.switch_on);
						}else{
							mViewHolder.mDeviceAlarmSwitch.setText(R.string.switch_off);
						}
					}
				}
			}else{
				mViewHolder.mDeviceAlarmSwitch.setText(R.string.unavailable);
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
			mViewHolder.mDeviceUuid.setText("");
		}
		
		if(position == selectPos){
			convertView.findViewById(R.id.lin_dev_container).setBackgroundColor(Color.GRAY);
		}else{
			convertView.findViewById(R.id.lin_dev_container).setBackgroundColor(Color.WHITE);
		}
		
		return convertView;
	}

	class ViewHolder {

		private TextView mDeviceName;

		private TextView mDeviceUuid;

		private ImageView mDeviceImg ;
		
		private TextView mDeviceUpgrade ;
		
		private TextView mDeviceAlarmSwitch ;

	}

	public List<Device> getDeviceList() {
		return mDeviceList;
	}

	public void setDeviceList(List<Device> DeviceList) {
		this.mDeviceList = DeviceList;
		selectPos = -1 ;
	}
	

	public int getSelectPos() {
		return selectPos;
	}

	public void setSelectPos(int selectPos) {
		this.selectPos = selectPos;
	}



}
