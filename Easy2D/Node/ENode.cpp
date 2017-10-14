#include "..\enodes.h"
#include "..\Win\winbase.h"
#include <algorithm>

e2d::ENode::ENode()
	: m_nOrder(0)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fWidth(0)
	, m_fHeight(0)
	, m_fScaleX(1)
	, m_fScaleY(1)
	, m_fRotation(0)
	, m_fSkewAngleX(0)
	, m_fSkewAngleY(0)
	, m_fDisplayOpacity(1)
	, m_fRealOpacity(1)
	, m_Matri(D2D1::Matrix3x2F::Identity())
	, m_bVisiable(true)
	, m_pParent(nullptr)
	, m_pParentScene(nullptr)
	, m_nHashName(0)
	, m_bSortNeeded(false)
	, m_bTransformNeeded(false)
{
}

e2d::ENode::ENode(EString name)
	: ENode()
{
	setName(name);
}

e2d::ENode::~ENode()
{
}

void e2d::ENode::callOn()
{
	if (!m_bVisiable)
	{
		return;
	}

	this->_transfrom();

	if (!m_vChildren.empty())
	{
		this->_sortChildren();

		size_t size = m_vChildren.size();
		size_t i;
		for (i = 0; i < size; i++)
		{
			auto child = m_vChildren[i];
			// 访问 Order 小于零的节点
			if (child->getOrder() < 0)
			{
				child->callOn();
			}
			else
			{
				break;
			}
		}

		GetRenderTarget()->SetTransform(m_Matri);
		// 渲染自身
		this->_onRender();

		// 访问剩余节点
		for (; i < size; i++)
			m_vChildren[i]->callOn();
	}
	else
	{
		GetRenderTarget()->SetTransform(m_Matri);
		// 渲染自身
		this->_onRender();
	}
}

void e2d::ENode::_onRender()
{
}

void e2d::ENode::_sortChildren()
{
	if (m_bSortNeeded)
	{
		// 子节点排序
		std::sort(
			std::begin(m_vChildren), 
			std::end(m_vChildren), 
			[](ENode * n1, ENode * n2) {
				return n1->getOrder() < n2->getOrder();
			}
		);

		m_bSortNeeded = false;
	}
}

void e2d::ENode::_transfrom()
{
	if (m_bTransformNeeded)
	{
		// 二维矩形变换
		m_Matri = D2D1::Matrix3x2F::Identity();
		m_Matri = m_Matri.Translation(m_fPosX, m_fPosY);
		m_Matri = m_Matri.Rotation(m_fRotation);
		m_Matri = m_Matri.Scale(m_fScaleX, m_fScaleY);
		m_Matri = m_Matri.Skew(m_fSkewAngleX, m_fSkewAngleY);
		
		if (m_pParent)
		{
			//m_Matri.SetProduct()
		}

		// 提示子节点更新属性
		for (const auto &child : m_vChildren)
		{
			child->m_bTransformNeeded = true;
		}

		m_bTransformNeeded = false;
	}
}

bool e2d::ENode::isVisiable() const
{
	return m_bVisiable;
}

float e2d::ENode::getX() const
{
	return m_fPosX;
}

float e2d::ENode::getY() const
{
	return m_fPosY;
}

float e2d::ENode::getWidth() const
{
	return m_fWidth;
}

float e2d::ENode::getHeight() const
{
	return m_fHeight;
}

float e2d::ENode::getScaleX() const
{
	return m_fScaleX;
}

float e2d::ENode::getScaleY() const
{
	return m_fScaleY;
}

float e2d::ENode::getSkewX() const
{
	return m_fSkewAngleX;
}

float e2d::ENode::getSkewY() const
{
	return m_fSkewAngleY;
}

float e2d::ENode::getRotation() const
{
	return m_fRotation;
}

float e2d::ENode::getOpacity() const
{
	return m_fDisplayOpacity;
}

int e2d::ENode::getOrder() const
{
	return m_nOrder;
}

void e2d::ENode::setOrder(int order)
{
	m_nOrder = order;
}

void e2d::ENode::setX(float x)
{
	this->setPos(x, m_fPosY);
}

void e2d::ENode::setY(float y)
{
	this->setPos(m_fPosX, y);
}

void e2d::ENode::setPos(float x, float y)
{
	if (m_fPosX == x && m_fPosY == y)
		return;

	//m_Matri.Translation(x, y);
	m_fPosX = x;
	m_fPosY = y;
	m_bTransformNeeded = true;
}

void e2d::ENode::move(float x, float y)
{
	this->setPos(m_fPosX + x, m_fPosY + y);
}

void e2d::ENode::setWidth(float width)
{
	this->setSize(width, m_fHeight);
}

void e2d::ENode::setHeight(float height)
{
	this->setSize(m_fWidth, height);
}

void e2d::ENode::setSize(float width, float height)
{
	if (m_fWidth == width && m_fHeight == height)
		return;

	m_fWidth = width;
	m_fHeight = height;
}

void e2d::ENode::setScaleX(float scaleX)
{
	this->setScale(scaleX, m_fScaleY);
}

void e2d::ENode::setScaleY(float scaleY)
{
	this->setScale(m_fScaleX, scaleY);
}

