#include "..\enodes.h"
#include "..\emanagers.h"
#include "..\etools.h"
#include "..\eactions.h"
#include "..\eshape.h"
#include <algorithm>

// 默认中心点位置
static float s_fDefaultPiovtX = 0;
static float s_fDefaultPiovtY = 0;
static bool s_fDefaultShapeEnabled = true;

e2d::Node::Node()
	: m_nOrder(0)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fWidth(0)
	, m_fHeight(0)
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
	, m_pShape(nullptr)
	, m_pParent(nullptr)
	, m_pParentScene(nullptr)
	, m_nHashName(0)
	, m_bSortChildrenNeeded(false)
	, m_bTransformNeeded(false)
	, m_bAutoUpdate(true)
{
	if (s_fDefaultShapeEnabled)
	{
		auto rect = new ShapeRectangle(this);
		this->setShape(rect);
	}
}

e2d::Node::~Node()
{
	ActionManager::__clearAllBindedWith(this);
	CollisionManager::__removeShape(m_pShape);
	for (auto child : m_vChildren)
	{
		SafeRelease(&child);
	}
}

void e2d::Node::_update()
{
	if (m_bTransformNeeded)
	{
		_updateTransform(this);
	}

	if (!m_vChildren.empty())
	{
		if (m_bSortChildrenNeeded)
		{
			// 子节点排序
			auto sortFunc = [](Node * n1, Node * n2) {
				return n1->getOrder() < n2->getOrder();
			};

			std::sort(
				std::begin(m_vChildren),
				std::end(m_vChildren),
				sortFunc
			);

			m_bSortChildrenNeeded = false;
		}

		// 遍历子节点
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

		if (m_bAutoUpdate)
		{
			if (!Game::isPaused())
			{
				this->onUpdate();
			}
			this->onFixedUpdate();
		}

		// 访问剩余节点
		for (; i < size; i++)
			m_vChildren[i]->_update();
	}
	else
	{
		if (m_bAutoUpdate)
		{
			if (!Game::isPaused())
			{
				this->onUpdate();
			}
			this->onFixedUpdate();
		}
	}
}

void e2d::Node::_render()
{
	if (!m_bVisiable)
	{
		return;
	}

	if (!m_vChildren.empty())
	{
		size_t size = m_vChildren.size();
		size_t i;
		for (i = 0; i < size; i++)
		{
			auto child = m_vChildren[i];
			// 访问 Order 小于零的节点
			if (child->getOrder() < 0)
			{
				child->_render();
			}
			else
			{
				break;
			}
		}

		// 转换渲染器的二维矩阵
		Renderer::getRenderTarget()->SetTransform(m_MatriFinal);
		// 渲染自身
		this->onRender();

		// 访问剩余节点
		for (; i < size; i++)
			m_vChildren[i]->_render();
	}
	else
	{
		// 转换渲染器的二维矩阵
		Renderer::getRenderTarget()->SetTransform(m_MatriFinal);
		// 渲染自身
		this->onRender();
	}
}

void e2d::Node::_drawShape()
{
	// 绘制自身的几何形状
	if (m_pShape && m_pShape->m_bIsVisiable)
	{
		m_pShape->_render();
	}

	// 绘制所有子节点的几何形状
	for (auto child : m_vChildren)
	{
		child->_drawShape();
	}
}

void e2d::Node::_onEnter()
{
	if (!this->m_bDisplayedInScene)
	{
		this->m_bDisplayedInScene = true;
		this->onEnter();

		for (auto child : m_vChildren)
		{
			child->_onEnter();
		}
	}
}

void e2d::Node::_onExit()
{
	if (this->m_bDisplayedInScene)
	{
		this->m_bDisplayedInScene = false;
		this->onExit();

		for (auto child : m_vChildren)
		{
			child->_onExit();
		}
	}
}

void e2d::Node::_updateTransform()
{
	// 计算中心点坐标
	D2D1_POINT_2F pivot = { m_fWidth * m_fPivotX, m_fHeight * m_fPivotY };
	// 变换 Initial 矩阵，子节点将根据这个矩阵进行变换
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
		m_fPosX,
		m_fPosY
	);
	// 根据自身中心点变换 Final 矩阵
	m_MatriFinal = m_MatriInitial * D2D1::Matrix3x2F::Translation(-pivot.x, -pivot.y);
	// 和父节点矩阵相乘
	if (m_pParent)
	{
		m_MatriInitial = m_MatriInitial * m_pParent->m_MatriInitial;
		m_MatriFinal = m_MatriFinal * m_pParent->m_MatriInitial;
	}
}

