#include "..\enodes.h"
#include "..\elisteners.h"

e2d::EMenu::EMenu()
	: m_bEnable(true)
{
}

e2d::EMenu::EMenu(int number, EButton * button1, ...)
	: m_bEnable(true)
{
	EButton ** ppButton = &button1;

	while (number > 0)
	{
		this->addButton(*ppButton);
		ppButton++;
		number--;
	}
}

bool e2d::EMenu::isEnable() const
{
	return m_bEnable;
}

size_t e2d::EMenu::getButtonCount() const
{
	return m_vButtons.size();
}

void e2d::EMenu::setEnable(bool enable)
{
	if (m_bEnable != enable)
	{
		m_bEnable = enable;

		for (auto button = m_vButtons.begin(); button != m_vButtons.end(); button++)
		{
			(*button)->setEnable(enable);
		}
	}
}

void e2d::EMenu::addButton(EButton * button)
{
	if (button)
	{
		this->addChild(button);
		m_vButtons.push_back(button);
		button->setEnable(m_bEnable);
	}
}

bool e2d::EMenu::removeButton(EButton * button)
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
