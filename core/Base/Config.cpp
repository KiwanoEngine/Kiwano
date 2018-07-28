#include "..\e2dbase.h"
#include "..\e2dtool.h"

e2d::Config::Config()
	: _gameName()
	, _soundEnabled(true)
	, _frameInterval(15)
	, _showFps(false)
	, _vSyncEnabled(true)
	, _outlineVisible(false)
	, _collisionEnabled(false)
	, _colliderVisible(false)
{
}

e2d::Config::~Config()
{
}

void e2d::Config::setGameName(const String & name)
{
	_gameName = name;
}

void e2d::Config::showFps(bool show)
{
	_showFps = show;
}

void e2d::Config::setVSyncEnabled(bool enabled)
{
	_vSyncEnabled = enabled;
}

void e2d::Config::setFrameInterval(int interval)
{
	_frameInterval = interval;
}

void e2d::Config::setOutlineVisible(bool visible)
{
	_outlineVisible = visible;
}

void e2d::Config::setSoundEnabled(bool enabled)
{
	_soundEnabled = enabled;
}

void e2d::Config::setCollisionEnabled(bool enabled)
{
	_collisionEnabled = enabled;
}

void e2d::Config::setColliderVisible(bool visible)
{
	_colliderVisible = visible;
}

e2d::String e2d::Config::getGameName() const
{
	return _gameName;
}

bool e2d::Config::isSoundEnabled() const
{
	return _soundEnabled;
}

bool e2d::Config::isVSyncEnabled() const
{
	return _vSyncEnabled;
}

bool e2d::Config::isFpsShow() const
{
	return _showFps;
}

int e2d::Config::getFrameInterval() const
{
	return _frameInterval;
}

bool e2d::Config::isOutlineVisible() const
{
	return _outlineVisible;
}

bool e2d::Config::isCollisionEnabled() const
{
	return _collisionEnabled;
}

bool e2d::Config::isColliderVisible() const
{
	return _colliderVisible;
}
