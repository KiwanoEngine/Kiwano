#include "..\e2dnode.h"

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
		_updateStatus();										\
		_updateVisible();										\
	}															\


e2d::ToggleButton::ToggleButton()
	: Button()
	, _checked(true)
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
	, _checked(true)
	, _normalOn(nullptr)
	, _mouseoverOn(nullptr)
	, _selectedOn(nullptr)
	, _disabledOn(nullptr)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->normal(toggleOnNormal);
	this->normalOff(toggleOffNormal);
	this->clickCallback(func);
}

e2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnSelected, Node * toggleOffSelected, const Function& func)
	: Button()
	, _checked(true)
	, _normalOn(nullptr)
	, _mouseoverOn(nullptr)
	, _selectedOn(nullptr)
	, _disabledOn(nullptr)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->normal(toggleOnNormal);
	this->normalOff(toggleOffNormal);
	this->selected(toggleOnSelected);
	this->selectedOff(toggleOffSelected);
	this->clickCallback(func);
}

e2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, const Function& func)
	: Button()
	, _checked(true)
	, _normalOn(nullptr)
	, _mouseoverOn(nullptr)
	, _selectedOn(nullptr)
	, _disabledOn(nullptr)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->normal(toggleOnNormal);
	this->normalOff(toggleOffNormal);
	this->mouseover(toggleOnMouseOver);
	this->mouseOverOff(toggleOffMouseOver);
	this->selected(toggleOnSelected);
	this->selectedOff(toggleOffSelected);
	this->clickCallback(func);
}

e2d::ToggleButton::ToggleButton(Node * toggleOnNormal, Node * toggleOffNormal, Node * toggleOnMouseOver, Node * toggleOffMouseOver, Node * toggleOnSelected, Node * toggleOffSelected, Node * toggleOnDisabled, Node * toggleOffDisabled, const Function& func)
	: Button()
	, _checked(true)
	, _normalOff(nullptr)
	, _mouseoverOff(nullptr)
	, _selectedOff(nullptr)
	, _disabledOff(nullptr)
{
	this->normal(toggleOnNormal);
	this->normalOff(toggleOffNormal);
	this->mouseover(toggleOnMouseOver);
	this->mouseOverOff(toggleOffMouseOver);
	this->selected(toggleOnSelected);
	this->selectedOff(toggleOffSelected);
	this->disabled(toggleOnDisabled);
	this->disabledOff(toggleOffDisabled);
	this->clickCallback(func);
}

bool e2d::ToggleButton::checked() const
{
	return _checked;
}

e2d::ToggleButton& e2d::ToggleButton::checked(bool checked)
{
	if (_checked != checked)
	{
		_checked = checked;
		_updateStatus();
		_updateVisible();
	}
	return *this;
}

e2d::ToggleButton& e2d::ToggleButton::normal(Node * normal)
{
	SET_BUTTON_NODE(_normalOn, normal);
	if (normal)
	{
		this->size(normal->width(), normal->height());
	}
	return *this;
}

e2d::ToggleButton& e2d::ToggleButton::mouseover(Node * mouseover)
{
	SET_BUTTON_NODE(_mouseoverOn, mouseover);
	return *this;
}

e2d::ToggleButton& e2d::ToggleButton::selected(Node * selected)
{
	SET_BUTTON_NODE(_selectedOn, selected);
	return *this;
}

e2d::ToggleButton& e2d::ToggleButton::disabled(Node * disabled)
{
	SET_BUTTON_NODE(_disabledOn, disabled);
	return *this;
}

e2d::ToggleButton& e2d::ToggleButton::normalOff(Node * normal)
{
	SET_BUTTON_NODE(_normalOff, normal);
	return *this;
}

e2d::ToggleButton& e2d::ToggleButton::mouseOverOff(Node * mouseover)
{
	SET_BUTTON_NODE(_mouseoverOff, mouseover);
	return *this;
}

e2d::ToggleButton& e2d::ToggleButton::selectedOff(Node * selected)
{
	SET_BUTTON_NODE(_selectedOff, selected);
	return *this;
}

e2d::ToggleButton& e2d::ToggleButton::disabledOff(Node * disabled)
{
	SET_BUTTON_NODE(_disabledOff, disabled);
	return *this;
}

e2d::ToggleButton& e2d::ToggleButton::anchor(float anchorX, float anchorY)
{
	Node::anchor(anchorX, anchorY);
	SAFE_SET(_normalOn, anchor, anchorX, anchorY);
	SAFE_SET(_mouseoverOn, anchor, anchorX, anchorY);
	SAFE_SET(_selectedOn, anchor, anchorX, anchorY);
	SAFE_SET(_disabledOn, anchor, anchorX, anchorY);
	SAFE_SET(_normalOff, anchor, anchorX, anchorY);
	SAFE_SET(_mouseoverOff, anchor, anchorX, anchorY);
	SAFE_SET(_selectedOff, anchor, anchorX, anchorY);
	SAFE_SET(_disabledOff, anchor, anchorX, anchorY);
	return *this;
}

void e2d::ToggleButton::_updateStatus()
{
	if (_checked)
	{
		_normal = _normalOn;
		_mouseover = _mouseoverOn;
		_selected = _selectedOn;
		_disabled = _disabledOn;

		SAFE_SET(_normalOff, visible, false);
		SAFE_SET(_mouseoverOff, visible, false);
		SAFE_SET(_selectedOff, visible, false);
		SAFE_SET(_disabledOff, visible, false);
	}
	else
	{
		_normal = _normalOff;
		_mouseover = _mouseoverOff;
		_selected = _selectedOff;
		_disabled = _disabledOff;

		SAFE_SET(_normalOn, visible, false);
		SAFE_SET(_mouseoverOn, visible, false);
		SAFE_SET(_selectedOn, visible, false);
		SAFE_SET(_disabledOn, visible, false);
	}
}

void e2d::ToggleButton::_runCallback()
{
	_checked = !_checked;
	_updateStatus();

	if (_func)
	{
		_func();
	}
}
