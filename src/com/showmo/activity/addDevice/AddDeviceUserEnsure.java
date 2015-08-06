package com.showmo.activity.addDevice;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.util.PwNetWorkHelper;
import com.showmo.widget.dialog.PwInfoDialog;
import com.showmo.widget.dialog.PwInfoDialog.OnOkClickListener;

import android.content.Intent;
import android.graphics.Paint;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class AddDeviceUserEnsure extends BaseActivity implements
		View.OnClickListener {
	private TextView m_haveNotGetTip;
	private Button m_nextBtn;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_add_device_user_ensure);
		init();
	}

	private void init() {
		//setBarTitleWithRightBtn(R.string.add_device_ensure_title);
		setBarTitle(R.string.add_device_ensure_title);
		m_haveNotGetTip=(TextView)this.findViewById(R.id.add_device_ensure_can_not_get_tip);
		m_haveNotGetTip.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
		m_haveNotGetTip.setOnClickListener(this);
		m_nextBtn=(Button)findViewById(R.id.btn_user_sure_next);
		m_nextBtn.setOnClickListener(this);
		
	}
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		PwNetWorkHelper.destoryInstance();
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.btn_bar_back:
			onBackPressed();
			break;
		case R.id.btn_user_sure_next:
			//PwNetWorkHelper netHelper=PwNetWorkHelper.getInstance(getApplicationContext());
			Intent intent=new Intent(this,AddDeviceSetNetworkActivity.class);
			this.startActivity(intent);
			slideInFromRight();
			finish();
			break;
		case R.id.add_device_ensure_can_not_get_tip:
			Intent intent2=new Intent(this,AddDeviceWithoutTipActivity.class);
			this.startActivity(intent2);
			break;
		default:
			break;
		}
	}

}
