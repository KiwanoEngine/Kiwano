#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dmanager.h"

e2d::Scene::Scene()
	: _outlineVisible(false)
	, _colliderVisible(false)
{
}

e2d::Scene::~Scene()
{
}

e2d::Scene& e2d::Scene::outlineVisible(bool visible)
{
	_outlineVisible = visible;
	return *this;
}

e2d::Scene& e2d::Scene::colliderVisible(bool visible)
{
	_colliderVisible = visible;
	return *this;
}

void e2d::Scene::visit(Game * game)
{
	Node::visit(game);

	if (_outlineVisible)
	{
		auto brush = game->renderer()->solidBrush();
		brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, 0.6f));
		brush->SetOpacity(1.f);
		this->drawOutline(game->renderer());
	}
	if (_colliderVisible)
	{
		game->renderer()->renderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		this->drawCollider();
	}
}
