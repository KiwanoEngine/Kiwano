#include "..\..\Easy2d.h"
#include "..\..\EasyX\easyx.h"


Button::Button() : 
	m_bEnable(true)
{
}

Button::~Button()
{
}

bool Button::_exec(bool active)
{
	// 按钮是否启用
	if (!m_bEnable || !m_bDisplay)
	{
		return false;
	}
	return MouseNode::_exec(active);
}

void Button::_onDraw()
{
	// 按钮是否启用
	if (!m_bEnable)
	{
		// 未启用时，绘制 Disable 状态
		_onDisable();
		return;
	}
	MouseNode::_onDraw();
}

void Button::_judge()
{
	// 判断按钮当前的状态
	// 若鼠标位置在按钮所在的矩形区域中
	if (MouseMsg::getMsg().x >= m_nX && MouseMsg::getMsg().x <= m_nX + m_nWidth &&
		MouseMsg::getMsg().y >= m_nY && MouseMsg::getMsg().y <= m_nY + m_nHeight)
	{
		_setMouseIn();

		// 若鼠标在按钮上，且鼠标左键按下
		if (MouseMsg::isLButtonDown())
		{
			_setSelected();
		}
	}
	else
	{
		_setNormal();
	}
}

bool Button::isEnable()
{
	return m_bEnable;
}

void Button::setEnable(bool enable)
{
	m_bEnable = enable;
}