void e2d::ENode::setScale(float scale)
{
	this->setScale(scale, scale);
}

void e2d::ENode::setScale(float scaleX, float scaleY)
{
	if (m_fScaleX == scaleX && m_fScaleY == scaleY)
		return;

	//m_Matri.Scale(scaleX, scaleY);
	m_fScaleX = scaleX;
	m_fScaleY = scaleY;
	m_bTransformNeeded = true;
}

void e2d::ENode::setSkewX(float angleX)
{
	this->setSkew(angleX, m_fSkewAngleY);
}

void e2d::ENode::setSkewY(float angleY)
{
	this->setSkew(m_fSkewAngleX, angleY);
}

void e2d::ENode::setSkew(float angleX, float angleY)
{
	if (m_fSkewAngleX == angleX && m_fSkewAngleY == angleY)
		return;

	//m_Matri.Skew(angleX, angleY);
	m_fSkewAngleX = angleX;
	m_fSkewAngleY = angleY;
	m_bTransformNeeded = true;
}

void e2d::ENode::setRotation(float angle)
{
	if (m_fRotation == angle)
		return;

	//m_Matri.Rotation(angle);
	m_fRotation = angle;
	m_bTransformNeeded = true;
}

void e2d::ENode::setOpacity(float opacity)
{
	m_fDisplayOpacity = m_fRealOpacity = opacity;
}

void e2d::ENode::setParent(ENode * parent)
{
	if (m_pParent)
	{
		m_pParent->addChild(this);
	}
	else
	{
		removeFromParent();
	}
}

void e2d::ENode::addChild(ENode * child, int order  /* = 0 */)
{
	WARN_IF(child == nullptr, "ENode::addChild NULL pointer exception.");
	ASSERT(child->m_pParent == nullptr, "Child already added. It can't be added again!");

	if (child)
	{
		for (ENode * parent = this; parent != nullptr; parent = parent->getParent())
		{
			ASSERT(child != parent, "A ENode cannot be the child of his own children!");
		}

		m_vChildren.push_back(child);

		child->m_pParent = this;

		child->setOrder(order);

		child->retain();

		m_bSortNeeded = true;
	}
}

e2d::ENode * e2d::ENode::getParent() const
{
	return m_pParent;
}

e2d::EScene * e2d::ENode::getParentScene() const
{
	return m_pParentScene;
}

std::vector<e2d::ENode*>& e2d::ENode::getChildren()
{
	return m_vChildren;
}

size_t e2d::ENode::getChildrenCount() const
{
	return m_vChildren.size();
}

e2d::ENode * e2d::ENode::getChild(EString name) const
{
	return ENode::getChild(name, this->m_vChildren);
}

e2d::ENode * e2d::ENode::getChild(EString name, const std::vector<ENode*> &children)
{
	WARN_IF(name.empty(), "Invalid ENode name.");

	std::hash<EString> h;
	size_t hash = h(name);

	for (const auto& child : children)
	{
		// 不同的名称可能会有相同的 Hash 值，但是先比较 Hash 可以提升搜索速度
		if (child->m_nHashName == hash && child->m_sName == name)
			return child;
	}
	return nullptr;
}

void e2d::ENode::setParentScene(EScene * scene)
{
	if (m_pParentScene)
	{
		m_pParentScene = scene;
	}
}

void e2d::ENode::removeFromParent(bool release /* = false */)
{
	if (m_pParent)
	{
		m_pParent->removeChild(this, release);
	}
}

void e2d::ENode::removeChild(ENode * child, bool release /* = false */)
{
	WARN_IF(child == nullptr, "ENode::removeChild NULL pointer exception.");

	if (m_vChildren.empty())
	{
		return;
	}

	if (child)
	{
		size_t size = m_vChildren.size();
		for (size_t i = 0; i < size; i++)
		{
			if (m_vChildren[i] == child)
			{
				m_vChildren.erase(m_vChildren.begin() + i);
				child->m_pParent = nullptr;
				child->release();
				if (release)
					child->autoRelease();
				return;
			}
		}
	}
}

void e2d::ENode::removeChild(EString childName, bool release /* = false */)
{
	WARN_IF(childName.empty(), "Invalid ENode name.");

	if (m_vChildren.empty())
	{
		return;
	}

	std::hash<EString> h;
	size_t hash = h(childName);
	size_t size = m_vChildren.size();
	for (size_t i = 0; i < size; i++)
	{
		auto child = m_vChildren[i];
		if (child->m_nHashName == hash && child->m_sName == childName)
		{
			m_vChildren.erase(m_vChildren.begin() + i);
			child->m_pParent = nullptr;
			child->release();
			if (release)
				child->autoRelease();
			return;
		}
	}
}

void e2d::ENode::setVisiable(bool value)
{
	m_bVisiable = value;
}

void e2d::ENode::setName(EString name)
{
	WARN_IF(name.empty(), "Invalid ENode name.");

	if (!name.empty())
	{
		// 保存节点名
		m_sName = name;
		// 保存节点 Hash 名
		std::hash<EString> h;
		m_nHashName = h(name);
	}
}
