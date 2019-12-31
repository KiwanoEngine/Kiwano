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

#include <kiwano/ui/Button.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/platform/Window.h>

namespace kiwano
{
	Button::Button()
		: enabled_(true)
		, is_selected_(false)
		, click_callback_(nullptr)
		, status_(Status::Normal)
	{
		SetResponsible(true);

		AddListener<MouseHoverEvent>(Closure(this, &Button::UpdateStatus));
		AddListener<MouseOutEvent>(Closure(this, &Button::UpdateStatus));
		AddListener<MouseDownEvent>(Closure(this, &Button::UpdateStatus));
		AddListener<MouseUpEvent>(Closure(this, &Button::UpdateStatus));
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

	void Button::SetReleasedCallback(const Callback& func)
	{
		released_callback_ = func;
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

	void Button::UpdateStatus(Event& evt)
	{
		KGE_ASSERT(evt.IsType<MouseEvent>());

		if (enabled_ && (evt.SafeCast<MouseEvent>().target == this))
		{
			if (evt.IsType<MouseHoverEvent>())
			{
				SetStatus(Status::Hover);
				Window::instance().SetCursor(CursorType::Hand);

				if (mouse_over_callback_)
					mouse_over_callback_(this);
			}
			else if (evt.IsType<MouseOutEvent>())
			{
				SetStatus(Status::Normal);
				Window::instance().SetCursor(CursorType::Arrow);

				if (mouse_out_callback_)
					mouse_out_callback_(this);
			}
			else if (evt.IsType<MouseDownEvent>() && status_ == Status::Hover)
			{
				SetStatus(Status::Pressed);

				if (pressed_callback_)
					pressed_callback_(this);
			}
			else if (evt.IsType<MouseUpEvent>() && status_ == Status::Pressed)
			{
				SetStatus(Status::Hover);

				if (released_callback_)
					released_callback_(this);

				if (click_callback_)
					click_callback_(this);
			}
		}
	}

}
