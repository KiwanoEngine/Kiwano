#include "..\e2dmodule.h"
#include "..\e2dnode.h"
#include "..\e2dmanager.h"

e2d::Scene::Scene()
	: border_visible_(false)
	, collider_visible_(false)
{
	parent_scene_ = this;
}

e2d::Scene::~Scene()
{
}

void e2d::Scene::ShowBorder(bool visible)
{
	border_visible_ = visible;
}

void e2d::Scene::ShowCollider(bool visible)
{
	collider_visible_ = visible;
}

void e2d::Scene::Visit()
{
	Node::Visit();

	if (border_visible_)
	{
		Renderer::GetInstance()->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		Renderer::GetInstance()->GetSolidBrush()->SetOpacity(1.f);
		this->DrawBorder();
	}

	if (collider_visible_)
	{
		Renderer::GetInstance()->GetRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		this->DrawCollider();
	}
}
