package com.showmo.widget.dialog;

import android.app.Dialog;
import android.content.Context;

public abstract class PwDialog extends Dialog {
	protected int m_layoutId;
	public PwDialog(Context context,int layoutId) {
		super(context);
		m_layoutId=layoutId;
		init();
	}

	public PwDialog(Context context, int theme,int layoutId) {
		super(context, theme);
		m_layoutId=layoutId;
		init();
	}
	private void init(){
		this.setContentView(m_layoutId);
		this.setViewAndListener();
	}
	public abstract void setViewAndListener();
}
