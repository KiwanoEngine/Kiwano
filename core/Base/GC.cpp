#include "..\e2dbase.h"
#include "..\e2dtool.h"
#include "..\e2dmanager.h"

using namespace e2d;

e2d::autorelease_t const e2d::autorelease = e2d::autorelease_t();

void * operator new(size_t size, e2d::autorelease_t const &) E2D_NOEXCEPT
{
	void* p = ::operator new(size, std::nothrow);
	if (p)
	{
		GC::autorelease(static_cast<Ref*>(p));
	}
	return p;
}

void* operator new[](size_t size, e2d::autorelease_t const&) E2D_NOEXCEPT
{
	void* p = ::operator new[](size, std::nothrow);
	if (p)
	{
		GC::autoreleaseArray(static_cast<Ref*>(p));
	}
	return p;
}

void operator delete(void * block, e2d::autorelease_t const &) E2D_NOEXCEPT
{
	::operator delete (block, std::nothrow);
}

void operator delete[](void* block, e2d::autorelease_t const&) E2D_NOEXCEPT
{
	::operator delete[](block, std::nothrow);
}


// GC 机制，用于销毁所有单例
GC GC::_instance;

e2d::GC::GC()
	: _notifyed(false)
	, _pool()
{
}

e2d::GC::~GC()
{
	// 删除所有单例
	Game::destroyInstance();
	Renderer::destroyInstance();
	Input::destroyInstance();
	Window::destroyInstance();
	Timer::destroyInstance();
	Player::destroyInstance();
	SceneManager::destroyInstance();
	ActionManager::destroyInstance();
	InputManager::destroyInstance();
	CollisionManager::destroyInstance();
}


void e2d::GC::flush()
{
	if (!_instance._notifyed)
		return;

	_instance._notifyed = false;
	for (auto iter = _instance._pool.begin(); iter != _instance._pool.end();)
	{
		if ((*iter).first->getRefCount() <= 0)
		{
			if ((*iter).second)
			{
				delete[] (*iter).first;
			}
			else
			{
				delete (*iter).first;
			}
			iter = _instance._pool.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void e2d::GC::clear()
{
	for (auto pair : _instance._pool)
	{
		delete pair.first;
	}
	_instance._pool.clear();
}

void e2d::GC::autorelease(Ref * ref)
{
	if (ref)
	{
		auto iter = _instance._pool.find(ref);
		if (iter == _instance._pool.end())
		{
			_instance._pool.insert(std::make_pair(ref, false));
		}
	}
}

void e2d::GC::autoreleaseArray(Ref * ref)
{
	if (ref)
	{
		auto iter = _instance._pool.find(ref);
		if (iter == _instance._pool.end())
		{
			_instance._pool.insert(std::make_pair(ref, true));
		}
	}
}

void e2d::GC::retain(Ref * ref)
{
	if (ref)
	{
		auto iter = _instance._pool.find(ref);
		if (iter != _instance._pool.end())
		{
			(*iter).first->retain();
		}
	}
}

void e2d::GC::release(Ref * ref)
{
	if (ref)
	{
		auto iter = _instance._pool.find(ref);
		if (iter != _instance._pool.end())
		{
			(*iter).first->release();
			_instance._notifyed = true;
		}
	}
}
