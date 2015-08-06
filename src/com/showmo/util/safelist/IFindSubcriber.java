package com.showmo.util.safelist;

/*
 * Safelist的 遍历查找订阅者，由客户决定每一项的等式判定
 */
public interface IFindSubcriber<T> {
	/*
	 * @return boolean 返回ture代表相等成立
	 * @para T 项
	 */
	boolean eqJudge(T item);
}
