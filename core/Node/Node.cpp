#include "..\e2dnode.h"
#include "..\e2devent.h"
#include "..\e2dmanager.h"
#include "..\e2daction.h"

const e2d::Node::Property e2d::Node::Property::Origin;

e2d::Node::Property e2d::Node::Property::operator+(Property const & prop) const
{
	Property result;
	result.position = this->position + prop.position;
	result.size = this->size + prop.size;
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
	result.position = this->position - prop.position;
	result.size = this->size - prop.size;
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
	, _position()
	, _size()
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
	ActionManager::instance()->clearAllBindedWith(this);
	for (const auto& child : _children)
	{
		GC::instance()->safeRelease(child);
	}
}

void e2d::Node::visit(Game * game)
{
	if (!_visible)
		return;

	if (!game->paused())
	{
		auto updatableNode = dynamic_cast<Updatable*>(this);
		if (updatableNode)
		{
			updatableNode->update();
		}

		// 更新转换矩阵
		_updateTransform();

		// 保留差别属性
		_extrapolate = this->properties();
	}

	auto renderer = game->renderer();
	auto renderTarget = renderer->renderTarget();
	if (_clipEnabled)
	{
		renderTarget->SetTransform(_finalMatri);
		renderTarget->PushAxisAlignedClip(
			D2D1::RectF(0, 0, _size.width, _size.height),
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
			if (child->order() < 0)
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
		renderer->renderTarget()->SetTransform(_finalMatri);
		renderer->renderTarget()->DrawRectangle(
			D2D1::RectF(0, 0, _size.width, _size.height),
			renderer->solidBrush(),
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
	D2D1_POINT_2F anchor = { _size.width * _anchorX, _size.height * _anchorY };
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
		_position.x,
		_position.y
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

	if (_collider.enabled() &&
		_collider.notify() &&
		_collider.shape() != Collider::Shape::None)
	{
		CollisionManager::instance()->__updateCollider(&_collider);
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
			[](Node * n1, Node * n2) { return n1->order() < n2->order(); }
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

bool e2d::Node::visible() const
{
	return _visible;
}

const e2d::String& e2d::Node::name() const
{
	return _name;
}

size_t e2d::Node::hashName() const
{
	return _hashName;
}

float e2d::Node::positionX() const
{
	return _position.x;
}

float e2d::Node::positionY() const
{
	return _position.y;
}

const e2d::Point& e2d::Node::position() const
{
	return _position;
}

float e2d::Node::width() const
{
	return _size.width * _scaleX;
}

float e2d::Node::height() const
{
	return _size.height * _scaleY;
}

e2d::Size e2d::Node::size() const
{
	return Size(width(), height());
}

float e2d::Node::realWidth() const
{
	return _size.width;
}

float e2d::Node::realHeight() const
{
	return _size.height;
}

const e2d::Size& e2d::Node::realSize() const
{
	return _size;
}

float e2d::Node::anchorX() const
{
	return _anchorX;
}

float e2d::Node::anchorY() const
{
	return _anchorY;
}

float e2d::Node::scaleX() const
{
	return _scaleX;
}

float e2d::Node::scaleY() const
{
	return _scaleY;
}

float e2d::Node::skewX() const
{
	return _skewAngleX;
}

float e2d::Node::skewY() const
{
	return _skewAngleY;
}

float e2d::Node::rotation() const
{
	return _rotation;
}

float e2d::Node::opacity() const
{
	return _realOpacity;
}

e2d::Node::Property e2d::Node::properties() const
{
	Property prop;
	prop.position = _position;
	prop.size = _size;
	prop.anchorX = _anchorX;
	prop.anchorY = _anchorY;
	prop.scaleX = _scaleX;
	prop.scaleY = _scaleY;
	prop.rotation = _rotation;
	prop.skewAngleX = _skewAngleX;
	prop.skewAngleY = _skewAngleY;
	return std::move(prop);
}

e2d::Node::Property e2d::Node::extrapolate() const
{
	return this->properties() - _extrapolate;
}

e2d::Collider* e2d::Node::collider()
{
	return &_collider;
}

int e2d::Node::order() const
{
	return _order;
}

e2d::Node& e2d::Node::order(int order)
{
	if (_order == order)
		return *this;

	_order = order;
	if (_parent)
	{
		_parent->_needSort = true;
	}
	return *this;
}

e2d::Node& e2d::Node::positionX(float x)
{
	return position(x, _position.y);
}

e2d::Node& e2d::Node::positionY(float y)
{
	return position(_position.x, y);
}

e2d::Node& e2d::Node::position(const Point & p)
{
	return position(p.x, p.y);
}

e2d::Node& e2d::Node::position(float x, float y)
{
	if (_position.x == x && _position.y == y)
		return *this;

	_position.x = x;
	_position.y = y;
	_needTransform = true;
	return *this;
}

e2d::Node& e2d::Node::positionFixed(bool fixed)
{
	if (_positionFixed == fixed)
		return *this;

	_positionFixed = fixed;
	_needTransform = true;
	return *this;
}

e2d::Node& e2d::Node::move(float x, float y)
{
	return position(_position.x + x, _position.y + y);
}

e2d::Node& e2d::Node::move(const Vector2 & v)
{
	return move(v.x, v.y);
}

e2d::Node& e2d::Node::scaleX(float scaleX)
{
	return scale(scaleX, _scaleY);
}

e2d::Node& e2d::Node::scaleY(float scaleY)
{
	return scale(_scaleX, scaleY);
}

e2d::Node& e2d::Node::scale(float scale0)
{
	return scale(scale0, scale0);
}

e2d::Node& e2d::Node::scale(float scaleX, float scaleY)
{
	if (_scaleX == scaleX && _scaleY == scaleY)
		return *this;

	_scaleX = scaleX;
	_scaleY = scaleY;
	_needTransform = true;
	return *this;
}

e2d::Node& e2d::Node::skewX(float angleX)
{
	return skew(angleX, _skewAngleY);
}

e2d::Node& e2d::Node::skewY(float angleY)
{
	return skew(_skewAngleX, angleY);
}

e2d::Node& e2d::Node::skew(float angleX, float angleY)
{
	if (_skewAngleX == angleX && _skewAngleY == angleY)
		return *this;

	_skewAngleX = angleX;
	_skewAngleY = angleY;
	_needTransform = true;
	return *this;
}

e2d::Node& e2d::Node::rotation(float angle)
{
	if (_rotation == angle)
		return *this;

	_rotation = angle;
	_needTransform = true;
	return *this;
}

e2d::Node& e2d::Node::opacity(float opacity)
{
	if (_realOpacity == opacity)
		return *this;

	_displayOpacity = _realOpacity = std::min(std::max(opacity, 0.f), 1.f);
	// 更新节点透明度
	_updateOpacity();
	return *this;
}

e2d::Node& e2d::Node::anchorX(float anchorX)
{
	return anchor(anchorX, _anchorY);
}

e2d::Node& e2d::Node::anchorY(float anchorY)
{
	return anchor(_anchorX, anchorY);
}

e2d::Node& e2d::Node::anchor(float anchorX, float anchorY)
{
	if (_anchorX == anchorX && _anchorY == anchorY)
		return *this;

	_anchorX = std::min(std::max(anchorX, 0.f), 1.f);
	_anchorY = std::min(std::max(anchorY, 0.f), 1.f);
	_needTransform = true;
	return *this;
}

e2d::Node& e2d::Node::width(float width)
{
	return size(width, _size.height);
}

e2d::Node& e2d::Node::height(float height)
{
	return size(_size.width, height);
}

e2d::Node& e2d::Node::size(float width, float height)
{
	if (_size.width == width && _size.height == height)
		return *this;

	_size.width = width;
	_size.height = height;
	_needTransform = true;
	return *this;
}

e2d::Node& e2d::Node::size(Size size0)
{
	return size(size0.width, size0.height);
}

e2d::Node& e2d::Node::properties(Property prop)
{
	this->position(prop.position.x, prop.position.y);
	this->size(prop.size.width, prop.size.height);
	this->anchor(prop.anchorX, prop.anchorY);
	this->scale(prop.scaleX, prop.scaleY);
	this->rotation(prop.rotation);
	this->skew(prop.skewAngleX, prop.skewAngleY);
	return *this;
}

e2d::Node& e2d::Node::clipEnabled(bool enabled)
{
	_clipEnabled = enabled;
	return *this;
}

e2d::Node& e2d::Node::addChild(Node * child, int order  /* = 0 */)
{
	WARN_IF(child == nullptr, "Node::addChild NULL pointer exception.");

	if (child)
	{
		if (child->_parent != nullptr)
		{
			throw Exception("节点已有父节点, 不能再添加到其他节点");
		}

		for (Node * parent = this; parent != nullptr; parent = parent->parent())
		{
			if (child == parent)
			{
				throw Exception("一个节点不能同时是另一个节点的父节点和子节点");
			}
		}

		child->retain();
		_children.push_back(child);
		child->order(order);
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
	return *this;
}

e2d::Node& e2d::Node::addChild(const std::vector<Node*>& nodes, int order)
{
	for (const auto& node : nodes)
	{
		this->addChild(node, order);
	}
	return *this;
}

e2d::Node * e2d::Node::parent() const
{
	return _parent;
}

e2d::Scene * e2d::Node::parentScene() const
{
	return _parentScene;
}

std::vector<e2d::Node*> e2d::Node::children(const String& name) const
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

e2d::Node * e2d::Node::child(const String& name) const
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

const std::vector<e2d::Node*>& e2d::Node::children() const
{
	return _children;
}

int e2d::Node::childrenCount() const
{
	return static_cast<int>(_children.size());
}

e2d::Node& e2d::Node::removeFromParent()
{
	if (_parent)
	{
		_parent->removeChild(this);
	}
	return *this;
}

e2d::Node& e2d::Node::removeChild(Node * child)
{
	WARN_IF(child == nullptr, "Node::removeChildren NULL pointer exception.");

	if (_children.empty())
	{
		return *this;
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
			return *this;
		}
	}
	return *this;
}

e2d::Node& e2d::Node::removeChildren(const String& childName)
{
	WARN_IF(childName.empty(), "Invalid Node name.");

	if (_children.empty())
	{
		return *this;
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
	return *this;
}

e2d::Node& e2d::Node::removeAllChildren()
{
	// 所有节点的引用计数减一
	for (const auto& child : _children)
	{
		child->release();
	}
	// 清空储存节点的容器
	_children.clear();
	return *this;
}

e2d::Node& e2d::Node::runAction(Action * action)
{
	ActionManager::instance()->start(action, this, false);
	return *this;
}

e2d::Node& e2d::Node::resumeAction(const String& name)
{
	auto& actions = ActionManager::instance()->actions(name);
	for (const auto& action : actions)
	{
		if (action->target() == this)
		{
			action->resume();
		}
	}
	return *this;
}

e2d::Node& e2d::Node::pauseAction(const String& name)
{
	auto& actions = ActionManager::instance()->actions(name);
	for (const auto& action : actions)
	{
		if (action->target() == this)
		{
			action->pause();
		}
	}
	return *this;
}

e2d::Node& e2d::Node::stopAction(const String& name)
{
	auto& actions = ActionManager::instance()->actions(name);
	for (const auto& action : actions)
	{
		if (action->target() == this)
		{
			action->stop();
		}
	}
	return *this;
}

bool e2d::Node::containsPoint(const Point& point)
{
	// 更新转换矩阵
	_updateTransform();

	// 为节点创建一个轮廓
	BOOL ret = 0;
	ID2D1RectangleGeometry * rectGeo = nullptr;
	auto factory = Game::instance()->renderer()->factory();

	ThrowIfFailed(
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, _size.width, _size.height),
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
	auto factory = Game::instance()->renderer()->factory();

	ThrowIfFailed(
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, _size.width, _size.height),
			&rectGeo
		)
	);

	ThrowIfFailed(
		factory->CreateRectangleGeometry(
			D2D1::RectF(0, 0, node->_size.width, node->_size.height),
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

e2d::Node& e2d::Node::resumeAllActions()
{
	ActionManager::instance()->resumeAllBindedWith(this);
	return *this;
}

e2d::Node& e2d::Node::pauseAllActions()
{
	ActionManager::instance()->pauseAllBindedWith(this);
	return *this;
}

e2d::Node& e2d::Node::stopAllActions()
{
	ActionManager::instance()->stopAllBindedWith(this);
	return *this;
}

e2d::Node& e2d::Node::visible(bool value)
{
	_visible = value;
	return *this;
}

e2d::Node& e2d::Node::name(const String& name)
{
	WARN_IF(name.empty(), "Invalid Node name.");

	if (!name.empty() && _name != name)
	{
		// 保存节点名
		_name = name;
		// 保存节点 Hash 名
		_hashName = name.hash();
	}

	return *this;
}

void e2d::Node::_setParentScene(Scene * scene)
{
	_parentScene = scene;
	for (const auto& child : _children)
	{
		child->_setParentScene(scene);
	}
}