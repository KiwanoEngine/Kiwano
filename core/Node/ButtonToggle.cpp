#include "..\enode.h"

e2d::ButtonToggle::ButtonToggle()
	: Button()
	, m_bState(true)
	, m_pNormalOn(nullptr)
	, m_pMouseoverOn(nullptr)
	, m_pSelectedOn(nullptr)
	, m_pDisabledOn(nullptr)
	, m_pNormalOff(nullptr)
	, m_pMouseoverOff(nullptr)
	, m_pSelectedOff(nullptr)
	, m_pDisabledOff(nullptr)
{
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Function func)
	: Button()
	, m_bState(true)
	, m_pNormalOn(nullptr)
	, m_pMouseoverOn(nullptr)
	, m_pSelectedOn(nullptr)
	, m_pDisabledOn(nullptr)
	, m_pNormalOff(nullptr)
	, m_pMouseoverOff(nullptr)
	, m_pSelectedOff(nullptr)
	, m_pDisabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setClickFunc(func);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnSelected, Node * toggleOffSelected, Function func)
	: Button()
	, m_bState(true)
	, m_pNormalOn(nullptr)
	, m_pMouseoverOn(nullptr)
	, m_pSelectedOn(nullptr)
	, m_pDisabledOn(nullptr)
	, m_pNormalOff(nullptr)
	, m_pMouseoverOff(nullptr)
	, m_pSelectedOff(nullptr)
	, m_pDisabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setClickFunc(func);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, Function func)
	: Button()
	, m_bState(true)
	, m_pNormalOn(nullptr)
	, m_pMouseoverOn(nullptr)
	, m_pSelectedOn(nullptr)
	, m_pDisabledOn(nullptr)
	, m_pNormalOff(nullptr)
	, m_pMouseoverOff(nullptr)
	, m_pSelectedOff(nullptr)
	, m_pDisabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setMouseOver(toggleOnMouseOver);
	this->setMouseOverOff(toggleOffMouseOver);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setClickFunc(func);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, Node * toggleOnDisabled, Node * toggleOffDisabled, Function func)
	: Button()
	, m_bState(true)
	, m_pNormalOn(nullptr)
	, m_pMouseoverOn(nullptr)
	, m_pSelectedOn(nullptr)
	, m_pDisabledOn(nullptr)
	, m_pNormalOff(nullptr)
	, m_pMouseoverOff(nullptr)
	, m_pSelectedOff(nullptr)
	, m_pDisabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setMouseOver(toggleOnMouseOver);
	this->setMouseOverOff(toggleOffMouseOver);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setDisabled(toggleOnDisabled);
	this->setDisabledOff(toggleOffDisabled);
	this->setClickFunc(func);
}

bool e2d::ButtonToggle::getState() const
{
	return m_bState;
}

void e2d::ButtonToggle::setState(bool bState)
{
	if (m_bState != bState)
	{
		m_bState = bState;
		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setNormal(Node * normal)
{
	if (normal != m_pNormalOn)
	{
		// 移除旧的
		if (m_pNormalOn)
		{
			this->removeChild(m_pNormalOn);
		}
		// 添加新的
		if (normal)
		{
			this->addChild(normal);
			this->setSize(normal->getWidth(), normal->getHeight());
		}
		m_pNormalOn = normal;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setMouseOver(Node * mouseover)
{
	if (mouseover != m_pMouseoverOn)
	{
		// 移除旧的
		if (m_pMouseoverOn)
		{
			this->removeChild(m_pMouseoverOn);
		}
		// 添加新的
		if (mouseover)
		{
			this->addChild(mouseover);
		}
		m_pMouseoverOn = mouseover;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setSelected(Node * selected)
{
	if (selected != m_pSelectedOn)
	{
		// 移除旧的
		if (m_pSelectedOn)
		{
			this->removeChild(m_pSelectedOn);
		}
		// 添加新的
		if (selected)
		{
			this->addChild(selected);
		}
		m_pSelectedOn = selected;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setDisabled(Node * disabled)
{
	if (disabled != m_pDisabledOn)
	{
		// 移除旧的
		if (m_pDisabledOn)
		{
			this->removeChild(m_pDisabledOn);
		}
		// 添加新的
		if (disabled)
		{
			this->addChild(disabled);
		}
		m_pDisabledOn = disabled;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setNormalOff(Node * normal)
{
	if (normal != m_pNormalOff)
	{
		// 移除旧的
		if (m_pNormalOff)
		{
			this->removeChild(m_pNormalOff);
		}
		// 添加新的
		if (normal)
		{
			this->addChild(normal);
		}
		m_pNormalOff = normal;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setMouseOverOff(Node * mouseover)
{
	if (mouseover != m_pMouseoverOff)
	{
		// 移除旧的
		if (m_pMouseoverOff)
		{
			this->removeChild(m_pMouseoverOff);
		}
		// 添加新的
		if (mouseover)
		{
			this->addChild(mouseover);
		}
		m_pMouseoverOff = mouseover;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setSelectedOff(Node * selected)
{
	if (selected != m_pSelectedOff)
	{
		// 移除旧的
		if (m_pSelectedOff)
		{
			this->removeChild(m_pSelectedOff);
		}
		// 添加新的
		if (selected)
		{
			this->addChild(selected);
		}
		m_pSelectedOff = selected;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::setDisabledOff(Node * disabled)
{
	if (disabled != m_pDisabledOff)
	{
		// 移除旧的
		if (m_pDisabledOff)
		{
			this->removeChild(m_pDisabledOff);
		}
		// 添加新的
		if (disabled)
		{
			this->addChild(disabled);
		}
		m_pDisabledOff = disabled;

		_updateState();
		_updateVisiable();
	}
}

void e2d::ButtonToggle::_updateState()
{
	if (m_bState)
	{
		m_pNormal = m_pNormalOn;
		m_pMouseover = m_pMouseoverOn;
		m_pSelected = m_pSelectedOn;
		m_pDisabled = m_pDisabledOn;

		if (m_pNormalOff) m_pNormalOff->setVisiable(false);
		if (m_pMouseoverOff) m_pMouseoverOff->setVisiable(false);
		if (m_pSelectedOff) m_pSelectedOff->setVisiable(false);
		if (m_pDisabledOff) m_pDisabledOff->setVisiable(false);
	}
	else
	{
		m_pNormal = m_pNormalOff;
		m_pMouseover = m_pMouseoverOff;
		m_pSelected = m_pSelectedOff;
		m_pDisabled = m_pDisabledOff;

		if (m_pNormalOn) m_pNormalOn->setVisiable(false);
		if (m_pMouseoverOn) m_pMouseoverOn->setVisiable(false);
		if (m_pSelectedOn) m_pSelectedOn->setVisiable(false);
		if (m_pDisabledOn) m_pDisabledOn->setVisiable(false);
	}
}

void e2d::ButtonToggle::_runCallback()
{
	m_bState = !m_bState;
	_updateState();

	if (m_Callback)
	{
		m_Callback();
	}
}
