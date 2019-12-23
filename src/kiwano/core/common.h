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

namespace kiwano
{
	using String = oc::wstring;

	using StringStream = std::wstringstream;

	template <typename _Ty, typename... _Args>
	using Vector = oc::vector<_Ty, _Args...>;

	template <typename _Ty, typename... _Args>
	using List = std::list<_Ty, _Args...>;

	template <typename _Ty, typename... _Args>
	using Queue = std::queue<_Ty, _Args...>;

	template <typename _Ty, typename... _Args>
	using Set = std::set<_Ty, _Args...>;

	template <typename _Ty1, typename _Ty2>
	using Pair = std::pair<_Ty1, _Ty2>;

	template <typename _Ty, typename... _Args>
	using UnorderedSet = std::unordered_set<_Ty, _Args...>;

	template <typename _Ty, typename... _Args>
	using Stack = std::stack<_Ty, _Args...>;

	template <typename _Kty, typename _Ty, typename... _Args>
	using Map = std::map<_Kty, _Ty, _Args...>;

	template <typename _Kty, typename _Ty, typename... _Args>
	using UnorderedMap = std::unordered_map<_Kty, _Ty, _Args...>;

	template <typename _FuncTy>
	using Function = oc::function<_FuncTy>;

	using Any = oc::any;

	using Json = oc::basic_json<Map, Vector, String, int, double, bool, std::allocator>;

	template <typename _Ty>
	using Singleton = oc::singleton<_Ty>;

	template <typename _Ty>
	using IntrusiveList = oc::intrusive_list<_Ty>;

	template <typename _Ty>
	using IntrusiveListItem = oc::intrusive_list_item<_Ty>;

	template <typename _Ty, typename _RefProxyTy>
	using IntrusivePtr = oc::intrusive_ptr<_Ty, _RefProxyTy>;

	using Noncopyable = oc::noncopyable;


	// Closure

	template<typename _Ty, typename _Uty, typename _Ret, typename... _Args>
	inline Function<_Ret(_Args...)> Closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...))
	{
		return oc::closure(ptr, func);
	}

	template<typename _Ty, typename _Uty, typename _Ret, typename... _Args>
	inline Function<_Ret(_Args...)> Closure(_Uty* ptr, _Ret(_Ty::* func)(_Args...) const)
	{
		return oc::closure(ptr, func);
	}
}
