package com.showmo.widget;

import com.showmo.R;
import com.showmo.util.HexTrans;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.RelativeLayout.LayoutParams;

public class PwTipImgButton extends AbstractTipButton {
	public PwTipImgButton(Context context) {
		// TODO Auto-generated constructor stub
		this(context, null);
	}
	public PwTipImgButton(Context context,AttributeSet attrs) {
		// TODO Auto-generated constructor stub
		this(context, attrs,0);
	}
	public PwTipImgButton(Context context,AttributeSet attrs,int style) {
		// TODO Auto-generated constructor stub
		super(context, attrs, style);
		this.init(attrs);
	}
	private ImageView mContentSrc;
	private void init(AttributeSet attrs){
		TypedArray ta=getContext().obtainStyledAttributes(attrs,R.styleable.PwTipButton);
		Drawable tipsrc=ta.getDrawable(R.styleable.PwTipButton_tip_src);
		Drawable contentsrc=ta.getDrawable(R.styleable.PwTipButton_content_src);
		mContentSrc=new ImageView(getContext());
		LayoutParams lParams=new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		lParams.addRule(CENTER_IN_PARENT);
		addView(mContentSrc, lParams);
		mContentSrc.setBackground(contentsrc);

		mTipSrc.setBackground(tipsrc);
		ta.recycle();
	}
}
