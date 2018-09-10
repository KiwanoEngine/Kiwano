#pragma once
#include "e2dutil.h"
#include "e2dimpl.h"

namespace e2d
{


	// 随机数产生器
	class Random
	{
	public:
		// 取得范围内的一个整型随机数
		template<typename T>
		static inline T Range(T min, T max)
		{
			return e2d::Random::RandomInt(min, max);
		}

		// 取得范围内的一个浮点数随机数
		static inline float Range(float min, float max)
		{
			return e2d::Random::RandomReal(min, max);
		}

		// 取得范围内的一个浮点数随机数
		static inline double Range(double min, double max)
		{
			return e2d::Random::RandomReal(min, max);
		}

	private:
		template<typename T>
		static T RandomInt(T min, T max)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(Random::GetEngine());
		}

		template<typename T>
		static T RandomReal(T min, T max)
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(Random::GetEngine());
		}

		// 获取随机数产生器
		static std::default_random_engine &GetEngine();
	};


	// 音乐
	class Music :
		public Ref
	{
	public:
		Music();

		explicit Music(
			const e2d::String& file_path	/* 音乐文件路径 */
		);

		explicit Music(
			const Resource& res		/* 音乐资源 */
		);

		virtual ~Music();

		// 打开音乐文件
		bool Open(
			const e2d::String& file_path	/* 音乐文件路径 */
		);

		// 打开音乐资源
		bool Open(
			const Resource& res
		);

		// 播放
		bool Play(
			int loopCount = 0
		);

		// 暂停
		void Pause();

		// 继续
		void Resume();

		// 停止
		void Stop();

		// 关闭并回收资源
		void Close();

		// 是否正在播放
		bool IsPlaying() const;

		// 设置音量
		bool SetVolume(
			float volume
		);

		// 设置播放结束时的执行函数
		void SetCallbackOnEnd(
			const Function& func
		);

		// 设置循环播放中每一次播放结束时的执行函数
		void SetCallbackOnLoopEnd(
			const Function& func
		);

		// 获取 IXAudio2SourceVoice 对象
		IXAudio2SourceVoice * GetSourceVoice() const;

	protected:
		bool ReadMMIO();

		bool ResetFile();

		bool Read(
			BYTE* buffer,
			DWORD size_to_read
		);

		bool FindMediaFileCch(
			wchar_t* dest_path,
			int cch_dest,
			const wchar_t * file_name
		);

	protected:
		bool					opened_;
		DWORD					size_;
		CHAR*					buffer_;
		BYTE*					wave_data_;
		HMMIO					hmmio_;
		MMCKINFO				ck_;
		MMCKINFO				ck_riff_;
		WAVEFORMATEX*			wfx_;
		VoiceCallback			callback_;
		IXAudio2SourceVoice*	voice_;
	};


	// 音乐播放器
	class Player
	{
	public:
		// 获取播放器实例
		static Player * GetInstance();

		// 销毁实例
		static void DestroyInstance();

		// 预加载音乐资源
		bool Preload(
			const String& file_path	/* 音乐文件路径 */
		);

		// 播放音乐
		bool Play(
			const String& file_path,	/* 音乐文件路径 */
			int loop_count = 0		/* 重复播放次数，设置 -1 为循环播放 */
		);

		// 暂停音乐
		void Pause(
			const String& file_path	/* 音乐文件路径 */
		);

		// 继续播放音乐
		void Resume(
			const String& file_path	/* 音乐文件路径 */
		);

		// 停止音乐
		void Stop(
			const String& file_path	/* 音乐文件路径 */
		);

		// 获取音乐播放状态
		bool IsPlaying(
			const String& file_path	/* 音乐文件路径 */
		);

		// 预加载音乐资源
		bool Preload(
			const Resource& res		/* 音乐资源 */
		);

		// 播放音乐
		bool Play(
			const Resource& res,	/* 音乐资源 */
			int loop_count = 0		/* 重复播放次数，设置 -1 为循环播放 */
		);

		// 暂停音乐
		void Pause(
			const Resource& res		/* 音乐资源 */
		);

		// 继续播放音乐
		void Resume(
			const Resource& res		/* 音乐资源 */
		);

		// 停止音乐
		void Stop(
			const Resource& res		/* 音乐资源 */
		);

		// 获取音乐播放状态
		bool IsPlaying(
			const Resource& res		/* 音乐资源 */
		);

		// 获取音量
		float GetVolume();

		// 设置音量
		void SetVolume(
			float volume	/* 音量范围为 -224 ~ 224，0 是静音，1 是正常音量 */
		);

		// 暂停所有音乐
		void PauseAll();

		// 继续播放所有音乐
		void ResumeAll();

		// 停止所有音乐
		void StopAll();

		// 清空音乐缓存
		void ClearCache();

	protected:
		Player();

		~Player();

		E2D_DISABLE_COPY(Player);

	protected:
		float volume_;
		std::map<size_t, Music*> musics_;

		static Player * instance_;
	};


	// 数据管理工具
	class Data
	{
	public:
		Data(
			const String& key,					/* 键值 */
			const String& field = L"Defalut"	/* 字段名称 */
		);

		// 该数据是否存在
		bool Exists() const;

		// 保存 int 类型的值
		bool SaveInt(
			int value
		);

		// 保存 float 类型的值
		bool SaveFloat(
			float value
		);

		// 保存 double 类型的值
		bool SaveDouble(
			double value
		);

		// 保存 bool 类型的值
		bool SaveBool(
			bool value
		);

		// 保存 String 类型的值
		bool SaveString(
			const String& value
		);

		// 获取 int 类型的值
		int GetInt() const;

		// 获取 float 类型的值
		float GetFloat() const;

		// 获取 double 类型的值
		double GetDouble() const;

		// 获取 bool 类型的值
		bool GetBool() const;

		// 获取 字符串 类型的值
		String GetString();

	protected:
		String key_;
		String field_;
		const String& data_path_;
	};


	// 文件
	class File
	{
	public:
		File();

		File(
			const String& file_name
		);

		virtual ~File();

		// 打开文件
		bool Open(
			const String& file_name
		);

		// 文件或文件夹是否存在
		bool Exists() const;

		// 是否是文件夹
		bool IsFolder() const;

		// 删除文件
		bool Delete();

		// 获取文件路径
		const String& GetPath() const;

		// 获取文件扩展名
		String GetExtension() const;

		// 释放资源到临时文件目录
		static File Extract(
			int resource_name,				/* 资源名称 */
			const String& resource_type,	/* 资源类型 */
			const String& dest_file_name	/* 目标文件名 */
		);

		// 添加文件搜索路径
		static void AddSearchPath(
			const String& path
		);

		// 创建文件夹
		static bool CreateFolder(
			const String& dir_path	/* 文件夹路径 */
		);

		// 弹出打开文件对话框
		static File ShowOpenDialog(
			const String& title = L"打开",	/* 对话框标题 */
			const String& filter = L""		/* 筛选扩展名，例如 "*.jpg;*.jpeg" */
		);

		// 弹出保存文件对话框
		static File ShowSaveDialog(
			const String& title = L"保存",	/* 对话框标题 */
			const String& def_file = L"",	/* 默认保存的文件名 */
			const String& def_ext = L""		/* 默认追加的扩展名，例如 "txt" */
		);

	protected:
		DWORD attributes_;
		String file_path_;

		static std::list<String> search_paths_;
	};


	// 路径
	class Path
	{
	public:
		// 获取数据的默认保存路径
		static const String& GetDataPath();

		// 获取临时文件目录
		static const String& GetTemporaryPath();

		// 获取 LocalAppData 目录
		static const String& GetLocalAppDataPath();

		// 获取当前程序的运行路径
		static const String& GetExeFilePath();
	};

}