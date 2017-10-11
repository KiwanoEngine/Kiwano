#include "..\easy2d.h"
#include "..\Win\winbase.h"

// 储存所有定时器的容器
static std::vector<Timer*> s_vTimers;

Timer::Timer(TString name, LONGLONG milliSeconds, const TIMER_CALLBACK & callback) :
	m_sName(name),
	m_bRunning(false),
	m_bWaiting(false),
	m_callback(callback),
	m_pParentScene(nullptr)
{
	setInterval(milliSeconds);		// 设置定时器的时间间隔
}

Timer::~Timer()
{
}

void Timer::start()
{
	// 标志该定时器正在运行
	m_bRunning = true;
	// 记录当前时间
	m_nLast = steady_clock::now();
}

void Timer::stop()
{
	m_bRunning = false;	// 标志该定时器已停止
}

void Timer::wait()
{
	m_bWaiting = true;
}

void Timer::notify()
{
	m_bWaiting = false;
}

bool Timer::isRunning()
{
	return m_bRunning && !m_bWaiting;			// 获取该定时器的运行状态
}

void Timer::setInterval(LONGLONG milliSeconds)
{
	// 设置定时器的时间间隔
	m_nAnimationInterval = milliSeconds;
}

void Timer::setCallback(const TIMER_CALLBACK & callback)
{
	m_callback = callback;		// 保存回调函数
}

void Timer::setName(TString name)
{
	m_sName = name;				// 修改定时器名称
}

LONGLONG Timer::getInterval() const
{
	return m_nAnimationInterval;// 获取定时器的时间间隔
}

TString Timer::getName() const
{
	return m_sName;				// 获取定时器的名称
}

void Timer::__exec()
{
	// 定时器容器为空
	if (!s_vTimers.size())
	{
		return;
	}
	// 循环遍历所有的定时器
	for (auto timer : s_vTimers)
	{
		// 若定时器未运行，跳过这个定时器
		if (!timer->isRunning()) 
		{
			continue;
		}
		// 判断时间间隔是否足够
		while (duration_cast<milliseconds>(GetNow() - timer->m_nLast).count() > timer->m_nAnimationInterval)
		{
			// 重新记录时间
			timer->m_nLast += milliseconds(timer->m_nAnimationInterval);
			// 运行回调函数
			timer->m_callback();
		}
	}
}

void Timer::addTimer(Timer * timer)
{
	// 启动定时器
	timer->start();
	// 绑定在场景上
	timer->m_pParentScene = EApp::getLoadingScene();
	// 将该定时器放入容器
	s_vTimers.push_back(timer);
}

void Timer::addTimer(TString name, const TIMER_CALLBACK & callback)
{
	addTimer(name, 20, callback);
}

void Timer::addTimer(TString name, LONGLONG milliSeconds, const TIMER_CALLBACK & callback)
{
	// 创建定时器
	auto timer = new Timer(name, milliSeconds, callback);
	// 添加定时器
	addTimer(timer);
}

void Timer::startTimer(TString name)
{
	// 查找名称相同的定时器
	for (auto timer : s_vTimers)
	{
		if (timer->m_sName == name && timer->m_pParentScene == EApp::getCurrentScene())
		{
			// 启动定时器
			timer->start();
		}
	}
}

void Timer::stopTimer(TString name)
{
	// 查找名称相同的定时器
	for (auto timer : s_vTimers)
	{
		if (timer->m_sName == name && timer->m_pParentScene == EApp::getCurrentScene())
		{
			// 停止定时器
			timer->stop();
		}
	}
}

void Timer::delTimer(TString name)
{
	// 创建迭代器
	std::vector<Timer*>::iterator iter;
	// 循环遍历所有定时器
	for (iter = s_vTimers.begin(); iter != s_vTimers.end();)
	{
		// 查找相同名称的定时器
		if ((*iter)->m_sName == name && (*iter)->m_pParentScene == EApp::getCurrentScene())
		{
			// 删除该定时器
			delete (*iter);
			iter = s_vTimers.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void Timer::clearAllTimers()
{
	// 删除所有定时器
	for (auto t : s_vTimers)
	{
		delete t;
	}
	// 清空容器
	s_vTimers.clear();
}

void Timer::notifyAllSceneTimers(Scene * scene)
{
	for (auto t : s_vTimers)
	{
		if (t->m_pParentScene == scene)
		{
			t->notify();
		}
	}
}

void Timer::waitAllSceneTimers(Scene * scene)
{
	for (auto t : s_vTimers)
	{
		if (t->m_pParentScene == scene)
		{
			t->wait();
		}
	}
}

void Timer::clearAllSceneTimers(Scene * scene)
{
	// 创建迭代器
	std::vector<Timer*>::iterator iter;
	// 循环遍历所有定时器
	for (iter = s_vTimers.begin(); iter != s_vTimers.end();)
	{
		// 查找相同名称的定时器
		if ((*iter)->m_pParentScene == scene)
		{
			// 删除该定时器
			delete (*iter);
			iter = s_vTimers.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}
