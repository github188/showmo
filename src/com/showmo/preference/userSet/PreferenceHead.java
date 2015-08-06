package com.showmo.preference.userSet;

import com.showmo.R;

import android.content.Context;
import android.content.DialogInterface.OnClickListener;
import android.preference.Preference;
import android.view.View;
import android.widget.Button;

public class PreferenceHead extends Preference {
	 private android.view.View.OnClickListener onBackButtonClickListener;
	 
	    public PreferenceHead(Context context) {
	        super(context);
	        setLayoutResource(R.layout.preference_head);
	    }
	 
	    @Override
	    protected void onBindView(View view) {
	        super.onBindView(view);
	        Button btBack = (Button) view.findViewById(R.id.preference_back);
	        btBack.setOnClickListener(new View.OnClickListener() {
	 
	            @Override
	            public void onClick(View v) {
	                if (onBackButtonClickListener != null) {
	                    onBackButtonClickListener.onClick(v);
	                }
	            }
	        });
	    }
	 
	    public void setOnBackButtonClickListener(View.OnClickListener onClickListener) {
	        this.onBackButtonClickListener = onClickListener;
	    }
}
