#include "..\enodes.h"

e2d::Menu::Menu()
	: m_bEnable(true)
{
}

e2d::Menu::Menu(int number, Button * button1, ...)
	: m_bEnable(true)
{
	Button ** ppButton = &button1;

	while (number > 0)
	{
		this->addButton(*ppButton);
		ppButton++;
		number--;
	}
}

bool e2d::Menu::isEnable() const
{
	return m_bEnable;
}

size_t e2d::Menu::getButtonCount() const
{
	return m_vButtons.size();
}

void e2d::Menu::setEnable(bool enable)
{
	if (m_bEnable != enable)
	{
		m_bEnable = enable;

		FOR_LOOP(button, m_vButtons)
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
		m_vButtons.push_back(button);
		button->setEnable(m_bEnable);
	}
}

bool e2d::Menu::removeButton(Button * button)
{
	if (m_vButtons.empty())
	{
		return false;
	}

	this->removeChild(button);

	if (button)
	{
		size_t size = m_vButtons.size();
		for (size_t i = 0; i < size; i++)
		{
			if (m_vButtons[i] == button)
			{
				// 移除按钮前，将它启用
				button->setEnable(true);
				m_vButtons.erase(m_vButtons.begin() + i);
				return true;
			}
		}
	}
	return false;
}