void e2d::Node::_updateChildrenTransform()
{
	for (auto child : m_vChildren)
	{
		_updateTransform(child);
	}
}

void e2d::Node::_updateTransform(Node * node)
{
	// 计算自身的转换矩阵
	node->_updateTransform();
	// 绑定于自身的形状也进行相应转换
	if (node->m_pShape)
	{
		node->m_pShape->_transform();
	}
	// 遍历子节点下的所有节点
	node->_updateChildrenTransform();
	// 标志已执行过变换
	node->m_bTransformNeeded = false;
}

void e2d::Node::_updateChildrenOpacity()
{
	for (auto child : m_vChildren)
	{
		_updateOpacity(child);
	}
}

void e2d::Node::_updateOpacity(Node * node)
{
	if (node->m_pParent)
	{
		node->m_fDisplayOpacity = node->m_fRealOpacity * node->m_pParent->m_fDisplayOpacity;
	}
	node->_updateChildrenOpacity();
}

bool e2d::Node::isVisiable() const
{
	return m_bVisiable;
}

e2d::String e2d::Node::getName() const
{
	return m_sName;
}

unsigned int e2d::Node::getHashName() const
{
	return m_nHashName;
}

double e2d::Node::getPosX() const
{
	return m_fPosX;
}

double e2d::Node::getPosY() const
{
	return m_fPosY;
}

e2d::Point e2d::Node::getPos() const
{
	return Point(m_fPosX, m_fPosY);
}

double e2d::Node::getWidth() const
{
	return m_fWidth * m_fScaleX;
}

double e2d::Node::getHeight() const
{
	return m_fHeight * m_fScaleY;
}

double e2d::Node::getRealWidth() const
{
	return m_fWidth;
}

double e2d::Node::getRealHeight() const
{
	return m_fHeight;
}

e2d::Size e2d::Node::getRealSize() const
{
	return Size(m_fWidth, m_fHeight);
}

double e2d::Node::getPivotX() const
{
	return m_fPivotX;
}

double e2d::Node::getPivotY() const
{
	return m_fPivotY;
}

e2d::Size e2d::Node::getSize() const
{
	return Size(getWidth(), getHeight());
}

double e2d::Node::getScaleX() const
{
	return m_fScaleX;
}

double e2d::Node::getScaleY() const
{
	return m_fScaleY;
}

double e2d::Node::getSkewX() const
{
	return m_fSkewAngleX;
}

double e2d::Node::getSkewY() const
{
	return m_fSkewAngleY;
}

double e2d::Node::getRotation() const
{
	return m_fRotation;
}

double e2d::Node::getOpacity() const
{
	return m_fRealOpacity;
}

e2d::Shape * e2d::Node::getShape() const
{
	return m_pShape;
}

int e2d::Node::getOrder() const
{
	return m_nOrder;
}

void e2d::Node::setOrder(int order)
{
	m_nOrder = order;
}

void e2d::Node::setPosX(double x)
{
	this->setPos(x, m_fPosY);
}

void e2d::Node::setPosY(double y)
{
	this->setPos(m_fPosX, y);
}

void e2d::Node::setPos(const Point & p)
{
	this->setPos(p.x, p.y);
}

void e2d::Node::setPos(double x, double y)
{
	if (m_fPosX == x && m_fPosY == y)
		return;

	m_fPosX = static_cast<float>(x);
	m_fPosY = static_cast<float>(y);
	m_bTransformNeeded = true;
}

void e2d::Node::movePosX(double x)
{
	this->movePos(x, 0);
}

void e2d::Node::movePosY(double y)
{
	this->movePos(0, y);
}

void e2d::Node::movePos(double x, double y)
{
	this->setPos(m_fPosX + x, m_fPosY + y);
}

void e2d::Node::movePos(const Vector & v)
{
	this->movePos(v.x, v.y);
}

void e2d::Node::setScaleX(double scaleX)
{
	this->setScale(scaleX, m_fScaleY);
}

void e2d::Node::setScaleY(double scaleY)
{
	this->setScale(m_fScaleX, scaleY);
}

