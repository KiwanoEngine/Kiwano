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

#include "Menu.h"

namespace easy2d
{
	namespace ui
	{
		Menu::Menu()
			: enabled_(true)
		{
		}

		Menu::Menu(Array<ButtonPtr> const& buttons)
			: enabled_(true)
		{
			for (const auto& button : buttons)
			{
				this->AddButton(button);
			}
		}

		bool Menu::IsEnable() const
		{
			return enabled_;
		}

		size_t Menu::GetButtonCount() const
		{
			return buttons_.size();
		}

		void Menu::SetEnabled(bool enabled)
		{
			if (enabled_ != enabled)
			{
				enabled_ = enabled;

				for (const auto& button : buttons_)
				{
					button->SetEnabled(enabled);
				}
			}
		}

		void Menu::AddButton(ButtonPtr const& button)
		{
			if (button)
			{
				this->AddChild(button);
				buttons_.push_back(button);
				button->SetEnabled(enabled_);
			}
		}

		bool Menu::RemoveButton(ButtonPtr const& button)
		{
			if (buttons_.empty())
			{
				return false;
			}

			this->RemoveChild(button);

			if (button)
			{
				auto iter = std::find(buttons_.begin(), buttons_.end(), button);
				if (iter != buttons_.end())
				{
					// �Ƴ���ťǰ����������
					button->SetEnabled(true);
					buttons_.erase(iter);
					return true;
				}
			}
			return false;
		}

		Array<ButtonPtr> const& Menu::GetAllButtons() const
		{
			return buttons_;
		}
	}
}