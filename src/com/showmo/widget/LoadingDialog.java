package com.showmo.widget;


import com.showmo.R;
import com.showmo.util.LogUtils;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

/**
 * 加载弹出框
 * 常用方法：
 * setHint()设置提示
 * 
 * @author Terry
 *
 */

public class LoadingDialog extends Dialog   {

	Context context;
	private TextView mTvHint;
	private ImageView mImgPic;
	private Button mCancelBtn;
	private int mHintResid;

	private View.OnClickListener mCancelClick;
	public LoadingDialog(Context context) {
		super(context);
		this.context = context;
	}
	public LoadingDialog(Context context, int theme){
		super(context, theme);
		this.context = context;
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.e("LoadingDialog", "onCreate");
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.dialog_loading);
		initView();
	}

	private void initView(){
		mTvHint =  (TextView)findViewById(R.id.tv_load_hint);
		mImgPic = (ImageView)findViewById(R.id.img_load_pic);
		mCancelBtn=(Button)findViewById(R.id.btn_cancel_loading);
		if(mCancelClick!=null){
			mCancelBtn.setVisibility(View.VISIBLE);
			mCancelBtn.setOnClickListener(mCancelClick);
		}
		LogUtils.e("loading", "mCancelBtn "+mCancelBtn);
		startAnim();
		mTvHint.setText(mHintResid);
	}

	public void setHint(int hintResid){
		mHintResid = hintResid ;
		if(mTvHint != null){
			mTvHint.setText(hintResid);

		}
	}

	private void startAnim(){
		Animation mAnimation = AnimationUtils.loadAnimation(context, R.anim.dialog_rotate);
		mImgPic.startAnimation(mAnimation);
	}


	@Override
	public void show() {
		if(mImgPic != null){
			startAnim();
		}
		super.show();
	}
	public void enableCancelBtn(View.OnClickListener lis){
		if(mCancelBtn ==null){
			mCancelClick = lis;
		}else {
			mCancelBtn.setVisibility(View.VISIBLE);
			mCancelBtn.setOnClickListener(lis);
		}
	}
	public void disableCancelBtn(){
		if(mCancelBtn!=null){
			mCancelBtn.setVisibility(View.GONE);
			mCancelBtn.setOnClickListener(null);
		}
		mCancelClick=null;
	}

}
