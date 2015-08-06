package com.showmo.rxcallback;

import rx.functions.Action1;

public abstract class RxActionsCallback<T>{
	private Action1<Throwable> erraction;
	private Action1<T> sucaction;
	public Action1<T> getNextAction(){
		return sucaction;
	}
	public Action1<Throwable> getErrAction(){
		return erraction;
	}
	public RxActionsCallback(){
		erraction=new Action1<Throwable>() {
			@Override
			public void call(Throwable t) {
				// TODO Auto-generated method stub
				RxActionsCallback.this.onError(t);
			}
		};
		sucaction=new Action1<T>() {
			@Override
			public void call(T t) {
				// TODO Auto-generated method stub
				RxActionsCallback.this.onNext(t);
			}
		};
	}
	public abstract void onNext(T sucinfo);
	public abstract void onError(Throwable errinfo);
}