#include "..\e2dnode.h"
#include "..\e2dmanager.h"

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }


e2d::Button::Button()
	: _func(nullptr)
	, _state(ButtonState::NORMAL)
	, _enable(true)
	, _isSelected(false)
	, _normal(nullptr)
	, _mouseover(nullptr)
	, _selected(nullptr)
	, _disabled(nullptr)
{
}

e2d::Button::Button(Node * normal, const Function& func)
	: _func(nullptr)
	, _state(ButtonState::NORMAL)
	, _enable(true)
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
	, _state(ButtonState::NORMAL)
	, _enable(true)
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
	, _state(ButtonState::NORMAL)
	, _enable(true)
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
	, _state(ButtonState::NORMAL)
	, _enable(true)
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

e2d::Button * e2d::Button::create()
{
	return Create<Button>();
}

e2d::Button * e2d::Button::create(Node * normal, const Function & func)
{
	return Create<Button>(normal, func);
}

e2d::Button * e2d::Button::create(Node * normal, Node * selected, const Function & func)
{
	return Create<Button>(normal, selected, func);
}

e2d::Button * e2d::Button::create(Node * normal, Node * mouseover, Node * selected, const Function & func)
{
	return Create<Button>(normal, mouseover, selected, func);
}

e2d::Button * e2d::Button::create(Node * normal, Node * mouseover, Node * selected, Node * disabled, const Function & func)
{
	return Create<Button>(normal, mouseover, selected, disabled, func);
}

bool e2d::Button::isEnable() const
{
	return _enable;
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

		_updateVisiable();
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
		_updateVisiable();
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
		_updateVisiable();
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
		_updateVisiable();
	}
}

void e2d::Button::setEnable(bool enable)
{
	if (_enable != enable)
	{
		_enable = enable;
		_updateVisiable();
	}
}

void e2d::Button::setClickFunc(const Function& func)
{
	WARN_IF(_normal == nullptr, "Button cannot work without anything to show. Please set its normal displayed.");
	
	_func = func;
}

void e2d::Button::onFixedUpdate()
{
	if (SceneManager::isTransitioning())
		return;

	if (_enable && _visiable && _normal)
	{
		if (Input::isRelease(Input::Mouse::LEFT))
		{
			// 鼠标左键抬起时，判断鼠标坐标是否在按钮内部
			if (_isSelected &&
				_normal->isPointIn(Input::getMousePos()))
			{
				_runCallback();
			}
			// 标记 _isSelected 为 false
			_isSelected = false;
		}

		if (Input::isPress(Input::Mouse::LEFT))
		{
			if (_normal->isPointIn(Input::getMousePos()))
			{
				// 鼠标左键按下，且位于按钮内时，标记 _isSelected 为 true
				_isSelected = true;
				return;
			}
		}

		if (_isSelected && Input::isDown(Input::Mouse::LEFT))
		{
			if (_normal->isPointIn(Input::getMousePos()))
			{
				_setState(ButtonState::SELECTED);
				Window::setCursor(Window::Cursor::HAND);
				return;
			}
		}
		else if (_normal->isPointIn(Input::getMousePos()))
		{
			_setState(ButtonState::MOUSEOVER);
			Window::setCursor(Window::Cursor::HAND);
			return;
		}

		_setState(ButtonState::NORMAL);
	}

	if (_visiable && !_enable && _normal && _normal->isPointIn(Input::getMousePos()))
	{
		Window::setCursor(Window::Cursor::NO);
	}
}

void e2d::Button::_setState(ButtonState state)
{
	if (_state != state)
	{
		_state = state;
		_updateVisiable();
	}
}

void e2d::Button::_updateVisiable()
{
	SAFE_SET(_normal, setVisiable, false);
	SAFE_SET(_mouseover, setVisiable, false);
	SAFE_SET(_selected, setVisiable, false);
	SAFE_SET(_disabled, setVisiable, false);

	if (_enable)
	{
		if (_state == ButtonState::SELECTED && _selected)
		{
			_selected->setVisiable(true);
		}
		else if (_state == ButtonState::MOUSEOVER && _mouseover)
		{
			_mouseover->setVisiable(true);
		}
		else
		{
			if (_normal) _normal->setVisiable(true);
		}
	}
	else
	{
		if (_disabled)
		{
			_disabled->setVisiable(true);
		}
		else
		{
			if (_normal) _normal->setVisiable(true);
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
