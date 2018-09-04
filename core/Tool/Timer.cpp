#include "..\e2dtool.h"


e2d::Timer * e2d::Timer::GetInstance()
{
	static Timer instance;
	return &instance;
}

e2d::Timer::Timer()
	: tasks_()
{
}

e2d::Timer::~Timer()
{
}

void e2d::Timer::AddTask(Task * task)
{
	if (task)
	{
		auto iter = std::find(tasks_.begin(), tasks_.end(), task);
		if (iter == tasks_.end())
		{
			task->Retain();
			task->last_time_ = Time::Now();
			tasks_.push_back(task);
		}
	}
}

void e2d::Timer::StopTasks(const String& name)
{
	for (const auto& task : tasks_)
	{
		if (task->GetName() == name)
		{
			task->Stop();
		}
	}
}

void e2d::Timer::StartTasks(const String& name)
{
	for (const auto& task : tasks_)
	{
		if (task->GetName() == name)
		{
			task->Start();
		}
	}
}

void e2d::Timer::RemoveTasks(const String& name)
{
	for (const auto& task : tasks_)
	{
		if (task->GetName() == name)
		{
			task->stopped_ = true;
		}
	}
}

void e2d::Timer::StopAllTasks()
{
	for (const auto& task : tasks_)
	{
		task->Stop();
	}
}

void e2d::Timer::StartAllTasks()
{
	for (const auto& task : tasks_)
	{
		task->Start();
	}
}

void e2d::Timer::RemoveAllTasks()
{
	for (const auto& task : tasks_)
	{
		task->stopped_ = true;
	}
}

void e2d::Timer::ClearAllTasks()
{
	if (tasks_.empty())
		return;

	for (const auto& task : tasks_)
	{
		task->Release();
	}
	tasks_.clear();
}

void e2d::Timer::Update()
{
	if (tasks_.empty())
		return;

	std::vector<Task*> currTasks;
	currTasks.reserve(tasks_.size());
	std::copy_if(
		tasks_.begin(),
		tasks_.end(),
		std::back_inserter(currTasks),
		[](Task* task) { return task->IsReady() && !task->stopped_; }
	);

	// 遍历就绪的任务
	for (const auto& task : currTasks)
		task->Update();

	// 清除结束的任务
	for (auto iter = tasks_.begin(); iter != tasks_.end();)
	{
		if ((*iter)->stopped_)
		{
			(*iter)->Release();
			iter = tasks_.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void e2d::Timer::UpdateTime()
{
	for (const auto& task : tasks_)
	{
		task->last_time_ = Time::Now();
	}
}
