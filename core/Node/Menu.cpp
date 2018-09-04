#include "..\e2dnode.h"

e2d::Menu::Menu()
	: enabled_(true)
{
}

e2d::Menu::Menu(const std::vector<Button*>& buttons)
	: enabled_(true)
{
	for (const auto& button : buttons)
	{
		this->AddButton(button);
	}
}

bool e2d::Menu::IsEnable() const
{
	return enabled_;
}

size_t e2d::Menu::GetButtonCount() const
{
	return buttons_.size();
}

void e2d::Menu::SetEnabled(bool enabled)
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

void e2d::Menu::AddButton(Button * button)
{
	if (button)
	{
		this->AddChild(button);
		buttons_.push_back(button);
		button->SetEnabled(enabled_);
	}
}

bool e2d::Menu::RemoveButton(Button * button)
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
			// 移除按钮前，将它启用
			button->SetEnabled(true);
			buttons_.erase(iter);
			return true;
		}
	}
	return false;
}

const std::vector<e2d::Button*>& e2d::Menu::GetAllButtons() const
{
	return buttons_;
}
