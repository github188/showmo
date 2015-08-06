package com.showmo.widget;

import java.util.jar.Attributes;

import com.showmo.R;
import com.showmo.util.HexTrans;
import com.showmo.util.LogUtils;

import android.R.integer;
import android.R.interpolator;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.support.v4.widget.SimpleCursorAdapter.ViewBinder;
import android.text.Editable;
import android.text.InputFilter;
import android.text.InputType;
import android.text.Spanned;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.KeyListener;
import android.text.method.NumberKeyListener;
import android.text.method.PasswordTransformationMethod;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class PWEditCompoundView extends RelativeLayout implements View.OnClickListener {
	private ImageView leftImg;
	private ImageView rightImg;
	private PWFilterEditView editText;
	private Drawable leftSrc;
	private Drawable rightSrc;
	private boolean leftImgVisible;
	private boolean rightImgVisible;
	private final int RIGHTIMGID=10000001;
	private final int LEFTIMGID=10000002;
	private RelativeLayout bgLayout;
	private LinearLayout underline;
	private String m_regEx;
	
	private OnRightImgClickListener m_rightImgClickListener;
	private OnLeftImgClickListener m_leftImgClickListener;
	
	public interface OnRightImgClickListener{
		void onRightImgClickListener(View v);
	}
	public interface OnLeftImgClickListener{
		void onLeftImgClickListener(View v);
	}
	public void setOnRightImgClickListener(OnRightImgClickListener listener){
		m_rightImgClickListener=listener;
	}
	public void setOnLeftImgClickListener(OnLeftImgClickListener listener){
		m_leftImgClickListener=listener;
	}
	
	public PWEditCompoundView(Context context){
		super(context);
		this.init();
	}
	
	public PWEditCompoundView(Context context,AttributeSet arrts){
		super(context,arrts);
		TypedArray tarr=context.obtainStyledAttributes(arrts, R.styleable.PwEditView);
		bgLayout=new RelativeLayout(context);
		LayoutParams bgLinLp=new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		//bgLayout.setPadding(0, HexTrans.dip2px(context, 15), 0, HexTrans.dip2px(context, 15));
	    leftImgVisible=tarr.getBoolean(R.styleable.PwEditView_leftIconVisible,true);
		if(leftImgVisible){
			leftSrc=tarr.getDrawable(R.styleable.PwEditView_leftIconSrc);
			leftImg=new ImageView(context);
			leftImg.setBackgroundDrawable(leftSrc);
			
			int leftHsize=tarr.getDimensionPixelSize(R.styleable.PwEditView_leftIconHsize, LayoutParams.WRAP_CONTENT);
			int leftWsize=tarr.getDimensionPixelSize(R.styleable.PwEditView_leftIconWsize, LayoutParams.WRAP_CONTENT);
			
			FrameLayout leftFrame=new FrameLayout(getContext());
			FrameLayout.LayoutParams llp=new FrameLayout.LayoutParams(leftWsize, leftHsize, Gravity.CENTER);
			leftFrame.addView(leftImg,llp);
			LayoutParams lframLp=new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.MATCH_PARENT);
			lframLp.addRule(RelativeLayout.CENTER_VERTICAL);
			lframLp.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			bgLayout.addView(leftFrame,lframLp);
			leftFrame.setId(LEFTIMGID);
			leftFrame.setOnClickListener(this);
		}
		
	    
	    rightImgVisible=tarr.getBoolean(R.styleable.PwEditView_rightIconVisible,true);
	    if(rightImgVisible){
	    	rightSrc=tarr.getDrawable(R.styleable.PwEditView_rightIconSrc);
	    	rightImg=new ImageView(context);
	    	rightImg.setBackgroundDrawable(rightSrc);
	    	
			int rightHsize=tarr.getDimensionPixelSize(R.styleable.PwEditView_rightIconHsize, LayoutParams.WRAP_CONTENT);
			int rightWsize=tarr.getDimensionPixelSize(R.styleable.PwEditView_rightIconWsize, LayoutParams.WRAP_CONTENT);
			
			FrameLayout rightFrame=new FrameLayout(getContext());
			rightFrame.setId(RIGHTIMGID);
			
			FrameLayout.LayoutParams rlp=new FrameLayout.LayoutParams(rightWsize, rightHsize, Gravity.CENTER);
			rightFrame.addView(rightImg,rlp);
			LayoutParams rframLp=new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.MATCH_PARENT);
			rframLp.addRule(RelativeLayout.CENTER_VERTICAL);
			rframLp.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
			bgLayout.addView(rightFrame,rframLp);
			
			rightFrame.setOnClickListener(this);
		}
	    editText=new PWFilterEditView(context);
	    editText.setSingleLine();
	    boolean isPsw=tarr.getBoolean(R.styleable.PwEditView_pswMode, false);
	    if(isPsw){
	    	LogUtils.v("PSW", "isPsw =TRUE");
	    	//editText.setInputType(EditorInfo.TYPE_TEXT_VARIATION_PASSWORD);
	    	editText.setTransformationMethod(PasswordTransformationMethod.getInstance());
	    	//editText.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
	    }
	    editText.setBackgroundColor(Color.argb(0,0,0,0));
	    
	    int maxLen=tarr.getInteger(R.styleable.PwEditView_maxLength, -1);
	    if(maxLen>=0){
	    	editText.setFilters(new InputFilter[]{new InputFilter.LengthFilter(maxLen)});
	    }
	    String hintStr=tarr.getString(R.styleable.PwEditView_hintText);
	    editText.setHint(hintStr);
	    editText.setImeOptions(EditorInfo.IME_ACTION_DONE);
	    
	    String str=tarr.getString(R.styleable.PwEditView_text);
	    editText.setText(str);
	    String reg=tarr.getString(R.styleable.PwEditView_regex);  
	    if(reg!=null){
	    	m_regEx=reg;
	    }else{
	    	m_regEx="([/s/S]*)";
	    }
	    LayoutParams editLP=new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
	    if(leftImgVisible){
	    	editLP.addRule(RelativeLayout.RIGHT_OF,LEFTIMGID);
	    }
	    if(rightImgVisible){
	    	editLP.addRule(RelativeLayout.LEFT_OF,RIGHTIMGID);
	    }
	    editLP.addRule(RelativeLayout.CENTER_VERTICAL);
	    //editLP.bottomMargin=HexTrans.dip2px(context, 15);
	    
	    bgLayout.addView(editText,editLP);
	    
	    
	    addView(bgLayout,bgLinLp);
	    underline=new LinearLayout(context);
	    int underlineColor=tarr.getColor(R.styleable.PwEditView_underlineColor, Color.argb(255, 12, 12, 12));
	    underline.setBackgroundColor(underlineColor);
	    LayoutParams underLp=new LayoutParams(LayoutParams.MATCH_PARENT, 1);
	    underLp.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
	    addView(underline,underLp);
		this.init();
	}
	public void setText(String str){
		editText.setText(str);
	}
	public void setPswVisible(boolean bVisible){
		if(bVisible){
			editText.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
		}else{
			editText.setTransformationMethod(PasswordTransformationMethod.getInstance());
		}
		editText.setSelection(editText.length());
	}
	public PWEditCompoundView(Context context,AttributeSet arrts,int defStyle){
		super(context,arrts,defStyle);
		this.init();
	}
	public void init(){
		m_rightImgClickListener=null;
		m_leftImgClickListener=null;
	}
	public boolean accepted(){
		LogUtils.v("reg", "reg  "+m_regEx);
		return editText.getText().toString().matches(m_regEx);
	}
	public EditText getEditText(){
		return editText;
	}
	public ImageView getRightButton(){
		return rightImg;
	}
	@Override
	public void onClick(View v){
		switch (v.getId()) {
		case RIGHTIMGID:
			if(m_rightImgClickListener!=null){
				m_rightImgClickListener.onRightImgClickListener(rightImg);
			}
			break;
		case LEFTIMGID:
			if(m_leftImgClickListener!=null){
				m_leftImgClickListener.onLeftImgClickListener(leftImg);
			}
			break;
		default:
			break;
		}
	}
	
}
