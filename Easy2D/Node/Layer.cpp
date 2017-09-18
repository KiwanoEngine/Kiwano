#include "..\easy2d.h"


Layer::Layer() :
	m_bBlock(true)
{
}

Layer::~Layer()
{
}

int Layer::getBlock() const
{
	return m_bBlock;
}

void Layer::setBlock(bool block)
{
	m_bBlock = block;
}

bool Layer::_exec(bool active)
{
	BatchNode::_exec(active);
	// 若图层阻塞消息，则永远取得画面焦点
	return m_bBlock;
}
