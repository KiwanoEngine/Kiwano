#include "..\enodes.h"
#include "..\elisteners.h"
#include "..\emanagers.h"
#include "..\Win\winbase.h"

e2d::EButton::EButton()
	: m_Callback([] {})
	, m_eStatus(STATUS::NORMAL)
	, m_bIsDisable(false)
	, m_bIsSelected(false)
	, m_pNormal(nullptr)
	, m_pMouseover(nullptr)
	, m_pSelected(nullptr)
	, m_pDisabled(nullptr)
	, m_pDisplayed(nullptr)
{
}

e2d::EButton::EButton(ENode * normal, const BUTTON_CLICK_CALLBACK & callback)
	: EButton()
{
	this->setNormal(normal);
	this->setCallback(callback);
}

e2d::EButton::EButton(ENode * normal, ENode * selected, const BUTTON_CLICK_CALLBACK & callback)
	: EButton()
{
	this->setNormal(normal);
	this->setSelected(selected);
	this->setCallback(callback);
}

e2d::EButton::EButton(ENode * normal, ENode * mouseover, ENode * selected, const BUTTON_CLICK_CALLBACK & callback)
	: EButton()
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setCallback(callback);
}

e2d::EButton::EButton(ENode * normal, ENode * mouseover, ENode * selected, ENode * disabled, const BUTTON_CLICK_CALLBACK & callback)
	: EButton()
{
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setDisabled(disabled);
	this->setCallback(callback);
}

void e2d::EButton::setNormal(ENode * normal)
{
	if (normal)
	{
		if (m_pNormal)
		{
			this->removeChild(m_pNormal);
		}
		m_pNormal = normal;
		this->addChild(m_pNormal);
	}
}

void e2d::EButton::setMouseOver(ENode * mouseover)
{
	if (mouseover)
	{
		if (m_pMouseover)
		{
			this->removeChild(m_pMouseover);
		}
		m_pMouseover = mouseover;
		this->addChild(m_pMouseover);
	}
}

void e2d::EButton::setSelected(ENode * selected)
{
	if (selected)
	{
		if (m_pSelected)
		{
			this->removeChild(m_pSelected);
		}
		m_pSelected = selected;
		this->addChild(m_pSelected);
	}
}

void e2d::EButton::setDisabled(ENode * disabled)
{
	if (disabled)
	{
		if (m_pDisabled)
		{
			this->removeChild(m_pDisabled);
		}
		m_pDisabled = disabled;
		this->addChild(m_pDisabled);
	}
}

void e2d::EButton::setDisable(bool disable)
{
	m_bIsDisable = disable;
}

void e2d::EButton::setCallback(const BUTTON_CLICK_CALLBACK & callback)
{
	auto listener = new EListenerMouse(std::bind(&EButton::_listenerCallback, this));
	EMsgManager::bindListener(listener, this, true);
	m_Callback = callback;
}

void e2d::EButton::_callOn()
{
	if (!m_bVisiable)
	{
		return;
	}

	if (m_bTransformChildrenNeeded)
	{
		_updateTransform(this);
	}

	if (!m_vChildren.empty())
	{
		GetRenderTarget()->SetTransform(m_Matri);

		if (m_eStatus == STATUS::DISABLED && m_pDisabled)
		{
			m_pDisabled->_callOn();
		}
		else if (m_eStatus == STATUS::SELECTED && m_pSelected)
		{
			m_pSelected->_callOn();
		}
		else if (m_eStatus == STATUS::MOUSEOVER && m_pMouseover)
		{
			m_pMouseover->_callOn();
		}
		else if (m_pNormal)
		{
			m_pNormal->_callOn();
		}
	}
}

void e2d::EButton::_listenerCallback()
{
	if (!m_bIsDisable)
	{
		if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_DOWN &&
			m_pDisplayed &&
			m_pDisplayed->isPointIn(EMouseMsg::getPos()))
		{
			// 鼠标左键按下，且位于按钮内时，标记 m_bIsSelected 为 true
			m_bIsSelected = true;
		}
		else if (EMouseMsg::getMsg() == EMouseMsg::LBUTTON_UP)
		{
			// 鼠标左键抬起时，判断鼠标坐标是否在按钮内部
			if (m_bIsSelected && 
				m_pDisplayed &&
				m_pDisplayed->isPointIn(EMouseMsg::getPos()))
			{
				m_Callback();
				m_eStatus = STATUS::NORMAL;
				m_pDisplayed = m_pNormal;
			}
			// 标记 m_bIsSelected 为 false
			m_bIsSelected = false;
		}

		if (EMouseMsg::isLButtonDown() &&
			m_bIsSelected && 
			m_pSelected && 
			m_pSelected->isPointIn(EMouseMsg::getPos()))
		{
			// 鼠标左键按下，且按钮是被选中的状态，且鼠标
			// 在按钮内时，按钮状态变为 SELECTED
			m_eStatus = STATUS::SELECTED;
			m_pDisplayed = m_pSelected;
		}
		else if (EMouseMsg::getMsg() == EMouseMsg::MOVE &&
			m_pDisplayed &&
			m_pDisplayed->isPointIn(EMouseMsg::getPos()) &&
			m_pMouseover)
		{
			m_eStatus = STATUS::MOUSEOVER;
			m_pDisplayed = m_pMouseover;
		}
		else
		{
			m_eStatus = STATUS::NORMAL;
			m_pDisplayed = m_pNormal;
		}
	}
	else
	{
		if (m_pDisplayed)
		{
			m_eStatus = STATUS::DISABLED;
		}
		else
		{
			m_eStatus = STATUS::NORMAL;
		}
	}
}
