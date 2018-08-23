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

bool e2d::Menu::isEnable() const
{
	return _enabled;
}

size_t e2d::Menu::getButtonCount() const
{
	return _buttons.size();
}

void e2d::Menu::setEnabled(bool enabled)
{
	if (_enabled != enabled)
	{
		_enabled = enabled;

		for (const auto& button : _buttons)
		{
			button->setEnabled(enabled);
		}
	}
}

void e2d::Menu::addButton(Button * button)
{
	if (button)
	{
		this->addChild(button);
		_buttons.push_back(button);
		button->setEnabled(_enabled);
	}
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
			button->setEnabled(true);
			_buttons.erase(iter);
			return true;
		}
	}
	return false;
}

const std::vector<e2d::Button*>& e2d::Menu::getAllButtons() const
{
	return _buttons;
}
