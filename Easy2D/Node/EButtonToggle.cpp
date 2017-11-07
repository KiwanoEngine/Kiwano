#include "..\enodes.h"
#include "..\elisteners.h"
#include "..\emanagers.h"
#include "..\Win\winbase.h"

e2d::EButtonToggle::EButtonToggle()
	: EButton()
	, m_bToggle(true)
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

e2d::EButtonToggle::EButtonToggle(ENode * toggleOnNormal, ENode * toggleOffNormal, const BUTTON_CLICK_CALLBACK & callback)
	: EButtonToggle()
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setCallback(callback);
}

e2d::EButtonToggle::EButtonToggle(ENode * toggleOnNormal, ENode * toggleOffNormal, ENode * toggleOnSelected, ENode * toggleOffSelected, const BUTTON_CLICK_CALLBACK & callback)
	: EButtonToggle()
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setCallback(callback);
}

e2d::EButtonToggle::EButtonToggle(ENode * toggleOnNormal, ENode * toggleOffNormal, ENode * toggleOnMouseOver, ENode * toggleOffMouseOver, ENode * toggleOnSelected, ENode * toggleOffSelected, const BUTTON_CLICK_CALLBACK & callback)
	: EButtonToggle()
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setMouseOver(toggleOnMouseOver);
	this->setMouseOverOff(toggleOffMouseOver);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setCallback(callback);
}

e2d::EButtonToggle::EButtonToggle(ENode * toggleOnNormal, ENode * toggleOffNormal, ENode * toggleOnMouseOver, ENode * toggleOffMouseOver, ENode * toggleOnSelected, ENode * toggleOffSelected, ENode * toggleOnDisabled, ENode * toggleOffDisabled, const BUTTON_CLICK_CALLBACK & callback)
	: EButtonToggle()
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setMouseOver(toggleOnMouseOver);
	this->setMouseOverOff(toggleOffMouseOver);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setDisabled(toggleOnDisabled);
	this->setDisabledOff(toggleOffDisabled);
	this->setCallback(callback);
}

void e2d::EButtonToggle::toggle()
{
	m_bToggle = !m_bToggle;
	_updateToggle();
	_updateVisiable();
}

bool e2d::EButtonToggle::isToggleOn() const
{
	return m_bToggle;
}

void e2d::EButtonToggle::setToggle(bool toggle)
{
	if (m_bToggle != toggle)
	{
		m_bToggle = toggle;
		_updateToggle();
	}
}

void e2d::EButtonToggle::setNormal(ENode * normal)
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
		}
		m_pNormalOn = normal;

		_updateToggle();
		_updateVisiable();
	}
}

void e2d::EButtonToggle::setMouseOver(ENode * mouseover)
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

		_updateToggle();
		_updateVisiable();
	}
}

void e2d::EButtonToggle::setSelected(ENode * selected)
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

		_updateToggle();
		_updateVisiable();
	}
}

void e2d::EButtonToggle::setDisabled(ENode * disabled)
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

		_updateToggle();
		_updateVisiable();
	}
}

void e2d::EButtonToggle::setNormalOff(ENode * normal)
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

		_updateToggle();
		_updateVisiable();
	}
}

void e2d::EButtonToggle::setMouseOverOff(ENode * mouseover)
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

		_updateToggle();
		_updateVisiable();
	}
}

void e2d::EButtonToggle::setSelectedOff(ENode * selected)
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

		_updateToggle();
		_updateVisiable();
	}
}

void e2d::EButtonToggle::setDisabledOff(ENode * disabled)
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

		_updateToggle();
		_updateVisiable();
	}
}

void e2d::EButtonToggle::_updateToggle()
{
	if (m_bToggle)
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

void e2d::EButtonToggle::_runCallback()
{
	m_bToggle = !m_bToggle;
	_updateToggle();

	if (m_Callback)
	{
		m_Callback();
	}
}
