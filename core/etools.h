#pragma once
#include "ebase.h"
#include <random>

namespace e2d
{

class ETimerManager;
class EMusicManager;

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


// 音乐播放器
class EMusic
{
	friend EMusicManager;

public:
	// 播放
	bool play(
		int nLoopCount = 0	/* 重复播放次数，设置 -1 为循环播放 */
	);

	// 暂停
	bool pause();

	// 继续
	bool resume();

	// 停止
	bool stop();

	// 获取音乐播放状态
	bool isPlaying();

	// 获取音量
	float getVolume() const;

	// 设置音量
	bool setVolume(
		float fVolume	/* 音量范围为 -224 ~ 224，其中 0 是静音，1 是正常音量 */
	);

	// 获取频率比
	float getFrequencyRatio() const;

	// 设置频率比
	bool setFrequencyRatio(
		float fFrequencyRatio	/* 频率比范围为 1/1024.0f ~ 1024.0f，其中 1.0 为正常声调 */
	);

	// 获取 IXAudio2SourceVoice 对象
	IXAudio2SourceVoice* getIXAudio2SourceVoice() const;

protected:
	EMusic();

	virtual ~EMusic();

	EMusic(const EMusic &) = delete;
	
	EMusic &operator =(const EMusic &) = delete;

	bool _open(LPWSTR strFileName);

	void _close();

	bool _readMMIO();

	bool _resetFile();

	bool _read(BYTE* pBuffer, DWORD dwSizeToRead);

	bool _findMediaFileCch(WCHAR* strDestPath, int cchDest, LPCWSTR strFilename);

protected:
	bool m_bOpened;
	bool m_bPlaying;
	DWORD m_dwSize;
	CHAR* m_pResourceBuffer;
	BYTE* m_pbWaveData;
	HMMIO m_hmmio;
	MMCKINFO m_ck;
	MMCKINFO m_ckRiff;
	WAVEFORMATEX* m_pwfx;
	IXAudio2SourceVoice* m_pSourceVoice;
};

}