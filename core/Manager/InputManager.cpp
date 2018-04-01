#include "..\emanagers.h"
#include "..\etools.h"

// ¼àÌıÆ÷ÈİÆ÷
static std::vector<e2d::InputListener*> s_vListeners;


void e2d::InputManager::__update()
{
	for (size_t i = 0; i < s_vListeners.size(); i++)
	{
		auto pListener = s_vListeners[i];
		// ¸üĞÂ¼àÌıÆ÷
		if (pListener->m_bClear)
		{
			pListener->release();
			s_vListeners.erase(s_vListeners.begin() + i);
		}
		else if (pListener->isRunning())
		{
			pListener->_update();
		}
	}
}

void e2d::InputManager::__add(InputListener * pListener)
{
	WARN_IF(pListener == nullptr, "InputListener NULL pointer exception!");

	if (pListener)
	{
		auto findListener = [](InputListener * pListener) -> bool
		{
			FOR_LOOP(l, s_vListeners)
			{
				if (pListener == l)
				{
					return true;
				}
			}
			return false;
		};

		bool bHasListener = findListener(pListener);
		WARN_IF(bHasListener, "The InputListener is already added, cannot be added again!");

		if (!bHasListener)
		{
			pListener->retain();
			s_vListeners.push_back(pListener);
		}
	}
}

void e2d::InputManager::add(Function func, String name)
{
	(new InputListener(func, name))->start();
}

void e2d::InputManager::start(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->getName() == name)
		{
			pListener->start();
		}
	}
}

void e2d::InputManager::stop(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->getName() == name)
		{
			pListener->stop();
		}
	}
}

void e2d::InputManager::clear(String name)
{
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->getName() == name)
		{
			pListener->stopAndClear();
		}
	}
}

void e2d::InputManager::startAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->start();
	}
}

void e2d::InputManager::stopAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->stop();
	}
}

void e2d::InputManager::clearAll()
{
	FOR_LOOP(pListener, s_vListeners)
	{
		pListener->stopAndClear();
	}
}

std::vector<e2d::InputListener*> e2d::InputManager::get(String name)
{
	std::vector<InputListener*> vListeners;
	FOR_LOOP(pListener, s_vListeners)
	{
		if (pListener->getName() == name)
		{
			vListeners.push_back(pListener);
		}
	}
	return std::move(vListeners);
}

std::vector<e2d::InputListener*> e2d::InputManager::getAll()
{
	return s_vListeners;
}