#include "..\e2dnode.h"

#define SAFE_SET(pointer, func, ...) if (pointer) { pointer->##func(__VA_ARGS__); }

#define SET_BUTTON_NODE(Old, New)								\
	if (New != Old)												\
	{															\
		if (Old) this->RemoveChild(Old);						\
		if (New)												\
		{														\
			New->SetAnchor(anchor_.x, anchor_.y);				\
			this->AddChild(New);								\
		}														\
		Old = New;												\
		UpdateStatus();											\
		UpdateVisible();										\
	}															\


e2d::ToggleButton::ToggleButton()
	: Button()
	, checked_(true)
	, normal_on_(nullptr)
	, mouseover_on_(nullptr)
	, selected_on_(nullptr)
	, disabled_on_(nullptr)
	, normal_off_(nullptr)
	, mouseover_off_(nullptr)
	, selected_off_(nullptr)
	, disabled_off_(nullptr)
{
}

e2d::ToggleButton::ToggleButton(Node * normal_on, Node * normal_off, const Function& func)
	: Button()
	, checked_(true)
	, normal_on_(nullptr)
	, mouseover_on_(nullptr)
	, selected_on_(nullptr)
	, disabled_on_(nullptr)
	, normal_off_(nullptr)
	, mouseover_off_(nullptr)
	, selected_off_(nullptr)
	, disabled_off_(nullptr)
{
	this->SetNormal(normal_on);
	this->SetNormalOff(normal_off);
	this->SetCallbackOnClick(func);
}

e2d::ToggleButton::ToggleButton(Node * normal_on, Node * normal_off, Node * selected_on, Node * selected_off, const Function& func)
	: Button()
	, checked_(true)
	, normal_on_(nullptr)
	, mouseover_on_(nullptr)
	, selected_on_(nullptr)
	, disabled_on_(nullptr)
	, normal_off_(nullptr)
	, mouseover_off_(nullptr)
	, selected_off_(nullptr)
	, disabled_off_(nullptr)
{
	this->SetNormal(normal_on);
	this->SetNormalOff(normal_off);
	this->SetSelected(selected_on);
	this->SetSelectedOff(selected_off);
	this->SetCallbackOnClick(func);
}

e2d::ToggleButton::ToggleButton(Node * normal_on, Node * normal_off, Node * mouseover_on, Node * mouseover_off, Node * selected_on, Node * selected_off, const Function& func)
	: Button()
	, checked_(true)
	, normal_on_(nullptr)
	, mouseover_on_(nullptr)
	, selected_on_(nullptr)
	, disabled_on_(nullptr)
	, normal_off_(nullptr)
	, mouseover_off_(nullptr)
	, selected_off_(nullptr)
	, disabled_off_(nullptr)
{
	this->SetNormal(normal_on);
	this->SetNormalOff(normal_off);
	this->SetMouseOver(mouseover_on);
	this->SetMouseOverOff(mouseover_off);
	this->SetSelected(selected_on);
	this->SetSelectedOff(selected_off);
	this->SetCallbackOnClick(func);
}

e2d::ToggleButton::ToggleButton(Node * normal_on, Node * normal_off, Node * mouseover_on, Node * mouseover_off, Node * selected_on, Node * selected_off, Node * disabled_on, Node * disabled_off, const Function& func)
	: Button()
	, checked_(true)
	, normal_off_(nullptr)
	, mouseover_off_(nullptr)
	, selected_off_(nullptr)
	, disabled_off_(nullptr)
{
	this->SetNormal(normal_on);
	this->SetNormalOff(normal_off);
	this->SetMouseOver(mouseover_on);
	this->SetMouseOverOff(mouseover_off);
	this->SetSelected(selected_on);
	this->SetSelectedOff(selected_off);
	this->SetDisabled(disabled_on);
	this->SetDisabledOff(disabled_off);
	this->SetCallbackOnClick(func);
}

bool e2d::ToggleButton::IsChecked() const
{
	return checked_;
}

void e2d::ToggleButton::SetChecked(bool checked)
{
	if (checked_ != checked)
	{
		checked_ = checked;
		UpdateStatus();
		UpdateVisible();
	}
}

void e2d::ToggleButton::SetNormal(Node * normal)
{
	SET_BUTTON_NODE(normal_on_, normal);
	if (normal)
	{
		this->SetSize(normal->GetWidth(), normal->GetHeight());
	}
}

void e2d::ToggleButton::SetMouseOver(Node * mouseover)
{
	SET_BUTTON_NODE(mouseover_on_, mouseover);
}

void e2d::ToggleButton::SetSelected(Node * selected)
{
	SET_BUTTON_NODE(selected_on_, selected);
}

void e2d::ToggleButton::SetDisabled(Node * disabled)
{
	SET_BUTTON_NODE(disabled_on_, disabled);
}

void e2d::ToggleButton::SetNormalOff(Node * normal)
{
	SET_BUTTON_NODE(normal_off_, normal);
}

void e2d::ToggleButton::SetMouseOverOff(Node * mouseover)
{
	SET_BUTTON_NODE(mouseover_off_, mouseover);
}

void e2d::ToggleButton::SetSelectedOff(Node * selected)
{
	SET_BUTTON_NODE(selected_off_, selected);
}

void e2d::ToggleButton::SetDisabledOff(Node * disabled)
{
	SET_BUTTON_NODE(disabled_off_, disabled);
}

void e2d::ToggleButton::SetAnchor(float anchor_x, float anchor_y)
{
	Node::SetAnchor(anchor_x, anchor_y);
	SAFE_SET(normal_on_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(mouseover_on_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(selected_on_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(disabled_on_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(normal_off_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(mouseover_off_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(selected_off_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(disabled_off_, SetAnchor, anchor_x, anchor_y);
}

void e2d::ToggleButton::UpdateStatus()
{
	if (checked_)
	{
		normal_ = normal_on_;
		mouseover_ = mouseover_on_;
		selected_ = selected_on_;
		disabled_ = disabled_on_;

		SAFE_SET(normal_off_, SetVisible, false);
		SAFE_SET(mouseover_off_, SetVisible, false);
		SAFE_SET(selected_off_, SetVisible, false);
		SAFE_SET(disabled_off_, SetVisible, false);
	}
	else
	{
		normal_ = normal_off_;
		mouseover_ = mouseover_off_;
		selected_ = selected_off_;
		disabled_ = disabled_off_;

		SAFE_SET(normal_on_, SetVisible, false);
		SAFE_SET(mouseover_on_, SetVisible, false);
		SAFE_SET(selected_on_, SetVisible, false);
		SAFE_SET(disabled_on_, SetVisible, false);
	}
}

void e2d::ToggleButton::OnClick()
{
	checked_ = !checked_;
	UpdateStatus();

	if (callback_)
	{
		callback_();
	}
}
