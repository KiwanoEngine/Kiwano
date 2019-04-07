// Copyright (c) 2016-2018 Easy2D - Nomango
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
#include <functional>

namespace easy2d
{
	//
	// Closure is a simple function for binding member functions
	//

	template<typename _Ty, typename _Ret, typename... _Args>
	std::function<_Ret(_Args...)> Closure(_Ty* _Ptr, _Ret(_Ty::*_Func)(_Args...));


	//
	// Details of Closure
	//
	namespace __closure__detail
	{
		// sequence & generater

		template<int... _Num>
		struct Seq
		{
			using NextType = Seq<_Num..., sizeof...(_Num)>;
		};

		template<typename...>
		struct Gen;

		template<>
		struct Gen<>
		{
			using SeqType = Seq<>;
		};

		template<typename _Ty1, typename... _Args>
		struct Gen<_Ty1, _Args...>
		{
			using SeqType = typename Gen<_Args...>::SeqType::NextType;
		};


		// ClosureHelper

		template<typename _Ty, typename _Ret, typename... _Args>
		struct ClosureHelper
		{
			template<int... _Num>
			static inline std::function<_Ret(_Args...)> MakeFunc(_Ty* _Ptr, _Ret(_Ty::*_Func)(_Args...), Seq<_Num...>)
			{
				return std::bind(_Func, _Ptr, std::_Ph<_Num + 1>()...);
			}

			template<int... _Num>
			static inline std::function<_Ret(_Args...)> MakeFunc(_Ty* _Ptr, _Ret(_Ty::* _Func)(_Args...) const, Seq<_Num...>)
			{
				return std::bind(_Func, _Ptr, std::_Ph<_Num + 1>()...);
			}
		};
	}

	template<typename _Ty, typename _Ret, typename... _Args>
	inline std::function<_Ret(_Args...)> Closure(_Ty* _Ptr, _Ret(_Ty::*_Func)(_Args...))
	{
		using namespace __closure__detail;
		return ClosureHelper<_Ty, _Ret, _Args...>::
			MakeFunc(_Ptr, _Func, typename Gen<_Args...>::SeqType{});
	}

	template<typename _Ty, typename _Ret, typename... _Args>
	inline std::function<_Ret(_Args...)> Closure(_Ty* _Ptr, _Ret(_Ty::*_Func)(_Args...) const)
	{
		using namespace __closure__detail;
		return ClosureHelper<_Ty, _Ret, _Args...>::
			MakeFunc(_Ptr, _Func, typename Gen<_Args...>::SeqType{});
	}
}
