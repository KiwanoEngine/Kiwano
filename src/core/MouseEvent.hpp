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
#include "Event.hpp"
#include "helper.hpp"

namespace easy2d
{
	class MouseEvent
		: public Event
	{
	public:
		enum Type
		{
			First = WM_MOUSEFIRST,

			Move,	// 移动
			Down,	// 按下
			Up,		// 抬起
			Wheel,	// 滚轮滚动

			Hover,	// 鼠标移入
			Out,	// 鼠标移出
			Click,	// 鼠标点击

			Last	// 结束标志
		};

		MouseEvent(EventType type, float x, float y, float wheel_delta) : Event(type), position(x, y), wheel_delta(wheel_delta), button_down(false) {}

		static bool Check(Event* e) { return e->type > Type::First && e->type < Type::Last; }

		Point position;
		float wheel_delta;
		bool button_down;
	};
}
