#include "..\enodes.h"

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

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, const ButtonCallback & callback)
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
	this->setCallback(callback);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnSelected, Node * toggleOffSelected, const ButtonCallback & callback)
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
	this->setCallback(callback);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, const ButtonCallback & callback)
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
	this->setCallback(callback);
}

e2d::ButtonToggle::ButtonToggle(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, Node * toggleOnDisabled, Node * toggleOffDisabled, const ButtonCallback & callback)
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
	this->setCallback(callback);
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
		// �Ƴ��ɵ�
		if (m_pNormalOn)
		{
			this->removeChild(m_pNormalOn);
		}
		// �����µ�
		if (normal)
		{
			this->addChild(normal);
			normal->setPivot(m_fPivotX, m_fPivotY);
			this->_setSize(normal->getWidth(), normal->getHeight());
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
		// �Ƴ��ɵ�
		if (m_pMouseoverOn)
		{
			this->removeChild(m_pMouseoverOn);
		}
		// �����µ�
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

void e2d::ButtonToggle::setSelected(Node * selected)
{
	if (selected != m_pSelectedOn)
	{
		// �Ƴ��ɵ�
		if (m_pSelectedOn)
		{
			this->removeChild(m_pSelectedOn);
		}
		// �����µ�
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

void e2d::ButtonToggle::setDisabled(Node * disabled)
{
	if (disabled != m_pDisabledOn)
	{
		// �Ƴ��ɵ�
		if (m_pDisabledOn)
		{
			this->removeChild(m_pDisabledOn);
		}
		// �����µ�
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

void e2d::ButtonToggle::setNormalOff(Node * normal)
{
	if (normal != m_pNormalOff)
	{
		// �Ƴ��ɵ�
		if (m_pNormalOff)
		{
			this->removeChild(m_pNormalOff);
		}
		// �����µ�
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

void e2d::ButtonToggle::setMouseOverOff(Node * mouseover)
{
	if (mouseover != m_pMouseoverOff)
	{
		// �Ƴ��ɵ�
		if (m_pMouseoverOff)
		{
			this->removeChild(m_pMouseoverOff);
		}
		// �����µ�
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

void e2d::ButtonToggle::setSelectedOff(Node * selected)
{
	if (selected != m_pSelectedOff)
	{
		// �Ƴ��ɵ�
		if (m_pSelectedOff)
		{
			this->removeChild(m_pSelectedOff);
		}
		// �����µ�
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

void e2d::ButtonToggle::setDisabledOff(Node * disabled)
{
	if (disabled != m_pDisabledOff)
	{
		// �Ƴ��ɵ�
		if (m_pDisabledOff)
		{
			this->removeChild(m_pDisabledOff);
		}
		// �����µ�
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

void e2d::ButtonToggle::setPivotX(float pivotX)
{
	Node::setPivotX(pivotX);
	if (m_pNormalOn) m_pNormalOn->setPivotX(pivotX);
	if (m_pMouseoverOn) m_pMouseoverOn->setPivotX(pivotX);
	if (m_pSelectedOn) m_pSelectedOn->setPivotX(pivotX);
	if (m_pDisabledOn) m_pDisabledOn->setPivotX(pivotX);
	if (m_pNormalOff) m_pNormalOff->setPivotX(pivotX);
	if (m_pMouseoverOff) m_pMouseoverOff->setPivotX(pivotX);
	if (m_pSelectedOff) m_pSelectedOff->setPivotX(pivotX);
	if (m_pDisabledOff) m_pDisabledOff->setPivotX(pivotX);
}

void e2d::ButtonToggle::setPivotY(float pivotY)
{
	Node::setPivotY(pivotY);
	if (m_pNormalOn) m_pNormalOn->setPivotY(pivotY);
	if (m_pMouseoverOn) m_pMouseoverOn->setPivotY(pivotY);
	if (m_pSelectedOn) m_pSelectedOn->setPivotY(pivotY);
	if (m_pDisabledOn) m_pDisabledOn->setPivotY(pivotY);
	if (m_pNormalOff) m_pNormalOff->setPivotY(pivotY);
	if (m_pMouseoverOff) m_pMouseoverOff->setPivotY(pivotY);
	if (m_pSelectedOff) m_pSelectedOff->setPivotY(pivotY);
	if (m_pDisabledOff) m_pDisabledOff->setPivotY(pivotY);
}

void e2d::ButtonToggle::setPivot(float pivotX, float pivotY)
{
	Node::setPivot(pivotX, pivotY);
	if (m_pNormalOn) m_pNormalOn->setPivot(pivotX, pivotY);
	if (m_pMouseoverOn) m_pMouseoverOn->setPivot(pivotX, pivotY);
	if (m_pSelectedOn) m_pSelectedOn->setPivot(pivotX, pivotY);
	if (m_pDisabledOn) m_pDisabledOn->setPivot(pivotX, pivotY);
	if (m_pNormalOff) m_pNormalOff->setPivot(pivotX, pivotY);
	if (m_pMouseoverOff) m_pMouseoverOff->setPivot(pivotX, pivotY);
	if (m_pSelectedOff) m_pSelectedOff->setPivot(pivotX, pivotY);
	if (m_pDisabledOff) m_pDisabledOff->setPivot(pivotX, pivotY);
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