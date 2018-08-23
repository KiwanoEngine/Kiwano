#include "..\e2dtool.h"


e2d::Timer * e2d::Timer::instance()
{
	static Timer instance;
	return &instance;
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
			task->_lastTime = Time::now();
			_tasks.push_back(task);
		}
	}
}

void e2d::Timer::stopTasks(const String& name)
{
	for (const auto& task : _tasks)
	{
		if (task->name() == name)
		{
			task->stop();
		}
	}
}

void e2d::Timer::startTasks(const String& name)
{
	for (const auto& task : _tasks)
	{
		if (task->name() == name)
		{
			task->start();
		}
	}
}

void e2d::Timer::removeTasks(const String& name)
{
	for (const auto& task : _tasks)
	{
		if (task->name() == name)
		{
			task->_stopped = true;
		}
	}
}

void e2d::Timer::stopAllTasks()
{
	for (const auto& task : _tasks)
	{
		task->stop();
	}
}

void e2d::Timer::startAllTasks()
{
	for (const auto& task : _tasks)
	{
		task->start();
	}
}

void e2d::Timer::removeAllTasks()
{
	for (const auto& task : _tasks)
	{
		task->_stopped = true;
	}
}

void e2d::Timer::clearAllTasks()
{
	if (_tasks.empty())
		return;

	for (const auto& task : _tasks)
	{
		task->release();
	}
	_tasks.clear();
}

void e2d::Timer::update()
{
	if (_tasks.empty())
		return;

	std::vector<Task*> currTasks;
	currTasks.reserve(_tasks.size());
	std::copy_if(
		_tasks.begin(),
		_tasks.end(),
		std::back_inserter(currTasks),
		[](Task* task) { return task->_isReady() && !task->_stopped; }
	);

	// 遍历就绪的任务
	for (const auto& task : currTasks)
		task->_update();

	// 清除结束的任务
	for (auto iter = _tasks.begin(); iter != _tasks.end();)
	{
		if ((*iter)->_stopped)
		{
			(*iter)->release();
			iter = _tasks.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void e2d::Timer::updateTime()
{
	for (const auto& task : _tasks)
	{
		task->_lastTime = Time::now();
	}
}
