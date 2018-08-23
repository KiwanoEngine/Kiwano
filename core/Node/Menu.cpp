#include "..\e2dnode.h"

e2d::Menu::Menu()
	: _enabled(true)
{
}

e2d::Menu::Menu(const std::vector<Button*>& buttons)
	: _enabled(true)
{
	for (const auto& button : buttons)
	{
		this->addButton(button);
	}
}

bool e2d::Menu::enabled() const
{
	return _enabled;
}

int e2d::Menu::buttonCount() const
{
	return static_cast<int>(_buttons.size());
}

e2d::Menu& e2d::Menu::enabled(bool enabled)
{
	if (_enabled != enabled)
	{
		_enabled = enabled;

		for (const auto& button : _buttons)
		{
			button->enabled(enabled);
		}
	}
	return *this;
}

e2d::Menu& e2d::Menu::addButton(Button * button)
{
	if (button)
	{
		this->addChild(button);
		_buttons.push_back(button);
		button->enabled(_enabled);
	}
	return *this;
}

bool e2d::Menu::removeButton(Button * button)
{
	if (_buttons.empty())
	{
		return false;
	}

	this->removeChild(button);

	if (button)
	{
		auto iter = std::find(_buttons.begin(), _buttons.end(), button);
		if (iter != _buttons.end())
		{
			// 移除按钮前，将它启用
			button->enabled(true);
			_buttons.erase(iter);
			return true;
		}
	}
	return false;
}

const std::vector<e2d::Button*>& e2d::Menu::buttons() const
{
	return _buttons;
}
