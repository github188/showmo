package com.showmo.util.safelist;
/*
 * SafeList循环遍历，item订阅者，由订阅者决定对item项的操作
 */
public interface ILoopSubcriber<T> {
	/*
	 * @para   T   项
	 * @return boolean ，返回true代表循环继续 false退出循环
	 */
	boolean loop(T item);//是否继续循环
}
