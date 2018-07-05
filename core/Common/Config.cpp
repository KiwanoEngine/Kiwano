#include "..\e2dbase.h"
#include "..\e2dtool.h"

e2d::Config::Config()
	: _gameName()
	, _nodeDefPivot()
	, _soundEnabled(true)
	, _collisionEnabled(false)
	, _colliderVisiable(false)
	, _objectsAutoRelease(false)
	, _nodeDefColliderType(Collider::Type::None)
	, _unconfigured(true)
{
}

e2d::Config::~Config()
{
}

void e2d::Config::setGameName(const String & name)
{
	_gameName = name;
}

void e2d::Config::setSoundEnabled(bool enabled)
{
	if (_soundEnabled != enabled)
	{
		_soundEnabled = enabled;
		_unconfigured = true;
	}
}

void e2d::Config::setObjectsAutoReleaseEnabled(bool enabled)
{
	_objectsAutoRelease = enabled;
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

void e2d::Config::setDefaultColliderType(Collider::Type type)
{
	_nodeDefColliderType = type;
}

void e2d::Config::setColliderVisiable(bool visiable)
{
	_colliderVisiable = visiable;
}

e2d::String e2d::Config::getGameName() const
{
	return _gameName;
}

bool e2d::Config::isSoundEnabled() const
{
	return _soundEnabled;
}

bool e2d::Config::isObjectsAutoReleaseEnabled() const
{
	return _objectsAutoRelease;
}

bool e2d::Config::isCollisionEnabled() const
{
	return _collisionEnabled;
}

e2d::Point e2d::Config::getNodeDefaultPivot() const
{
	return _nodeDefPivot;
}

e2d::Collider::Type e2d::Config::getDefaultColliderType() const
{
	return _nodeDefColliderType;
}

bool e2d::Config::isColliderVisiable() const
{
	return _colliderVisiable;
}

void e2d::Config::_update()
{
	_unconfigured = false;

	if (_soundEnabled)
	{
		Player::getInstance()->getXAudio2()->StartEngine();
	}
	else
	{
		Player::getInstance()->getXAudio2()->StopEngine();
	}
}
