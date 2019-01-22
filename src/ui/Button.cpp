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

#include "Button.h"
#include "../core/MouseEvent.hpp"

namespace easy2d
{
	namespace ui
	{
		Button::Button()
			: enabled_(true)
			, is_selected_(false)
			, click_callback_(nullptr)
			, status_(Status::Normal)
		{
			AddListener(MouseEvent::Hover, std::bind(&Button::UpdateStatus, this, std::placeholders::_1));
			AddListener(MouseEvent::Out, std::bind(&Button::UpdateStatus, this, std::placeholders::_1));
			AddListener(MouseEvent::Down, std::bind(&Button::UpdateStatus, this, std::placeholders::_1));
			AddListener(MouseEvent::Up, std::bind(&Button::UpdateStatus, this, std::placeholders::_1));
		}

		Button::Button(const Callback& click)
			: Button()
		{
			this->SetClickCallback(click);
		}

		Button::Button(Callback const & click, Callback const & pressed, Callback const & mouse_over, Callback const & mouse_out)
			: Button()
		{
			this->SetClickCallback(click);
			this->SetPressedCallback(pressed);
			this->SetMouseOverCallback(mouse_over);
			this->SetMouseOutCallback(mouse_out);
		}

		Button::~Button()
		{
		}

		bool Button::IsEnable() const
		{
			return enabled_;
		}

		void Button::SetEnabled(bool enabled)
		{
			if (enabled_ != enabled)
			{
				enabled_ = enabled;
			}
		}

		void Button::SetClickCallback(const Callback& func)
		{
			click_callback_ = func;
		}

		void Button::SetPressedCallback(const Callback & func)
		{
			pressed_callback_ = func;
		}

		void Button::SetMouseOverCallback(const Callback & func)
		{
			mouse_over_callback_ = func;
		}

		void Button::SetMouseOutCallback(const Callback & func)
		{
			mouse_out_callback_ = func;
		}

		void Button::SetStatus(Status status)
		{
			if (status_ != status)
			{
				status_ = status;
			}
		}

		void Button::UpdateStatus(Event * e)
		{
			E2D_ASSERT(MouseEvent::Check(e));

			MouseEvent* me = static_cast<MouseEvent*>(e);
			if (enabled_)
			{
				if (me->type == MouseEvent::Hover)
				{
					SetStatus(Status::Hover);

					if (mouse_over_callback_)
						mouse_over_callback_();
				}
				else if (me->type == MouseEvent::Out)
				{
					SetStatus(Status::Normal);

					if (mouse_out_callback_)
						mouse_out_callback_();
				}
				else if (me->type == MouseEvent::Down && status_ == Status::Hover)
				{
					SetStatus(Status::Pressed);

					if (pressed_callback_)
						pressed_callback_();
				}
				else if (me->type == MouseEvent::Up && status_ == Status::Pressed)
				{
					SetStatus(Status::Hover);

					if (click_callback_)
						click_callback_();
				}
			}
		}

	}
}