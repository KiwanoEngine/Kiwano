#pragma once
#include "ebase.h"
#include <chrono>
#include <random>

namespace e2d
{

class ETimerManager;
class EAction;


// 定时器
class ETimer :
	public EObject
{
	friend ETimerManager;

public:
	ETimer();

	ETimer(
		const TIMER_CALLBACK &callback,	/* 定时器回调函数 */
		int repeatTimes = -1,			/* 定时器执行次数 */
		LONGLONG interval = 0LL,		/* 时间间隔（毫秒） */
		bool atOnce = false				/* 是否立即执行 */
	);

	ETimer(
		const EString &name,			/* 定时器名称 */
		const TIMER_CALLBACK &callback,	/* 定时器回调函数 */
		int repeatTimes = -1,			/* 定时器执行次数 */
		LONGLONG interval = 0LL,		/* 时间间隔（毫秒） */
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

	// 设置定时器执行间隔
	void setInterval(
		LONGLONG interval
	);

	// 设置定时器回调函数
	void setCallback(
		const TIMER_CALLBACK & callback
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
	ENode *			m_pParentNode;
	TIMER_CALLBACK	m_Callback;
	LONGLONG		m_nInterval;
	std::chrono::steady_clock::time_point m_tLast;
};


class EFileUtils
{
public:
	// 获取系统的 AppData Local 路径
	static EString getLocalAppDataPath();

	// 获取临时文件目录
	static EString getTempPath();

	// 获取默认的保存路径
	static EString getDefaultSavePath();

	// 保存 int 型的值
	static void saveInt(
		const EString & key,
		int value
	);

	// 保存 float 型的值
	static void saveFloat(
		const EString & key,
		float value
	);

	// 保存 字符串 型的值（不要在 Unicode 字符集下保存中文字符）
	static void saveString(
		const EString & key,
		const EString & value
	);

	// 获取 int 型的值（若不存在则返回 default 参数的值）
	static int getInt(
		const EString & key,
		int default
	);

	// 获取 float 型的值（若不存在则返回 default 参数的值）
	static float getFloat(
		const EString & key,
		float default
	);

	// 获取 字符串 型的值（若不存在则返回 default 参数的值）
	static EString getString(
		const EString & key,
		const EString & default
	);

	// 得到文件扩展名（小写）
	static EString getFileExtension(
		const EString & filePath
	);

	/**
	*  打开保存文件对话框，得到有效保存路径返回 true
	*  参数：返回文件路径的字符串，窗口标题，设置扩展名过滤，设置默认扩展名
	*/
	static EString getSaveFilePath(
		const EString & title = L"保存到",
		const EString & defExt = NULL
	);
};


class ERandom
{
public:
	// 取得整型范围内的一个随机数
	template<typename T>
	static inline T between(T min, T max) { return e2d::ERandom::randomInt(min, max); }

	// 取得浮点数范围内的一个随机数
	static inline float between(float min, float max) { return e2d::ERandom::randomReal(min, max); }

	// 取得浮点数范围内的一个随机数
	static inline double between(double min, double max) { return e2d::ERandom::randomReal(min, max); }

	// 取得浮点数范围内的一个随机数
	static inline long double between(long double min, long double max) { return e2d::ERandom::randomReal(min, max); }

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


class EMusicUtils
{
public:
	// 播放音乐
	static UINT playMusic(
		const EString & musicFilePath,		/* 音乐文件路径 */
		int repeatTimes = 1
	);

	// 播放音乐
	static UINT playMusic(
		const EString & musicResourceName,	/* 资源名称 */
		const EString & musicResourceType,	/* 资源类别 */
		const EString & musicExtension,		/* 指定资源的扩展名 */
		int repeatTimes = 1
	);

	// 暂停音乐
	static bool pauseMusic(
		UINT musicId
	);

	// 暂停音乐
	static bool pauseMusic(
		const EString& musicName
	);

	// 继续播放音乐
	static bool resumeMusic(
		UINT musicId
	);

	// 继续播放音乐
	static bool resumeMusic(
		const EString& musicName
	);

	// 停止音乐
	static bool stopMusic(
		UINT musicId
	);

	// 停止音乐
	static bool stopMusic(
		const EString& musicName
	);

	// 预加载音乐
	static UINT preloadMusic(
		const EString & musicFilePath
	);

	// 预加载音乐
	static UINT preloadMusic(
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