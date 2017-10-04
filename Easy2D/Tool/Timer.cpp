#include "..\easy2d.h"

// 储存所有定时器的容器
static std::vector<Timer*> s_nTimers;

Timer::Timer(TString name, UINT ms, const TIMER_CALLBACK & callback) :
	m_sName(name),
	m_bRunning(false),
	m_callback(callback)
{
	setInterval(ms);			// 设置定时器的时间间隔
}

Timer::~Timer()
{
}

void Timer::start()
{
	// 标志该定时器正在运行
	this->m_bRunning = true;
	// 记录当前时间
	QueryPerformanceCounter(&m_nLast);
}

void Timer::stop()
{
	this->m_bRunning = false;	// 标志该定时器已停止
}

bool Timer::isRunning()
{
	return m_bRunning;			// 获取该定时器的运行状态
}

void Timer::setInterval(UINT ms)
{
	// 设置定时器的时间间隔
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	m_nAnimationInterval.QuadPart = (LONGLONG)(ms / 1000.0 * nFreq.QuadPart);
	// 保存时间间隔的时长
	this->m_nMilliSeconds = ms;
}

void Timer::setCallback(const TIMER_CALLBACK & callback)
{
	m_callback = callback;		// 保存回调函数
}

void Timer::setName(TString name)
{
	m_sName = name;				// 修改定时器名称
}

UINT Timer::getInterval() const
{
	return m_nMilliSeconds;		// 获取定时器的时间间隔
}

TString Timer::getName() const
{
	return m_sName;				// 获取定时器的名称
}

void Timer::__exec()
{
	// 定时器容器为空
	if (!s_nTimers.size())
	{
		return;
	}
	// 获取当前时间
	static LARGE_INTEGER nNow;
	QueryPerformanceCounter(&nNow);
	// 循环遍历所有的定时器
	for (auto timer : s_nTimers)
	{
		// 若定时器未运行，跳过这个定时器
		if (!timer->m_bRunning) 
		{
			continue;
		}
		// 判断时间间隔是否足够
		if (nNow.QuadPart - timer->m_nLast.QuadPart > timer->m_nAnimationInterval.QuadPart)
		{
			// 用求余的方法重新记录时间
			timer->m_nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % timer->m_nAnimationInterval.QuadPart);
			// 运行回调函数
			timer->m_callback();
		}
	}
}

void Timer::addTimer(Timer * timer)
{
	// 启动定时器
	timer->start();
	// 将该定时器放入容器
	s_nTimers.push_back(timer);
}

void Timer::addTimer(TString name, UINT ms, const TIMER_CALLBACK & callback)
{
	// 创建定时器
	auto timer = new Timer(name, ms, callback);
	// 添加定时器
	addTimer(timer);
}

Timer * Timer::getTimer(TString name)
{
	// 查找是否有相同名称的定时器
	for (auto timer : s_nTimers)
	{
		if (timer->m_sName == name)
		{
			// 若找到，返回该定时器的指针
			return timer;
		}
	}
	// 若未找到，返回空指针
	return nullptr;
}

bool Timer::startTimer(TString name)
{
	// 启动指定名称的定时器，先找到该定时器
	auto t = getTimer(name);
	if (t)
	{
		// 启动定时器
		t->start();
		return true;
	}
	// 若未找到同样名称的定时器，返回 false
	return false;
}

bool Timer::stopTimer(TString name)
{
	// 停止指定名称的定时器，先找到该定时器
	auto t = getTimer(name);
	if (t)
	{
		// 停止定时器
		t->stop();
		return true;
	}
	// 若未找到同样名称的定时器，返回 false
	return false;
}

bool Timer::delTimer(TString name)
{
	// 创建迭代器
	std::vector<Timer*>::iterator iter;
	// 循环遍历所有定时器
	for (iter = s_nTimers.begin(); iter != s_nTimers.end(); iter++)
	{
		// 查找相同名称的定时器
		if ((*iter)->m_sName == name)
		{
			// 删除该定时器
			delete (*iter);
			s_nTimers.erase(iter);
			return true;
		}
	}
	// 若未找到同样名称的定时器，返回 false
	return false;
}

void Timer::clearAllTimers()
{
	// 删除所有定时器
	for (auto t : s_nTimers)
	{
		delete t;
	}
	// 清空容器
	s_nTimers.clear();
}
