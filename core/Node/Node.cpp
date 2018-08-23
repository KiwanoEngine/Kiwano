#include "..\e2dnode.h"
#include "..\e2devent.h"
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
	result.anchorX = this->anchorX + prop.anchorX;
	result.anchorY = this->anchorY + prop.anchorY;
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
	result.anchorX = this->anchorX - prop.anchorX;
	result.anchorY = this->anchorY - prop.anchorY;
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
	, _anchorX(0.f)
	, _anchorY(0.f)
	, _initialMatri(D2D1::Matrix3x2F::Identity())
	, _finalMatri(D2D1::Matrix3x2F::Identity())
	, _visible(true)
	, _parent(nullptr)
	, _parentScene(nullptr)
	, _hashName(0)
	, _clipEnabled(false)
	, _needSort(false)
	, _needTransform(false)
	, _positionFixed(false)
	, _collider(this)
	, _extrapolate(Property::Origin)
{
}

e2d::Node::~Node()
{
	ActionManager::getInstance()->clearAllBindedWith(this);
	for (const auto& child : _children)
	{
		GC::getInstance()->safeRelease(child);
	}
}

void e2d::Node::visit(Game * game)
{
	if (!_visible)
		return;

	if (!game->isPaused())
	{
		auto updatableNode = dynamic_cast<Updatable*>(this);
		if (updatableNode)
		{
			updatableNode->update();
		}

		// 更新转换矩阵
		_updateTransform();

		// 保留差别属性
		_extrapolate = this->getProperty();
	}

	auto renderer = game->getRenderer();
	auto renderTarget = renderer->getRenderTarget();
	if (_clipEnabled)
	{
		renderTarget->SetTransform(_finalMatri);
		renderTarget->PushAxisAlignedClip(
			D2D1::RectF(0, 0, _width, _height),
			D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
		);
	}

	if (_children.empty())
	{
		auto drawableNode = dynamic_cast<Drawable*>(this);
		if (drawableNode)
		{
			renderTarget->SetTransform(_finalMatri);
			drawableNode->draw(renderer);
		}
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
				child->visit(game);
			}
			else
			{
				break;
			}
		}
		
		auto drawableNode = dynamic_cast<Drawable*>(this);
		if (drawableNode)
		{
			renderTarget->SetTransform(_finalMatri);
			drawableNode->draw(renderer);
		}

		// 访问剩余节点
		for (; i < _children.size(); ++i)
			_children[i]->visit(game);
	}

	if (_clipEnabled)
	{
		renderTarget->PopAxisAlignedClip();
	}
}

void e2d::Node::drawOutline(Renderer * renderer)
{
	if (_visible)
	{
		renderer->getRenderTarget()->SetTransform(_finalMatri);
		renderer->getRenderTarget()->DrawRectangle(
			D2D1::RectF(0, 0, _width, _height),
			renderer->getSolidColorBrush(),
			1.5f
		);

		// 渲染所有子节点的轮廓
		for (const auto& child : _children)
		{
			child->drawOutline(renderer);
		}
	}
}

void e2d::Node::drawCollider()
{
	if (_visible)
	{
		_collider.render();

		// 绘制所有子节点的几何碰撞体
		for (const auto& child : _children)
		{
			child->drawCollider();
		}
	}
}

void e2d::Node::_updateTransform()
{
	if (!_needTransform)
		return;

	_needTransform = false;

	// 计算锚点坐标
	D2D1_POINT_2F anchor = { _width * _anchorX, _height * _anchorY };
	// 变换 Initial 矩阵，子节点将根据这个矩阵进行变换
	_initialMatri = D2D1::Matrix3x2F::Scale(
		_scaleX,
		_scaleY,
		anchor
	) * D2D1::Matrix3x2F::Skew(
		_skewAngleX,
		_skewAngleY,
		anchor
	) * D2D1::Matrix3x2F::Rotation(
		_rotation,
		anchor
	) * D2D1::Matrix3x2F::Translation(
		_posX,
		_posY
	);
	// 根据自身锚点变换 Final 矩阵
	_finalMatri = _initialMatri * D2D1::Matrix3x2F::Translation(-anchor.x, -anchor.y);
	// 和父节点矩阵相乘
	if (!_positionFixed && _parent)
	{
		_initialMatri = _initialMatri * _parent->_initialMatri;
		_finalMatri = _finalMatri * _parent->_initialMatri;
	}

	// 通知子节点进行转换
	for (const auto& child : _children)
	{
		child->_needTransform = true;
	}

	// 更新碰撞体
	_collider.recreate();

	if (_collider.isEnabled() &&
		_collider.isCollisionNotify() &&
		_collider.getShape() != Collider::Shape::None)
	{
		CollisionManager::getInstance()->__updateCollider(&_collider);
	}
}

