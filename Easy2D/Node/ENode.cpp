#include "..\enodes.h"
#include "..\Win\winbase.h"
#include <algorithm>

e2d::ENode::ENode()
	: m_nOrder(0)
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

		// 渲染自身
		this->_onRender();

		// 访问剩余节点
		for (; i < size; i++)
			m_vChildren[i]->callOn();
	}
	else
	{
		// 渲染自身
		this->_onRender();
	}
}

void e2d::ENode::_onRender()
{
}

void e2d::ENode::_onTransfrom()
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
		// 更新自身属性
		if (m_pParent)
		{
			this->setPos(m_pParent->getX() + m_Pos.x, m_pParent->getY() + m_Pos.y);
		}

		// 根据子节点特殊性进行更新
		_onTransfrom();

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

int e2d::ENode::getX() const
{
	return m_Rect.TopLeft().x;
}

int e2d::ENode::getY() const
{
	return m_Rect.TopLeft().y;
}

CPoint e2d::ENode::getPos() const
{
	return m_Rect.TopLeft();
}

UINT32 e2d::ENode::getWidth() const
{
	return UINT32(m_Rect.Size().cx);
}

UINT32 e2d::ENode::getHeight() const
{
	return UINT32(m_Rect.Size().cy);
}

e2d::ESize e2d::ENode::getSize() const
{
	return e2d::ESize(m_Rect.Size());
}

e2d::ERect e2d::ENode::getRect() const
{
	return e2d::ERect(m_Rect);
}

void e2d::ENode::setX(int x)
{
	this->setPos(x, m_Rect.TopLeft().y);
}

void e2d::ENode::setY(int y)
{
	this->setPos(m_Rect.TopLeft().x, y);
}

void e2d::ENode::setPos(EPoint p)
{
	this->setPos(p.x, p.y);
}

void e2d::ENode::setPos(int x, int y)
{
	if (getX() == x && getY() == y)
		return;

	if (!m_bTransformNeeded)
	{
		m_Pos.x = x;
		m_Pos.y = y;
		m_bTransformNeeded = true;
	}
	m_Rect.MoveToXY(x, y);
}

void e2d::ENode::move(int x, int y)
{
	m_Rect.OffsetRect(x, y);
}

void e2d::ENode::move(EVector v)
{
	m_Rect.OffsetRect(v);
}

void e2d::ENode::setWidth(UINT32 width)
{
	m_Rect.BottomRight().x = m_Rect.TopLeft().x + width;
}

void e2d::ENode::setHeight(UINT32 height)
{
	m_Rect.BottomRight().y = m_Rect.TopLeft().y + height;
}

void e2d::ENode::setSize(UINT32 width, UINT32 height)
{
	m_Rect.BottomRight().x = m_Rect.TopLeft().x + width;
	m_Rect.BottomRight().y = m_Rect.TopLeft().y + height;
}

void e2d::ENode::setSize(ESize size)
{
	this->setSize(size.cx, size.cy);
}

void e2d::ENode::setRect(int x1, int y1, int x2, int y2)
{
	m_Rect.SetRect(x1, y1, x2, y2);
}

void e2d::ENode::setRect(EPoint leftTop, EPoint rightBottom)
{
	m_Rect.TopLeft() = leftTop;
	m_Rect.BottomRight() = rightBottom;
}

void e2d::ENode::setRect(ERect rect)
{
	m_Rect = rect;
}

int e2d::ENode::getOrder() const
{
	return m_nOrder;
}

void e2d::ENode::setOrder(int order)
{
	m_nOrder = order;
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