void e2d::Node::setScale(double scale)
{
	this->setScale(scale, scale);
}

void e2d::Node::setScale(double scaleX, double scaleY)
{
	if (m_fScaleX == scaleX && m_fScaleY == scaleY)
		return;

	m_fScaleX = static_cast<float>(scaleX);
	m_fScaleY = static_cast<float>(scaleY);
	m_bTransformNeeded = true;
}

void e2d::Node::setSkewX(double angleX)
{
	this->setSkew(angleX, m_fSkewAngleY);
}

void e2d::Node::setSkewY(double angleY)
{
	this->setSkew(m_fSkewAngleX, angleY);
}

void e2d::Node::setSkew(double angleX, double angleY)
{
	if (m_fSkewAngleX == angleX && m_fSkewAngleY == angleY)
		return;

	m_fSkewAngleX = static_cast<float>(angleX);
	m_fSkewAngleY = static_cast<float>(angleY);
	m_bTransformNeeded = true;
}

void e2d::Node::setRotation(double angle)
{
	if (m_fRotation == angle)
		return;

	m_fRotation = static_cast<float>(angle);
	m_bTransformNeeded = true;
}

void e2d::Node::setOpacity(double opacity)
{
	if (m_fRealOpacity == opacity)
		return;

	m_fDisplayOpacity = m_fRealOpacity = min(max(static_cast<float>(opacity), 0), 1);
	// 更新节点透明度
	_updateOpacity(this);
}

void e2d::Node::setPivotX(double pivotX)
{
	this->setPivot(pivotX, m_fPivotY);
}

void e2d::Node::setPivotY(double pivotY)
{
	this->setPivot(m_fPivotX, pivotY);
}

void e2d::Node::setPivot(double pivotX, double pivotY)
{
	if (m_fPivotX == pivotX && m_fPivotY == pivotY)
		return;

	m_fPivotX = min(max(static_cast<float>(pivotX), 0), 1);
	m_fPivotY = min(max(static_cast<float>(pivotY), 0), 1);
	m_bTransformNeeded = true;
}

void e2d::Node::setWidth(double width)
{
	this->setSize(width, m_fHeight);
}

void e2d::Node::setHeight(double height)
{
	this->setSize(m_fWidth, height);
}

void e2d::Node::setSize(double width, double height)
{
	if (m_fWidth == width && m_fHeight == height)
		return;

	m_fWidth = static_cast<float>(width);
	m_fHeight = static_cast<float>(height);
	m_bTransformNeeded = true;
}

void e2d::Node::setSize(Size size)
{
	this->setSize(size.width, size.height);
}

void e2d::Node::setShape(Shape::TYPE type)
{
	switch (type)
	{
	case Shape::TYPE::RECTANGLE:
	{
		auto rect = new ShapeRectangle(this);
		this->setShape(rect);
		break;
	}

	case Shape::TYPE::CIRCLE:
	{
		auto rect = new ShapeCircle(this);
		this->setShape(rect);
		break;
	}

	case Shape::TYPE::ELLIPSE:
	{
		auto rect = new ShapeEllipse(this);
		this->setShape(rect);
		break;
	}

	default:
		break;
	}
}

void e2d::Node::setShape(Shape * pShape)
{
	// 删除旧的形状
	CollisionManager::__removeShape(m_pShape);
	// 添加新的形状
	CollisionManager::__addShape(pShape);

	if (pShape)
	{
		// 双向绑定
		this->m_pShape = pShape;
		pShape->m_pParentNode = this;
	}
	else
	{
		this->m_pShape = nullptr;
	}
}

void e2d::Node::addCollider(String collliderName)
{
	unsigned int hash = collliderName.getHashCode();
	m_vColliders.insert(hash);
}

void e2d::Node::addCollider(std::initializer_list<String>& vCollliderName)
{
	for (const auto &name : vCollliderName)
	{
		this->addCollider(name);
	}
}

void e2d::Node::removeCollider(String collliderName)
{
	unsigned int hash = collliderName.getHashCode();
	m_vColliders.erase(hash);
}

