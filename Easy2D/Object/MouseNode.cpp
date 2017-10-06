#include "..\easy2d.h"


MouseNode::MouseNode() :
	m_bBlock(true),
	m_bTarget(false),
	m_ClickCallback([]() {}),
	m_OnMouseInCallback([]() {}),
	m_OnMouseOutCallback([]() {}),
	m_OnSelectCallback([]() {}),
	m_OnUnselectCallback([]() {})
{
}

MouseNode::~MouseNode()
{
}

bool MouseNode::_exec(bool active)
{
	// 若 display 属性为 false，退出函数
	if (!m_bDisplay)
	{
		return false;
	}
	// 若画面已取得焦点，重置按钮属性并退出
	if (!active)
	{
		reset();
		return false;
	}
	// 判断节点当前的状态
	// 若节点未取得焦点，则重新判断节点状态
	if (!m_bTarget)
	{
		// 若鼠标位置在节点所在的矩形区域中
		if (_isMouseIn())
		{
			// 状态设为 MOUSEIN
			_setStatus(MOUSEIN);
			// 若此时按下鼠标左键
			if (MouseMsg::getMsg() == MouseMsg::LBUTTON_DOWN)
			{
				m_bTarget = true;		// 取得焦点标记
				_setStatus(SELECTED);	// 状态设为 SELECTED
			}
			// 若节点阻塞鼠标消息，则取得画面焦点
			if (m_bBlock) return true;
		}
		else
		{
			reset();		// 恢复默认状态
		}
	}
	else
	{
		// 节点取得焦点时鼠标左键抬起
		if (MouseMsg::getMsg() == MouseMsg::LBUTTON_UP)
		{
			// 若左键抬起时鼠标仍在节点内
			if (_isMouseIn())
			{
				m_ClickCallback();	// 执行回调函数
			}
			reset();				// 恢复默认状态
		}
		// 若节点阻塞鼠标消息，则取得画面焦点
		if (m_bBlock) return true;
	}
	return false;
}

void MouseNode::_onDraw()
{
	// 节点是否显示
	if (!m_bDisplay)
	{
		return;
	}
	// 节点是否被选中
	if (m_eStatus == SELECTED)
	{
		_onSelected();
	}
	else
	{
		// 鼠标是否在节点上
		if (m_eStatus == MOUSEIN)
		{
			_onMouseIn();
		}
		else
		{
			_onNormal();
		}
	}
}

bool MouseNode::_isMouseIn()
{
	return isPointIn(MouseMsg::getPos());
}

void MouseNode::_setStatus(Status status)
{
	if (m_eStatus != status)
	{
		// 退出某个状态的回调函数
		if (m_eStatus == MOUSEIN)
		{
			m_OnMouseOutCallback();
		}
		else if (m_eStatus == SELECTED)
		{
			m_OnUnselectCallback();
		}
		// 进入某个状态的回调函数
		if (status == MOUSEIN)
		{
			m_OnMouseInCallback();
		}
		else if (status == SELECTED)
		{
			m_OnSelectCallback();
		}
		m_eStatus = status;
	}
}

bool MouseNode::isMouseIn()
{
	return m_eStatus == MOUSEIN || m_eStatus == SELECTED;
}

bool MouseNode::isSelected()
{
	return m_eStatus == SELECTED;
}

void MouseNode::setClickedCallback(const CLICK_CALLBACK & callback)
{
	m_ClickCallback = callback;
}

void MouseNode::setMouseInCallback(const CLICK_CALLBACK & callback)
{
	m_OnMouseInCallback = callback;
}

void MouseNode::setMouseOutCallback(const CLICK_CALLBACK & callback)
{
	m_OnMouseOutCallback = callback;
}

void MouseNode::setSelectCallback(const CLICK_CALLBACK & callback)
{
	m_OnSelectCallback = callback;
}

void MouseNode::setUnselectCallback(const CLICK_CALLBACK & callback)
{
	m_OnUnselectCallback = callback;
}

void MouseNode::reset()
{
	m_bTarget = false;		// 失去焦点标记
	_setStatus(NORMAL);		// 恢复默认状态
}

void MouseNode::setBlock(bool block)
{
	m_bBlock = block;
}
