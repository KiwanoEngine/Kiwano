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
#include "../base/input.h"

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }

#define SET_BUTTON_NODE(Old, New)								\
	if (New != Old)												\
	{															\
		if (Old) this->RemoveChild(Old);						\
		if (New)												\
		{														\
			New->SetPivot(GetPivotX(), GetPivotY());			\
			this->AddChild(New);								\
		}														\
		Old = New;												\
		UpdateVisible();										\
	}															\


namespace easy2d
{
	namespace ui
	{
		Button::Button()
			: callback_(nullptr)
			, status_(Status::Normal)
			, enabled_(true)
			, is_selected_(false)
			, normal_(nullptr)
			, mouseover_(nullptr)
			, selected_(nullptr)
			, disabled_(nullptr)
		{
		}

		Button::Button(spNode const& normal, const Callback& func)
			: callback_(nullptr)
			, status_(Status::Normal)
			, enabled_(true)
			, is_selected_(false)
			, normal_(nullptr)
			, mouseover_(nullptr)
			, selected_(nullptr)
			, disabled_(nullptr)
		{
			this->SetNormal(normal);
			this->SetCallbackOnClick(func);
		}

		Button::Button(spNode const& normal, spNode const& selected, const Callback& func)
			: callback_(nullptr)
			, status_(Status::Normal)
			, enabled_(true)
			, is_selected_(false)
			, normal_(nullptr)
			, mouseover_(nullptr)
			, selected_(nullptr)
			, disabled_(nullptr)
		{
			this->SetNormal(normal);
			this->SetSelected(selected);
			this->SetCallbackOnClick(func);
		}

		Button::Button(spNode const& normal, spNode const& mouseover, spNode const& selected, const Callback& func)
			: callback_(nullptr)
			, status_(Status::Normal)
			, enabled_(true)
			, is_selected_(false)
			, normal_(nullptr)
			, mouseover_(nullptr)
			, selected_(nullptr)
			, disabled_(nullptr)
		{
			this->SetNormal(normal);
			this->SetMouseOver(mouseover);
			this->SetSelected(selected);
			this->SetCallbackOnClick(func);
		}

		Button::Button(spNode const& normal, spNode const& mouseover, spNode const& selected, spNode const& disabled, const Callback& func)
			: callback_(nullptr)
			, status_(Status::Normal)
			, enabled_(true)
			, is_selected_(false)
			, normal_(nullptr)
			, mouseover_(nullptr)
			, selected_(nullptr)
			, disabled_(nullptr)
		{
			this->SetNormal(normal);
			this->SetMouseOver(mouseover);
			this->SetSelected(selected);
			this->SetDisabled(disabled);
			this->SetCallbackOnClick(func);
		}

		Button::~Button()
		{
		}

		bool Button::IsEnable() const
		{
			return enabled_;
		}

		void Button::SetNormal(spNode const& normal)
		{
			SET_BUTTON_NODE(normal_, normal);
			if (normal)
			{
				this->SetSize(normal->GetWidth(), normal->GetHeight());
			}
		}

		void Button::SetMouseOver(spNode const& mouseover)
		{
			SET_BUTTON_NODE(mouseover_, mouseover);
		}

		void Button::SetSelected(spNode const& selected)
		{
			SET_BUTTON_NODE(selected_, selected);
		}

		void Button::SetDisabled(spNode const& disabled)
		{
			SET_BUTTON_NODE(disabled_, disabled);
		}

		void Button::SetEnabled(bool enabled)
		{
			if (enabled_ != enabled)
			{
				enabled_ = enabled;
				UpdateVisible();
			}
		}

		void Button::SetCallbackOnClick(const Callback& func)
		{
			callback_ = func;
		}

		void Button::SetPivot(float pivot_x, float pivot_y)
		{
			Node::SetPivot(pivot_x, pivot_y);
			SAFE_SET(normal_, SetPivot, pivot_x, pivot_y);
			SAFE_SET(mouseover_, SetPivot, pivot_x, pivot_y);
			SAFE_SET(selected_, SetPivot, pivot_x, pivot_y);
			SAFE_SET(disabled_, SetPivot, pivot_x, pivot_y);
		}

		bool Button::Dispatch(const MouseEvent & e, bool handled)
		{
			if (!handled && enabled_ && IsVisible() && normal_)
			{
				bool contains = normal_->ContainsPoint(e.GetPosition());
				if (e.GetType() == MouseEvent::Type::LeftUp && is_selected_ && contains)
				{
					if (callback_)
					{
						callback_();
					}
					is_selected_ = false;
					SetStatus(Status::Normal);
					return true;
				}
				else if (e.GetType() == MouseEvent::Type::LeftDown)
				{
					is_selected_ = contains;
					SetStatus(contains ? Status::Selected : Status::Normal);

					if (contains)
						return true;
				}
				else if (e.GetType() == MouseEvent::Type::LeftUp)
				{
					is_selected_ = false;
				}
				else if (e.GetType() == MouseEvent::Type::MoveBy && is_selected_ && contains)
				{
					SetStatus(Status::Selected);
					return true;
				}
				else
				{
					if (!e.IsLButtonDown() && is_selected_)
					{
						is_selected_ = false;
					}

					SetStatus(contains ? Status::Mouseover : Status::Normal);

					if (contains)
						return true;
				}
			}

			return Node::Dispatch(e, handled);
		}

		void Button::Visit()
		{
			Node::Visit();

			if (IsVisible() &&
				!enabled_ &&
				normal_ &&
				normal_->ContainsPoint(devices::Input::Instance().GetMousePos()))
			{
				HCURSOR hcursor = ::LoadCursor(nullptr, IDC_NO);
				if (hcursor)
				{
					::SetCursor(hcursor);
				}
			}
			else if (status_ == Status::Mouseover || status_ == Status::Selected)
			{
				HCURSOR hcursor = ::LoadCursor(nullptr, IDC_HAND);
				if (hcursor)
				{
					::SetCursor(hcursor);
				}
			}
		}

		void Button::SetStatus(Status status)
		{
			if (status_ != status)
			{
				status_ = status;
				UpdateVisible();
			}
		}

		void Button::UpdateVisible()
		{
			SAFE_SET(normal_, SetVisible, false);
			SAFE_SET(mouseover_, SetVisible, false);
			SAFE_SET(selected_, SetVisible, false);
			SAFE_SET(disabled_, SetVisible, false);

			if (enabled_)
			{
				if (status_ == Status::Selected && selected_)
				{
					selected_->SetVisible(true);
				}
				else if (status_ == Status::Mouseover && mouseover_)
				{
					mouseover_->SetVisible(true);
				}
				else
				{
					if (normal_) normal_->SetVisible(true);
				}
			}
			else
			{
				if (disabled_)
				{
					disabled_->SetVisible(true);
				}
				else
				{
					if (normal_) normal_->SetVisible(true);
				}
			}
		}
	}
}