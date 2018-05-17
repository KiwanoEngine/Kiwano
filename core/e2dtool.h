#pragma once
#include "e2dbase.h"

namespace e2d
{

class InputManager;
class ColliderManager;

// 随机数产生器
class Random
{
public:
	// 取得范围内的一个整型随机数
	template<typename T>
	static inline T range(T min, T max) 
	{ 
		return e2d::Random::__randomInt(min, max); 
	}

	// 取得范围内的一个浮点数随机数
	static inline double range(float min, float max) 
	{ 
		return e2d::Random::__randomReal(min, max); 
	}

	// 取得范围内的一个浮点数随机数
	static inline double range(double min, double max) 
	{ 
		return e2d::Random::__randomReal(min, max); 
	}

private:
	template<typename T>
	static T __randomInt(T min, T max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(Random::__getEngine());
	}

	template<typename T>
	static T __randomReal(T min, T max)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(Random::__getEngine());
	}

	// 获取随机数产生器
	static std::default_random_engine &__getEngine();
};


// 音乐播放器
class Music
{
	friend Game;

public:
	// 预加载音乐资源
	static bool preload(
		const String& filePath	/* 音乐文件路径 */
	);

	// 预加载音乐资源
	static bool preload(
		int resNameId,			/* 音乐资源名称 */
		const String& resType	/* 音乐资源类型 */
	);

	// 播放音乐
	static bool play(
		const String& filePath,	/* 音乐文件路径 */
		int nLoopCount = 0		/* 重复播放次数，设置 -1 为循环播放 */
	);

	// 播放音乐
	static bool play(
		int resNameId,			/* 音乐资源名称 */
		const String& resType,	/* 音乐资源类型 */
		int nLoopCount = 0		/* 重复播放次数，设置 -1 为循环播放 */
	);

	// 暂停音乐
	static void pause(
		const String& filePath	/* 音乐文件路径 */
	);

	// 暂停音乐
	static void pause(
		int resNameId,			/* 音乐资源名称 */
		const String& resType	/* 音乐资源类型 */
	);

	// 继续播放音乐
	static void resume(
		const String& filePath	/* 音乐文件路径 */
	);

	// 继续播放音乐
	static void resume(
		int resNameId,			/* 音乐资源名称 */
		const String& resType	/* 音乐资源类型 */
	);

	// 停止音乐
	static void stop(
		const String& filePath	/* 音乐文件路径 */
	);

	// 停止音乐
	static void stop(
		int resNameId,			/* 音乐资源名称 */
		const String& resType	/* 音乐资源类型 */
	);

	// 获取音乐播放状态
	static bool isPlaying(
		const String& filePath	/* 音乐文件路径 */
	);

	// 获取音乐播放状态
	static bool isPlaying(
		int resNameId,			/* 音乐资源名称 */
		const String& resType	/* 音乐资源类型 */
	);

	// 获取音量
	static double getVolume();

	// 设置音量
	static void setVolume(
		double fVolume			/* 音量范围为 -224 ~ 224，0 是静音，1 是正常音量 */
	);

	// 暂停所有音乐
	static void pauseAll();

	// 继续播放所有音乐
	static void resumeAll();

	// 停止所有音乐
	static void stopAll();

	// 获取 IXAudio2 对象
	static IXAudio2 * getIXAudio2();

	// 获取 IXAudio2MasteringVoice 对象
	static IXAudio2MasteringVoice * getIXAudio2MasteringVoice();

private:
	static bool __init();

	static void __uninit();
};


// 定时器
class Timer
{
	friend Game;

public:
	// 启动定时器（每帧执行一次）
	static void start(
		const Function& func,		/* 执行函数 */
		const String& name = L""	/* 定时器名称 */
	);

	// 启动定时器
	static void start(
		const Function& func,		/* 执行函数 */
		double delay,				/* 时间间隔（秒） */
		int times = -1,				/* 执行次数（设 -1 为永久执行） */
		bool paused = false,		/* 是否暂停 */
		const String& name = L""	/* 定时器名称 */
	);

	// 启动仅执行一次的定时器
	static void startOnce(
		const Function& func,		/* 执行的函数 */
		double timeOut				/* 等待的时长（秒） */
	);

	// 暂停具有相同名称的定时器
	static void pause(
		const String& name
	);

	// 继续具有相同名称的定时器
	static void resume(
		const String& name
	);

	// 停止具有相同名称的定时器
	static void stop(
		const String& name
	);

	// 暂停所有定时器
	static void pauseAll();

	// 继续所有定时器
	static void resumeAll();

	// 停止所有定时器
	static void stopAll();

private:
	// 更新定时器
	static void __update();

	// 重置定时器状态
	static void __resetAll();

	// 清空定时器
	static void __uninit();
};


// 数据管理工具
class Data
{
public:
	// 保存 int 类型的值
	static void saveInt(
		const String& key,					/* 键值 */
		int value,							/* 数据 */
		const String& field = L"Defalut"	/* 字段名称 */
	);

	// 保存 double 类型的值
	static void saveDouble(
		const String& key,					/* 键值 */
		double value,						/* 数据 */
		const String& field = L"Defalut"	/* 字段名称 */
	);

	// 保存 bool 类型的值
	static void saveBool(
		const String& key,					/* 键值 */
		bool value,							/* 数据 */
		const String& field = L"Defalut"	/* 字段名称 */
	);

	// 保存 字符串 类型的值
	static void saveString(
		const String& key,					/* 键值 */
		const String& value,				/* 数据 */
		const String& field = L"Defalut"	/* 字段名称 */
	);

	// 获取 int 类型的值
	// （若不存在则返回 defaultValue 参数的值）
	static int getInt(
		const String& key,					/* 键值 */
		int defaultValue,					/* 默认值 */
		const String& field = L"Defalut"	/* 字段名称 */
	);

	// 获取 double 类型的值
	// （若不存在则返回 defaultValue 参数的值）
	static double getDouble(
		const String& key,					/* 键值 */
		double defaultValue,				/* 默认值 */
		const String& field = L"Defalut"	/* 字段名称 */
	);

	// 获取 bool 类型的值
	// （若不存在则返回 defaultValue 参数的值）
	static bool getBool(
		const String& key,					/* 键值 */
		bool defaultValue,					/* 默认值 */
		const String& field = L"Defalut"	/* 字段名称 */
	);

	// 获取 字符串 类型的值
	// （若不存在则返回 defaultValue 参数的值）
	static String getString(
		const String& key,					/* 键值 */
		const String& defaultValue,			/* 默认值 */
		const String& field = L"Defalut"	/* 字段名称 */
	);

	// 修改数据文件的名称
	static void setDataFileName(
		const String& filePath				/* 文件名称 */
	);

	// 获取数据文件的完整路径
	static String getDataFilePath();
};


// 路径工具
class Path
{
	friend Game;

public:
	// 获取数据的默认保存路径
	static String getDefaultSavePath();

	// 获取临时文件目录
	static String getTempPath();

	// 获取文件扩展名
	static String getFileExtension(
		const String& filePath
	);

	// 打开保存文件对话框
	static String getSaveFilePath(
		const String& title = L"保存到",		/* 对话框标题 */
		const String& defExt = L""			/* 默认扩展名 */
	);

	// 创建文件夹
	static bool createFolder(
		const String& dirPath	/* 文件夹路径 */
	);

private:
	// 初始化
	static bool __init();
};

}