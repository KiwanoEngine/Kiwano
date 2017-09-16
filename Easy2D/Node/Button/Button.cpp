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
	if (!m_bEnable)
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

bool Button::isEnable()
{
	return m_bEnable;
}

void Button::setEnable(bool enable)
{
	m_bEnable = enable;
}
