package com.showmo.commonAdapter;

import java.util.List;

import com.showmo.R;
import com.showmo.dataDef.PWDeviceInfo;
import com.showmo.deviceManage.Device;
import com.showmo.util.LogUtils;
import com.showmo.util.StringUtil;

import android.R.integer;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.widget.ImageView;
import android.widget.TextView;
public class PWDevAdapter extends CommonAdapter<Device> {
	private Context m_context;
	public PWDevAdapter(Context context,List<Device> dataList,int resourceId){
		super(context, dataList, resourceId);
		m_context=context;
	}
	public void displayViewLayout(AdapterViewHolder viewHolder,Device data){
		TextView textView=(TextView)viewHolder.getView(R.id.device_name_text);
		
		ImageView imgBg=(ImageView)viewHolder.getView(R.id.device_tiny_img);
		ImageView imgOnline=(ImageView)viewHolder.getView(R.id.device_online_img);
//		if(data.m_cameraId==0){
//			textView.setText(m_context.getResources().getString(R.string.add_device));
//			imgBg.setImageDrawable(m_context.getResources().getDrawable(R.drawable.dev_add_press));
//			imgOnline.setAlpha(0);
//		}else{
		//LogUtils.e("thumbnail", "displayViewLayout cameraid "+data.getmCameraId()+data.getmTinyImgFilePath());
			textView.setText(data.getmDeviceName());
			if(!StringUtil.isNotEmpty(data.getmTinyImgFilePath())){
				imgBg.setImageResource(R.drawable.dev_init);
			}else{
				//LogUtils.e("thumbnail", "displayViewLayout setImageDrawable "+data.getmCameraId());
				Drawable drawable=new BitmapDrawable(data.getmTinyImgFilePath());
				imgBg.setImageDrawable(drawable);
			}
			imgOnline.setAlpha(255);
			imgOnline.bringToFront();
			if(data.ismOnlineState()){
				imgOnline.setImageDrawable(m_context.getResources().getDrawable(R.drawable.online));
			}else{
				imgOnline.setImageDrawable(m_context.getResources().getDrawable(R.drawable.underline));
			}
//		}
	}
}
