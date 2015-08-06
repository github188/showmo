#define FUNCTION_FUNCTION MACRO_JOIN(TFunction,FUNCTION_NUMBER)

/// \class FUNCTION_FUNCTION
/// \brief 函数指针类模块
///
template <FUNCTION_CLASS_TYPES>
class FUNCTION_FUNCTION
{
	class X{};

	enum FunctionType
	{
		typeEmpty,
		typeMember,
		typePointer,
		typeObject,
	};
	typedef R (X::*MEM_FUNCTION)(FUNCTION_TYPES);
	typedef R (*PTR_FUNCTION)(FUNCTION_TYPES);

	union
	{
		struct
		{
			MEM_FUNCTION proc;
			X* obj;
		}memFunction;
		PTR_FUNCTION ptrFunction;
	}m_function;

	FunctionType m_type;

public:
	FUNCTION_FUNCTION( ) 
		:m_type(typeEmpty)
	{
	}

	template<typename O>
		FUNCTION_FUNCTION(R(O::*f)(FUNCTION_TYPES), const O * o)
	{
		m_type = typeMember;
		m_function.memFunction.proc = horrible_cast<MEM_FUNCTION>(f); 
		m_function.memFunction.obj = horrible_cast<X*>(o);
	}

	FUNCTION_FUNCTION(PTR_FUNCTION f)
	{
		m_type = typePointer;
		m_function.ptrFunction = f;
	}

	/// 空构造函数
	FUNCTION_FUNCTION(int zero)
	{
		m_type = typeEmpty;
		assert(zero == 0);
	}

	FUNCTION_FUNCTION& operator=(const FUNCTION_FUNCTION& f)
	{
		if (&f == this)
			return *this;
		m_function = f.m_function;
		m_type = f.m_type;
		return *this;
	}

	/// 将类的成员函数和类对象的指针绑定并保存。其他类型的函数指针可以=运算符和隐式转换直接完成。
	template<typename O>
	void bind(R(O::*f)(FUNCTION_TYPES), const O * o)
	{
		*this = FUNCTION_FUNCTION(f, o);
	}

	bool empty()
	{
		return (m_type == typeEmpty);
	}

	bool operator==(const FUNCTION_FUNCTION& f)
	{
		if(m_type != f.m_type)
		{
			return false;
		}
		if(m_type == typeMember)
		{
			return (m_function.memFunction.proc == f.m_function.memFunction.proc
				&& m_function.memFunction.obj == f.m_function.memFunction.obj);
		}
		else if(m_type == typePointer)
		{
			return (m_function.ptrFunction == f.m_function.ptrFunction);
		}
		else
		{
			return true;
		}
	}

	inline R operator()(FUNCTION_TYPE_ARGS)
	{
		if(m_type == typeMember)
		{
			return (m_function.memFunction.obj->*m_function.memFunction.proc)(FUNCTION_ARGS);
		}
		else
		{
			return m_function.ptrFunction(FUNCTION_ARGS);
		}
	}
};

#undef FUNCTION_FUNCTION

