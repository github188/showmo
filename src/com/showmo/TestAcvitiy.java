package com.showmo;

import java.util.Random;

import com.showmo.base.BaseActivity;
import com.showmo.widget.PwSoundView;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;

public class TestAcvitiy extends BaseActivity {
	private View.OnClickListener mLis=null;
	private boolean flag=true;
	private Thread mThread=null;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_test);
		testBtn=(Button)findViewById(R.id.testBtn);
		soundView=(PwSoundView)findViewById(R.id.testSound);
			mLis=new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					//soundView.startTest();
					//soundView.setVolum(100);
					if(mThread!=null){
						flag=false;
					}
					mThread=new Thread(){
						public void run() {
							while (flag) {
								soundView.setVolum((int)(Math.random()*50));
								try {
									Thread.sleep(200);
								} catch (Exception e) {
									// TODO: handle exception
								}
								
							}
						};
					};
					mThread.start();
				}
			};
			testBtn.setOnClickListener(mLis);
		
		
	}
	Button testBtn;
	PwSoundView soundView;
}