bool e2d::Node::dispatch(const MouseEvent & e, bool handled)
{
	if (_visible)
	{
		for (auto riter = _children.crbegin(); riter != _children.crend(); ++riter)
			handled = (*riter)->dispatch(e, handled);

		auto handler = dynamic_cast<MouseEventHandler*>(this);
		if (handler)
			handler->handle(e);
	}

	return handled;
}

bool e2d::Node::dispatch(const KeyEvent & e, bool handled)
{
	if (_visible)
	{
		for (auto riter = _children.crbegin(); riter != _children.crend(); ++riter)
			handled = (*riter)->dispatch(e, handled);

		auto handler = dynamic_cast<KeyEventHandler*>(this);
		if (handler)
			handler->handle(e);
	}

	return handled;
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
	for (const auto& child : _children)
	{
		child->_updateOpacity();
	}
}

bool e2d::Node::isVisible() const
{
	return _visible;
}

const e2d::String& e2d::Node::getName() const
{
	return _name;
}

size_t e2d::Node::getHashName() const
{
	return _hashName;
}

float e2d::Node::getPosX() const
{
	return _posX;
}

float e2d::Node::getPosY() const
{
	return _posY;
}

e2d::Point e2d::Node::getPos() const
{
	return Point(_posX, _posY);
}

float e2d::Node::getWidth() const
{
	return _width * _scaleX;
}

float e2d::Node::getHeight() const
{
	return _height * _scaleY;
}

float e2d::Node::getRealWidth() const
{
	return _width;
}

float e2d::Node::getRealHeight() const
{
	return _height;
}

e2d::Size e2d::Node::getRealSize() const
{
	return Size(_width, _height);
}

float e2d::Node::getAnchorX() const
{
	return _anchorX;
}

float e2d::Node::getAnchorY() const
{
	return _anchorY;
}

e2d::Size e2d::Node::getSize() const
{
	return Size(getWidth(), getHeight());
}

float e2d::Node::getScaleX() const
{
	return _scaleX;
}

float e2d::Node::getScaleY() const
{
	return _scaleY;
}

float e2d::Node::getSkewX() const
{
	return _skewAngleX;
}

float e2d::Node::getSkewY() const
{
	return _skewAngleY;
}

float e2d::Node::getRotation() const
{
	return _rotation;
}

float e2d::Node::getOpacity() const
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
	prop.anchorX = _anchorX;
	prop.anchorY = _anchorY;
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
	if (_order == order)
		return;

	_order = order;
	if (_parent)
	{
		_parent->_needSort = true;
	}
}

void e2d::Node::setPosX(float x)
{
	this->setPos(x, _posY);
}

void e2d::Node::setPosY(float y)
{
	this->setPos(_posX, y);
}

void e2d::Node::setPos(const Point & p)
{
	this->setPos(p.x, p.y);
}

void e2d::Node::setPos(float x, float y)
{
	if (_posX == x && _posY == y)
		return;

	_posX = x;
	_posY = y;
	_needTransform = true;
}

void e2d::Node::setPosFixed(bool fixed)
{
	if (_positionFixed == fixed)
		return;

	_positionFixed = fixed;
	_needTransform = true;
}

void e2d::Node::movePosX(float x)
{
	this->movePos(x, 0);
}

void e2d::Node::movePosY(float y)
{
	this->movePos(0, y);
}

void e2d::Node::movePos(float x, float y)
{
	this->setPos(_posX + x, _posY + y);
}

void e2d::Node::movePos(const Vector2 & v)
{
	this->movePos(v.x, v.y);
}

void e2d::Node::setScaleX(float scaleX)
{
	this->setScale(scaleX, _scaleY);
}

void e2d::Node::setScaleY(float scaleY)
{
	this->setScale(_scaleX, scaleY);
}

void e2d::Node::setScale(float scale)
{
	this->setScale(scale, scale);
}

void e2d::Node::setScale(float scaleX, float scaleY)
{
	if (_scaleX == scaleX && _scaleY == scaleY)
		return;

	_scaleX = scaleX;
	_scaleY = scaleY;
	_needTransform = true;
}

void e2d::Node::setSkewX(float angleX)
{
	this->setSkew(angleX, _skewAngleY);
}

void e2d::Node::setSkewY(float angleY)
{
	this->setSkew(_skewAngleX, angleY);
}

void e2d::Node::setSkew(float angleX, float angleY)
{
	if (_skewAngleX == angleX && _skewAngleY == angleY)
		return;

	_skewAngleX = angleX;
	_skewAngleY = angleY;
	_needTransform = true;
}

void e2d::Node::setRotation(float angle)
{
	if (_rotation == angle)
		return;

	_rotation = angle;
	_needTransform = true;
}

void e2d::Node::setOpacity(float opacity)
{
	if (_realOpacity == opacity)
		return;

	_displayOpacity = _realOpacity = std::min(std::max(opacity, 0.f), 1.f);
	// 更新节点透明度
	_updateOpacity();
}

void e2d::Node::setAnchorX(float anchorX)
{
	this->setAnchor(anchorX, _anchorY);
}

