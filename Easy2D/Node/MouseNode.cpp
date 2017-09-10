#include "..\easy2d.h"


MouseNode::MouseNode() :
	m_bBlock(true),
	m_bTarget(false),
	m_callback([]() {})
{
}

MouseNode::~MouseNode()
{
}

bool MouseNode::_exec(bool active)
{
	// 若画面已取得焦点，或 display 属性为 false，退出函数
	if (!active || !m_bDisplay)
	{
		return false;
	}
	// 判断节点状态
	_judge();
	// 鼠标在节点上（被选中时鼠标也在节点上）
	if (m_eStatus == MOUSEIN || m_eStatus == SELECTED)
	{
		// 节点被鼠标选中，且鼠标左键抬起
		if (m_bTarget && MouseMsg::getLButtonUpMsg())
		{
			onClicked();	// 执行回调函数
		}
		// 若节点不阻塞鼠标消息，则取得画面焦点
		if (!m_bBlock) return true;
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

void MouseNode::_setNormal()
{
	m_bTarget = false;		// 失去焦点标记
	m_eStatus = NORMAL;
}

void MouseNode::_setMouseIn()
{
	m_eStatus = MOUSEIN;
}

void MouseNode::_setSelected()
{
	m_bTarget = true;		// 取得焦点标记
	m_eStatus = SELECTED;
}

void MouseNode::onClicked()
{
	m_callback();
}

bool MouseNode::isMouseIn()
{
	return m_eStatus == MOUSEIN || m_eStatus == SELECTED;
}

bool MouseNode::isSelected()
{
	return m_eStatus == SELECTED;
}

void MouseNode::setOnMouseClicked(const CLICK_CALLBACK & callback)
{
	m_callback = callback;
}

void MouseNode::reset()
{
	m_eStatus = NORMAL;
}

void MouseNode::setBlock(bool block)
{
	m_bBlock = block;
}
