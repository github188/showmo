package com.showmo.commonAdapter;

import java.util.List;

import com.showmo.R;
import com.showmo.deviceManage.Device;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.widget.ImageView;
import android.widget.TextView;

public class PwSsidAdapter extends CommonAdapter<String> {
	private Context m_context;
	public PwSsidAdapter(Context context,List<String> dataList,int resourceId){
		super(context, dataList, resourceId);
		m_context=context;
	}
	public void displayViewLayout(AdapterViewHolder viewHolder,String data){
		TextView textView=(TextView)viewHolder.getView(R.id.ssid);
		textView.setText(data);
	}
}
