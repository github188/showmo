package com.showmo.util.safelist;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public abstract class AbstractSafeList<E> extends ArrayList<E> {
	private ReadWriteLock lock=new ReentrantReadWriteLock();

	@Override
	public void clear() {
		// TODO Auto-generated method stub
		writeBegin();
		super.clear();
		writeEnd();
	}
	@Override
	public boolean add(E object) {
		// TODO Auto-generated method stub
		writeBegin();
		boolean badd=super.add(object);
		writeEnd();
		return badd;
	}

	@Override
	public boolean addAll(Collection<? extends E> collection) {
		// TODO Auto-generated method stub
		writeBegin();
		boolean badd=super.addAll(collection);
		writeEnd();
		return badd;
	}

	@Override
	public void add(int index, E object) {
		// TODO Auto-generated method stub
		writeBegin();
		super.add(index,object);
		writeEnd();
	}

	@Override
	public boolean addAll(int index, Collection<? extends E> collection) {
		// TODO Auto-generated method stub
		writeBegin();
		boolean badd=super.addAll(index,collection);
		writeEnd();
		return badd;
	}

	@Override
	public E remove(int index) {
		// TODO Auto-generated method stub
		writeBegin();
		E t=super.remove(index);
		writeEnd();
		return t;
	}
	@Override
	public boolean removeAll(Collection<?> collection) {
		// TODO Auto-generated method stub
		writeBegin();
		boolean bremove =super.removeAll(collection);
		writeEnd();
		return bremove;
	}
	@Override
	public boolean remove(Object object) {
		// TODO Auto-generated method stub
		writeBegin();
		boolean bremove =super.remove(object);
		writeEnd();
		return bremove;
	}
	protected void readBegin(){
		lock.readLock().lock();
	}
	protected void readEnd(){
		lock.readLock().unlock();
	}
	protected void writeBegin(){
		lock.writeLock().lock();
	}
	protected void writeEnd(){
		lock.writeLock().unlock();
	}
	
	public abstract  void Loop(ILoopSubcriber<E> lcb);
	public abstract <K> void Loop(ILoopSubcriber1<E,K> lcb,K para);
	public abstract E find(IFindSubcriber<E> fcb);
	public abstract <K> E find(IFindSubcriber1<E, K> fcb,K para);
}
