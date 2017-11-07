#include "..\enodes.h"

e2d::EMenu::EMenu()
	: m_bEnable(true)
{
}

e2d::EMenu::EMenu(int number, EButton * button1, ...)
	: EMenu()
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
	m_bEnable = enable;

	for (auto &buttons : m_vButtons)
	{
		buttons->setEnable(enable);
	}
}

void e2d::EMenu::addButton(EButton * button)
{
	this->addChild(button);
	m_vButtons.push_back(button);
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
				m_vButtons.erase(m_vButtons.begin() + i);
				return true;
			}
		}
	}
	return false;
}
