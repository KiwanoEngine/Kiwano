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
#include <memory>
#include <mutex>

// Class that will implement the singleton mode,
// must use the macro in its delare file

#ifndef KGE_DECLARE_SINGLETON
#define KGE_DECLARE_SINGLETON( CLASS )			\
	friend ::kiwano::common::singleton< CLASS >;  \
	friend typename std::unique_ptr< CLASS >::deleter_type
#endif

namespace kiwano
{
namespace common
{

template <typename _Ty>
struct singleton
{
public:
	static inline _Ty* GetInstance()
	{
		if (!instance_)
		{
			std::call_once(once_, InitInstance);
		}
		return instance_.get();
	}

	static inline void DestroyInstance()
	{
		instance_.reset();
	}

protected:
	singleton() = default;

private:
	singleton(const singleton&) = delete;

	singleton& operator=(const singleton&) = delete;

	static inline void InitInstance()
	{
		if (!instance_)
		{
			instance_.reset(new (std::nothrow) _Ty);
		}
	}

private:
	static std::once_flag once_;
	static std::unique_ptr<_Ty> instance_;
};

template <typename _Ty>
std::once_flag singleton<_Ty>::once_;

template <typename _Ty>
std::unique_ptr<_Ty> singleton<_Ty>::instance_;

}  // namespace common
}  // namespace kiwano