void e2d::Node::addChild(Node * child, int order  /* = 0 */)
{
	WARN_IF(child == nullptr, "Node::addChild NULL pointer exception.");

	if (child)
	{
		ASSERT(child->m_pParent == nullptr, "Node already added. It can't be added again!");

		for (Node * parent = this; parent != nullptr; parent = parent->getParent())
		{
			ASSERT(child != parent, "A Node cannot be the child of his own children!");
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

void e2d::Node::addChild(std::initializer_list<Node*>& vNodes, int order)
{
	for (const auto &node : vNodes)
	{
		this->addChild(node, order);
	}
}

e2d::Node * e2d::Node::getParent() const
{
	return m_pParent;
}

e2d::Scene * e2d::Node::getParentScene() const
{
	return m_pParentScene;
}

std::vector<e2d::Node*> e2d::Node::getChildren(String name)
{
	std::vector<Node*> vChildren;
	unsigned int hash = name.getHashCode();

	for (auto child : m_vChildren)
	{
		// 不同的名称可能会有相同的 Hash 值，但是先比较 Hash 可以提升搜索速度
		if (child->m_nHashName == hash && child->m_sName == name)
		{
			vChildren.push_back(child);
		}
	}
	return std::move(vChildren);
}

std::vector<e2d::Node*> e2d::Node::getChildren()
{
	return m_vChildren;
}

int e2d::Node::getChildrenCount() const
{
	return static_cast<int>(m_vChildren.size());
}

void e2d::Node::removeFromParent()
{
	if (m_pParent)
	{
		m_pParent->removeChild(this);
	}
}

bool e2d::Node::removeChild(Node * child)
{
	WARN_IF(child == nullptr, "Node::removeChildren NULL pointer exception.");

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
				if (child->m_bDisplayedInScene)
				{
					child->_onExit();
				}

				child->release();
				return true;
			}
		}
	}
	return false;
}

void e2d::Node::removeChildren(String childName)
{
	WARN_IF(childName.isEmpty(), "Invalid Node name.");

	if (m_vChildren.empty())
	{
		return;
	}

	// 计算名称 Hash 值
	unsigned int hash = childName.getHashCode();

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
			if (child->m_bDisplayedInScene)
			{
				child->_onExit();
			}
			child->release();
		}
	}
}

void e2d::Node::clearAllChildren()
{
	// 所有节点的引用计数减一
	for (auto child : m_vChildren)
	{
		if (child->m_bDisplayedInScene)
		{
			child->_onExit();
		}
		child->release();
	}
	// 清空储存节点的容器
	m_vChildren.clear();
}

void e2d::Node::runAction(Action * action)
{
	if (this != action->getTarget())
	{
		WARN_IF(
			nullptr != action->getTarget(),
			"The action has already got a target, The clone of the action will be created automatically!"
		);

		if (nullptr != action->getTarget())
		{
			action = action->clone();
		}
		ActionManager::__startAction(action, this);
	}
	else
	{
		action->reset();
	}
}

void e2d::Node::resumeAction(String strActionName)
{
	auto actions = ActionManager::get(strActionName);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->resume();
		}
	}
}

void e2d::Node::pauseAction(String strActionName)
{
	auto actions = ActionManager::get(strActionName);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->pause();
		}
	}
}

void e2d::Node::stopAction(String strActionName)
{
	auto actions = ActionManager::get(strActionName);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->stop();
		}
	}
}

e2d::Action * e2d::Node::getAction(String strActionName)
{
	auto actions = ActionManager::get(strActionName);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			return action;
		}
	}
	return nullptr;
}

