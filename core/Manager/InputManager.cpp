#include "..\e2dmanager.h"
#include "..\e2dtool.h"

// ¼àÌıÆ÷
class Listener
{
public:
	Listener(
		e2d::Function func,
		e2d::String name,
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


void e2d::InputManager::add(Function func, String name, bool paused)
{
	auto listener = new Listener(func, name, paused);
	s_vListeners.push_back(listener);
}

void e2d::InputManager::pause(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->name == name)
		{
			pListener->running = false;
		}
	}
}

void e2d::InputManager::resume(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->name == name)
		{
			pListener->running = true;
		}
	}
}

void e2d::InputManager::stop(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->name == name)
		{
			pListener->stopped = true;
		}
	}
}

void e2d::InputManager::pauseAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->running = false;
	}
}

void e2d::InputManager::resumeAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->running = true;
	}
}

void e2d::InputManager::stopAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->stopped = true;
	}
}

void e2d::InputManager::__update()
{
	if (s_vListeners.empty() || Game::isPaused())
		return;

	for (size_t i = 0; i < s_vListeners.size(); i++)
	{
		auto pListener = s_vListeners[i];
		// Çå³ıÒÑÍ£Ö¹µÄ¼àÌıÆ÷
		if (pListener->stopped)
		{
			delete pListener;
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else
		{
			// ¸üĞÂ¼àÌıÆ÷
			pListener->update();
			++i;
		}
	}
}

void e2d::InputManager::__uninit()
{
	FOR_LOOP(listener, s_vListeners)
	{
		delete listener;
	}
	s_vListeners.clear();
}