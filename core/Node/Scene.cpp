#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dmanager.h"

e2d::Scene::Scene()
	: _borderVisible(false)
	, _colliderVisible(false)
{
	_parentScene = this;
}

e2d::Scene::~Scene()
{
}

void e2d::Scene::showBorder(bool visible)
{
	_borderVisible = visible;
}

void e2d::Scene::showCollider(bool visible)
{
	_colliderVisible = visible;
}

void e2d::Scene::visit()
{
	Node::visit();

	if (_borderVisible)
	{
		Renderer::getInstance()->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		Renderer::getInstance()->getSolidColorBrush()->SetOpacity(1.f);
		this->_drawBorder();
	}

	if (_colliderVisible)
	{
		Renderer::getInstance()->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		this->_drawCollider();
	}
}
