/// \class SIGNAL_SIGNAL
/// \brief 信号类模块。
///
/// 一个信号被触发时，会调用所有连接到其上的TFunctionN对象，只支持
/// 返回值为void的TFunctionN。SIGNAL_SIGNAL是一个宏，根据参数个数会被替换成
/// TSignalN，用户通过 TSignalN<T1, T2, T3,..,TN>方式来使用，TN表示参数类型， 
/// N表示函数参数个数，目前最大参数为6。
/// \see FUNCTION_FUNCTION

#define SIGNAL_SIGNAL MACRO_JOIN(TSignal,SIGNAL_NUMBER)
#define FUNCTION_FUNCTION MACRO_JOIN(TFunction, SIGNAL_NUMBER)

#if (SIGNAL_NUMBER != 0)
template <SIGNAL_CLASS_TYPES>
#endif
class SIGNAL_SIGNAL
{
	/// 信号节点状态
	enum SlotState
	{
		slotStateEmpty,		///< 节点为空
		slotStateNormal,	///< 节点以连接
	};

public:
	/// 信号操作错误码
	enum ErrorCode
	{
		errorNoFound = -1,		///< 没有找到指定的对象
		errorExsist = -2,		///< 对象已经存在
		errorFull = -3,			///< 已经到达能够连接的对象个数上限
		errorEmptyProc = -4,	///< 对象包含的函数指针为空，没有意义
	};

	/// 与信号模板参数类型匹配的函数指针对象类型
	typedef FUNCTION_FUNCTION<void SIGNAL_TYPES_COMMA> SigProc;

private:
	/// 信号节点结构
	struct SignalSlot
	{
		SigProc proc;
		SlotState state;
	};

	int m_numberMax;
	int m_number;
	SignalSlot* m_slots;
	CMutex m_mutex;

public:
	/// 构造函数
	/// \param maxSlots 能够连接的最大函数指针对象的个数
	SIGNAL_SIGNAL(int maxSlots) :
	  m_numberMax(maxSlots), m_number(0)
	{
		m_slots = new SignalSlot[maxSlots];
		for(int i = 0; i < m_numberMax; i++)
		{
			m_slots[i].state = slotStateEmpty;
		}
	}
	
	 /// 析构函数
	~SIGNAL_SIGNAL()
	{
		delete []m_slots;
	}

	/// 清除所有回调
	void Clear()
	{
		m_mutex.Enter();
		for(int i = 0; i < m_numberMax; i++)
		{
			m_slots[i].state = slotStateEmpty;
		}
		m_mutex.Leave();
	}

	/// 挂载函数指针对象
	int Attach(SigProc proc)
	{
		if(proc.empty())
		{
			return errorEmptyProc;
		}

		if(IsAttached(proc))
		{
			return errorExsist;
		}
		
		m_mutex.Enter();
			
		for(int i = 0; i < m_numberMax; i++)
		{
			if(m_slots[i].state == slotStateEmpty)
			{
				m_slots[i].proc  = proc;
				m_slots[i].state = slotStateNormal;
				m_number++;
				m_mutex.Leave();
				return m_number;
			}
		}
		m_mutex.Leave();
		return errorFull;
	}			

	/// 卸载函数指针对象，根据对象中保存的函数指针来匹配。
	int Detach(SigProc proc)
	{
		if(proc.empty())
		{
			return errorEmptyProc;
		}
		m_mutex.Enter();

		for(int i = 0; i < m_numberMax; i++)
		{
			if(m_slots[i].proc ==  proc && m_slots[i].state != slotStateEmpty)
			{
				m_slots[i].state = slotStateEmpty;
				m_number--;
				m_mutex.Leave();
				return m_number;
			}
		};
		m_mutex.Leave();
		return errorNoFound;
	}

	/// 判断卸载函数指针对象是否挂载，根据对象中保存的函数指针来匹配。
	bool IsAttached(SigProc proc)
	{
		m_mutex.Enter();
		if(proc.empty())
		{
			m_mutex.Leave();
			return false;
		}

		for(int i = 0; i < m_numberMax; i++)
		{
			if(m_slots[i].proc ==  proc
				&& m_slots[i].state == slotStateNormal)
			{
				m_mutex.Leave();
				return true;
			}
		}
		m_mutex.Leave();
		return false;
	}

	// 重载括号运算符，依次调用各个以注册的回调函数
	// 处理在回调函数内Detach()自己的情况
	inline void operator()(SIGNAL_TYPE_ARGS)
	{
		for(int i = 0; i < m_numberMax; i++)
		{
			if(m_slots[i].state == slotStateNormal)
			{		
				m_slots[i].proc(SIGNAL_ARGS);

				// 回调函数内部自己Detach()自己的情况
				m_mutex.Enter();
				if (m_slots[i].state != slotStateNormal)
				{
					m_slots[i].proc = 0;
				}
				m_mutex.Leave();
			}
		}
	}
};

#undef SIGNAL_SIGNAL
#undef FUNCTION_FUNCTION
