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

#include <kiwano/core/Component.h>

#define KGE_DEFINE_COMPONENT_FLAG(OFFSET) ( 0x01 << (OFFSET % 32) )

namespace kiwano
{
	const int RenderComponent::flag = KGE_DEFINE_COMPONENT_FLAG(0);
	const int UpdateComponent::flag = KGE_DEFINE_COMPONENT_FLAG(1);
	const int EventComponent::flag = KGE_DEFINE_COMPONENT_FLAG(2);

	ComponentBase::ComponentBase()
		: flag_(0)
	{
	}

	bool ComponentBase::Check(const int flag)
	{
		return !!(flag_ & flag);
	}

	RenderComponent::RenderComponent()
	{
		flag_ |= flag;
	}

	UpdateComponent::UpdateComponent()
	{
		flag_ |= flag;
	}

	EventComponent::EventComponent()
	{
		flag_ |= flag;
	}

}