std::vector<e2d::Action*> e2d::Node::getActions(String strActionName)
{
	std::vector<Action*>::iterator iter;
	auto actions = ActionManager::get(strActionName);
	for (iter = actions.begin(); iter != actions.end();)
	{
		if ((*iter)->getTarget() != this)
		{
			iter = actions.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return std::move(actions);
}

bool e2d::Node::isPointIn(Point point) const
{
	BOOL ret = 0;
	// 如果存在形状，用形状判断
	if (m_pShape)
	{
		m_pShape->getD2dGeometry()->FillContainsPoint(
			D2D1::Point2F(
				static_cast<float>(point.x),
				static_cast<float>(point.y)),
			m_MatriFinal,
			&ret
		);
	}
	else
	{
		// 为节点创建一个临时形状
		ID2D1RectangleGeometry * rect;
		Renderer::getID2D1Factory()->CreateRectangleGeometry(
			D2D1::RectF(0, 0, m_fWidth, m_fHeight),
			&rect
		);
		// 判断点是否在形状内
		rect->FillContainsPoint(
			D2D1::Point2F(
				static_cast<float>(point.x),
				static_cast<float>(point.y)),
			m_MatriFinal,
			&ret
		);
		// 删除临时创建的形状
		SafeReleaseInterface(&rect);
	}

	if (ret)
	{
		return true;
	}

	// 判断点是否在子节点内
	for (auto child : m_vChildren)
		if (child->isPointIn(point))
			return true;

	return false;
}

bool e2d::Node::isIntersectWith(const Node * pNode) const
{
	// 如果存在形状，用形状判断
	if (this->m_pShape && pNode->m_pShape)
	{
		int relation = this->m_pShape->getRelationWith(pNode->m_pShape);
		if ((relation != Relation::UNKNOWN) && 
			(relation != Relation::DISJOINT))
		{
			return true;
		}
	}
	else
	{
		// 为节点创建一个临时形状
		ID2D1RectangleGeometry * pRect1;
		ID2D1RectangleGeometry * pRect2;
		ID2D1TransformedGeometry * pShape;
		D2D1_GEOMETRY_RELATION relation;

		// 根据自身大小位置创建矩形
		Renderer::getID2D1Factory()->CreateRectangleGeometry(
			D2D1::RectF(0, 0, m_fWidth, m_fHeight),
			&pRect1
		);
		// 根据二维矩阵进行转换
		Renderer::getID2D1Factory()->CreateTransformedGeometry(
			pRect1,
			m_MatriFinal,
			&pShape
		);
		// 根据相比较节点的大小位置创建矩形
		Renderer::getID2D1Factory()->CreateRectangleGeometry(
			D2D1::RectF(0, 0, pNode->m_fWidth, pNode->m_fHeight),
			&pRect2
		);
		// 获取相交状态
		pShape->CompareWithGeometry(
			pRect2,
			pNode->m_MatriFinal,
			&relation
		);
		// 删除临时创建的形状
		SafeReleaseInterface(&pRect1);
		SafeReleaseInterface(&pRect2);
		SafeReleaseInterface(&pShape);
		if ((relation != D2D1_GEOMETRY_RELATION::D2D1_GEOMETRY_RELATION_UNKNOWN) &&
			(relation != D2D1_GEOMETRY_RELATION::D2D1_GEOMETRY_RELATION_DISJOINT))
		{
			return true;
		}
	}

	// 判断和其子节点是否相交
	for (auto child : pNode->m_vChildren)
		if (this->isIntersectWith(child))
			return true;

	// 判断子节点和其是否相交
	for (auto child : m_vChildren)
		if (child->isIntersectWith(pNode))
			return true;

	// 都不相交，返回 false
	return false;
}

void e2d::Node::setAutoUpdate(bool bAutoUpdate)
{
	m_bAutoUpdate = bAutoUpdate;
}

void e2d::Node::setDefaultPiovt(double defaultPiovtX, double defaultPiovtY)
{
	s_fDefaultPiovtX = min(max(static_cast<float>(defaultPiovtX), 0), 1);
	s_fDefaultPiovtY = min(max(static_cast<float>(defaultPiovtY), 0), 1);
}

void e2d::Node::setDefaultShapeEnable(bool bEnable)
{
	s_fDefaultShapeEnabled = bEnable;
}

void e2d::Node::resumeAllActions()
{
	ActionManager::__resumeAllBindedWith(this);
}

void e2d::Node::pauseAllActions()
{
	ActionManager::__pauseAllBindedWith(this);
}

void e2d::Node::stopAllActions()
{
	ActionManager::__stopAllBindedWith(this);
}

void e2d::Node::setVisiable(bool value)
{
	m_bVisiable = value;
}

void e2d::Node::setName(String name)
{
	WARN_IF(name.isEmpty(), "Invalid Node name.");

	if (!name.isEmpty() && m_sName != name)
	{
		// 保存节点名
		m_sName = name;
		// 保存节点 Hash 名
		m_nHashName = name.getHashCode();
	}
}

void e2d::Node::_setParentScene(Scene * scene)
{
	m_pParentScene = scene;
	for (auto child : m_vChildren)
	{
		child->_setParentScene(scene);
	}
}