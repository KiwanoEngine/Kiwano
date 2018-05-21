#include "..\e2dmanager.h"
#include "..\e2dtool.h"

// ¼àÌıÆ÷
class Listener
{
public:
	Listener(
		const e2d::Function& func,
		const e2d::String& name,
		bool paused
	)
		: name(name)
		, callback(func)
		, running(!paused)
		, stopped(false)
	{
	}

	// ¸üĞÂ¼àÌıÆ÷×´Ì¬
	virtual void update()
	{
		if (callback)
		{
			callback();
		}
	}

public:
	bool running;
	bool stopped;
	e2d::String name;
	e2d::Function callback;
};

// ¼àÌıÆ÷ÈİÆ÷
static std::vector<Listener*> s_vListeners;


void e2d::InputManager::add(const Function& func, const String& name, bool paused)
{
	auto listener = new (std::nothrow) Listener(func, name, paused);
	s_vListeners.push_back(listener);
}

void e2d::InputManager::pause(const String& name)
{
	if (s_vListeners.empty() || name.isEmpty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->name == name)
		{
			listener->running = false;
		}
	}
}

void e2d::InputManager::resume(const String& name)
{
	if (s_vListeners.empty() || name.isEmpty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->name == name)
		{
			listener->running = true;
		}
	}
}

void e2d::InputManager::stop(const String& name)
{
	if (s_vListeners.empty() || name.isEmpty())
		return;

	for (auto listener : s_vListeners)
	{
		if (listener->name == name)
		{
			listener->stopped = true;
		}
	}
}

void e2d::InputManager::pauseAll()
{
	for (auto listener : s_vListeners)
	{
		listener->running = false;
	}
}

void e2d::InputManager::resumeAll()
{
	for (auto listener : s_vListeners)
	{
		listener->running = true;
	}
}

void e2d::InputManager::stopAll()
{
	for (auto listener : s_vListeners)
	{
		listener->stopped = true;
	}
}

void e2d::InputManager::__update()
{
	if (s_vListeners.empty() || Game::isPaused())
		return;

	for (size_t i = 0; i < s_vListeners.size(); ++i)
	{
		auto listener = s_vListeners[i];
		// Çå³ıÒÑÍ£Ö¹µÄ¼àÌıÆ÷
		if (listener->stopped)
		{
			delete listener;
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// ¸üĞÂ¼àÌıÆ÷
			listener->update();
			++i;
		}
	}
}

void e2d::InputManager::__uninit()
{
	for (auto listener : s_vListeners)
	{
		delete listener;
	}
	s_vListeners.clear();
}