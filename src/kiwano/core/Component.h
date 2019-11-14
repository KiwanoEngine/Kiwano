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
#include <kiwano/macros.h>
#include <kiwano/core/time.h>

namespace kiwano
{
	class RenderTarget;
	class Event;

	// �������
	class KGE_API ComponentBase
	{
	public:
		virtual void SetupComponent() = 0;
		virtual void DestroyComponent() = 0;

		bool Check(const int flag);

	protected:
		ComponentBase();

	protected:
		int flag_;
	};


	// ��Ⱦ֧�����
	class KGE_API RenderComponent
		: public virtual ComponentBase
	{
	public:
		virtual void BeforeRender() {}

		virtual void OnRender(RenderTarget*) {}

		virtual void AfterRender() {}

	public:
		static const int flag;

		RenderComponent();
	};


	// ����֧�����
	class KGE_API UpdateComponent
		: public virtual ComponentBase
	{
	public:
		virtual void BeforeUpdate() {}

		virtual void OnUpdate(Duration) {}

		virtual void AfterUpdate() {}

	public:
		static const int flag;

		UpdateComponent();
	};


	// �¼�֧�����
	class KGE_API EventComponent
		: public virtual ComponentBase
	{
	public:
		virtual void HandleEvent(Event&) {}

		virtual void HandleMessage(HWND, UINT32, WPARAM, LPARAM) {}

	public:
		static const int flag;

		EventComponent();
	};

}
