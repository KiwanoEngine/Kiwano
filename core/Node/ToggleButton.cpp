#include "..\e2dnode.h"

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }

#define SET_BUTTON_NODE(Old, New)								\
	if (New != Old)												\
	{															\
		if (Old) this->removeChild(Old);						\
		if (New)												\
		{														\
			this->addChild(New);								\
			this->setSize(New->getWidth(), New->getHeight());	\
		}														\
		Old = New;												\
		_updateState();											\
		_updateVisible();										\
	}															\


e2d::ToggleButton::ToggleButton()
	: Button()
	, _toggle(true)
	, _normalOn(nullptr)
	, _mouseoverOn(nullptr)
	, _selectedOn(nullptr)
	, _disabledOn(nullptr)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
}

e2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, const Function& func)
	: Button()
	, _toggle(true)
	, _normalOn(nullptr)
	, _mouseoverOn(nullptr)
	, _selectedOn(nullptr)
	, _disabledOn(nullptr)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setClickFunc(func);
}

e2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnSelected, Node * toggleOffSelected, const Function& func)
	: Button()
	, _toggle(true)
	, _normalOn(nullptr)
	, _mouseoverOn(nullptr)
	, _selectedOn(nullptr)
	, _disabledOn(nullptr)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setClickFunc(func);
}

e2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, const Function& func)
	: Button()
	, _toggle(true)
	, _normalOn(nullptr)
	, _mouseoverOn(nullptr)
	, _selectedOn(nullptr)
	, _disabledOn(nullptr)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setMouseOver(toggleOnMouseOver);
	this->setMouseOverOff(toggleOffMouseOver);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setClickFunc(func);
}

e2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, Node * toggleOnDisabled, Node * toggleOffDisabled, const Function& func)
	: Button()
	, _toggle(true)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->setNormal(toggleOnNormal);
	this->setNormalOff(toggleOffNormal);
	this->setMouseOver(toggleOnMouseOver);
	this->setMouseOverOff(toggleOffMouseOver);
	this->setSelected(toggleOnSelected);
	this->setSelectedOff(toggleOffSelected);
	this->setDisabled(toggleOnDisabled);
	this->setDisabledOff(toggleOffDisabled);
	this->setClickFunc(func);
}

bool e2d::ToggleButton::getState() const
{
	return _toggle;
}

void e2d::ToggleButton::setState(bool bState)
{
	if (_toggle != bState)
	{
		_toggle = bState;
		_updateState();
		_updateVisible();
	}
}

void e2d::ToggleButton::setNormal(Node * normal)
{
	SET_BUTTON_NODE(_normalOn, normal);
}

void e2d::ToggleButton::setMouseOver(Node * mouseover)
{
	SET_BUTTON_NODE(_mouseoverOn, mouseover);
}

void e2d::ToggleButton::setSelected(Node * selected)
{
	SET_BUTTON_NODE(_selectedOn, selected);
}

void e2d::ToggleButton::setDisabled(Node * disabled)
{
	SET_BUTTON_NODE(_disabledOn, disabled);
}

void e2d::ToggleButton::setNormalOff(Node * normal)
{
	SET_BUTTON_NODE(_normalOff, normal);
}

void e2d::ToggleButton::setMouseOverOff(Node * mouseover)
{
	SET_BUTTON_NODE(_mouseoverOff, mouseover);
}

void e2d::ToggleButton::setSelectedOff(Node * selected)
{
	SET_BUTTON_NODE(_selectedOff, selected);
}

void e2d::ToggleButton::setDisabledOff(Node * disabled)
{
	SET_BUTTON_NODE(_disabledOff, disabled);
}

void e2d::ToggleButton::_updateState()
{
	if (_toggle)
	{
		_normal = _normalOn;
		_mouseover = _mouseoverOn;
		_selected = _selectedOn;
		_disabled = _disabledOn;

		SAFE_SET(_normalOff, setVisible, false);
		SAFE_SET(_mouseoverOff, setVisible, false);
		SAFE_SET(_selectedOff, setVisible, false);
		SAFE_SET(_disabledOff, setVisible, false);
	}
	else
	{
		_normal = _normalOff;
		_mouseover = _mouseoverOff;
		_selected = _selectedOff;
		_disabled = _disabledOff;

		SAFE_SET(_normalOn, setVisible, false);
		SAFE_SET(_mouseoverOn, setVisible, false);
		SAFE_SET(_selectedOn, setVisible, false);
		SAFE_SET(_disabledOn, setVisible, false);
	}
}

void e2d::ToggleButton::_runCallback()
{
	_toggle = !_toggle;
	_updateState();

	if (_func)
	{
		_func();
	}
}
