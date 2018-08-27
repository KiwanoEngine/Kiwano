#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dmanager.h"

e2d::Scene::Scene()
	: _outlineVisible(false)
	, _colliderVisible(false)
{
	_parentScene = this;
}

e2d::Scene::~Scene()
{
}

void e2d::Scene::setOutlineVisible(bool visible)
{
	_outlineVisible = visible;
}

void e2d::Scene::setColliderVisible(bool visible)
{
	_colliderVisible = visible;
}

void e2d::Scene::visit()
{
	Node::visit();

	if (_outlineVisible)
	{
		auto brush = Renderer::getInstance()->getSolidColorBrush();
		brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, 0.6f));
		brush->SetOpacity(1.f);
		this->drawOutline();
	}

	if (_colliderVisible)
	{
		Renderer::getInstance()->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		this->drawCollider();
	}
}
