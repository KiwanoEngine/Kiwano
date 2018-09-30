#include "..\e2dmodule.h"
#include "..\e2dobject.h"

e2d::Scene::Scene()
	: root_(nullptr)
	, transform_(D2D1::Matrix3x2F::Identity())
{
}

e2d::Scene::Scene(Node * root)
	: root_(nullptr)
	, transform_(D2D1::Matrix3x2F::Identity())
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

void e2d::Scene::Draw()
{
	if (root_)
	{
		root_->Visit();
	}
}

void e2d::Scene::Dispatch(const MouseEvent & e)
{
	if (root_)
	{
		root_->Dispatch(e, false);
	}
}

void e2d::Scene::Dispatch(const KeyEvent & e)
{
	if (root_)
	{
		root_->Dispatch(e, false);
	}
}

void e2d::Scene::SetTransform(const D2D1::Matrix3x2F& matrix)
{
	transform_ = matrix;

	if (root_)
	{
		root_->dirty_transform_ = true;
	}
}

const D2D1::Matrix3x2F & e2d::Scene::GetTransform() const
{
	return transform_;
}
