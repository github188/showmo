package com.showmo.widget;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.RelativeLayout.LayoutParams;

import com.showmo.R;
import com.showmo.util.HexTrans;

public class AbstractTipButton extends RelativeLayout{
	public AbstractTipButton(Context context) {
		// TODO Auto-generated constructor stub
		this(context, null);
	}
	public AbstractTipButton(Context context,AttributeSet attrs) {
		// TODO Auto-generated constructor stub
		this(context, attrs,0);
	}
	public AbstractTipButton(Context context,AttributeSet attrs,int style) {
		// TODO Auto-generated constructor stub
		super(context, attrs, style);
		this.init(attrs);
	}
	protected ImageView mTipSrc;
	private void init(AttributeSet attrs){
	//(getContext().getResources().getDimension(R.dimen.dimen_showmo_text_medium));
		LayoutParams lParamsTipSrc=new LayoutParams(HexTrans.dip2px(getContext(), 10), 
										HexTrans.dip2px(getContext(), 10));
		lParamsTipSrc.addRule(ALIGN_PARENT_TOP);
		lParamsTipSrc.addRule(ALIGN_PARENT_RIGHT);
		mTipSrc=new ImageView(getContext());
		mTipSrc.setVisibility(View.GONE);
		addView(mTipSrc,lParamsTipSrc);
	}
	public void showTipImg(){
		mTipSrc.setVisibility(View.VISIBLE);
	}
	public void hideTipImg(){
		mTipSrc.setVisibility(View.GONE);
	}
	public boolean isTipShow(){
		return mTipSrc.getVisibility()==View.VISIBLE;
	}
}
