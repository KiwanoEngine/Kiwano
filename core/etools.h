#pragma once
#include "ebase.h"
#include <random>

namespace e2d
{

class ETimerManager;
class EAction;

// 随机数产生器
class ERandom
{
public:
	// 取得整型范围内的一个随机数
	template<typename T>
	static inline T range(T min, T max) { return e2d::ERandom::randomInt(min, max); }

	// 取得浮点数范围内的一个随机数
	static inline float range(float min, float max) { return e2d::ERandom::randomReal(min, max); }

	// 取得浮点数范围内的一个随机数
	static inline double range(double min, double max) { return e2d::ERandom::randomReal(min, max); }

	// 取得浮点数范围内的一个随机数
	static inline long double range(long double min, long double max) { return e2d::ERandom::randomReal(min, max); }

	// 取得整型范围内的一个随机数
	template<typename T>
	static T randomInt(
		T min,
		T max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(getEngine());
	}

	// 取得浮点数类型范围内的一个随机数
	template<typename T>
	static T randomReal(
		T min,
		T max)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(getEngine());
	}

	// 获取随机数产生器
	static std::default_random_engine &getEngine();
};


// 定时器
class ETimer :
	public EObject
{
	friend ETimerManager;

public:
	ETimer();

	ETimer(
		const TimerCallback &callback,	/* 定时器回调函数 */
		int repeatTimes = -1,			/* 定时器执行次数 */
		float interval = 0LL,			/* 时间间隔（秒） */
		bool atOnce = false				/* 是否立即执行 */
	);

	ETimer(
		const EString &name,			/* 定时器名称 */
		const TimerCallback &callback,	/* 定时器回调函数 */
		int repeatTimes = -1,			/* 定时器执行次数 */
		float interval = 0,				/* 时间间隔（秒） */
		bool atOnce = false				/* 是否立即执行 */
	);

	// 获取定时器状态
	bool isRunning() const;

	// 启动定时器
	void start();

	// 停止定时器
	void stop();

	// 获取定时器名称
	EString getName() const;

	// 获取定时器所在节点
	ENode * getParentNode() const;

	// 设置定时器名称
	void setName(
		const EString &name
	);

	// 设置定时器执行间隔（秒）
	void setInterval(
		float interval
	);

	// 设置定时器回调函数
	void setCallback(
		const TimerCallback & callback
	);

	// 设置定时器重复执行次数
	void setRepeatTimes(
		int nRepeatTimes
	);

	// 设置定时器在绑定后立即执行一次
	virtual void setRunAtOnce(
		bool bAtOnce
	);

	// 绑定定时器到场景
	virtual void bindWith(
		EScene * pParentScene
	);

	// 绑定定时器到节点
	virtual void bindWith(
		ENode * pParentNode
	);

protected:
	// 执行回调函数
	virtual void _callOn();

	// 判断是否达到执行状态
	bool _isReady();

protected:
	EString			m_sName;
	bool			m_bRunning;
	bool			m_bAtOnce;
	int				m_nRunTimes;
	int				m_nRepeatTimes;
	float			m_fInterval;
	float			m_fLast;
	ENode *			m_pParentNode;
	TimerCallback	m_Callback;
};


// 数据管理工具
class EData
{
public:
	// 保存 int 类型的值
	static void saveInt(
		const EString & key,
		int value
	);

	// 保存 float 类型的值
	static void saveFloat(
		const EString & key,
		float value
	);

	// 保存 字符串 类型的值
	static void saveString(
		const EString & key,
		const EString & value
	);

	// 获取 int 类型的值
	// （若不存在则返回 defaultValue 参数的值）
	static int getInt(
		const EString & key,
		int defaultValue
	);

	// 获取 float 类型的值
	// （若不存在则返回 defaultValue 参数的值）
	static float getFloat(
		const EString & key,
		float defaultValue
	);

	// 获取 字符串 类型的值
	// （若不存在则返回 defaultValue 参数的值）
	static EString getString(
		const EString & key,
		const EString & defaultValue
	);
};


// 文件管理工具
class EFile
{
public:
	// 获取系统的 AppData Local 路径
	static EString getLocalAppDataPath();

	// 获取临时文件目录
	static EString getTempPath();

	// 获取默认的保存路径
	static EString getDefaultSavePath();

	// 获取文件扩展名
	static EString getFileExtension(
		const EString & filePath
	);

	// 打开保存文件对话框
	static EString getSaveFilePath(
		const EString & title = L"保存到",	/* 对话框标题 */
		const EString & defExt = L""		/* 默认扩展名 */
	);
};


// 音乐管理工具
class EMusic
{
public:
	// 播放音乐
	static UINT play(
		const EString & musicFilePath,		/* 音乐文件路径 */
		int repeatTimes = 1
	);

	// 播放音乐
	static UINT play(
		const EString & musicResourceName,	/* 资源名称 */
		const EString & musicResourceType,	/* 资源类别 */
		const EString & musicExtension,		/* 指定资源的扩展名 */
		int repeatTimes = 1
	);

	// 暂停音乐
	static bool pause(
		UINT musicId
	);

	// 暂停音乐
	static bool pause(
		const EString& musicName
	);

	// 继续播放音乐
	static bool resume(
		UINT musicId
	);

	// 继续播放音乐
	static bool resume(
		const EString& musicName
	);

	// 停止音乐
	static bool stop(
		UINT musicId
	);

	// 停止音乐
	static bool stop(
		const EString& musicName
	);

	// 预加载音乐
	static UINT preload(
		const EString & musicFilePath
	);

	// 预加载音乐
	static UINT preload(
		const EString & musicResourceName,	/* 资源名称 */
		const EString & musicResourceType,	/* 资源类别 */
		const EString & musicExtension		/* 指定资源的扩展名 */
	);

	// 暂停所有音乐
	static void pauseAllMusics();

	// 继续播放所有音乐
	static void resumeAllMusics();

	// 停止所有音乐
	static void stopAllMusics();
};

}