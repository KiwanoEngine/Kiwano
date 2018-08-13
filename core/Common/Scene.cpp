#include "..\e2dbase.h"
#include "..\e2dnode.h"
#include "..\e2dmanager.h"

e2d::Scene::Scene()
	: _root(nullptr)
{
	_root = new (e2d::autorelease) Node();
	_root->retain();
	_root->_setParentScene(this);
}

e2d::Scene::~Scene()
{
	GC::getInstance()->safeRelease(_root);
}

void e2d::Scene::render()
{
	_root->_render();

	if (Game::getInstance()->getConfig().isOutlineVisible())
	{
		auto brush = Renderer::getInstance()->getSolidColorBrush();
		brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, 0.6f));
		brush->SetOpacity(1.f);
		_root->_renderOutline();
	}

	if (Game::getInstance()->getConfig().isColliderVisible())
	{
		Renderer::getInstance()->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		_root->_renderCollider();
	}
}

void e2d::Scene::dispatch(const MouseEvent & e)
{
	auto dispatcher = dynamic_cast<EventHandler*>(this);
	if (dispatcher)
		dispatcher->handle(e);

	_root->dispatch(e, false);
}

void e2d::Scene::dispatch(const KeyEvent & e)
{
	auto dispatcher = dynamic_cast<EventHandler*>(this);
	if (dispatcher)
		dispatcher->handle(e);

	_root->dispatch(e, false);
}

void e2d::Scene::add(Node * child, int order /* = 0 */)
{
	_root->addChild(child, order);
}

void e2d::Scene::add(const std::vector<Node*>& nodes, int order)
{
	for (const auto& node : nodes)
	{
		this->add(node, order);
	}
}

bool e2d::Scene::remove(Node * child)
{
	return _root->removeChild(child);
}

std::vector<e2d::Node*> e2d::Scene::getChildren(const String& name) const
{
	return _root->getChildren(name);
}

e2d::Node * e2d::Scene::getChild(const String& name) const
{
	return _root->getChild(name);
}

const std::vector<e2d::Node*>& e2d::Scene::getAllChildren() const
{
	return _root->getAllChildren();
}

e2d::Node * e2d::Scene::getRoot() const
{
	return _root;
}
