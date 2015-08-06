package com.showmo.widget.timeline;

import java.security.PublicKey;

import com.showmo.util.LogUtils;

import android.R.bool;
import android.R.integer;
import android.content.Context;
import android.graphics.Point;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;

public class PWGestureDetector extends GestureDetector {
	private final PwOnGestureListener m_pinchListener;
	private PWPinchEvent m_pinchEvent;

	public PWGestureDetector(Context context, OnGestureListener listener) {
		super(context, listener);
		m_pinchListener = null;
		m_pinchEvent = new PWPinchEvent();
	}

	public PWGestureDetector(Context context, PwOnGestureListener listener) {
		super(context, listener);
		m_pinchListener = listener;
		m_pinchEvent = new PWPinchEvent();
		// TODO Auto-generated constructor stub
	}

	public static class PWPoint {
		public float x;
		public float y;

		public PWPoint(float x, float y) {
			this.x = x;
			this.y = y;
		}
	}

	public static class PWPinchEvent {
		public PWPinchEvent() {
			pinchBeginPoint1 = new PWPoint(-1, -1);
			pinchBeginPoint2 = new PWPoint(-1, -1);
			pinchPrePoint1 = new PWPoint(-1, -1);
			pinchPrePoint2 = new PWPoint(-1, -1);
			pinchCurPoint1 = new PWPoint(-1, -1);
			pinchCurPoint2 = new PWPoint(-1, -1);
		}

		public PWPoint pinchBeginPoint1;
		public PWPoint pinchBeginPoint2;
		public PWPoint pinchPrePoint1;
		public PWPoint pinchPrePoint2;
		public PWPoint pinchCurPoint1;
		public PWPoint pinchCurPoint2;
	}

	public interface PwOnGestureListener extends OnGestureListener {
		boolean onPinchBegin();

		boolean onPinch(PWPinchEvent ev);

		boolean onPinchEnd();

		boolean onTouchUp();
	}

	@Override
	public boolean onTouchEvent(MotionEvent ev) {
		// LogUtils.v("onPinch", "PWGestureDetector onTouchEvent");
		int count = ev.getPointerCount();
		int action = ev.getAction();
		// LogUtils.v("test", "onTouchEvent "+action);
		switch (action & MotionEvent.ACTION_MASK) {
		case MotionEvent.ACTION_DOWN:
			// LogUtils.v("onPinch", "MotionEvent.ACTION_DOWN ");
			break;
		case MotionEvent.ACTION_CANCEL:
			// LogUtils.v("onPinch", "MotionEvent.ACTION_CANCEL ");
			break;
		case MotionEvent.ACTION_MOVE:
			if (count >= 2) {
			//	LogUtils.v("test", "MotionEvent.ACTION_MOVE count>=2");
				if (m_pinchListener != null) {
					m_pinchEvent.pinchCurPoint1.x = ev.getX(0);
					m_pinchEvent.pinchCurPoint1.y = ev.getY(0);
					m_pinchEvent.pinchCurPoint2.x = ev.getX(1);
					m_pinchEvent.pinchCurPoint2.y = ev.getY(1);
					boolean handled = m_pinchListener.onPinch(m_pinchEvent);
					if (handled) {
						m_pinchEvent.pinchPrePoint1.x = ev.getX(0);
						m_pinchEvent.pinchPrePoint1.y = ev.getY(0);
						m_pinchEvent.pinchPrePoint2.x = ev.getX(1);
						m_pinchEvent.pinchPrePoint2.y = ev.getY(1);
					}
					return handled;
				}
			}
			// LogUtils.v("motion", "ACTION_MOVE "+m_touchFingCount);
			break;
		case MotionEvent.ACTION_POINTER_DOWN:
		// LogUtils.v("onPinch", "MotionEvent.ACTION_POINTER_DOWN "+count);
			if (count == 2) {
				m_pinchEvent.pinchBeginPoint1.x = ev.getX(0);
				m_pinchEvent.pinchBeginPoint1.y = ev.getY(0);
				m_pinchEvent.pinchBeginPoint2.x = ev.getX(1);
				m_pinchEvent.pinchBeginPoint2.y = ev.getY(1);

				m_pinchEvent.pinchPrePoint1.x = ev.getX(0);
				m_pinchEvent.pinchPrePoint1.y = ev.getY(0);
				m_pinchEvent.pinchPrePoint2.x = ev.getX(1);
				m_pinchEvent.pinchPrePoint2.y = ev.getY(1);
				if (m_pinchListener != null) {
					m_pinchListener.onPinchBegin();
				}
			}
			// LogUtils.v("motion", "ACTION_POINTER_DOWN "+m_touchFingCount);
			break;
		case MotionEvent.ACTION_UP:
			LogUtils.v("onPinch", "ACTION_UP "+count);
			if (m_pinchListener != null) {
				m_pinchListener.onTouchUp();
			}

			break;
		case MotionEvent.ACTION_POINTER_UP:
			LogUtils.v("onPinch", "ACTION_POINTER_UP  "+count);
			if (count == 1) {
				if (m_pinchListener != null) {
					m_pinchListener.onPinchEnd();
				}
			}
		default:
			break;
		}
		return super.onTouchEvent(ev);
	}
}
