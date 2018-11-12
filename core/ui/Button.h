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
#include "../base/Node.h"
#include <functional>

namespace easy2d
{
	namespace ui
	{
		class Button
			: public Node
		{
			E2D_DISABLE_COPY(Button);

			typedef std::function<void()> Callback;

		public:
			Button();

			explicit Button(
				Node * normal,					/* 普通状态 */
				const Callback& func = nullptr	/* 按钮点击后的回调函数 */
			);

			explicit Button(
				Node * normal,					/* 普通状态 */
				Node * selected,				/* 鼠标按下状态 */
				const Callback& func = nullptr	/* 按钮点击后的回调函数 */
			);

			explicit Button(
				Node * normal,					/* 普通状态 */
				Node * mouseover,				/* 鼠标移入状态 */
				Node * selected,				/* 鼠标按下状态 */
				const Callback& func = nullptr	/* 按钮点击后的回调函数 */
			);

			explicit Button(
				Node * normal,					/* 普通状态 */
				Node * mouseover,				/* 鼠标移入状态 */
				Node * selected,				/* 鼠标移入状态 */
				Node * disabled,				/* 按钮禁用状态 */
				const Callback& func = nullptr	/* 按钮点击后的回调函数 */
			);

			// 获取按钮状态是启用还是禁用
			bool IsEnable() const;

			// 设置按钮启用或禁用
			void SetEnabled(
				bool enabled
			);

			// 设置一般情况下显示的按钮
			virtual void SetNormal(
				Node * normal
			);

			// 设置鼠标移入按钮时显示的按钮
			virtual void SetMouseOver(
				Node * mouseover
			);

			// 设置鼠标按下按钮时显示的按钮
			virtual void SetSelected(
				Node * selected
			);

			// 设置按钮被禁用时显示的按钮
			virtual void SetDisabled(
				Node * disabled
			);

			// 设置按钮点击后的回调函数
			void SetCallbackOnClick(
				const Callback& func
			);

			// 设置支点位置
			// 默认为 (0, 0), 范围 [0, 1]
			virtual void SetPivot(
				float pivot_x,
				float pivot_y
			) override;

		private:
			// 按钮状态枚举
			enum class Status { Normal, Mouseover, Selected };

			// 设置按钮状态
			virtual void SetStatus(
				Status status
			);

			// 刷新按钮显示
			virtual void UpdateVisible();

			// 分发鼠标消息
			virtual bool Dispatch(
				const MouseEvent& e,
				bool handled
			) override;

			// 遍历节点
			virtual void Visit() override;

		private:
			Node * normal_;
			Node *		mouseover_;
			Node *		selected_;
			Node *		disabled_;
			bool		enabled_;
			bool		is_selected_;
			Status		status_;
			Callback	callback_;
		};
	}
}