#include "..\e2dmodule.h"
#include "..\e2dobject.h"

e2d::Scene::Scene()
	: root_(nullptr)
	, border_visible_(false)
	, collider_visible_(false)
{
}

e2d::Scene::Scene(Node * root)
	: root_(nullptr)
	, border_visible_(false)
	, collider_visible_(false)
{
	this->SetRoot(root);
}

e2d::Scene::~Scene()
{
	if (root_)
	{
		root_->SetParentScene(nullptr);
		root_->Release();
	}
}

void e2d::Scene::SetRoot(Node * root)
{
	if (root_ == root)
		return;

	if (root_)
	{
		root_->SetParentScene(nullptr);
		root_->Release();
	}

	if (root)
	{
		root->Retain();
		root->SetParentScene(this);
	}

	root_ = root;
}

e2d::Node * e2d::Scene::GetRoot() const
{
	return root_;
}

void e2d::Scene::ShowBorder(bool visible)
{
	border_visible_ = visible;
}

void e2d::Scene::ShowCollider(bool visible)
{
	collider_visible_ = visible;
}

void e2d::Scene::Draw()
{
	if (root_)
	{
		root_->Visit();

		if (border_visible_)
		{
			Renderer::GetInstance()->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
			Renderer::GetInstance()->GetSolidBrush()->SetOpacity(1.f);
			root_->DrawBorder();
		}

		if (collider_visible_)
		{
			Renderer::GetInstance()->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
			root_->DrawCollider();
		}
	}
}

bool e2d::Scene::Dispatch(const MouseEvent & e, bool handled)
{
	if (root_)
	{
		return root_->Dispatch(e, handled);
	}
	return false;
}

bool e2d::Scene::Dispatch(const KeyEvent & e, bool handled)
{
	if (root_)
	{
		return root_->Dispatch(e, handled);
	}
	return false;
}
