// Copyright (c) 2016-2018 Kiwano - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include <set>
#include <map>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

#include <3rd-party/OuterC/oc/oc.h>
#include <kiwano/macros.h>
#include <kiwano/core/Singleton.h>

namespace kiwano
{
	/// \~chinese
	/// @brief 字符串容器
	using String = oc::wstring;

	/// \~chinese
	/// @brief 窄字符串容器
	using ByteString = oc::string;

	/// \~chinese
	/// @brief 字符串流
	using StringStream = std::wstringstream;

	/// \~chinese
	/// @brief 线性数组容器
	template <typename _Ty, typename... _Args>
	using Vector = oc::vector<_Ty, _Args...>;

	/// \~chinese
	/// @brief 链表容器
	template <typename _Ty, typename... _Args>
	using List = std::list<_Ty, _Args...>;

	/// \~chinese
	/// @brief 队列容器
	template <typename _Ty, typename... _Args>
	using Queue = std::queue<_Ty, _Args...>;

	/// \~chinese
	/// @brief 集合容器
	template <typename _Ty, typename... _Args>
	using Set = std::set<_Ty, _Args...>;

	/// \~chinese
	/// @brief 对容器
	template <typename _Ty1, typename _Ty2>
	using Pair = std::pair<_Ty1, _Ty2>;

	/// \~chinese
	/// @brief 无序集合容器
	template <typename _Ty, typename... _Args>
	using UnorderedSet = std::unordered_set<_Ty, _Args...>;

	/// \~chinese
	/// @brief 栈容器
	template <typename _Ty, typename... _Args>
	using Stack = std::stack<_Ty, _Args...>;

	/// \~chinese
	/// @brief 字符串容器
	template <typename _Kty, typename _Ty, typename... _Args>
	using Map = std::map<_Kty, _Ty, _Args...>;

	/// \~chinese
	/// @brief 字符串容器
	template <typename _Kty, typename _Ty, typename... _Args>
	using UnorderedMap = std::unordered_map<_Kty, _Ty, _Args...>;

	/// \~chinese
	/// @brief 函数封装器
	/// @par
	///   使用函数封装器可以储存、复制和调用任何可调用目标，示例代码如下：
	///   @code
	///     Function<bool(int)> func1 = StaticFunc;               // bool StaticFunc(int x);
	///     Function<bool(int)> func2 = Closure(&t, &T::Func);    // bool T::Func(int x);
	///     Function<bool(int)> func3 = T::StaticFunc;            // static bool T::StaticFunc(int x);
	///     Function<bool(int)> func4 = [](int x) -> bool {};     // Lambda function
	///     Function<bool(int)> func5 = std::bind(&T::Func, &t);  // std::bind
	///     Function<bool(int)> func5 = Callable();               // Callable objects: struct Callable { bool operator()(int x) {} };
	///   @endcode
	template <typename _FuncTy>
	using Function = oc::function<_FuncTy>;

	/// \~chinese
	/// @brief 单值容器
	using Any = oc::any;

	/// \~chinese
	/// @brief JSON对象容器
	using Json = oc::basic_json<Map, Vector, String, int, double, bool, std::allocator>;

	/// \~chinese
	/// @brief 侵入式链表容器
	template <typename _Ty>
	using IntrusiveList = oc::intrusive_list<_Ty>;

	/// \~chinese
	/// @brief 侵入式链表元素
	template <typename _Ty>
	using IntrusiveListItem = oc::intrusive_list_item<_Ty>;

	/// \~chinese
	/// @brief 侵入式智能指针
	template <typename _Ty, typename _RefProxyTy>
	using IntrusivePtr = oc::intrusive_ptr<_Ty, _RefProxyTy>;

	/// \~chinese
	/// @brief 不可拷贝对象
	using Noncopyable = oc::noncopyable;


	/// \~chinese
	/// @brief 闭包函数
	template<typename _Ty, typename _Uty, typename _Ret, typename... _Args>
	inline Function<_Ret(_Args...)> Closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...))
	{
		return oc::closure(ptr, func);
	}

	/// \~chinese
	/// @brief 闭包函数
	template<typename _Ty, typename _Uty, typename _Ret, typename... _Args>
	inline Function<_Ret(_Args...)> Closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...) const)
	{
		return oc::closure(ptr, func);
	}

	inline ByteString WideToMultiByte(const String& str)
	{
		return oc::wide_to_string(str);
	}

	inline String MultiByteToWide(const ByteString& str)
	{
		return oc::string_to_wide(str);
	}
}
