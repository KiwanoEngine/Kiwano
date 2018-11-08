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
#include "macros.h"
#include "BaseTypes.h"

namespace easy2d
{
	// 按键消息
	class KeyEvent
	{
	public:
		// 按键消息类型
		enum class Type : int
		{
			Down = 0x0100,	// 按下
			Up				// 抬起
		};

	public:
		explicit KeyEvent(
			UINT message,
			WPARAM w_param,
			LPARAM l_param
		);

		// 获取事件类型
		KeyEvent::Type GetType() const;

		// 获取按键键值
		KeyCode GetCode() const;

		// 获取按键次数
		int GetCount() const;

	protected:
		UINT	message_;
		WPARAM	w_param_;
		LPARAM	l_param_;
	};


	// 按键消息处理接口
	class KeyEventHandler
	{
	public:
		// 处理按键消息
		virtual void Handle(KeyEvent e) = 0;
	};
}
