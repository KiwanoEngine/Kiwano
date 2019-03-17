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
#include "Array.h"
#include "String.h"
#include <set>
#include <map>
#include <list>
#include <unordered_set>
#include <unordered_map>

namespace easy2d
{
	using StringStream = std::wstringstream;

	template<typename Type1, typename Type2>
	using Pair = std::pair<Type1, Type2>;

	template<typename Type>
	using List = std::list<Type>;

	template<typename Type>
	using Set = std::set<Type>;

	template<typename Type>
	using UnorderedSet = std::unordered_set<Type>;

	template<typename KeyType, typename MappedType>
	using Map = std::map<KeyType, MappedType>;

	template<typename KeyType, typename MappedType>
	using UnorderedMap = std::unordered_map<KeyType, MappedType>;
}
