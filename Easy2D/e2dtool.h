#pragma once
#include <e2dbase.h>

namespace easy2d 
{
	class Action;

	class FileUtils
	{
	public:
		// 获取系统的 AppData\Local 路径
		static tstring getLocalAppDataPath();
		// 获取默认的保存路径
		static tstring getDefaultSavePath();
		// 保存 int 型的值
		static void saveInt(LPCTSTR key, int value);
		// 保存 double 型的值
		static void saveDouble(LPCTSTR key, double value);
		// 保存 字符串 型的值（不要在 Unicode 字符集下保存中文字符）
		static void saveString(LPCTSTR key, tstring value);
		// 获取 int 型的值（若不存在则返回 default 参数的值）
		static int getInt(LPCTSTR key, int default);
		// 获取 double 型的值（若不存在则返回 default 参数的值）
		static double getDouble(LPCTSTR key, double default);
		// 获取 字符串 型的值（若不存在则返回 default 参数的值）
		static tstring getString(LPCTSTR key, tstring default);
		// 得到文件扩展名（小写）
		static tstring getFileExtension(const tstring& filePath);
		/**
		*  打开保存文件对话框，得到有效保存路径返回 true
		*  参数：返回文件路径的字符串，窗口标题，设置扩展名过滤，设置默认扩展名
		*/
		static bool getSaveFilePath(tstring& path, LPCTSTR title = _T("保存到"), LPCTSTR defExt = NULL);
	};


	class MusicUtils
	{
	public:
		// 播放背景音乐
		static void playBackgroundMusic(tstring pszFilePath, bool bLoop = true);
		// 停止背景音乐
		static void stopBackgroundMusic(bool bReleaseData = false);
		// 暂停背景音乐
		static void pauseBackgroundMusic();
		// 继续播放背景音乐
		static void resumeBackgroundMusic();
		// 从头播放背景音乐
		static void rewindBackgroundMusic();
		// 背景音乐是否正在播放
		static bool isBackgroundMusicPlaying();
		// 设置背景音乐音量，0 ~ 1.0f
		static void setBackgroundMusicVolume(float volume);

		// 播放音效
		static unsigned int playMusic(tstring pszFilePath, bool loop = false);
		// 停止音效
		static void stopMusic(unsigned int nSoundId);
		// 预加载音效
		static void preloadMusic(tstring pszFilePath);
		// 暂停音效
		static void pauseMusic(unsigned int nSoundId);
		// 继续播放音效
		static void resumeMusic(unsigned int nSoundId);
		// 卸载音效
		static void unloadMusic(LPCTSTR pszFilePath);
		// 设置特定音乐的音量，0 ~ 1.0f
		static void setVolume(tstring pszFilePath, float volume);

		// 暂停所有音乐
		static void pauseAllMusics();
		// 继续播放所有音乐
		static void resumeAllMusics();
		// 停止所有音乐
		static void stopAllMusics();
		// 停止所有音乐，并释放内存
		static void end();
		// 设置总音量，0 ~ 1.0f
		static void setVolume(float volume);
	};


	class Timer
	{
		friend class App;

	public:
		Timer(tstring name, UINT ms, const TIMER_CALLBACK & callback);
		~Timer();

		// 启动定时器
		void start();
		// 停止定时器
		void stop();
		// 定时器是否正在运行
		bool isRunning();
		// 设置间隔时间
		void setInterval(UINT ms);
		// 设置回调函数
		void setCallback(const TIMER_CALLBACK& callback);
		// 设置定时器名称
		void setName(tstring name);
		// 获取定时器间隔时间
		UINT getInterval() const;
		// 获取定时器名称
		tstring getName() const;

		// 添加定时器
		static void addTimer(Timer * timer);
		// 添加定时器
		static void addTimer(tstring name, UINT ms, const TIMER_CALLBACK & callback);
		// 根据名称获取定时器
		static Timer * getTimer(tstring name);
		// 启动特定定时器
		static bool startTimer(tstring name);
		// 停止特定定时器
		static bool stopTimer(tstring name);
		// 删除特定定时器
		static bool delTimer(tstring name);
		// 删除所有定时器
		static void clearAllTimers();

	protected:
		bool			m_bRunning;
		tstring			m_sName;
		TIMER_CALLBACK	m_callback;
		LARGE_INTEGER	m_nLast;
		LARGE_INTEGER	m_nAnimationInterval;
		UINT			m_nMilliSeconds;

	private:
		static void __exec();
	};


	class ActionManager
	{
		friend class App;
		friend class Sprite;
	public:
		// 继续一个特定的动作
		static void startAction(Action * action);
		// 继续一个特定的动作
		static void resumeAction(Action * action);
		// 暂停一个特定的动作
		static void pauseAction(Action * action);
		// 停止一个特定的动作
		static void stopAction(Action * action);

		// 继续一个 Sprite 的所有动作
		static void startSpriteAllActions(Sprite * sprite);
		// 继续一个 Sprite 的所有动作
		static void resumeSpriteAllActions(Sprite * sprite);
		// 暂停一个 Sprite 的所有动作
		static void pauseSpriteAllActions(Sprite * sprite);
		// 停止一个 Sprite 的所有动作
		static void stopSpriteAllActions(Sprite * sprite);

		// 继续当前存在的所有动作
		static void startAllActions();
		// 继续当前存在的所有动作
		static void resumeAllActions();
		// 暂停当前存在的所有动作
		static void pauseAllActions();
		// 停止当前存在的所有动作
		static void stopAllActions();

	private:
		static void __exec();
		// 将一个动作添加进动作管理器
		static void addAction(Action * action);
	};

}	// End of easy2d namespace