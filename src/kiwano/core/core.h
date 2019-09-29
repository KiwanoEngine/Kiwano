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
#include "vector.hpp"
#include "string.hpp"
#include "intrusive_list.hpp"
#include "intrusive_ptr.hpp"
#include "noncopyable.hpp"
#include "singleton.hpp"
#include "function.hpp"
#include "basic_json.hpp"
#include <set>
#include <map>
#include <list>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

namespace kiwano
{
	using String = kiwano::core::wstring;

	using StringStream = std::wstringstream;

	template <typename _Ty, typename... _Args>
	using Vector = kiwano::core::vector<_Ty, _Args...>;

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

	template <typename _Kty, typename _Ty, typename... _Args>
	using Map = std::map<_Kty, _Ty, _Args...>;

	template <typename _Kty, typename _Ty, typename... _Args>
	using UnorderedMap = std::unordered_map<_Kty, _Ty, _Args...>;

	template <typename _FuncTy>
	using Function = kiwano::core::function<_FuncTy>;

	using Json = kiwano::core::basic_json<kiwano::Map, kiwano::Vector, kiwano::String,
		int, double, bool, std::allocator>;
}

namespace std
{
	template<>
	struct hash<::kiwano::Json>
	{
		::std::size_t operator()(const ::kiwano::Json& json) const
		{
			return hash<::kiwano::Json::string_type>{}(json.dump());
		}
	};

	template<>
	inline void swap<::kiwano::Json>(::kiwano::Json& lhs, ::kiwano::Json& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}
