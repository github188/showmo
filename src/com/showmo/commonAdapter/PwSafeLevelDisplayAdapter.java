package com.showmo.commonAdapter;

import java.lang.reflect.Field;
import java.util.List;

import com.showmo.R;
import com.showmo.safe.Safe;
import com.showmo.safe.Safe.SafeType;
import com.showmo.util.LogUtils;

import android.content.Context;
import android.util.Log;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.TextView;

public class PwSafeLevelDisplayAdapter extends CommonAdapter<SafeType> {
	private Context m_context;
	private Safe m_disSafe;
	private PwOnCheckChangedListener m_checkLis;
	public PwSafeLevelDisplayAdapter(Context context,List<SafeType> dataList,int resourceId){
		super(context, dataList, resourceId);
		m_context=context;
		m_disSafe=new Safe();
		m_checkLis=new PwOnCheckChangedListener();
	}
	public Safe getDisSafe(){
		LogUtils.v("getDisSafe", m_disSafe.toString());
		return m_disSafe;
	}
	
	public class PwOnCheckChangedListener implements OnCheckedChangeListener{
		@Override
		public void onCheckedChanged(CompoundButton buttonView,
				boolean isChecked) {
			// TODO Auto-generated method stub
			Object objtag=buttonView.getTag();
			if(!(objtag instanceof SafeType)){
				return;
			}
			SafeType tagData=(SafeType)objtag;
			tagData.value=isChecked;
			String fieldname=tagData.fieldName;
			Class<Safe> safeClass=Safe.class;
			try {
				Field field=safeClass.getDeclaredField(fieldname);
				field.setAccessible(true);
				if(field.getGenericType().toString().equals("boolean")){
					field.setBoolean(m_disSafe, isChecked);
				}
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
				return;
			}
			
			
		}
	}
	public void displayViewLayout(AdapterViewHolder viewHolder,SafeType data){
		TextView details=(TextView)viewHolder.getView(R.id.safe_details);
		details.setText(data.fieldDescribe);
		CheckBox cb=(CheckBox)viewHolder.getView(R.id.select_checkbox);
		//cb.setEnabled(true);
		cb.setChecked(data.value);
		cb.setTag(data);
		cb.setOnCheckedChangeListener(m_checkLis);
		//LogUtils.v("details", "displayViewLayout "+data.value);
	}
}
