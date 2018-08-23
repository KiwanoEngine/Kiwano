#include "..\e2dnode.h"
#include "..\e2dmanager.h"

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }

#define SET_BUTTON_NODE(Old, New)								\
	if (New != Old)												\
	{															\
		if (Old) this->removeChild(Old);						\
		if (New)												\
		{														\
			New->anchor(_anchorX, _anchorY);					\
			this->addChild(New);								\
		}														\
		Old = New;												\
		_updateVisible();										\
	}															\


e2d::Button::Button()
	: _func(nullptr)
	, _status(Status::Normal)
	, _enabled(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
}

e2d::Button::Button(Node * normal, const Function& func)
	: _func(nullptr)
	, _status(Status::Normal)
	, _enabled(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
	this->normal(normal);
	this->clickCallback(func);
}

e2d::Button::Button(Node * normal, Node * selected, const Function& func)
	: _func(nullptr)
	, _status(Status::Normal)
	, _enabled(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
	this->normal(normal);
	this->selected(selected);
	this->clickCallback(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, const Function& func)
	: _func(nullptr)
	, _status(Status::Normal)
	, _enabled(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
	this->normal(normal);
	this->mouseover(mouseover);
	this->selected(selected);
	this->clickCallback(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, Node * disabled, const Function& func)
	: _func(nullptr)
	, _status(Status::Normal)
	, _enabled(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
	this->normal(normal);
	this->mouseover(mouseover);
	this->selected(selected);
	this->disabled(disabled);
	this->clickCallback(func);
}

bool e2d::Button::enabled() const
{
	return _enabled;
}

e2d::Button& e2d::Button::normal(Node * normal)
{
	SET_BUTTON_NODE(_normal, normal);
	if (normal)
	{
		this->size(normal->width(), normal->height());
	}
	return *this;
}

e2d::Button& e2d::Button::mouseover(Node * mouseover)
{
	SET_BUTTON_NODE(_mouseover, mouseover);
	return *this;
}

e2d::Button& e2d::Button::selected(Node * selected)
{
	SET_BUTTON_NODE(_selected, selected);
	return *this;
}

e2d::Button& e2d::Button::disabled(Node * disabled)
{
	SET_BUTTON_NODE(_disabled, disabled);
	return *this;
}

e2d::Button& e2d::Button::enabled(bool enabled)
{
	if (_enabled != enabled)
	{
		_enabled = enabled;
		_updateVisible();
	}
	return *this;
}

e2d::Button& e2d::Button::clickCallback(const Function& func)
{
	_func = func;
	return *this;
}

e2d::Button& e2d::Button::anchor(float anchorX, float anchorY)
{
	Node::anchor(anchorX, anchorY);
	SAFE_SET(_normal, anchor, anchorX, anchorY);
	SAFE_SET(_mouseover, anchor, anchorX, anchorY);
	SAFE_SET(_selected, anchor, anchorX, anchorY);
	SAFE_SET(_disabled, anchor, anchorX, anchorY);
	return *this;
}

bool e2d::Button::dispatch(const MouseEvent & e, bool handled)
{
	if (!handled && _enabled && _visible && _normal)
	{
		bool contains = _normal->containsPoint(e.position());
		if (e.type() == MouseEvent::Type::LeftUp && _isSelected && contains)
		{
			_runCallback();
			_isSelected = false;
			_setStatus(Status::Normal);
			return true;
		}
		else if (e.type() == MouseEvent::Type::LeftDown)
		{
			_isSelected = contains;
			_setStatus(contains ? Status::Selected : Status::Normal);
			
			if (contains)
				return true;
		}
		else if (e.type() == MouseEvent::Type::LeftUp)
		{
			_isSelected = false;
		}
		else if (e.type() == MouseEvent::Type::Move && _isSelected && contains)
		{
			_setStatus(Status::Selected);
			return true;
		}
		else
		{
			if (!e.isLButtonDown() && _isSelected)
			{
				_isSelected = false;
			}

			_setStatus(contains ? Status::Mouseover : Status::Normal);

			if (contains)
				return true;
		}
	}

	return Node::dispatch(e, handled);
}

void e2d::Button::visit(Game * game)
{
	Node::visit(game);

	if (_visible &&
		!_enabled &&
		_normal &&
		_normal->containsPoint(game->input()->mousePos()))
	{
		game->window()->cursor(Window::Cursor::No);
	}
	else if (_status == Status::Mouseover || _status == Status::Selected)
	{
		game->window()->cursor(Window::Cursor::Hand);
	}
}

void e2d::Button::_setStatus(Status status)
{
	if (_status != status)
	{
		_status = status;
		_updateVisible();
	}
}

void e2d::Button::_updateVisible()
{
	SAFE_SET(_normal, visible, false);
	SAFE_SET(_mouseover, visible, false);
	SAFE_SET(_selected, visible, false);
	SAFE_SET(_disabled, visible, false);

	if (_enabled)
	{
		if (_status == Status::Selected && _selected)
		{
			_selected->visible(true);
		}
		else if (_status == Status::Mouseover && _mouseover)
		{
			_mouseover->visible(true);
		}
		else
		{
			if (_normal) _normal->visible(true);
		}
	}
	else
	{
		if (_disabled)
		{
			_disabled->visible(true);
		}
		else
		{
			if (_normal) _normal->visible(true);
		}
	}
}

void e2d::Button::_runCallback()
{
	if (_func)
	{
		_func();
	}
}
