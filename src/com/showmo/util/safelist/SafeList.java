package com.showmo.util.safelist;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class SafeList<E> extends AbstractSafeList<E>{

	public void Loop(ILoopSubcriber<E> lcb){
		readBegin();
		Iterator<E> itr=iterator();
		while (itr.hasNext()) {
			if(!lcb.loop(itr.next())){
				break;
			}
		}
		readEnd();
	}
	public E find(IFindSubcriber<E> fcb){
		readBegin();
		Iterator<E> itr=iterator();
		E ret=null;
		while (itr.hasNext()) {
			E temp=itr.next();
			if(fcb.eqJudge(temp)){
				ret=temp;
				break;
			}
		}
		readEnd();
		return ret;
	}
	@Override
	public <K> void Loop(ILoopSubcriber1<E, K> lcb,K para) {
		// TODO Auto-generated method stub
		readBegin();
		Iterator<E> itr=iterator();
		while (itr.hasNext()) {
			if(!lcb.loop(itr.next(), para)){
				break;
			}
		}
		readEnd();
	}
	@Override
	public <K> E find(IFindSubcriber1<E, K> fcb,K para) {
		// TODO Auto-generated method stub
		readBegin();
		Iterator<E> itr=iterator();
		E ret=null;
		while (itr.hasNext()) {
			E temp=itr.next();
			if(fcb.eqJudge(temp,para)){
				ret=temp;
				break;
			}
		}
		readEnd();
		return ret;
	}
	
	
}
