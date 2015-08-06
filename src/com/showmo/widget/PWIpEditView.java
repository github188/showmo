package com.showmo.widget;

import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;

import android.content.Context;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable.Orientation;
import android.text.Editable;
import android.text.Selection;
import android.text.Spannable;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;

public class PWIpEditView extends LinearLayout {
	EditText m_ip1;
	EditText m_ip2;
	EditText m_ip3;
	EditText m_ip4;

	public PWIpEditView(Context context) {
		// TODO Auto-generated constructor stub
		super(context);
		this.init();
	}

	public PWIpEditView(Context context, AttributeSet attrs) {
		super(context, attrs);
		this.init();
	}

	public PWIpEditView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		this.init();
	}

	class IpEditor extends EditText {
		public IpEditor(Context context) {
			// TODO Auto-generated constructor stub
			super(context);
			this.init();
		}

		public IpEditor(Context context, AttributeSet attrs) {
			super(context, attrs);
			this.init();
		}

		public IpEditor(Context context, AttributeSet attrs, int defStyle) {
			super(context, attrs, defStyle);
			this.init();
		}

		private void init() {
			setBackgroundDrawable(null);
			setInputType(EditorInfo.TYPE_CLASS_NUMBER);
			setText("0");
			setGravity(Gravity.CENTER);
			addTextChangedListener(new TextWatcher() {

				@Override
				public void onTextChanged(CharSequence s, int start,
						int before, int count) {
					// TODO Auto-generated method stub
					if (s.length() > 3) {
						s = s.subSequence(0, 3);
						setText(s);
					}
					
					if (s.length() > 0) {
						if (Integer.valueOf(s.toString()) > 255) {
							s = "255";
							setText(s);
						}
					}
					
					setSelection(length());
					
				}

				@Override
				public void beforeTextChanged(CharSequence s, int start,
						int count, int after) {
					// TODO Auto-generated method stub
					
					
					//setText("");
//					if(s.length()==1  ){
//						if(Integer.valueOf(s.toString())==0){
//							setText("");
//						}
//					}
				}

				@Override
				public void afterTextChanged(Editable s) {
					// TODO Auto-generated method stub
					
				}
			});
			setOnFocusChangeListener(new OnFocusChangeListener() {
				
				@Override
				public void onFocusChange(View v, boolean hasFocus) {
					// TODO Auto-generated method stub
					if(hasFocus){
						LogUtils.v("tes",hasFocus+getText().toString());
						if(length()==1){
							if(Integer.valueOf(getText().toString())==0){
								setText("");
							}
						}
					}else{
						if(length()==0){
							setText("0");
						}
					}
				}
			});
		}
	}

	private void init() {
		this.setOrientation(VERTICAL);
		LinearLayout ipLin = new LinearLayout(this.getContext());
		ipLin.setOrientation(HORIZONTAL);
		LayoutParams linLp = new LayoutParams(LayoutParams.MATCH_PARENT,
				LayoutParams.MATCH_PARENT, 1);
		m_ip1 = new IpEditor(this.getContext());
		m_ip2 = new IpEditor(this.getContext());
		m_ip3 = new IpEditor(this.getContext());
		m_ip4 = new IpEditor(this.getContext());
		int pading = HexTrans.dip2px(this.getContext(), 1);
		m_ip1.setPadding(0, 0, pading, 0);
		m_ip2.setPadding(pading, 0, pading, 0);
		m_ip3.setPadding(pading, 0, pading, 0);
		m_ip4.setPadding(pading, 0, 0, 0);
		LinearLayout.LayoutParams editLp = new LayoutParams(
				LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT, 1);
		ipLin.addView(m_ip1, editLp);
		LayoutParams pointLp = new LayoutParams(LayoutParams.WRAP_CONTENT,
				LayoutParams.WRAP_CONTENT);
		TextView v1 = new TextView(this.getContext());
		v1.setText(".");
		v1.setTextSize(25);
		v1.setTextColor(Color.BLACK);
		ipLin.addView(v1, pointLp);
		ipLin.addView(m_ip2, editLp);
		v1 = new TextView(this.getContext());
		v1.setText(".");
		v1.setTextSize(25);
		v1.setTextColor(Color.BLACK);
		ipLin.addView(v1, pointLp);
		ipLin.addView(m_ip3, editLp);
		v1 = new TextView(this.getContext());
		v1.setText(".");
		v1.setTextColor(Color.BLACK);
		v1.setTextSize(25);
		ipLin.addView(v1, pointLp);
		ipLin.addView(m_ip4, editLp);
		FrameLayout underline = new FrameLayout(this.getContext());
		underline.setBackgroundColor(Color.argb(155, 0, 0, 0));
		this.addView(ipLin, linLp);
		LayoutParams underLp = new LayoutParams(LayoutParams.MATCH_PARENT, 1, 0);
		this.addView(underline, underLp);
	}
	public String getIp(){
		return m_ip1.getText().toString()+"."+m_ip2.getText().toString()+"."+m_ip3.getText().toString()+"."+m_ip4.getText().toString();
	}
}
