#include "..\e2dnode.h"
#include "..\e2dmanager.h"

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }


e2d::Button::Button()
	: _func(nullptr)
	, _state(ButtonState::Normal)
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
	, _state(ButtonState::Normal)
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
	, _state(ButtonState::Normal)
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
	, _state(ButtonState::Normal)
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
	, _state(ButtonState::Normal)
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
	if (normal != _normal)
	{
		// 移除旧的
		if (_normal)
		{
			this->removeChild(_normal);
		}
		// 添加新的
		if (normal)
		{
			this->addChild(normal);
			this->setSize(normal->getWidth(), normal->getHeight());
		}
		_normal = normal;

		_updateVisible();
	}
}

void e2d::Button::setMouseOver(Node * mouseover)
{
	if (mouseover != _normal)
	{
		// 移除旧的
		if (_mouseover)
		{
			this->removeChild(_mouseover);
		}
		// 添加新的
		if (mouseover)
		{
			this->addChild(mouseover);
		}
		_mouseover = mouseover;
		_updateVisible();
	}
}

void e2d::Button::setSelected(Node * selected)
{
	if (selected != _normal)
	{
		// 移除旧的
		if (_selected)
		{
			this->removeChild(_selected);
		}
		// 添加新的
		if (selected)
		{
			this->addChild(selected);
		}
		_selected = selected;
		_updateVisible();
	}
}

void e2d::Button::setDisabled(Node * disabled)
{
	if (disabled != _normal)
	{
		// 移除旧的
		if (_disabled)
		{
			this->removeChild(_disabled);
		}
		// 添加新的
		if (disabled)
		{
			this->addChild(disabled);
		}
		_disabled = disabled;
		_updateVisible();
	}
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

void e2d::Button::_fixedUpdate()
{
	if (SceneManager::getInstance()->isTransitioning())
		return;

	auto input = Input::getInstance();
	auto window = Window::getInstance();

	if (_enabled && _visible && _normal)
	{
		if (input->isRelease(Input::Mouse::Left))
		{
			// 鼠标左键抬起时，判断鼠标坐标是否在按钮内部
			if (_isSelected &&
				_normal->containsPoint(input->getMousePos()))
			{
				_runCallback();
			}
			// 标记 _isSelected 为 false
			_isSelected = false;
		}

		if (input->isPress(Input::Mouse::Left))
		{
			if (_normal->containsPoint(input->getMousePos()))
			{
				// 鼠标左键按下，且位于按钮内时，标记 _isSelected 为 true
				_isSelected = true;
				return;
			}
		}

		if (_isSelected && input->isDown(Input::Mouse::Left))
		{
			if (_normal->containsPoint(input->getMousePos()))
			{
				_setState(ButtonState::Selected);
				window->setCursor(Window::Cursor::Hand);
				return;
			}
		}
		else if (_normal->containsPoint(input->getMousePos()))
		{
			_setState(ButtonState::Mouseover);
			window->setCursor(Window::Cursor::Hand);
			return;
		}

		_setState(ButtonState::Normal);
	}

	if (_visible && !_enabled && _normal && _normal->containsPoint(input->getMousePos()))
	{
		window->setCursor(Window::Cursor::No);
	}
}

void e2d::Button::_setState(ButtonState state)
{
	if (_state != state)
	{
		_state = state;
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
		if (_state == ButtonState::Selected && _selected)
		{
			_selected->setVisible(true);
		}
		else if (_state == ButtonState::Mouseover && _mouseover)
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
