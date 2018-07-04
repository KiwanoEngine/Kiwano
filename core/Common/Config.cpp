#include "..\e2dbase.h"


e2d::Config::Config()
	: _gameName()
	, _nodeDefPivot()
	, _collisionEnabled(false)
{
}

e2d::Config::~Config()
{
}

void e2d::Config::setGameName(const String & name)
{
	_gameName = name;
}

void e2d::Config::setCollisionEnabled(bool enabled)
{
	_collisionEnabled = enabled;
}

void e2d::Config::setNodeDefaultPivot(Point pivot)
{
	_nodeDefPivot = Point(
		std::min(std::max(pivot.x, 0.0), 1.0),
		std::min(std::max(pivot.y, 0.0), 1.0)
	);
}

e2d::String e2d::Config::getGameName() const
{
	return _gameName;
}

bool e2d::Config::isCollisionEnabled() const
{
	return _collisionEnabled;
}

e2d::Point e2d::Config::getNodeDefaultPivot() const
{
	return _nodeDefPivot;
}
