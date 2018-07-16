#pragma once
#include "e2dbase.h"

namespace e2d
{


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


// 音乐
class Music :
	public Ref
{
	friend class VoiceCallback;

public:
	Music();

	explicit Music(
		const e2d::String& filePath	/* 音乐文件路径 */
	);

	explicit Music(
		const Resource& res
	);

	virtual ~Music();

	// 打开音乐文件
	bool open(
		const e2d::String& filePath	/* 音乐文件路径 */
	);

	// 打开音乐资源
	bool open(
		const Resource& res
	);

	// 播放
	bool play(
		int nLoopCount = 0
	);

	// 暂停
	void pause();

	// 继续
	void resume();

	// 停止
	void stop();

	// 关闭并回收资源
	void close();

	// 是否正在播放
	bool isPlaying() const;

	// 设置音量
	bool setVolume(
		double volume
	);

	// 设置播放结束时的执行函数
	void setFuncOnEnd(
		const Function& func
	);

	// 设置循环播放中每一次播放结束时的执行函数
	void setFuncOnLoopEnd(
		const Function& func
	);

	// 获取 IXAudio2SourceVoice 对象
	IXAudio2SourceVoice * getIXAudio2SourceVoice() const;

protected:
	bool _readMMIO();

	bool _resetFile();

	bool _read(
		BYTE* pBuffer,
		DWORD dwSizeToRead
	);

	bool _findMediaFileCch(
		wchar_t* strDestPath,
		int cchDest,
		const wchar_t * strFilename
	);

protected:
	bool					_opened;
	bool					_playing;
	DWORD					_dwSize;
	CHAR*					_resBuffer;
	BYTE*					_waveData;
	HMMIO					_hmmio;
	MMCKINFO				_ck;
	MMCKINFO				_ckRiff;
	WAVEFORMATEX*			_wfx;
	VoiceCallback			_voiceCallback;
	IXAudio2SourceVoice*	_voice;
};


// 音乐播放器
class Player
{
	friend class Game;
	typedef std::map<Resource, Music*> MusicMap;

public:
	// 获取播放器实例
	static Player * getInstance();

	// 销毁实例
	static void destroyInstance();

	// 预加载音乐资源
	bool preload(
		const String& filePath	/* 音乐文件路径 */
	);

	// 播放音乐
	bool play(
		const String& filePath,	/* 音乐文件路径 */
		int nLoopCount = 0		/* 重复播放次数，设置 -1 为循环播放 */
	);

	// 暂停音乐
	void pause(
		const String& filePath	/* 音乐文件路径 */
	);

	// 继续播放音乐
	void resume(
		const String& filePath	/* 音乐文件路径 */
	);

	// 停止音乐
	void stop(
		const String& filePath	/* 音乐文件路径 */
	);

	// 获取音乐播放状态
	bool isPlaying(
		const String& filePath	/* 音乐文件路径 */
	);

	// 预加载音乐资源
	bool preload(
		const Resource& res		/* 音乐资源 */
	);

	// 播放音乐
	bool play(
		const Resource& res,	/* 音乐资源 */
		int nLoopCount = 0		/* 重复播放次数，设置 -1 为循环播放 */
	);

	// 暂停音乐
	void pause(
		const Resource& res		/* 音乐资源 */
	);

	// 继续播放音乐
	void resume(
		const Resource& res		/* 音乐资源 */
	);

	// 停止音乐
	void stop(
		const Resource& res		/* 音乐资源 */
	);

	// 获取音乐播放状态
	bool isPlaying(
		const Resource& res		/* 音乐资源 */
	);

	// 获取音量
	double getVolume();

	// 设置音量
	void setVolume(
		double volume			/* 音量范围为 -224 ~ 224，0 是静音，1 是正常音量 */
	);

	// 暂停所有音乐
	void pauseAll();

	// 继续播放所有音乐
	void resumeAll();

	// 停止所有音乐
	void stopAll();

	// 清空音乐缓存
	void clearCache();

	// 获取 IXAudio2 对象
	IXAudio2 * getXAudio2();

private:
	Player();

	~Player();

	E2D_DISABLE_COPY(Player);

private:
	float					_volume;
	MusicMap				_musicList;
	IXAudio2*				_xAudio2;
	IXAudio2MasteringVoice*	_masteringVoice;

	static Player * _instance;
};


class Timer;

// 定时任务
class Task :
	public Ref
{
	friend class Timer;

public:
	explicit Task(
		const Function& func,		/* 执行函数 */
		const String& name = L""	/* 定时器名称 */
	);

	explicit Task(
		const Function& func,		/* 执行函数 */
		double delay,				/* 时间间隔（秒） */
		int times = -1,				/* 执行次数（设 -1 为永久执行） */
		const String& name = L""	/* 定时器名称 */
	);

	// 暂停任务
	void pause();

	// 继续任务
	void resume();

	// 任务是否就绪
	bool isReady() const;

	// 任务是否正在执行
	bool isRunning() const;

	// 获取任务名称
	String getName() const;

	// 执行任务
	void update();

	// 刷新任务计时
	void updateTime();

private:
	bool		_running;
	bool		_stopped;
	int			_runTimes;
	int			_totalTimes;
	double		_delay;
	double		_lastTime;
	String		_name;
	Function	_callback;
};


// 定时器
class Timer
{
public:
	// 获取定时器实例
	static Timer * getInstance();

	// 销毁实例
	static void destroyInstance();

	// 添加任务
	void addTask(
		Task * task
	);

	// 继续具有相同名称的任务
	void resumeTasks(
		const String& name
	);

	// 暂停具有相同名称的任务
	void pauseTasks(
		const String& name
	);

	// 移除具有相同名称的任务
	void removeTasks(
		const String& name
	);

	// 继续所有任务
	void resumeAllTasks();

	// 暂停所有任务
	void pauseAllTasks();

	// 移除所有任务
	void removeAllTasks();

	// 强制清空所有任务
	void clearAllTasks();

	// 更新定时器
	void update();

	// 刷新所有任务计时
	void updateTime();

private:
	Timer();

	~Timer();

	E2D_DISABLE_COPY(Timer);

private:
	std::vector<Task*> _tasks;

	static Timer * _instance;
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
};


// 文件
class File
{
public:
	File();

	File(
		const String& fileName
	);

	virtual ~File();

	// 打开文件
	bool open(
		const String& fileName
	);

	// 文件或文件夹是否存在
	bool exists() const;

	// 是否是文件夹
	bool isFolder() const;

	// 删除文件
	bool deleteFile();

	// 获取文件路径
	String getFilePath() const;

	// 获取文件扩展名
	String getExtension() const;

	// 释放资源到临时文件目录
	static File extract(
		int resNameId,				/* 资源名称 */
		const String& resType,		/* 资源类型 */
		const String& destFileName	/* 目标文件名 */
	);

	// 添加文件搜索路径
	static void addSearchPath(
		const String& path
	);

	// 创建文件夹
	static bool createFolder(
		const String& dirPath	/* 文件夹路径 */
	);

	// 打开保存文件对话框
	static String getSaveFilePath(
		const String& title = L"保存到",		/* 对话框标题 */
		const String& defExt = L""			/* 默认扩展名 */
	);

protected:
	DWORD _attributes;
	String _fileName;

	static std::list<String> _searchPaths;
};


// 路径
class Path
{
	friend class Game;

public:
	// 获取数据的默认保存路径
	static String getDataPath();

	// 获取临时文件目录
	static String getTempPath();

	// 获取 LocalAppData 目录
	static String getLocalAppDataPath();

	// 获取当前程序的运行路径
	static String getCurrentFilePath();
};

}