package com.showmo.activity.register;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;




import com.showmo.R;
import com.showmo.base.BaseActivity;

import android.os.Bundle;
import android.text.Html;
import android.widget.TextView;

public class AgreementActivity extends BaseActivity{

	private TextView tvContent;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_agreement);
		initView();
		tvContent.setText(Html.fromHtml( getContent()));
		
		
	}

	private void initView() {
		setBarTitle(R.string.software_use_agreement);
		findViewById(R.id.btn_bar_back).setOnClickListener(this);
		tvContent = (TextView)findViewById(R.id.tv_agree_content);
		
	}
	
	private String getContent(){
		StringBuffer strBuffer = null ;
		BufferedReader bufferedReader = null;
		try {
			InputStream in = getAssets().open("agreement_unicode.txt");
			bufferedReader = new BufferedReader(
					new InputStreamReader(in,"unicode"));  
			strBuffer = new StringBuffer();
			String temp ;
			while(  (temp=bufferedReader.readLine()) != null  ){
				strBuffer.append(temp);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}finally{
			if(bufferedReader != null){
				try {
					bufferedReader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}finally{
					bufferedReader = null;
				}
			}
				
		}
 
		return new String(strBuffer);
	}
	
	 @Override
	protected void onClick(int viewId) {
		 switch (viewId) {
		case R.id.btn_bar_back:
			onBackPressed();
			break;

		default:
			break;
		}
	 
	 
	 }
	
	
}
