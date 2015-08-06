package com.showmo.widget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.widget.TextView;

public class MyTextView extends TextView{

	Paint paint= new Paint();

	public MyTextView(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
		// TODO Auto-generated constructor stub
	}

	public MyTextView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	public MyTextView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		//倾斜度45,上下左右居中
		paint.setColor(Color.RED);
		canvas.drawCircle(8, 8, 8, paint);
		//canvas.rotate(-30, getMeasuredWidth()/3, getMeasuredHeight()/3);
	//	canvas.drawRect(0, 0, 50, 60, paint);
	//	canvas.drawText("new", 0, 0, paint);
		super.onDraw(canvas);
	}
}
