#include "..\e2dnode.h"
#include "..\e2dmanager.h"
#include "..\e2daction.h"

const e2d::Node::Property e2d::Node::Property::Origin = { 0 };

e2d::Node::Property e2d::Node::Property::operator+(Property const & prop) const
{
	Property result;
	result.posX = this->posX + prop.posX;
	result.posY = this->posY + prop.posY;
	result.width = this->width + prop.width;
	result.height = this->height + prop.height;
	result.pivotX = this->pivotX + prop.pivotX;
	result.pivotY = this->pivotY + prop.pivotY;
	result.scaleX = this->scaleX + prop.scaleX;
	result.scaleY = this->scaleY + prop.scaleY;
	result.rotation = this->rotation + prop.rotation;
	result.skewAngleX = this->skewAngleX + prop.skewAngleX;
	result.skewAngleY = this->skewAngleY + prop.skewAngleY;
	return std::move(result);
}

e2d::Node::Property e2d::Node::Property::operator-(Property const & prop) const
{
	Property result;
	result.posX = this->posX - prop.posX;
	result.posY = this->posY - prop.posY;
	result.width = this->width - prop.width;
	result.height = this->height - prop.height;
	result.pivotX = this->pivotX - prop.pivotX;
	result.pivotY = this->pivotY - prop.pivotY;
	result.scaleX = this->scaleX - prop.scaleX;
	result.scaleY = this->scaleY - prop.scaleY;
	result.rotation = this->rotation - prop.rotation;
	result.skewAngleX = this->skewAngleX - prop.skewAngleX;
	result.skewAngleY = this->skewAngleY - prop.skewAngleY;
	return std::move(result);
}


e2d::Node::Node()
	: _order(0)
	, _posX(0)
	, _posY(0)
	, _width(0)
	, _height(0)
	, _scaleX(1.0f)
	, _scaleY(1.0f)
	, _rotation(0)
	, _skewAngleX(0)
	, _skewAngleY(0)
	, _displayOpacity(1.f)
	, _realOpacity(1.f)
	, _pivotX(0.f)
	, _pivotY(0.f)
	, _initialMatri(D2D1::Matrix3x2F::Identity())
	, _finalMatri(D2D1::Matrix3x2F::Identity())
	, _visible(true)
	, _parent(nullptr)
	, _parentScene(nullptr)
	, _hashName(0)
	, _needSort(false)
	, _needTransform(false)
	, _autoUpdate(true)
	, _positionFixed(false)
	, _outline(nullptr)
	, _collider(this)
	, _extrapolate(Property::Origin)
{
	// 设置默认中心点位置
	Point defPivot = Game::getInstance()->getConfig().getNodeDefaultPivot();
	this->_pivotX = float(defPivot.x);
	this->_pivotY = float(defPivot.y);
}

e2d::Node::~Node()
{
	SafeRelease(_outline);

	ActionManager::getInstance()->clearAllBindedWith(this);
	for (auto child : _children)
	{
		GC::getInstance()->safeRelease(child);
	}
}

void e2d::Node::_update()
{
	if (_children.empty())
	{
		_updateSelf();
	}
	else
	{
		// 遍历子节点
		size_t i;
		for (i = 0; i < _children.size(); ++i)
		{
			auto child = _children[i];
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

		_updateSelf();

		// 访问其他节点
		for (; i < _children.size(); ++i)
			_children[i]->_update();
	}
}

void e2d::Node::_updateSelf()
{
	if (_needTransform)
	{
		updateTransform();
		if (_collider.isEnabled() &&
			_collider.isCollisionNotify() &&
			_collider.getShape() != Collider::Shape::None)
		{
			CollisionManager::getInstance()->__updateCollider(&_collider);
		}
	}

	if (_autoUpdate && !Game::getInstance()->isPaused())
	{
		this->onUpdate();
	}
}

void e2d::Node::_render()
{
	if (!_visible)
	{
		return;
	}

	// 更新转换矩阵
	updateTransform();
	// 保留差别属性
	_extrapolate = this->getProperty();

	if (_children.empty())
	{
		// 转换渲染器的二维矩阵
		Renderer::getInstance()->getRenderTarget()->SetTransform(_finalMatri);
		// 渲染自身
		this->onRender();
	}
	else
	{
		// 子节点排序
		_sortChildren();

		size_t i;
		for (i = 0; i < _children.size(); ++i)
		{
			auto child = _children[i];
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
		Renderer::getInstance()->getRenderTarget()->SetTransform(_finalMatri);
		// 渲染自身
		this->onRender();

		// 访问剩余节点
		for (; i < _children.size(); ++i)
			_children[i]->_render();
	}
}

void e2d::Node::_renderOutline()
{
	if (_outline && _visible)
	{
		auto renderer = Renderer::getInstance();
		// 获取纯色画刷
		ID2D1SolidColorBrush * brush = renderer->getSolidColorBrush();
		// 设置画刷颜色和透明度
		brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, 0.7f));
		brush->SetOpacity(1.f);
		// 渲染轮廓
		renderer->getRenderTarget()->DrawGeometry(_outline, brush);
	}

	// 渲染所有子节点的轮廓
	for (auto child : _children)
	{
		child->_renderOutline();
	}
}

