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
#include "../base/Sprite.h"
#include <functional>

namespace easy2d
{
	namespace ui
	{
		class Button
			: public Sprite
		{
			using Callback = std::function<void()>;

		public:
			Button();

			explicit Button(
				Callback const& func	/* 按钮回调函数 */
			);

			virtual ~Button();

			// 获取按钮状态是启用还是禁用
			bool IsEnable() const;

			// 设置按钮启用或禁用
			void SetEnabled(
				bool enabled
			);

			// 设置按钮点击后的回调函数
			void SetClickCallback(
				const Callback& func
			);

		private:
			enum class Status { Normal, Hover, Selected };

			void SetStatus(
				Status status
			);

			void UpdateStatus(Event* e);

		private:
			bool		enabled_;
			bool		is_selected_;
			Status		status_;
			Callback	callback_;
		};
	}
}