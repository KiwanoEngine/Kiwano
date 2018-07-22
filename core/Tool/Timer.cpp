#include "..\e2dtool.h"

e2d::Timer * e2d::Timer::_instance = nullptr;

e2d::Timer * e2d::Timer::getInstance()
{
	if (!_instance)
		_instance = new (std::nothrow) Timer;
	return _instance;
}

void e2d::Timer::destroyInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

e2d::Timer::Timer()
	: _tasks()
{
}

e2d::Timer::~Timer()
{
}

void e2d::Timer::addTask(Task * task)
{
	if (task)
	{
		auto iter = std::find(_tasks.begin(), _tasks.end(), task);
		if (iter == _tasks.end())
		{
			task->retain();
			task->updateTime();
			_tasks.push_back(task);
		}
	}
}

void e2d::Timer::pauseTasks(const String& name)
{
	for (auto task : _tasks)
	{
		if (task->getName() == name)
		{
			task->pause();
		}
	}
}

void e2d::Timer::resumeTasks(const String& name)
{
	for (auto task : _tasks)
	{
		if (task->getName() == name)
		{
			task->resume();
		}
	}
}

void e2d::Timer::removeTasks(const String& name)
{
	for (auto task : _tasks)
	{
		if (task->getName() == name)
		{
			task->_stopped = true;
		}
	}
}

void e2d::Timer::pauseAllTasks()
{
	for (auto task : _tasks)
	{
		task->pause();
	}
}

void e2d::Timer::resumeAllTasks()
{
	for (auto task : _tasks)
	{
		task->resume();
	}
}

void e2d::Timer::removeAllTasks()
{
	for (auto task : _tasks)
	{
		task->_stopped = true;
	}
}

void e2d::Timer::clearAllTasks()
{
	if (_tasks.empty())
		return;

	for (auto task : _tasks)
	{
		task->release();
	}
	_tasks.clear();
}

void e2d::Timer::update()
{
	if (_tasks.empty() || Game::getInstance()->isPaused())
		return;

	for (size_t i = 0; i < _tasks.size();)
	{
		auto task = _tasks[i];
		// 清除已停止的任务
		if (task->_stopped)
		{
			task->release();
			_tasks.erase(_tasks.begin() + i);
		}
		else
		{
			// 更新定时器
			if (task->isReady())
			{
				task->update();
			}

			++i;
		}
	}
}

void e2d::Timer::updateTime()
{
	for (auto task : _tasks)
	{
		task->updateTime();
	}
}
