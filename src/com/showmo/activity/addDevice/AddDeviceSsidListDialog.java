package com.showmo.activity.addDevice;

import com.showmo.R;
import com.showmo.widget.dialog.PwDialog;

import android.R.interpolator;
import android.content.Context;
import android.view.View;
import android.widget.Button;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;

public class AddDeviceSsidListDialog extends PwDialog {
	private ListView m_ssidListView;
	private Button m_preBtn;
	private Button m_nextBtn;
	private Button m_searchBtn;

	public AddDeviceSsidListDialog(Context context) {
		super(context, R.style.PwDialog, R.layout.dialog_add_device_ssid_list);
	}

	@Override
	public void setViewAndListener() {
		// TODO Auto-generated method stub
		m_ssidListView = (ListView) findViewById(R.id.add_device_search_ssid_list);
		m_preBtn = (Button) findViewById(R.id.btn_previou);
		m_nextBtn = (Button) findViewById(R.id.btn_next);
		m_searchBtn = (Button) findViewById(R.id.btn_research);
	}
	public int getPreBtnId(){
		return m_preBtn.getId();
	}
	public int getNextBtnId(){
		return m_nextBtn.getId();
	}
	public int getSearchBtnId(){
		return m_searchBtn.getId();
	}
	public void setPreListener(View.OnClickListener listener) {
		m_preBtn.setOnClickListener(listener);
	}

	public void setNextListener(View.OnClickListener listener) {
		m_nextBtn.setOnClickListener(listener);
	}

	public void setResearchListener(View.OnClickListener listener) {
		m_searchBtn.setOnClickListener(listener);
	}
	public void setSsidListAdapter(ListAdapter adapter){
		m_ssidListView.setAdapter(adapter);
	}
	public ListView getSsidListView(){
		return m_ssidListView;
	}
}