void e2d::Node::setAnchorY(float anchorY)
{
	this->setAnchor(_anchorX, anchorY);
}

void e2d::Node::setAnchor(float anchorX, float anchorY)
{
	if (_anchorX == anchorX && _anchorY == anchorY)
		return;

	_anchorX = std::min(std::max(anchorX, 0.f), 1.f);
	_anchorY = std::min(std::max(anchorY, 0.f), 1.f);
	_needTransform = true;
}

void e2d::Node::setWidth(float width)
{
	this->setSize(width, _height);
}

void e2d::Node::setHeight(float height)
{
	this->setSize(_width, height);
}

void e2d::Node::setSize(float width, float height)
{
	if (_width == width && _height == height)
		return;

	_width = width;
	_height = height;
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
	this->setAnchor(prop.anchorX, prop.anchorY);
	this->setScale(prop.scaleX, prop.scaleY);
	this->setRotation(prop.rotation);
	this->setSkew(prop.skewAngleX, prop.skewAngleY);
}

void e2d::Node::setClipEnabled(bool enabled)
{
	_clipEnabled = enabled;
}

void e2d::Node::addChild(Node * child, int order  /* = 0 */)
{
	WARN_IF(child == nullptr, "Node::addChild NULL pointer exception.");

	if (child)
	{
		if (child->_parent != nullptr)
		{
			throw Exception("节点已有父节点, 不能再添加到其他节点");
		}

		for (Node * parent = this; parent != nullptr; parent = parent->getParent())
		{
			if (child == parent)
			{
				throw Exception("一个节点不能同时是另一个节点的父节点和子节点");
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
	for (const auto& node : nodes)
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
	size_t hash = name.hash();

	for (const auto& child : _children)
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
	size_t hash = name.hash();

	for (const auto& child : _children)
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
	size_t hash = childName.hash();

	auto iter = std::find_if(
		_children.begin(),
		_children.end(),
		[childName, hash](Node* child) ->bool { return child->_hashName == hash && child->_name == childName; }
	);

	if (iter != _children.end())
	{
		(*iter)->_parent = nullptr;
		if ((*iter)->_parentScene)
		{
			(*iter)->_setParentScene(nullptr);
		}
		(*iter)->release();
		_children.erase(iter);
	}
}

void e2d::Node::removeAllChildren()
{
	// 所有节点的引用计数减一
	for (const auto& child : _children)
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
	for (const auto& action : actions)
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
	for (const auto& action : actions)
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
	for (const auto& action : actions)
	{
		if (action->getTarget() == this)
		{
			action->stop();
		}
	}
}

bool e2d::Node::containsPoint(const Point& point)
{
	// 更新转换矩阵
	_updateTransform();

	// 为节点创建一个轮廓
	BOOL ret = 0;
	ID2D1RectangleGeometry * rectGeo = nullptr;
	auto factory = Game::getInstance()->getRenderer()->getFactory();

	ThrowIfFailed(
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, _width, _height),
			&rectGeo
		)
	);

	ThrowIfFailed(
		rectGeo->FillContainsPoint(
			D2D1::Point2F(point.x, point.y),
			_finalMatri,
			&ret
		)
	);

	SafeRelease(rectGeo);

	return ret != 0;
}

bool e2d::Node::intersects(Node * node)
{
	// 更新转换矩阵
	_updateTransform();
	node->_updateTransform();

	// 为节点创建轮廓
	D2D1_GEOMETRY_RELATION relation = D2D1_GEOMETRY_RELATION_UNKNOWN;
	ID2D1RectangleGeometry *rectGeo = nullptr, *rectGeo2 = nullptr;
	ID2D1TransformedGeometry *transGeo = nullptr, *transGeo2 = nullptr;
	auto factory = Game::getInstance()->getRenderer()->getFactory();

	ThrowIfFailed(
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, _width, _height),
			&rectGeo
		)
	);

	ThrowIfFailed(
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, node->_width, node->_height),
			&rectGeo2
		)
	);

	ThrowIfFailed(
		factory->CreateTransformedGeometry(
			rectGeo,
			_finalMatri,
			&transGeo
		)
	);

	ThrowIfFailed(
		factory->CreateTransformedGeometry(
			rectGeo2,
			node->_finalMatri,
			&transGeo2
		)
	);

	ThrowIfFailed(
		// 获取相交状态
		transGeo->CompareWithGeometry(
			transGeo2,
			D2D1::Matrix3x2F::Identity(),
			&relation
		)
	);

	SafeRelease(rectGeo);
	SafeRelease(rectGeo2);
	SafeRelease(transGeo);
	SafeRelease(transGeo2);

	return relation != D2D1_GEOMETRY_RELATION_UNKNOWN &&
		relation != D2D1_GEOMETRY_RELATION_DISJOINT;
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
		_hashName = name.hash();
	}
}

void e2d::Node::_setParentScene(Scene * scene)
{
	_parentScene = scene;
	for (const auto& child : _children)
	{
		child->_setParentScene(scene);
	}
}