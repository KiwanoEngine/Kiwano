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
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <kiwano/macros.h>
#include <kiwano/core/String.h>
#include <kiwano/core/Function.h>
#include <kiwano/core/Singleton.h>
#include <kiwano/core/Any.h>
#include <kiwano/core/Allocator.h>
#include <kiwano/core/BitOperator.h>
#include <kiwano/core/Flag.h>
#include <kiwano/core/Value.h>

namespace kiwano
{

/// \~chinese
/// @brief 输入流
using InputStream = std::istream;

/// \~chinese
/// @brief 输出流
using OutputStream = std::ostream;

/// \~chinese
/// @brief 字符串流
using StringStream = std::stringstream;

/// \~chinese
/// @brief 宽字符串流
using WideStringStream = std::wstringstream;

/// \~chinese
/// @brief 线性数组容器
template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
using Vector = std::vector<_Ty, _Alloc>;

/// \~chinese
/// @brief 链表容器
template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
using List = std::list<_Ty, _Alloc>;

/// \~chinese
/// @brief 双端队列容器
template <typename _Ty, typename _Alloc = std::allocator<_Ty>>
using Deque = std::deque<_Ty, _Alloc>;

/// \~chinese
/// @brief 队列容器
template <typename _Ty, typename _Container = Deque<_Ty>>
using Queue = std::queue<_Ty, _Container>;

/// \~chinese
/// @brief 集合容器
template <typename _Kty, typename _Compare = std::less<_Kty>, typename _Alloc = std::allocator<_Kty>>
using Set = std::set<_Kty, _Compare, _Alloc>;

/// \~chinese
/// @brief 对容器
template <typename _Ty1, typename _Ty2>
using Pair = std::pair<_Ty1, _Ty2>;

/// \~chinese
/// @brief 无序集合容器
template <typename _Kty, typename _Hash = std::hash<_Kty>, typename _Keq = std::equal_to<_Kty>,
          typename _Alloc = std::allocator<_Kty>>
using UnorderedSet = std::unordered_set<_Kty, _Hash, _Keq, _Alloc>;

/// \~chinese
/// @brief 栈容器
template <typename _Ty, typename _Container = Deque<_Ty>>
using Stack = std::stack<_Ty, _Container>;

/// \~chinese
/// @brief 排序关联容器
template <typename _Kty, typename _Ty, typename _Compare = std::less<_Kty>, typename _Alloc = std::allocator<Pair<const _Kty, _Ty>>>
using Map = std::map<_Kty, _Ty, _Compare, _Alloc>;

/// \~chinese
/// @brief 非排序关联容器
template <typename _Kty, typename _Ty, typename _Hash = std::hash<_Kty>, typename _Keq = std::equal_to<_Kty>,
          typename _Alloc = std::allocator<Pair<const _Kty, _Ty>>>
using UnorderedMap = std::unordered_map<_Kty, _Ty, _Hash, _Keq, _Alloc>;

/// \~chinese
/// @brief 不可拷贝对象
class Noncopyable
{
protected:
    Noncopyable() = default;

private:
    Noncopyable(const Noncopyable&) = delete;

    Noncopyable& operator=(const Noncopyable&) = delete;
};

}  // namespace kiwano
