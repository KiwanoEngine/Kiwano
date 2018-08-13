#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dmanager.h"

e2d::Scene::Scene()
{
}

e2d::Scene::~Scene()
{
}

void e2d::Scene::visit()
{
	Node::visit();

	if (Game::getInstance()->getConfig().isOutlineVisible())
	{
		auto brush = Renderer::getInstance()->getSolidColorBrush();
		brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, 0.6f));
		brush->SetOpacity(1.f);
		Node::_renderOutline();
	}

	if (Game::getInstance()->getConfig().isColliderVisible())
	{
		Renderer::getInstance()->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		Node::_renderCollider();
	}
}
