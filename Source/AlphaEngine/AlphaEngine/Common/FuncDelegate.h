#include <functional>
namespace AJ
{
	//===========================================================================
	//Original Class Template
	template<typename RetType, typename Arg>
	class FuncDelegate
	{
	public:
		FuncDelegate() {}
		//-----------------------------------------------------------------------
		template<class ClassName>
		FuncDelegate(RetType(ClassName::*SomeFunction)(Arg), ClassName instance)
		{
			m_del = std::bind(SomeFunction, instance, std::placeholders::_1);
		}
		//-----------------------------------------------------------------------
		FuncDelegate(RetType(*SomeFunction)(Arg))
		{
			m_del = SomeFunction;
		}
		//-----------------------------------------------------------------------
		RetType operator()(Arg a)
		{
			m_del(a);
		}
		//-----------------------------------------------------------------------
		template<class ClassName>
		void Bind(RetType(ClassName::*SomeFunction)(Arg), ClassName instance)
		{
			m_del = std::bind(SomeFunction, instance, std::placeholders::_1);
		}
		//-----------------------------------------------------------------------
		void Bind(RetType(*SomeFunction)(Arg))
		{
			m_del = SomeFunction;
		}
		//-----------------------------------------------------------------------
	private:
		std::function<RetType(Arg)> m_del;
	};
	//===========================================================================
	//Void argument
	template<typename RetType>
	class FuncDelegate<RetType, void>
	{
	public:
		FuncDelegate() {}
		//-----------------------------------------------------------------------
		template<class ClassName>
		FuncDelegate(RetType(ClassName::*SomeFunction)(void), ClassName instance)
		{
			m_del = std::bind(SomeFunction, instance);
		}
		//-----------------------------------------------------------------------
		FuncDelegate(RetType(*SomeFunction)(void))
		{
			m_del = SomeFunction;
		}
		//-----------------------------------------------------------------------
		RetType operator()()
		{
			return m_del();
		}
		//-----------------------------------------------------------------------
		template<class ClassName>
		void Bind(RetType(ClassName::*SomeFunction)(void), ClassName instance)
		{
			m_del = std::bind(SomeFunction, instance);
		}
		//-----------------------------------------------------------------------
		void Bind(RetType(*SomeFunction)(void))
		{
			m_del = SomeFunction;
		}
		//-----------------------------------------------------------------------
	private:
		std::function<RetType(void)> m_del;
	};
	//===========================================================================
}
