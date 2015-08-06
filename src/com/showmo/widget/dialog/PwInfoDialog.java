package com.showmo.widget.dialog;

import java.util.HashMap;
import java.util.List;

import com.showmo.R;
import com.showmo.util.HexTrans;

import android.R.bool;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.support.v4.widget.SimpleCursorAdapter.ViewBinder;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;

public class PwInfoDialog extends PwDialog implements OnClickListener {
	private TextView m_titleTextView;
	private TextView m_conTextView;
	private Button m_cancelBtn;
	private Button m_okBtn;
	private LinearLayout m_btnLayout;
	private FrameLayout m_btnSepLine;
	private LayoutParams sepLp;
	private RelativeLayout m_contentLayout;
	private EditText m_inputView = null;
	private OnCancelClickListener m_cancelListener = null;
	private OnOkClickListener m_okListener = null;
	private final static int CONTENT_VIEW_ID = 123455;
	private boolean canleVisible;
	private boolean okVisible;
	
	public interface OnCancelClickListener {
		void onClick();
	}

	public interface OnOkClickListener {
		void onClick();
	}

	
	public PwInfoDialog setCancelBtnTextAndListener(String text,
			OnCancelClickListener listener) {
		if (text != null) {
			m_cancelBtn.setText(text);
		}
		m_cancelListener = listener;
		return this;
	}
	public PwInfoDialog setCancelBtnTextAndListener(int textId,
			OnCancelClickListener listener) {
		m_cancelBtn.setText(textId);
		m_cancelListener = listener;
		return this;
	}
	/*
	 * 设置Ok键的text和监听 null表示采用默认值
	 */
	public PwInfoDialog setOkBtnTextAndListener(String text,
			OnOkClickListener listener) {
		if (text != null) {
			m_okBtn.setText(text);
		}
		m_okListener = listener;
		return this;
	}

	public PwInfoDialog setOkBtnTextAndListener(int textId,
			OnOkClickListener listener) {
		m_okBtn.setText(textId);
		m_okListener = listener;
		return this;
	}
	
	public void setCanleVisible(boolean canleVisible) {
		this.canleVisible=canleVisible;
	}
	public boolean getCanleVisible(){
		return canleVisible;
	}
	public void setOkVisible(boolean okVisible) {
		this.okVisible=okVisible;
	}
	public boolean getOkVisible(){
		return okVisible;
	}
	/*
	 * 设置标题 默认标题：提示
	 */
	public PwInfoDialog setDialogTitle(String title) {
		m_titleTextView.setText(title);
		return this;
	}

	public PwInfoDialog setDialogTitle(int titleResId) {
		m_titleTextView.setText(titleResId);
		return this;
	}
	@Override
	public void setTitle(CharSequence title) {
		// TODO Auto-generated method stub
		super.setTitle(title);
		setDialogTitle(title.toString());
	}
	@Override
	public void setTitle(int titleId) {
		// TODO Auto-generated method stub
		super.setTitle(titleId);
		setDialogTitle(titleId);
	}
	/*
	 * 设置内容
	 */
	public PwInfoDialog setContentText(String content) {
		m_conTextView.setText(content);
		return this;
	}

	public PwInfoDialog setContentText(int resId) {

		m_conTextView.setText(resId);
		return this;
	}

