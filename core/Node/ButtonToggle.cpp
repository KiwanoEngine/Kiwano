#include "..\enodes.h"

e2d::EButtonToggle::EButtonToggle()
	: EButton()
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

e2d::EButtonToggle::EButtonToggle(ENode * toggleOnNormal, ENode * toggleOffNormal, const BtnClkCallback & callback)
	: EButton()
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
	this->setCallback(callback);
}

e2d::EButtonToggle::EButtonToggle(ENode * toggleOnNormal, ENode * toggleOffNormal, ENode * toggleOnSelected, ENode * toggleOffSelected, const BtnClkCallback & callback)
	: EButton()
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
	this->setCallback(callback);
}

e2d::EButtonToggle::EButtonToggle(ENode * toggleOnNormal, ENode * toggleOffNormal, ENode * toggleOnMouseOver, ENode * toggleOffMouseOver, ENode * toggleOnSelected, ENode * toggleOffSelected, const BtnClkCallback & callback)
	: EButton()
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
	this->setCallback(callback);
}

e2d::EButtonToggle::EButtonToggle(ENode * toggleOnNormal, ENode * toggleOffNormal, ENode * toggleOnMouseOver, ENode * toggleOffMouseOver, ENode * toggleOnSelected, ENode * toggleOffSelected, ENode * toggleOnDisabled, ENode * toggleOffDisabled, const BtnClkCallback & callback)
	: EButton()
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
	this->setCallback(callback);
}

void e2d::EButtonToggle::toggle()
{
	// 设置按钮状态
	setState(!m_bState);
	// 执行回调函数
	if (m_Callback)
	{
		m_Callback();
	}
}

bool e2d::EButtonToggle::getState() const
{
	return m_bState;
}

void e2d::EButtonToggle::setState(bool bState)
{
	if (m_bState != bState)
	{
		m_bState = bState;
		_updateState();
		_updateVisiable();
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
			normal->setPivot(m_fPivotX, m_fPivotY);
		}
		m_pNormalOn = normal;

		_updateState();
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
			mouseover->setPivot(m_fPivotX, m_fPivotY);
		}
		m_pMouseoverOn = mouseover;

		_updateState();
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
			selected->setPivot(m_fPivotX, m_fPivotY);
		}
		m_pSelectedOn = selected;

		_updateState();
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
			disabled->setPivot(m_fPivotX, m_fPivotY);
		}
		m_pDisabledOn = disabled;

		_updateState();
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
			normal->setPivot(m_fPivotX, m_fPivotY);
		}
		m_pNormalOff = normal;

		_updateState();
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
			mouseover->setPivot(m_fPivotX, m_fPivotY);
		}
		m_pMouseoverOff = mouseover;

		_updateState();
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
			selected->setPivot(m_fPivotX, m_fPivotY);
		}
		m_pSelectedOff = selected;

		_updateState();
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
			disabled->setPivot(m_fPivotX, m_fPivotY);
		}
		m_pDisabledOff = disabled;

		_updateState();
		_updateVisiable();
	}
}

void e2d::EButtonToggle::setPivotX(float pivotX)
{
	ENode::setPivotX(pivotX);
	if (m_pNormalOn) m_pNormalOn->setPivotX(pivotX);
	if (m_pMouseoverOn) m_pMouseoverOn->setPivotX(pivotX);
	if (m_pSelectedOn) m_pSelectedOn->setPivotX(pivotX);
	if (m_pDisabledOn) m_pDisabledOn->setPivotX(pivotX);
	if (m_pNormalOff) m_pNormalOff->setPivotX(pivotX);
	if (m_pMouseoverOff) m_pMouseoverOff->setPivotX(pivotX);
	if (m_pSelectedOff) m_pSelectedOff->setPivotX(pivotX);
	if (m_pDisabledOff) m_pDisabledOff->setPivotX(pivotX);
}

void e2d::EButtonToggle::setPivotY(float pivotY)
{
	ENode::setPivotY(pivotY);
	if (m_pNormalOn) m_pNormalOn->setPivotY(pivotY);
	if (m_pMouseoverOn) m_pMouseoverOn->setPivotY(pivotY);
	if (m_pSelectedOn) m_pSelectedOn->setPivotY(pivotY);
	if (m_pDisabledOn) m_pDisabledOn->setPivotY(pivotY);
	if (m_pNormalOff) m_pNormalOff->setPivotY(pivotY);
	if (m_pMouseoverOff) m_pMouseoverOff->setPivotY(pivotY);
	if (m_pSelectedOff) m_pSelectedOff->setPivotY(pivotY);
	if (m_pDisabledOff) m_pDisabledOff->setPivotY(pivotY);
}

void e2d::EButtonToggle::setPivot(float pivotX, float pivotY)
{
	ENode::setPivot(pivotX, pivotY);
	if (m_pNormalOn) m_pNormalOn->setPivot(pivotX, pivotY);
	if (m_pMouseoverOn) m_pMouseoverOn->setPivot(pivotX, pivotY);
	if (m_pSelectedOn) m_pSelectedOn->setPivot(pivotX, pivotY);
	if (m_pDisabledOn) m_pDisabledOn->setPivot(pivotX, pivotY);
	if (m_pNormalOff) m_pNormalOff->setPivot(pivotX, pivotY);
	if (m_pMouseoverOff) m_pMouseoverOff->setPivot(pivotX, pivotY);
	if (m_pSelectedOff) m_pSelectedOff->setPivot(pivotX, pivotY);
	if (m_pDisabledOff) m_pDisabledOff->setPivot(pivotX, pivotY);
}

void e2d::EButtonToggle::_updateState()
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

void e2d::EButtonToggle::_runCallback()
{
	m_bState = !m_bState;
	_updateState();

	if (m_Callback)
	{
		m_Callback();
	}
}