void e2d::Node::_renderCollider()
{
	// 绘制自身的几何碰撞体
	if (_visible)
	{
		_collider.render();
	}

	// 绘制所有子节点的几何碰撞体
	for (auto child : _children)
	{
		child->_renderCollider();
	}
}

void e2d::Node::updateTransform()
{
	if (!_needTransform)
		return;

	_needTransform = false;

	// 计算中心点坐标
	D2D1_POINT_2F pivot = { _width * _pivotX, _height * _pivotY };
	// 变换 Initial 矩阵，子节点将根据这个矩阵进行变换
	_initialMatri = D2D1::Matrix3x2F::Scale(
		_scaleX,
		_scaleY,
		pivot
	) * D2D1::Matrix3x2F::Skew(
		_skewAngleX,
		_skewAngleY,
		pivot
	) * D2D1::Matrix3x2F::Rotation(
		_rotation,
		pivot
	) * D2D1::Matrix3x2F::Translation(
		_posX,
		_posY
	);
	// 根据自身中心点变换 Final 矩阵
	_finalMatri = _initialMatri * D2D1::Matrix3x2F::Translation(-pivot.x, -pivot.y);
	// 和父节点矩阵相乘
	if (!_positionFixed && _parent)
	{
		_initialMatri = _initialMatri * _parent->_initialMatri;
		_finalMatri = _finalMatri * _parent->_initialMatri;
	}

	// 更新碰撞体
	_collider.recreate();

	// 为节点创建一个轮廓
	ID2D1RectangleGeometry * rectGeo = nullptr;
	ID2D1TransformedGeometry * transformedGeo = nullptr;

	auto factory = Renderer::getFactory();
	HRESULT hr = factory->CreateRectangleGeometry(
		D2D1::RectF(0, 0, _width, _height),
		&rectGeo
	);

	if (SUCCEEDED(hr))
	{
		factory->CreateTransformedGeometry(
			rectGeo,
			_finalMatri,
			&transformedGeo
		);
	}

	SafeRelease(rectGeo);
	SafeRelease(_outline);
	_outline = transformedGeo;

	// 通知子节点进行转换
	for (auto& child : _children)
	{
		child->_needTransform = true;
	}
}

bool e2d::Node::dispatch(const MouseEvent & e)
{
	if (_children.empty())
	{
		return onMouseEvent(e);
	}
	else
	{
		size_t i;
		for (i = 0; i < _children.size(); ++i)
		{
			auto child = _children[i];
			if (child->getOrder() < 0)
			{
				if (!child->dispatch(e))
				{
					return false;
				}
			}
			else
			{
				break;
			}
		}

		if (!onMouseEvent(e))
		{
			return false;
		}

		for (; i < _children.size(); ++i)
			if (!_children[i]->dispatch(e))
				return false;

		return true;
	}
}

bool e2d::Node::dispatch(const KeyEvent & e)
{
	if (_children.empty())
	{
		return onKeyEvent(e);
	}
	else
	{
		size_t i;
		for (i = 0; i < _children.size(); ++i)
		{
			auto child = _children[i];
			if (child->getOrder() < 0)
			{
				if (!child->dispatch(e))
				{
					return false;
				}
			}
			else
			{
				break;
			}
		}

		if (!onKeyEvent(e))
		{
			return false;
		}

		for (; i < _children.size(); ++i)
			if (!_children[i]->dispatch(e))
				return false;

		return true;
	}
}

