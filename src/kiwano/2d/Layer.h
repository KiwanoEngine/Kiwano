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
#include <kiwano/2d/Actor.h>
#include <kiwano/2d/..\renderer\LayerArea.h>
#include <kiwano/2d/..\renderer\RenderTarget.h>

namespace kiwano
{
	class KGE_API Layer
		: public Actor
	{
	public:
		Layer();

		virtual ~Layer();

		// �������к����Ի�ȡͼ���¼�
		virtual void OnMouseButtonDown(MouseButton::Value btn, Point const& p) {}
		virtual void OnMouseButtonUp(MouseButton::Value btn, Point const& p) {}
		virtual void OnMouseMoved(Point const& p) {}
		virtual void OnMouseWheel(float wheel) {}
		virtual void OnKeyDown(int key) {}
		virtual void OnKeyUp(int key) {}
		virtual void OnChar(char c) {}

		// �Ƿ�����Ϣ��û
		inline bool IsSwallowEventsEnabled() const	{ return swallow_; }

		// ��û��Ϣ
		inline void SetSwallowEvents(bool enabled)	{ swallow_ = enabled; }

		// ���òü�����
		void SetClipRect(Rect const& clip_rect);

		// ����ͼ��͸����
		void SetOpacity(float opacity) override;

		// ���ü����ɲ�
		void SetMaskGeometry(Geometry const& mask);

		// ���ü����ɲ�Ķ�ά�任
		void SetMaskTransform(Matrix3x2 const& transform);

		// ����ͼ������
		inline void SetArea(LayerArea const& area)	{ area_ = area; }

		// ��ȡͼ������
		inline LayerArea const& GetArea() const		{ return area_; }

	public:
		void Dispatch(Event& evt) override;

	protected:
		void Render(RenderTarget* rt) override;

		void HandleMessages(Event& evt);

	protected:
		bool		swallow_;
		LayerArea	area_;
	};
}
