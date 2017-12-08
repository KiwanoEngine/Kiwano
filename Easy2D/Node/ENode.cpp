#include "..\enodes.h"
#include "..\emanagers.h"
#include "..\etools.h"
#include "..\eactions.h"
#include "..\egeometry.h"
#include "..\Win\winbase.h"
#include <algorithm>

// 默认中心点位置
static float s_fDefaultPiovtX = 0;
static float s_fDefaultPiovtY = 0;

e2d::ENode::ENode()
	: m_nOrder(0)
	, m_fScaleX(1.0f)
	, m_fScaleY(1.0f)
	, m_fRotation(0)
	, m_fSkewAngleX(0)
	, m_fSkewAngleY(0)
	, m_fDisplayOpacity(1.0f)
	, m_fRealOpacity(1.0f)
	, m_fPivotX(s_fDefaultPiovtX)
	, m_fPivotY(s_fDefaultPiovtY)
	, m_MatriInitial(D2D1::Matrix3x2F::Identity())
	, m_MatriFinal(D2D1::Matrix3x2F::Identity())
	, m_bVisiable(true)
	, m_bDisplayedInScene(false)
	, m_pGeometry(nullptr)
	, m_pParent(nullptr)
	, m_pParentScene(nullptr)
	, m_nHashName(0)
	, m_bSortChildrenNeeded(false)
	, m_bTransformNeeded(false)
{
}

e2d::ENode::ENode(const EString & name)
	: ENode()
{
	setName(name);
}

e2d::ENode::~ENode()
{
	ETimerManager::_clearAllTimersBindedWith(this);
	EMsgManager::_clearAllMouseListenersBindedWith(this);
	EMsgManager::_clearAllKeyboardListenersBindedWith(this);
	EActionManager::_clearAllActionsBindedWith(this);
	EPhysicsManager::_clearAllListenersBindedWith(this);
	EPhysicsManager::_delGeometry(m_pGeometry);
	for (auto child : m_vChildren)
	{
		SafeRelease(&child);
	}
}

void e2d::ENode::onEnter()
{
}

void e2d::ENode::onExit()
{
}

void e2d::ENode::_update()
{
	if (!m_bVisiable)
	{
		return;
	}

	if (m_bTransformNeeded)
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
				child->_update();
			}
			else
			{
				break;
			}
		}

		GetRenderTarget()->SetTransform(m_MatriFinal);
		// 渲染自身
		this->_render();

		// 访问剩余节点
		for (; i < size; i++)
			m_vChildren[i]->_update();
	}
	else
	{
		GetRenderTarget()->SetTransform(m_MatriFinal);
		// 渲染自身
		this->_render();
	}
}

void e2d::ENode::_render()
{
}

void e2d::ENode::_drawGeometry()
{
	// 绘制自身的几何形状
	if (m_pGeometry && m_pGeometry->m_bIsVisiable)
	{
		m_pGeometry->_render();
	}

	// 绘制所有子节点的几何形状
	for (auto &child : m_vChildren)
	{
		child->_drawGeometry();
	}
}

