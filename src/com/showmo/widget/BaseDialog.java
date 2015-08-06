package com.showmo.widget;

import com.showmo.R;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;

public abstract class BaseDialog extends Dialog {
	protected int m_layoutId;

	public BaseDialog(Context context, int theme,int layoutId) {
		// TODO Auto-generated constructor stub
		super(context, theme);
		this.m_layoutId=layoutId;
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(m_layoutId);
		findViewAndSet();
	}
	protected abstract void findViewAndSet();
}
