package com.showmo.util.safelist;


/*
 * SafeList循环遍历，item订阅者，由订阅者决定对item项的操作
 * 
 * @return boolean ，返回true代表循环继续 false退出循环
 */
public abstract class ILoopSubcriber1<T,K>{
	public abstract boolean loop(T item,K para);
}
