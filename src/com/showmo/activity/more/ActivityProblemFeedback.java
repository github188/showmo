package com.showmo.activity.more;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.base.ShowmoSystem;
import com.showmo.util.LogUtils;
import com.showmo.util.MailUtils;
import com.showmo.util.PathUtils;
import com.showmo.util.StringUtil;
import com.showmo.util.ToastUtil;

import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class ActivityProblemFeedback extends BaseActivity {
	private EditText m_titleEditText;
	private EditText m_contentText;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_problem_feedback);
		findView();
	}

	private void findView() {
		setBarTitleWithBackFunc(R.string.more_problem_feedback);
		findViewAndSet(R.id.problem_btn_send);
		m_titleEditText = (EditText) findViewById(R.id.problem_feed_back);
		m_contentText = (EditText) findViewById(R.id.problem_describ);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		super.onClick(v);
		switch (v.getId()) {
		case R.id.problem_btn_send:
			String content=m_contentText.getText().toString();
			if(StringUtil.isNotEmpty(content)){
				
				Runnable runnable=new Runnable(){
					public void run() {
	
						StringBuilder stringBuilder=new StringBuilder();
						stringBuilder.append("###Username:")
						.append(ShowmoSystem.getInstance().getCurUser().getUserName()+"###\n")
						.append("###Contact information:")
						.append(m_titleEditText.getText().toString()+"###\n")
						.append(m_contentText.getText().toString());
						String sendContent=stringBuilder.toString();
						String[] filesStrings={LogUtils.LogAppFile,LogUtils.LogSdkFile};
						boolean bres=MailUtils.sendMail("problemFeedback", 
								sendContent, 
								MailUtils.InternalEmailAddr, 
								MailUtils.InternalEmailAddrPsw, 
								MailUtils.InternalEmailAddr, 
								filesStrings);
						if(bres){
							ToastUtil.toastShort(ActivityProblemFeedback.this, R.string.more_feedback_suc);
						}else{
							
							ToastUtil.toastShort(ActivityProblemFeedback.this, R.string.more_feedback_err);
						}
					};
				};
				ShowmoSystem.getInstance().sendMailInThread(runnable);
				ToastUtil.toastShort(ActivityProblemFeedback.this, R.string.more_thanks_for_feedback);
				finish();
				slideInFromLeft();
			}else {
				ToastUtil.toastShort(ActivityProblemFeedback.this, R.string.more_problem_describ_need);
			}
			break;

		default:
			break;
		}
	}
}
