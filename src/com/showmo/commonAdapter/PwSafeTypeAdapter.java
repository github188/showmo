package com.showmo.commonAdapter;

import java.util.List;

import com.showmo.R;
import com.showmo.deviceManage.Device;
import com.showmo.safe.Safe.SafeType;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.support.v4.widget.SimpleCursorAdapter.ViewBinder;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.CheckBox;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.TextView;

public class PwSafeTypeAdapter extends CommonAdapter<SafeType>{
	private Context m_context;
	public PwSafeTypeAdapter(Context context,List<SafeType> dataList,int resourceId){
		super(context, dataList, resourceId);
		m_context=context;
	}

	
	public void displayViewLayout(AdapterViewHolder viewHolder,SafeType data){
		TextView details=(TextView)viewHolder.getView(R.id.safe_details);
		details.setText(data.fieldDescribe);
		CheckBox cb=(CheckBox)viewHolder.getView(R.id.select_checkbox);
		cb.setChecked(data.value);
		Log.v("details", "displayViewLayout "+data.value);
	}
}
