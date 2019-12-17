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
#include <type_traits>
#include <kiwano/common/common.h>
#include <Unknwnbase.h>

namespace kiwano
{
	struct ComPtrProxy
	{
		static inline void add_ref(IUnknown* ptr)
		{
			if (ptr) ptr->AddRef();
		}

		static inline void release(IUnknown* ptr)
		{
			if (ptr) ptr->Release();
		}
	};

	// ComPtr<> is a smart pointer for COM
	template<
		typename _Ty,
		typename = typename std::enable_if<std::is_base_of<IUnknown, _Ty>::value, int>::type>
	using ComPtr = IntrusivePtr<_Ty, ComPtrProxy>;

}
