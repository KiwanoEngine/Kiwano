#include "..\e2dnode.h"

e2d::Menu::Menu()
	: _enable(true)
{
}

e2d::Menu::Menu(const std::vector<Button*>& buttons)
	: _enable(true)
{
	for (auto button : buttons)
	{
		this->addButton(button);
	}
}

bool e2d::Menu::isEnable() const
{
	return _enable;
}

size_t e2d::Menu::getButtonCount() const
{
	return _buttons.size();
}

void e2d::Menu::setEnable(bool enable)
{
	if (_enable != enable)
	{
		_enable = enable;

		for (auto button : _buttons)
		{
			button->setEnable(enable);
		}
	}
}

void e2d::Menu::addButton(Button * button)
{
	if (button)
	{
		this->addChild(button);
		_buttons.push_back(button);
		button->setEnable(_enable);
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
		size_t size = _buttons.size();
		for (size_t i = 0; i < size; i++)
		{
			if (_buttons[i] == button)
			{
				// 移除按钮前，将它启用
				button->setEnable(true);
				_buttons.erase(_buttons.begin() + i);
				return true;
			}
		}
	}
	return false;
}

const std::vector<e2d::Button*>& e2d::Menu::getAllButtons() const
{
	return _buttons;
}
