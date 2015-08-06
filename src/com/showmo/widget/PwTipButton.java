package com.showmo.widget;

import com.showmo.R;
import com.showmo.R.style;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;

import android.R.anim;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class PwTipButton extends AbstractTipButton {
	public PwTipButton(Context context) {
		// TODO Auto-generated constructor stub
		this(context, null);
	}
	public PwTipButton(Context context,AttributeSet attrs) {
		// TODO Auto-generated constructor stub
		this(context, attrs,0);
	}
	public PwTipButton(Context context,AttributeSet attrs,int style) {
		// TODO Auto-generated constructor stub
		super(context, attrs, style);
		this.init(attrs);
	}
	private TextView mContentText;
	private void init(AttributeSet attrs){
		TypedArray ta=getContext().obtainStyledAttributes(attrs,R.styleable.PwTipButton);
		Drawable tipsrc=ta.getDrawable(R.styleable.PwTipButton_tip_src);//getResourceId(R.styleable.PwTipButton_tip_src, 0);
		String text=ta.getString(R.styleable.PwTipButton_content_text);
		
		mContentText=new TextView(getContext());
		LayoutParams lParams=new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		lParams.addRule(CENTER_IN_PARENT);
		addView(mContentText, lParams);
		mContentText.setText(text);
		mContentText.setTextColor(Color.BLACK);
		mContentText.setTextAppearance(getContext(), R.style.style_showmo_text_medium);//(getContext().getResources().getDimension(R.dimen.dimen_showmo_text_medium));
		mContentText.setTextColor(getResources().getColor(R.color.color_primary_black));
		mTipSrc.setBackground(tipsrc);
		mContentText.bringToFront();
		ta.recycle();
		mTipSrc.setVisibility(View.GONE);
	}
}
