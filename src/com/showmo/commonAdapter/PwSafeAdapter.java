package com.showmo.commonAdapter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;
import java.util.Map.Entry;

import com.showmo.R;
import com.showmo.deviceManage.Device;
import com.showmo.safe.Safe;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

public class PwSafeAdapter extends CommonAdapter<Safe> implements View.OnClickListener{
	private Context m_context;
	private LevelCheckListener m_LevelCheckListener;
	private OnLevelDetailsClick m_LevelDetailsListener=null;
	public PwSafeAdapter(Context context, List<Safe> dataList, int resourceId) {
		super(context, dataList, resourceId);
		m_context = context;
		m_LevelCheckListener=new LevelCheckListener();
	}
	public interface OnLevelDetailsClick{
		void onLevelDetailsClick(Safe data);
	}
	public void setOnLevelDetailsClick(OnLevelDetailsClick lis){
		m_LevelDetailsListener=lis;
	}
	public class LevelCheckListener implements OnCheckedChangeListener{
		@Override
		public void onCheckedChanged(CompoundButton buttonView,
				boolean isChecked) {
			// TODO Auto-generated method stub
			if(isChecked){
				for (int i = 0; i < m_dataList.size(); i++) {
					if(m_dataList.get(i)==buttonView.getTag()){
						m_curItemPos=i;
					}
				}
				PwSafeAdapter.this.notifyDataSetChanged();
			}
		}
	}
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.describe:
			if(v instanceof LinearLayout){
				if(m_LevelDetailsListener!=null){
					m_LevelDetailsListener.onLevelDetailsClick((Safe)v.getTag());
				}
			}
			break;

		default:
			break;
		}
	}
	
	public void displayViewLayout(AdapterViewHolder viewHolder, Safe data) {
		TextView tvTitle = (TextView) viewHolder.getView(R.id.safe_level_title);
		tvTitle.setText(data.safeName);
		TextView sumTitle = (TextView) viewHolder
				.getView(R.id.safe_level_summary);
		if (data.SysLevel) {
			sumTitle.setText(R.string.safe_level_sys_summ);
		} else {
			sumTitle.setText(R.string.safe_level_dis_summ);
		}
		RadioButton rbtn = (RadioButton) viewHolder.getView(R.id.select_radio);
		//Log.v("cg", "displayViewLayout "+data.selected+" "+data.safeName);
		//rbtn.setSelected(data.selected);
		Safe curSafe=getCurData();
		if(curSafe!=null){
			if(curSafe.safeName.equals(data.safeName)){
				rbtn.setChecked(true);
			}else{
				rbtn.setChecked(false);
			}
		}else{
			rbtn.setChecked(false);
		}
		
		rbtn.setTag(data);
		rbtn.setOnCheckedChangeListener(m_LevelCheckListener);
		
		LinearLayout describeLy=(LinearLayout)viewHolder.getView(R.id.describe);
		describeLy.setTag(data);
		describeLy.setOnClickListener(PwSafeAdapter.this);
	}

}