void e2d::Node::_sortChildren()
{
	if (_needSort)
	{
		std::sort(
			std::begin(_children),
			std::end(_children),
			[](Node * n1, Node * n2) { return n1->getOrder() < n2->getOrder(); }
		);

		_needSort = false;
	}
}

void e2d::Node::_updateOpacity()
{
	if (_parent)
	{
		_displayOpacity = _realOpacity * _parent->_displayOpacity;
	}
	for (auto child : _children)
	{
		child->_updateOpacity();
	}
}

bool e2d::Node::isVisible() const
{
	return _visible;
}

e2d::String e2d::Node::getName() const
{
	return _name;
}

size_t e2d::Node::getHashName() const
{
	return _hashName;
}

double e2d::Node::getPosX() const
{
	return _posX;
}

double e2d::Node::getPosY() const
{
	return _posY;
}

e2d::Point e2d::Node::getPos() const
{
	return Point(_posX, _posY);
}

double e2d::Node::getWidth() const
{
	return _width * _scaleX;
}

double e2d::Node::getHeight() const
{
	return _height * _scaleY;
}

double e2d::Node::getRealWidth() const
{
	return _width;
}

double e2d::Node::getRealHeight() const
{
	return _height;
}

e2d::Size e2d::Node::getRealSize() const
{
	return Size(_width, _height);
}

double e2d::Node::getPivotX() const
{
	return _pivotX;
}

double e2d::Node::getPivotY() const
{
	return _pivotY;
}

e2d::Size e2d::Node::getSize() const
{
	return Size(getWidth(), getHeight());
}

double e2d::Node::getScaleX() const
{
	return _scaleX;
}

double e2d::Node::getScaleY() const
{
	return _scaleY;
}

double e2d::Node::getSkewX() const
{
	return _skewAngleX;
}

double e2d::Node::getSkewY() const
{
	return _skewAngleY;
}

double e2d::Node::getRotation() const
{
	return _rotation;
}

double e2d::Node::getOpacity() const
{
	return _realOpacity;
}

e2d::Node::Property e2d::Node::getProperty() const
{
	Property prop;
	prop.posX = _posX;
	prop.posY = _posY;
	prop.width = _width;
	prop.height = _height;
	prop.pivotX = _pivotX;
	prop.pivotY = _pivotY;
	prop.scaleX = _scaleX;
	prop.scaleY = _scaleY;
	prop.rotation = _rotation;
	prop.skewAngleX = _skewAngleX;
	prop.skewAngleY = _skewAngleY;
	return std::move(prop);
}

e2d::Node::Property e2d::Node::getExtrapolate() const
{
	return this->getProperty() - _extrapolate;
}

e2d::Collider* e2d::Node::getCollider()
{
	return &_collider;
}

int e2d::Node::getOrder() const
{
	return _order;
}

void e2d::Node::setOrder(int order)
{
	_order = order;
}

void e2d::Node::setPosX(double x)
{
	this->setPos(x, _posY);
}

void e2d::Node::setPosY(double y)
{
	this->setPos(_posX, y);
}

void e2d::Node::setPos(const Point & p)
{
	this->setPos(p.x, p.y);
}

void e2d::Node::setPos(double x, double y)
{
	if (_posX == x && _posY == y)
		return;

	_posX = float(x);
	_posY = float(y);
	_needTransform = true;
}

void e2d::Node::setPosFixed(bool fixed)
{
	if (_positionFixed == fixed)
		return;

	_positionFixed = fixed;
	_needTransform = true;
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
	this->setPos(_posX + x, _posY + y);
}

void e2d::Node::movePos(const Vector2 & v)
{
	this->movePos(v.x, v.y);
}

void e2d::Node::setScaleX(double scaleX)
{
	this->setScale(scaleX, _scaleY);
}

void e2d::Node::setScaleY(double scaleY)
{
	this->setScale(_scaleX, scaleY);
}

void e2d::Node::setScale(double scale)
{
	this->setScale(scale, scale);
}

void e2d::Node::setScale(double scaleX, double scaleY)
{
	if (_scaleX == scaleX && _scaleY == scaleY)
		return;

	_scaleX = float(scaleX);
	_scaleY = float(scaleY);
	_needTransform = true;
}

