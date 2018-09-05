#include "..\e2dmodule.h"
#include "..\e2dtool.h"
#include "..\e2dmanager.h"

using namespace e2d;

e2d::autorelease_t const e2d::autorelease = e2d::autorelease_t();

void * operator new(size_t size, e2d::autorelease_t const &) E2D_NOEXCEPT
{
	void* p = ::operator new(size, std::nothrow);
	if (p)
	{
		GC::GetInstance()->AutoRelease(static_cast<Ref*>(p));
	}
	return p;
}

void operator delete(void * block, e2d::autorelease_t const &) E2D_NOEXCEPT
{
	::operator delete (block, std::nothrow);
}


e2d::GC * e2d::GC::GetInstance()
{
	static GC instance_;
	return &instance_;
}

e2d::GC::GC()
	: notifyed_(false)
	, cleanup_(false)
	, pool_()
{
}

e2d::GC::~GC()
{
	// 删除所有对象
	cleanup_ = true;
	for (const auto& ref : pool_)
	{
		delete ref;
	}
	pool_.clear();
	cleanup_ = false;

	// 清除缓存
	Image::ClearCache();

	// 清除单例
	Player::DestroyInstance();
	Audio::DestroyInstance();
	Renderer::DestroyInstance();
	Input::DestroyInstance();
	Window::DestroyInstance();
	Game::DestroyInstance();
}

void e2d::GC::Flush()
{
	if (!notifyed_)
		return;

	notifyed_ = false;
	for (auto iter = pool_.begin(); iter != pool_.end();)
	{
		if ((*iter)->GetRefCount() <= 0)
		{
			delete (*iter);
			iter = pool_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void e2d::GC::AutoRelease(Ref * ref)
{
	if (ref)
	{
		pool_.insert(ref);
	}
}

void e2d::GC::SafeRelease(Ref* ref)
{
	if (cleanup_)
		return;

	if (ref)
	{
		ref->Release();
		notifyed_ = true;
	}
}
