#ifndef __INFRA_MUTEX_H__
#define __INFRA_MUTEX_H__

/// 互斥量类型
enum mutex_type
{
	/// 快速互斥量，如果临界区已经被一个线程获取，其他线程或者该线程再次获取时，
	/// 调用线程直接被挂起。
	MUTEX_FAST = 0,

	/// 递归互斥量，已经获取临界区的线程可以多次获取，互斥量对象内部应有获取次数
	/// 计数。如果要完全释放临界区，一定要保证获取次数和释放次数一致。
	MUTEX_RECURSIVE,
};

class CMutex
{
public:
	/// 创建互斥量。
	/// 
	/// \param [in] nType 互斥量的类型，取mutex_type枚举类型值
	CMutex(int nType = MUTEX_FAST);

	/// 销毁互斥量。
	~CMutex();

	/// 进入临界区。
	/// \retval true  操作成功
	/// \retval false  操作失败
	bool Enter();

	/// 离开临界区。
	/// 
	/// \retval true  操作成功
	/// \retval false  操作失败
	bool Leave();

	bool TryEnter();

private:
	void  *m_hMutex;
};

/// 读写锁封装
/// 
class CReadWriteMutex
{
public:
	CReadWriteMutex();
	~CReadWriteMutex();
	bool LeaveRead();	
	bool EnterRead(); 
	bool EnterWrite();
	bool LeaveWrite();
	bool TryEnterRead();
	bool TryEnterWrite();

protected: 
	CReadWriteMutex& operator=( const CReadWriteMutex &rhs );

private:
	void  *m_hrwMutex;
};
#endif 