void e2d::Node::setSkewX(double angleX)
{
	this->setSkew(angleX, _skewAngleY);
}

void e2d::Node::setSkewY(double angleY)
{
	this->setSkew(_skewAngleX, angleY);
}

void e2d::Node::setSkew(double angleX, double angleY)
{
	if (_skewAngleX == angleX && _skewAngleY == angleY)
		return;

	_skewAngleX = float(angleX);
	_skewAngleY = float(angleY);
	_needTransform = true;
}

void e2d::Node::setRotation(double angle)
{
	if (_rotation == angle)
		return;

	_rotation = float(angle);
	_needTransform = true;
}

void e2d::Node::setOpacity(double opacity)
{
	if (_realOpacity == opacity)
		return;

	_displayOpacity = _realOpacity = std::min(std::max(float(opacity), 0.f), 1.f);
	// 更新节点透明度
	_updateOpacity();
}

void e2d::Node::setPivotX(double pivotX)
{
	this->setPivot(pivotX, _pivotY);
}

void e2d::Node::setPivotY(double pivotY)
{
	this->setPivot(_pivotX, pivotY);
}

void e2d::Node::setPivot(double pivotX, double pivotY)
{
	if (_pivotX == pivotX && _pivotY == pivotY)
		return;

	_pivotX = std::min(std::max(float(pivotX), 0.f), 1.f);
	_pivotY = std::min(std::max(float(pivotY), 0.f), 1.f);
	_needTransform = true;
}

void e2d::Node::setWidth(double width)
{
	this->setSize(width, _height);
}

void e2d::Node::setHeight(double height)
{
	this->setSize(_width, height);
}

void e2d::Node::setSize(double width, double height)
{
	if (_width == width && _height == height)
		return;

	_width = float(width);
	_height = float(height);
	_needTransform = true;
}

void e2d::Node::setSize(Size size)
{
	this->setSize(size.width, size.height);
}

void e2d::Node::setProperty(Property prop)
{
	this->setPos(prop.posX, prop.posY);
	this->setSize(prop.width, prop.height);
	this->setPivot(prop.pivotX, prop.pivotY);
	this->setScale(prop.scaleX, prop.scaleY);
	this->setRotation(prop.rotation);
	this->setSkew(prop.skewAngleX, prop.skewAngleY);
}

void e2d::Node::addChild(Node * child, int order  /* = 0 */)
{
	WARN_IF(child == nullptr, "Node::addChild NULL pointer exception.");

	if (child)
	{
		if (child->_parent != nullptr)
		{
			throw Exception(L"节点已有父节点, 不能再添加到其他节点");
		}

		for (Node * parent = this; parent != nullptr; parent = parent->getParent())
		{
			if (child == parent)
			{
				throw Exception(L"一个节点不能同时是另一个节点的父节点和子节点");
			}
		}

		child->retain();
		_children.push_back(child);
		child->setOrder(order);
		child->_parent = this;
		if (this->_parentScene)
		{
			child->_setParentScene(this->_parentScene);
		}

		// 更新子节点透明度
		child->_updateOpacity();
		// 更新节点转换
		child->_needTransform = true;
		// 更新子节点排序
		_needSort = true;
	}
}

void e2d::Node::addChild(const std::vector<Node*>& nodes, int order)
{
	for (auto node : nodes)
	{
		this->addChild(node, order);
	}
}

e2d::Node * e2d::Node::getParent() const
{
	return _parent;
}

e2d::Scene * e2d::Node::getParentScene() const
{
	return _parentScene;
}

std::vector<e2d::Node*> e2d::Node::getChildren(const String& name) const
{
	std::vector<Node*> vChildren;
	size_t hash = name.getHashCode();

	for (auto child : _children)
	{
		// 不同的名称可能会有相同的 Hash 值，但是先比较 Hash 可以提升搜索速度
		if (child->_hashName == hash && child->_name == name)
		{
			vChildren.push_back(child);
		}
	}
	return std::move(vChildren);
}

e2d::Node * e2d::Node::getChild(const String& name) const
{
	size_t hash = name.getHashCode();

	for (auto child : _children)
	{
		// 不同的名称可能会有相同的 Hash 值，但是先比较 Hash 可以提升搜索速度
		if (child->_hashName == hash && child->_name == name)
		{
			return child;
		}
	}
	return nullptr;
}

