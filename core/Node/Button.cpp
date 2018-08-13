#include "..\e2dnode.h"
#include "..\e2dmanager.h"

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }

#define SET_BUTTON_NODE(Old, New)								\
	if (New != Old)												\
	{															\
		if (Old) this->removeChild(Old);						\
		if (New)												\
		{														\
			New->setPivot(_pivotX, _pivotY);					\
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
	this->setNormal(normal);
	this->setClickFunc(func);
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
	this->setNormal(normal);
	this->setSelected(selected);
	this->setClickFunc(func);
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
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setClickFunc(func);
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
	this->setNormal(normal);
	this->setMouseOver(mouseover);
	this->setSelected(selected);
	this->setDisabled(disabled);
	this->setClickFunc(func);
}

bool e2d::Button::isEnable() const
{
	return _enabled;
}

void e2d::Button::setNormal(Node * normal)
{
	SET_BUTTON_NODE(_normal, normal);
	if (normal)
	{
		this->setSize(normal->getWidth(), normal->getHeight());
	}
}

void e2d::Button::setMouseOver(Node * mouseover)
{
	SET_BUTTON_NODE(_mouseover, mouseover);
}

void e2d::Button::setSelected(Node * selected)
{
	SET_BUTTON_NODE(_selected, selected);
}

void e2d::Button::setDisabled(Node * disabled)
{
	SET_BUTTON_NODE(_disabled, disabled);
}

void e2d::Button::setEnabled(bool enabled)
{
	if (_enabled != enabled)
	{
		_enabled = enabled;
		_updateVisible();
	}
}

void e2d::Button::setClickFunc(const Function& func)
{
	_func = func;
}

void e2d::Button::setPivot(float pivotX, float pivotY)
{
	Node::setPivot(pivotX, pivotY);
	SAFE_SET(_normal, setPivot, pivotX, pivotY);
	SAFE_SET(_mouseover, setPivot, pivotX, pivotY);
	SAFE_SET(_selected, setPivot, pivotX, pivotY);
	SAFE_SET(_disabled, setPivot, pivotX, pivotY);
}

bool e2d::Button::dispatch(const MouseEvent & e, bool handled)
{
	if (!handled && _enabled && _visible && _normal)
	{
		bool contains = _normal->containsPoint(e.getPos());
		if (e.getType() == MouseEvent::Type::LeftUp && _isSelected && contains)
		{
			_runCallback();
			_isSelected = false;
			_setStatus(Status::Normal);
			return true;
		}
		else if (e.getType() == MouseEvent::Type::LeftDown)
		{
			_isSelected = contains;
			_setStatus(contains ? Status::Selected : Status::Normal);
			
			if (contains)
				return true;
		}
		else if (e.getType() == MouseEvent::Type::LeftUp)
		{
			_isSelected = false;
		}
		else if (e.getType() == MouseEvent::Type::Move && _isSelected && contains)
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

void e2d::Button::_render()
{
	Node::_render();

	if (_visible &&
		!_enabled &&
		_normal &&
		_normal->containsPoint(Input::getInstance()->getMousePos()))
	{
		Window::getInstance()->setCursor(Window::Cursor::No);
	}
	else if (_status == Status::Mouseover || _status == Status::Selected)
	{
		Window::getInstance()->setCursor(Window::Cursor::Hand);
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
	SAFE_SET(_normal, setVisible, false);
	SAFE_SET(_mouseover, setVisible, false);
	SAFE_SET(_selected, setVisible, false);
	SAFE_SET(_disabled, setVisible, false);

	if (_enabled)
	{
		if (_status == Status::Selected && _selected)
		{
			_selected->setVisible(true);
		}
		else if (_status == Status::Mouseover && _mouseover)
		{
			_mouseover->setVisible(true);
		}
		else
		{
			if (_normal) _normal->setVisible(true);
		}
	}
	else
	{
		if (_disabled)
		{
			_disabled->setVisible(true);
		}
		else
		{
			if (_normal) _normal->setVisible(true);
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
