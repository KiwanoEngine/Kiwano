#include "..\e2dnode.h"

e2d::Menu::Menu()
	: _bEnable(true)
{
}

#ifdef HIGHER_THAN_VS2012
e2d::Menu::Menu(const std::initializer_list<Button*>& vButtons)
	: _bEnable(true)
{
	for (auto button : vButtons)
	{
		this->addButton(button);
	}
}

#else

e2d::Menu::Menu(int number, Button * button1, ...)
	: _bEnable(true)
{
	Button ** ppButton = &button1;

	while (number > 0)
	{
		this->addButton(*ppButton);
		ppButton++;
		number--;
	}
}
#endif

bool e2d::Menu::isEnable() const
{
	return _bEnable;
}

size_t e2d::Menu::getButtonCount() const
{
	return _vButtons.size();
}

void e2d::Menu::setEnable(bool enable)
{
	if (_bEnable != enable)
	{
		_bEnable = enable;

		for (auto button : _vButtons)
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
		_vButtons.push_back(button);
		button->setEnable(_bEnable);
	}
}

bool e2d::Menu::removeButton(Button * button)
{
	if (_vButtons.empty())
	{
		return false;
	}

	this->removeChild(button);

	if (button)
	{
		size_t size = _vButtons.size();
		for (size_t i = 0; i < size; i++)
		{
			if (_vButtons[i] == button)
			{
				// 移除按钮前，将它启用
				button->setEnable(true);
				_vButtons.erase(_vButtons.begin() + i);
				return true;
			}
		}
	}
	return false;
}
