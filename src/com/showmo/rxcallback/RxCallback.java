package com.showmo.rxcallback;

public abstract class RxCallback<T> extends rx.Subscriber<T>{
	@Override
	public void onNext(T t) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void onError(Throwable e) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void onCompleted() {
		// TODO Auto-generated method stub
		
	}
}