void e2d::ENode::_onEnter()
{
	if (!this->m_bDisplayedInScene && this->isVisiable())
	{
		this->m_bDisplayedInScene = true;
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
		this->onExit();

		for (const auto &child : m_vChildren)
		{
			child->_onExit();
		}
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

void e2d::ENode::_updateTransform()
{
	// 计算中心点坐标
	D2D1_POINT_2F pivot = D2D1::Point2F(
		getRealWidth() * m_fPivotX,
		getRealHeight() * m_fPivotY
	);
	// 初步的二维矩形变换，子节点将根据这个矩阵进行变换
	m_MatriInitial = D2D1::Matrix3x2F::Scale(
		m_fScaleX,
		m_fScaleY,
		pivot
	) * D2D1::Matrix3x2F::Skew(
		m_fSkewAngleX,
		m_fSkewAngleY,
		pivot
	) * D2D1::Matrix3x2F::Rotation(
		m_fRotation,
		pivot
	) * D2D1::Matrix3x2F::Translation(
		m_Pos.x,
		m_Pos.y
	);
	// 和父节点矩阵相乘
	if (m_pParent)
	{
		m_MatriInitial = m_MatriInitial * m_pParent->m_MatriInitial;
	}
	// 根据自身中心点做最终变换
	m_MatriFinal = m_MatriInitial * D2D1::Matrix3x2F::Translation(-pivot.x, -pivot.y);
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
	// 计算自身的转换矩阵
	node->_updateTransform();
	// 绑定于自身的形状也进行相应转换
	if (node->m_pGeometry)
	{
		node->m_pGeometry->_transform();
	}
	// 遍历子节点下的所有节点
	node->_updateChildrenTransform();
	// 标志已执行过变换
	node->m_bTransformNeeded = false;
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

float e2d::ENode::getPivotX() const
{
	return m_fPivotX;
}

float e2d::ENode::getPivotY() const
{
	return m_fPivotY;
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
	m_bTransformNeeded = true;
}

void e2d::ENode::movePosX(float x)
{
	this->movePos(x, 0);
}

void e2d::ENode::movePosY(float y)
{
	this->movePos(0, y);
}

void e2d::ENode::movePos(float x, float y)
{
	this->setPos(m_Pos.x + x, m_Pos.y + y);
}

void e2d::ENode::movePos(const EVec & v)
{
	this->movePos(v.x, v.y);
}

void e2d::ENode::_setWidth(float width)
{
	this->_setSize(width, m_Size.height);
}

void e2d::ENode::_setHeight(float height)
{
	this->_setSize(m_Size.width, height);
}

void e2d::ENode::_setSize(const ESize & size)
{
	this->_setSize(size.width, size.height);
}

void e2d::ENode::_setSize(float width, float height)
{
	if (m_Size.width == width && m_Size.height == height)
		return;

	m_Size.width = width;
	m_Size.height = height;
	m_bTransformNeeded = true;
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

	m_fSkewAngleX = angleX;
	m_fSkewAngleY = angleY;
	m_bTransformNeeded = true;
}

void e2d::ENode::setRotation(float angle)
{
	if (m_fRotation == angle)
		return;

	m_fRotation = angle;
	m_bTransformNeeded = true;
}

void e2d::ENode::setOpacity(float opacity)
{
	if (m_fRealOpacity == opacity)
		return;

	m_fDisplayOpacity = m_fRealOpacity = min(max(opacity, 0), 1);
	// 更新节点透明度
	_updateOpacity(this);
}

void e2d::ENode::setPivotX(float pivotX)
{
	this->setPivot(pivotX, m_fPivotY);
}

void e2d::ENode::setPivotY(float pivotY)
{
	this->setPivot(m_fPivotX, pivotY);
}

void e2d::ENode::setPivot(float pivotX, float pivotY)
{
	if (m_fPivotX == pivotX && m_fPivotY == pivotY)
		return;

	m_fPivotX = min(max(pivotX, 0), 1);
	m_fPivotY = min(max(pivotY, 0), 1);
	m_bTransformNeeded = true;
}

void e2d::ENode::setGeometry(EGeometry * geometry)
{
	// 删除旧的形状
	EPhysicsManager::_delGeometry(m_pGeometry);
	// 添加新的形状
	EPhysicsManager::_addGeometry(geometry);

	if (geometry)
	{
		// 双向绑定
		this->m_pGeometry = geometry;
		geometry->m_pParentNode = this;
	}
	else
	{
		this->m_pGeometry = nullptr;
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

		if (this->m_pParentScene)
		{
			child->_setParentScene(this->m_pParentScene);
		}

		if (this->m_bDisplayedInScene)
		{
			child->_onEnter();
		}

		// 更新子节点透明度
		_updateOpacity(child);
		// 更新节点转换
		child->m_bTransformNeeded = true;
		// 更新子节点排序
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

void e2d::ENode::removeFromParent()
{
	if (m_pParent)
	{
		m_pParent->removeChild(this);
	}
}

bool e2d::ENode::removeChild(ENode * child)
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
				return true;
			}
		}
	}
	return false;
}

void e2d::ENode::removeChild(const EString & childName)
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
			return;
		}
	}
}

void e2d::ENode::clearAllChildren()
{
	// 所有节点的引用计数减一
	for (auto child : m_vChildren)
	{
		child->_onExit();
		child->release();
	}
	// 清空储存节点的容器
	m_vChildren.clear();
}

void e2d::ENode::runAction(EAction * action)
{
	ASSERT(
		(!action->getTarget()),
		"The action is already running, it cannot run again!"
	);
	action->setTarget(this);
	EActionManager::addAction(action);
}

void e2d::ENode::resumeAction(EAction * action)
{
	if (action->getTarget() == this)
	{
		action->resume();
	}
}

void e2d::ENode::pauseAction(EAction * action)
{
	if (action->getTarget() == this)
	{
		action->pause();
	}
}

bool e2d::ENode::isPointIn(EPoint point)
{
	if (m_bTransformNeeded)
	{
		_updateTransform(this);
	}
	// 为节点创建一个形状
	ID2D1RectangleGeometry * rect;
	GetFactory()->CreateRectangleGeometry(
		D2D1::RectF(0, 0, getRealWidth(), getRealHeight()),
		&rect
	);
	// 判断点是否在形状内
	BOOL ret;
	rect->FillContainsPoint(
		D2D1::Point2F(
			point.x,
			point.y),
		&m_MatriFinal,
		&ret
	);
	if (ret)
	{
		return true;
	}
	else
	{
		for (const auto & child : m_vChildren)
			if (child->isPointIn(point))
				return true;
	}
	return false;
}

void e2d::ENode::setDefaultPiovt(float defaultPiovtX, float defaultPiovtY)
{
	s_fDefaultPiovtX = min(max(defaultPiovtX, 0), 1);
	s_fDefaultPiovtY = min(max(defaultPiovtY, 0), 1);
}

void e2d::ENode::stopAction(EAction * action)
{
	if (action->getTarget() == this)
	{
		action->stop();
	}
}

void e2d::ENode::startAllActions()
{
	EActionManager::startAllActionsBindedWith(this);
}

void e2d::ENode::pauseAllActions()
{
	EActionManager::pauseAllActionsBindedWith(this);
}

void e2d::ENode::stopAllActions()
{
	EActionManager::stopAllActionsBindedWith(this);
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