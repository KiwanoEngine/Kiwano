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
#include "../macros.h"
#include "time.h"
#include "Event.hpp"

namespace kiwano
{

#define KGE_DEFINE_COMPONENT_FLAG ( 0x01 << (__COUNTER__ % 32) )

	class KGE_API ComponentBase
	{
	public:
		virtual void SetupComponent() = 0;
		virtual void DestroyComponent() = 0;

		bool Check(const Int32 flag);

	protected:
		ComponentBase();

	protected:
		Int32 flag_;
	};


	class RenderTarget;

	class KGE_API RenderComponent
		: public virtual ComponentBase
	{
	public:
		static const Int32 flag = KGE_DEFINE_COMPONENT_FLAG;

		RenderComponent();

		virtual void BeforeRender() {}

		virtual void OnRender(RenderTarget*) {}

		virtual void AfterRender() {}
	};


	class KGE_API UpdateComponent
		: public virtual ComponentBase
	{
	public:
		static const Int32 flag = KGE_DEFINE_COMPONENT_FLAG;

		UpdateComponent();

		virtual void BeforeUpdate() {}

		virtual void OnUpdate(Duration) {}

		virtual void AfterUpdate() {}
	};


	class KGE_API EventComponent
		: public virtual ComponentBase
	{
	public:
		static const Int32 flag = KGE_DEFINE_COMPONENT_FLAG;

		EventComponent();

		virtual void HandleEvent(Event&) {}

		virtual void HandleMessage(HWND, UInt32, WPARAM, LPARAM) {}
	};


#undef KGE_DEFINE_COMPONENT_FLAG

}