const std::vector<e2d::Node*>& e2d::Node::getAllChildren() const
{
	return _children;
}

int e2d::Node::getChildrenCount() const
{
	return static_cast<int>(_children.size());
}

void e2d::Node::removeFromParent()
{
	if (_parent)
	{
		_parent->removeChild(this);
	}
}

bool e2d::Node::removeChild(Node * child)
{
	WARN_IF(child == nullptr, "Node::removeChildren NULL pointer exception.");

	if (_children.empty())
	{
		return false;
	}

	if (child)
	{
		auto iter = std::find(_children.begin(), _children.end(), child);
		if (iter != _children.end())
		{
			_children.erase(iter);
			child->_parent = nullptr;

			if (child->_parentScene)
			{
				child->_setParentScene(nullptr);
			}

			child->release();
			return true;
		}
	}
	return false;
}

void e2d::Node::removeChildren(const String& childName)
{
	WARN_IF(childName.isEmpty(), "Invalid Node name.");

	if (_children.empty())
	{
		return;
	}

	// 计算名称 Hash 值
	size_t hash = childName.getHashCode();

	size_t size = _children.size();
	for (size_t i = 0; i < size; ++i)
	{
		auto child = _children[i];
		if (child->_hashName == hash && child->_name == childName)
		{
			_children.erase(_children.begin() + i);
			child->_parent = nullptr;
			if (child->_parentScene)
			{
				child->_setParentScene(nullptr);
			}
			child->release();
		}
	}
}

void e2d::Node::removeAllChildren()
{
	// 所有节点的引用计数减一
	for (auto child : _children)
	{
		child->release();
	}
	// 清空储存节点的容器
	_children.clear();
}

void e2d::Node::runAction(Action * action)
{
	ActionManager::getInstance()->start(action, this, false);
}

void e2d::Node::resumeAction(const String& name)
{
	auto& actions = ActionManager::getInstance()->get(name);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->resume();
		}
	}
}

void e2d::Node::pauseAction(const String& name)
{
	auto& actions = ActionManager::getInstance()->get(name);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->pause();
		}
	}
}

void e2d::Node::stopAction(const String& name)
{
	auto& actions = ActionManager::getInstance()->get(name);
	for (auto action : actions)
	{
		if (action->getTarget() == this)
		{
			action->stop();
		}
	}
}

bool e2d::Node::containsPoint(const Point& point)
{
	updateTransform();
	if (_outline)
	{
		BOOL ret = 0;
		_outline->FillContainsPoint(
			D2D1::Point2F(float(point.x), float(point.y)),
			D2D1::Matrix3x2F::Identity(),
			&ret
		);
		if (ret)
		{
			return true;
		}
	}
	return false;
}

bool e2d::Node::intersects(Node * node)
{
	updateTransform();
	node->updateTransform();
	if (_outline)
	{
		D2D1_GEOMETRY_RELATION relation;
		// 获取相交状态
		_outline->CompareWithGeometry(
			node->_outline,
			D2D1::Matrix3x2F::Identity(),
			&relation
		);
		if ((relation != D2D1_GEOMETRY_RELATION_UNKNOWN) &&
			(relation != D2D1_GEOMETRY_RELATION_DISJOINT))
		{
			return true;
		}
	}
	return false;
}

void e2d::Node::setAutoUpdate(bool bAutoUpdate)
{
	_autoUpdate = bAutoUpdate;
}

void e2d::Node::resumeAllActions()
{
	ActionManager::getInstance()->resumeAllBindedWith(this);
}

void e2d::Node::pauseAllActions()
{
	ActionManager::getInstance()->pauseAllBindedWith(this);
}

void e2d::Node::stopAllActions()
{
	ActionManager::getInstance()->stopAllBindedWith(this);
}

void e2d::Node::setVisible(bool value)
{
	_visible = value;
}

void e2d::Node::setName(const String& name)
{
	WARN_IF(name.isEmpty(), "Invalid Node name.");

	if (!name.isEmpty() && _name != name)
	{
		// 保存节点名
		_name = name;
		// 保存节点 Hash 名
		_hashName = name.getHashCode();
	}
}

void e2d::Node::_setParentScene(Scene * scene)
{
	_parentScene = scene;
	for (auto child : _children)
	{
		child->_setParentScene(scene);
	}
}