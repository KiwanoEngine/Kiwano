#include "..\enodes.h"
#include "..\emsg.h"
#include "..\etools.h"
#include "..\Win\winbase.h"
#include <algorithm>

e2d::ENode::ENode()
	: m_nOrder(0)
	, m_fScaleX(1.0f)
	, m_fScaleY(1.0f)
	, m_fRotation(0)
	, m_fSkewAngleX(0)
	, m_fSkewAngleY(0)
	, m_fDisplayOpacity(1.0f)
	, m_fRealOpacity(1.0f)
	, m_fAnchorX(0)
	, m_fAnchorY(0)
	, m_Matri(D2D1::Matrix3x2F::Identity())
	, m_bVisiable(true)
	, m_bDisplayedInScene(false)
	, m_pParent(nullptr)
	, m_pParentScene(nullptr)
	, m_nHashName(0)
	, m_bSortChildrenNeeded(false)
	, m_bTransformChildrenNeeded(false)
{
}

e2d::ENode::ENode(const EString & name)
	: ENode()
{
	setName(name);
}

e2d::ENode::~ENode()
{
}

void e2d::ENode::onEnter()
{
}

void e2d::ENode::onExit()
{
}

void e2d::ENode::_callOn()
{
	if (!m_bVisiable)
	{
		return;
	}

	if (m_bTransformChildrenNeeded)
	{
		_updateTransform(this);
	}

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
				child->_callOn();
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
			m_vChildren[i]->_callOn();
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

void e2d::ENode::_onEnter()
{
	if (!this->m_bDisplayedInScene && this->isVisiable())
	{
		this->m_bDisplayedInScene = true;

		ETimerManager::_notifyAllTimersBindedWith(this);
		EMsgManager::_notifyAllMouseListenersBindedWith(this);
		EMsgManager::_notifyAllKeyboardListenersBindedWith(this);
		this->onEnter();

		for (const auto &child : m_vChildren)
		{
			child->_onEnter();
		}
	}
}

void e2d::ENode::_onExit()
{
	if (this->m_bDisplayedInScene)
	{
		this->m_bDisplayedInScene = false;

		ETimerManager::_waitAllTimersBindedWith(this);
		EMsgManager::_waitAllMouseListenersBindedWith(this);
		EMsgManager::_waitAllKeyboardListenersBindedWith(this);
		this->onExit();

		for (const auto &child : m_vChildren)
		{
			child->_onExit();
		}
	}
}

void e2d::ENode::_onClear()
{
	ETimerManager::clearAllTimersBindedWith(this);
	EMsgManager::clearAllMouseListenersBindedWith(this);
	EMsgManager::clearAllKeyboardListenersBindedWith(this);
	for (const auto &child : m_vChildren)
	{
		child->_onClear();
	}
}

void e2d::ENode::_sortChildren()
{
	if (m_bSortChildrenNeeded)
	{
		// 子节点排序
		std::sort(
			std::begin(m_vChildren), 
			std::end(m_vChildren), 
			[](ENode * n1, ENode * n2) {
				return n1->getOrder() < n2->getOrder();
			}
		);

		m_bSortChildrenNeeded = false;
	}
}

void e2d::ENode::_updateTransformToReal()
{
	// 计算锚点坐标
	D2D1_POINT_2F anchorPos = D2D1::Point2F(
		m_Size.width * m_fAnchorX,
		m_Size.height * m_fAnchorY
	);
	// 计算左上角坐标
	D2D1_POINT_2F upperLeftCorner = D2D1::Point2F(
		m_Pos.x - m_Size.width * m_fAnchorX,
		m_Pos.y - m_Size.height * m_fAnchorY
	);
	// 二维矩形变换
	m_Matri = D2D1::Matrix3x2F::Scale(
		m_fScaleX,
		m_fScaleY,
		anchorPos
	) * D2D1::Matrix3x2F::Skew(
		m_fSkewAngleX,
		m_fSkewAngleY,
		anchorPos
	) * D2D1::Matrix3x2F::Rotation(
		m_fRotation,
		anchorPos
	) * D2D1::Matrix3x2F::Translation(
		upperLeftCorner.x,
		upperLeftCorner.y
	);
}

void e2d::ENode::_updateChildrenTransform()
{
	for (const auto &child : m_vChildren)
	{
		_updateTransform(child);
	}
}

void e2d::ENode::_updateTransform(ENode * node)
{
	node->_updateTransformToReal();
	if (node->m_pParent)
	{
		node->m_Matri = node->m_Matri * node->m_pParent->m_Matri;
	}
	// 遍历子节点下的所有节点
	node->_updateChildrenTransform();
	node->m_bTransformChildrenNeeded = false;
}

void e2d::ENode::_updateChildrenOpacity()
{
	for (const auto &child : m_vChildren)
	{
		_updateOpacity(child);
	}
}

void e2d::ENode::_updateOpacity(ENode * node)
{
	if (node->m_pParent)
	{
		node->m_fDisplayOpacity = node->m_fRealOpacity * node->m_pParent->m_fDisplayOpacity;
	}
	node->_updateChildrenOpacity();
}

bool e2d::ENode::isVisiable() const
{
	return m_bVisiable;
}

e2d::EString e2d::ENode::getName() const
{
	return m_sName;
}

float e2d::ENode::getPosX() const
{
	return m_Pos.x;
}

float e2d::ENode::getPosY() const
{
	return m_Pos.y;
}

e2d::EPoint e2d::ENode::getPos() const
{
	return m_Pos;
}

float e2d::ENode::getWidth() const
{
	return m_Size.width * m_fScaleX;
}

float e2d::ENode::getHeight() const
{
	return m_Size.height * m_fScaleY;
}

float e2d::ENode::getRealWidth() const
{
	return m_Size.width;
}

float e2d::ENode::getRealHeight() const
{
	return m_Size.height;
}

e2d::ESize e2d::ENode::getRealSize() const
{
	return m_Size;
}

e2d::ESize e2d::ENode::getSize() const
{
	return ESize(getWidth(), getHeight());
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
	return m_fRealOpacity;
}

int e2d::ENode::getOrder() const
{
	return m_nOrder;
}

void e2d::ENode::setOrder(int order)
{
	m_nOrder = order;
}

void e2d::ENode::setPosX(float x)
{
	this->setPos(x, m_Pos.y);
}

void e2d::ENode::setPosY(float y)
{
	this->setPos(m_Pos.x, y);
}

void e2d::ENode::setPos(const EPoint & p)
{
	this->setPos(p.x, p.y);
}

void e2d::ENode::setPos(float x, float y)
{
	if (m_Pos.x == x && m_Pos.y == y)
		return;

	m_Pos.x = x;
	m_Pos.y = y;
	m_bTransformChildrenNeeded = true;
}

void e2d::ENode::move(float x, float y)
{
	this->setPos(m_Pos.x + x, m_Pos.y + y);
}

void e2d::ENode::move(const EVec & v)
{
	this->move(v.x, v.y);
}

void e2d::ENode::setWidth(float width)
{
	this->setSize(width, m_Size.height);
}

void e2d::ENode::setHeight(float height)
{
	this->setSize(m_Size.width, height);
}

void e2d::ENode::setSize(const ESize & size)
{
	this->setSize(size.width, size.height);
}

void e2d::ENode::setSize(float width, float height)
{
	if (m_Size.width == width && m_Size.height == height)
		return;

	m_Size.width = width;
	m_Size.height = height;
	m_bTransformChildrenNeeded = true;
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

	m_fScaleX = scaleX;
	m_fScaleY = scaleY;
	m_bTransformChildrenNeeded = true;
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

	m_fSkewAngleX = angleX;
	m_fSkewAngleY = angleY;
	m_bTransformChildrenNeeded = true;
}

void e2d::ENode::setRotation(float angle)
{
	if (m_fRotation == angle)
		return;

	m_fRotation = angle;
	m_bTransformChildrenNeeded = true;
}

void e2d::ENode::setOpacity(float opacity)
{
	if (m_fRealOpacity == opacity)
		return;

	m_fDisplayOpacity = m_fRealOpacity = min(max(opacity, 0), 1);
	// 更新子节点透明度
	_updateChildrenOpacity();
}

void e2d::ENode::setAnchorX(float anchorX)
{
	this->setAnchor(anchorX, m_fAnchorY);
}

void e2d::ENode::setAnchorY(float anchorY)
{
	this->setAnchor(m_fAnchorX, anchorY);
}

void e2d::ENode::setAnchor(float anchorX, float anchorY)
{
	if (m_fAnchorX == anchorX && m_fAnchorY == anchorY)
		return;

	m_fAnchorX = min(max(anchorX, 0), 1);
	m_fAnchorY = min(max(anchorY, 0), 1);
	m_bTransformChildrenNeeded = true;
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
	ASSERT(child->m_pParent == nullptr, "ENode already added. It can't be added again!");

	if (child)
	{
		for (ENode * parent = this; parent != nullptr; parent = parent->getParent())
		{
			ASSERT(child != parent, "A ENode cannot be the child of his own children!");
		}

		m_vChildren.push_back(child);

		child->setOrder(order);

		child->retain();

		child->m_pParent = this;

		_updateOpacity(child);

		if (this->m_pParentScene)
		{
			child->_setParentScene(this->m_pParentScene);
		}

		if (this->m_bDisplayedInScene)
		{
			child->_onEnter();
		}

		m_bSortChildrenNeeded = true;
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

e2d::EVector<e2d::ENode*>& e2d::ENode::getChildren()
{
	return m_vChildren;
}

size_t e2d::ENode::getChildrenCount() const
{
	return m_vChildren.size();
}

e2d::ENode * e2d::ENode::getChild(const EString & name)
{
	WARN_IF(name.empty(), "Invalid ENode name.");

	std::hash<EString> h;
	size_t hash = h(name);

	for (const auto& child : m_vChildren)
	{
		// 不同的名称可能会有相同的 Hash 值，但是先比较 Hash 可以提升搜索速度
		if (child->m_nHashName == hash && child->m_sName == name)
			return child;
	}
	return nullptr;
}

void e2d::ENode::removeFromParent(bool release /* = false */)
{
	if (m_pParent)
	{
		m_pParent->removeChild(this, release);
	}
}

bool e2d::ENode::removeChild(ENode * child, bool release /* = false */)
{
	WARN_IF(child == nullptr, "ENode::removeChild NULL pointer exception.");

	if (m_vChildren.empty())
	{
		return false;
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
				if (child->m_pParentScene)
				{
					child->_setParentScene(nullptr);
				}
				child->_onExit();
				child->release();
				if (release)
					child->autoRelease();
				return true;
			}
		}
	}
	return false;
}

void e2d::ENode::removeChild(const EString & childName, bool release /* = false */)
{
	WARN_IF(childName.empty(), "Invalid ENode name.");

	if (m_vChildren.empty())
	{
		return;
	}

	// 计算名称 Hash 值
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
			if (child->m_pParentScene)
			{
				child->_setParentScene(nullptr);
			}
			child->_onExit();
			child->release();
			if (release)
				child->autoRelease();
			return;
		}
	}
}

void e2d::ENode::clearAllChildren(bool release /* = false */)
{
	// 所有节点的引用计数减一
	for (auto child : m_vChildren)
	{
		if (release)
		{
			child->_onClear();
			child->autoRelease();
		}
		else
		{
			child->_onExit();
		}
		child->release();
	}
	// 清空储存节点的容器
	m_vChildren.clear();
}

void e2d::ENode::setVisiable(bool value)
{
	m_bVisiable = value;
	if (m_bDisplayedInScene == false)
	{
		this->_onEnter();
	}
}

void e2d::ENode::setName(const EString & name)
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

void e2d::ENode::_setParentScene(EScene * scene)
{
	m_pParentScene = scene;
	for (const auto &child : m_vChildren)
	{
		child->_setParentScene(scene);
	}
}