	public PwInfoDialog removeCancelBtn() {
		m_cancelBtn.setVisibility(View.GONE);
		return this;
	}

	
	/*
	 * 从右边添加按钮
	 */
	public PwInfoDialog addBtn(int strId,final View.OnClickListener listener) {
		m_btnSepLine = new FrameLayout(getContext());
		m_btnSepLine.setBackgroundColor(Color.argb(255, 102, 102, 102));
		Button btn=new Button(getContext());
		btn.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				dismiss();
				listener.onClick(v);
			}
		});
		btn.setText(strId);
		btn.setTextColor(getContext().getResources().getColor(R.color.color_primary_black));
	//	btn.setTextSize(getContext().getResources().getDimension(R.dimen.dimen_showmo_text_tiny));
		btn.setBackgroundResource(R.drawable.dialog_btn_selector);// (getContext().getResources().getDrawable());
		LayoutParams lParams = new LayoutParams(LayoutParams.WRAP_CONTENT,
				LayoutParams.MATCH_PARENT, 1);
		m_btnLayout.addView(m_btnSepLine, sepLp);
		m_btnLayout.addView(btn, lParams);
		return this;
	}

	public PwInfoDialog(Context context) {
		super(context, R.style.PwDialog, R.layout.dialog_info);
		Window dialogWindow = this.getWindow();
		WindowManager.LayoutParams lp = dialogWindow.getAttributes();
		WindowManager wm = (WindowManager) getContext().getSystemService(
				Context.WINDOW_SERVICE);
		// lp.width=WindowManager.LayoutParams.MATCH_PARENT;
		dialogWindow.setGravity(Gravity.CENTER);
		lp.width = wm.getDefaultDisplay().getWidth()
				- HexTrans.dip2px(getContext(), 60);
		lp.height = WindowManager.LayoutParams.WRAP_CONTENT;
		dialogWindow.setAttributes(lp);
		this.setOnDismissListener(new OnDismissListener() {

			@Override
			public void onDismiss(DialogInterface dialog) {
				// TODO Auto-generated method stub
				if (m_cancelListener != null) {
					m_cancelListener.onClick();
				}
			}
		});
	}

	// public PwInfoDialog(Context context, int theme){
	// super(context,theme,R.layout.dialog_info);
	// }
	@Override
	public void setViewAndListener() {
		// TODO Auto-generated method stub
		m_titleTextView = (TextView) findViewById(R.id.dialog_title);
		m_conTextView = (TextView) findViewById(R.id.dialog_content);
		m_conTextView.setId(CONTENT_VIEW_ID);
		m_cancelBtn = (Button) findViewById(R.id.dialog_cancel);
		m_okBtn = (Button) findViewById(R.id.dialog_ok);
		m_btnLayout = (LinearLayout) findViewById(R.id.dialog_btn_layout);
		m_contentLayout = (RelativeLayout) findViewById(R.id.dialog_content_layout);
		sepLp = new LayoutParams(1, LayoutParams.MATCH_PARENT, 0);
		m_okBtn.setOnClickListener(this);
		m_cancelBtn.setOnClickListener(this);
	}

	/*
	 * 设置content为输入框
	 */
	public PwInfoDialog setInputMode(boolean bIsInput, String inputHint) {
		if (bIsInput) {
			if (m_inputView == null) {
				m_inputView = new EditText(getContext());
				m_inputView.setId(CONTENT_VIEW_ID);
			}
			m_inputView.setHint(inputHint);
			;
			RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(
					LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
			lp.addRule(RelativeLayout.CENTER_IN_PARENT);
			m_inputView.setBackgroundDrawable(null);
			m_inputView.setSingleLine();
			FrameLayout flayout = new FrameLayout(getContext());
			RelativeLayout.LayoutParams lpunderline = new RelativeLayout.LayoutParams(
					LayoutParams.MATCH_PARENT, 1);
			lpunderline.addRule(RelativeLayout.BELOW, m_inputView.getId());
			flayout.setBackgroundColor(Color.argb(255, 102, 102, 102));
			m_contentLayout.removeAllViews();
			m_contentLayout.addView(m_inputView, lp);
			m_contentLayout.addView(flayout, lpunderline);
		}
		return this;
	}

	public void setInputText(String str){
		if (m_inputView != null) {
			 m_inputView.setText(str);
		}
	}
	public String getInputText() {
		if (m_inputView != null) {
			return m_inputView.getText().toString();
		}
		return "";
	}

	public PwInfoDialog setInputMode(boolean bIsInput, int hIntRes) {
		String str = getContext().getResources().getString(hIntRes);
		return setInputMode(bIsInput, str);
	}

	/*
	 * 设置content为指定View
	 */
	public PwInfoDialog setDialogContentView(View v) {
		v.setId(CONTENT_VIEW_ID);
		RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(
				LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
		lp.addRule(RelativeLayout.CENTER_IN_PARENT);
		m_contentLayout.removeAllViews();
		m_contentLayout.addView(v, lp);

		return this;
	}

	public PwInfoDialog setDialogContentView(int viewLayoutId) {
		m_contentLayout.removeAllViews();
		LayoutInflater inflater = LayoutInflater.from(getContext());
		View v = inflater.inflate(viewLayoutId, m_contentLayout, false);
		v.setId(CONTENT_VIEW_ID);
		RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(
				LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
		lp.addRule(RelativeLayout.CENTER_IN_PARENT);
		m_contentLayout.addView(v, lp);
		return this;
	}

	public View getDialogContentView() {
		return findViewById(CONTENT_VIEW_ID);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub

		switch (v.getId()) {
		case R.id.dialog_cancel:
			if (m_cancelListener != null) {
				m_cancelListener.onClick();
			}
			break;
		case R.id.dialog_ok:
			if (m_okListener != null) {
				m_okListener.onClick();
			}
			break;
		default:
			break;
		}
		this.dismiss();
	}

}
