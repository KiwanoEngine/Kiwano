#include "..\e2dcomponent.h"
#include "..\e2dmanager.h"
#include "..\e2dmodule.h"

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
		UpdateVisible();										\
	}															\


e2d::Button::Button()
	: callback_(nullptr)
	, status_(Status::Normal)
	, enabled_(true)
	, is_selected_(false)
	, normal_(nullptr)
	, mouseover_(nullptr)
	, selected_(nullptr)
	, disabled_(nullptr)
{
}

e2d::Button::Button(Node * normal, const Function& func)
	: callback_(nullptr)
	, status_(Status::Normal)
	, enabled_(true)
	, is_selected_(false)
	, normal_(nullptr)
	, mouseover_(nullptr)
	, selected_(nullptr)
	, disabled_(nullptr)
{
	this->SetNormal(normal);
	this->SetCallbackOnClick(func);
}

e2d::Button::Button(Node * normal, Node * selected, const Function& func)
	: callback_(nullptr)
	, status_(Status::Normal)
	, enabled_(true)
	, is_selected_(false)
	, normal_(nullptr)
	, mouseover_(nullptr)
	, selected_(nullptr)
	, disabled_(nullptr)
{
	this->SetNormal(normal);
	this->SetSelected(selected);
	this->SetCallbackOnClick(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, const Function& func)
	: callback_(nullptr)
	, status_(Status::Normal)
	, enabled_(true)
	, is_selected_(false)
	, normal_(nullptr)
	, mouseover_(nullptr)
	, selected_(nullptr)
	, disabled_(nullptr)
{
	this->SetNormal(normal);
	this->SetMouseOver(mouseover);
	this->SetSelected(selected);
	this->SetCallbackOnClick(func);
}

e2d::Button::Button(Node * normal, Node * mouseover, Node * selected, Node * disabled, const Function& func)
	: callback_(nullptr)
	, status_(Status::Normal)
	, enabled_(true)
	, is_selected_(false)
	, normal_(nullptr)
	, mouseover_(nullptr)
	, selected_(nullptr)
	, disabled_(nullptr)
{
	this->SetNormal(normal);
	this->SetMouseOver(mouseover);
	this->SetSelected(selected);
	this->SetDisabled(disabled);
	this->SetCallbackOnClick(func);
}

bool e2d::Button::IsEnable() const
{
	return enabled_;
}

void e2d::Button::SetNormal(Node * normal)
{
	SET_BUTTON_NODE(normal_, normal);
	if (normal)
	{
		this->SetSize(normal->GetWidth(), normal->GetHeight());
	}
}

void e2d::Button::SetMouseOver(Node * mouseover)
{
	SET_BUTTON_NODE(mouseover_, mouseover);
}

void e2d::Button::SetSelected(Node * selected)
{
	SET_BUTTON_NODE(selected_, selected);
}

void e2d::Button::SetDisabled(Node * disabled)
{
	SET_BUTTON_NODE(disabled_, disabled);
}

void e2d::Button::SetEnabled(bool enabled)
{
	if (enabled_ != enabled)
	{
		enabled_ = enabled;
		UpdateVisible();
	}
}

void e2d::Button::SetCallbackOnClick(const Function& func)
{
	callback_ = func;
}

void e2d::Button::SetAnchor(float anchor_x, float anchor_y)
{
	Node::SetAnchor(anchor_x, anchor_y);
	SAFE_SET(normal_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(mouseover_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(selected_, SetAnchor, anchor_x, anchor_y);
	SAFE_SET(disabled_, SetAnchor, anchor_x, anchor_y);
}

bool e2d::Button::Dispatch(const MouseEvent & e, bool handled)
{
	if (!handled && enabled_ && visible_ && normal_)
	{
		bool contains = normal_->ContainsPoint(e.GetPos());
		if (e.GetType() == MouseEvent::Type::LeftUp && is_selected_ && contains)
		{
			OnClick();
			is_selected_ = false;
			SetStatus(Status::Normal);
			return true;
		}
		else if (e.GetType() == MouseEvent::Type::LeftDown)
		{
			is_selected_ = contains;
			SetStatus(contains ? Status::Selected : Status::Normal);
			
			if (contains)
				return true;
		}
		else if (e.GetType() == MouseEvent::Type::LeftUp)
		{
			is_selected_ = false;
		}
		else if (e.GetType() == MouseEvent::Type::Move && is_selected_ && contains)
		{
			SetStatus(Status::Selected);
			return true;
		}
		else
		{
			if (!e.IsLButtonDown() && is_selected_)
			{
				is_selected_ = false;
			}

			SetStatus(contains ? Status::Mouseover : Status::Normal);

			if (contains)
				return true;
		}
	}

	return Node::Dispatch(e, handled);
}

void e2d::Button::Visit()
{
	Node::Visit();

	if (visible_ &&
		!enabled_ &&
		normal_ &&
		normal_->ContainsPoint(Input::GetInstance()->GetMousePos()))
	{
		Window::GetInstance()->SetCursor(Window::Cursor::No);
	}
	else if (status_ == Status::Mouseover || status_ == Status::Selected)
	{
		Window::GetInstance()->SetCursor(Window::Cursor::Hand);
	}
}

void e2d::Button::SetStatus(Status status)
{
	if (status_ != status)
	{
		status_ = status;
		UpdateVisible();
	}
}

void e2d::Button::UpdateVisible()
{
	SAFE_SET(normal_, SetVisible, false);
	SAFE_SET(mouseover_, SetVisible, false);
	SAFE_SET(selected_, SetVisible, false);
	SAFE_SET(disabled_, SetVisible, false);

	if (enabled_)
	{
		if (status_ == Status::Selected && selected_)
		{
			selected_->SetVisible(true);
		}
		else if (status_ == Status::Mouseover && mouseover_)
		{
			mouseover_->SetVisible(true);
		}
		else
		{
			if (normal_) normal_->SetVisible(true);
		}
	}
	else
	{
		if (disabled_)
		{
			disabled_->SetVisible(true);
		}
		else
		{
			if (normal_) normal_->SetVisible(true);
		}
	}
}

void e2d::Button::OnClick()
{
	if (callback_)
	{
		callback_();
	}
}