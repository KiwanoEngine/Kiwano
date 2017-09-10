#include "..\Easy2d.h"

Node::Node() :
	m_nZOrder(0),
	m_bDisplay(true),
	m_nX(0), 
	m_nY(0)
{
}

Node::Node(int x, int y) :
	m_nZOrder(0),
	m_bDisplay(true),
	m_nX(x),
	m_nY(y)
{
}

Node::~Node()
{
}

bool Node::_exec(bool active)
{
	return false;
}

void Node::_onDraw()
{
}

const int Node::getX() const
{
	return m_nX;
}

const int Node::getY() const
{
	return m_nY;
}

void Node::setX(int x)
{
	m_nX = x;
}

void Node::setY(int y)
{
	m_nY = y;
}

void Node::setPos(int x, int y)
{
	m_nX = x;
	m_nY = y;
}

void Node::move(int x, int y)
{
	m_nX += x;
	m_nY += y;
}

int Node::getZOrder() const
{
	return m_nZOrder;
}

void Node::setZOrder(int z)
{
	m_nZOrder = z;
}

Scene * Node::getParentScene()
{
	return m_pScene;
}

void Node::setParentScene(Scene * scene)
{
	m_pScene = scene;
}

void Node::setDisplay(bool value)
{
	m_bDisplay = value;
}

bool Node::display() const
{
	return m_bDisplay;
}