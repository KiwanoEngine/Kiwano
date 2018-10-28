// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __E2D_TOOL_H__
#define __E2D_TOOL_H__


#include "e2dutil.h"

namespace easy2d
{

	// 音乐
	class Music
		: public Ref
	{
	public:
		Music();

		Music(
			const std::wstring& file_path	/* 音乐文件路径 */
		);

		Music(
			Resource& res					/* 音乐资源 */
		);

		virtual ~Music();

		// 打开音乐文件
		bool Load(
			const std::wstring& file_path	/* 音乐文件路径 */
		);

		// 打开音乐资源
		bool Load(
			Resource& res					/* 音乐资源 */
		);

		// 播放
		bool Play(
			int loop_count = 0				/* 播放循环次数 (-1 为循环播放) */
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

		// 获取音量
		float GetVolume() const;

		// 设置音量
		bool SetVolume(
			float volume	/* 1 为原始音量, 大于 1 为放大音量, 0 为最小音量 */
		);

		// 获取 IXAudio2SourceVoice 对象
		IXAudio2SourceVoice * GetSourceVoice() const;

	protected:
		E2D_DISABLE_COPY(Music);

	protected:
		bool					opened_;
		bool					playing_;
		UINT32					size_;
		BYTE*					wave_data_;
		IXAudio2SourceVoice*	voice_;
	};


	// 音乐播放器
	class Player
	{
	public:
		Player();

		~Player();

		// 预加载音乐资源
		bool Load(
			const std::wstring& file_path	/* 音乐文件路径 */
		);

		// 播放音乐
		bool Play(
			const std::wstring& file_path,	/* 音乐文件路径 */
			int loop_count = 0				/* 播放循环次数 (-1 为循环播放) */
		);

		// 暂停音乐
		void Pause(
			const std::wstring& file_path	/* 音乐文件路径 */
		);

		// 继续播放音乐
		void Resume(
			const std::wstring& file_path	/* 音乐文件路径 */
		);

		// 停止音乐
		void Stop(
			const std::wstring& file_path	/* 音乐文件路径 */
		);

		// 获取音乐播放状态
		bool IsPlaying(
			const std::wstring& file_path	/* 音乐文件路径 */
		);

		// 预加载音乐资源
		bool Load(
			Resource& res		/* 音乐资源 */
		);

		// 播放音乐
		bool Play(
			Resource& res,		/* 音乐资源 */
			int loop_count = 0	/* 播放循环次数 (-1 为循环播放) */
		);

		// 暂停音乐
		void Pause(
			Resource& res		/* 音乐资源 */
		);

		// 继续播放音乐
		void Resume(
			Resource& res		/* 音乐资源 */
		);

		// 停止音乐
		void Stop(
			Resource& res		/* 音乐资源 */
		);

		// 获取音乐播放状态
		bool IsPlaying(
			Resource& res		/* 音乐资源 */
		);

		// 获取音量
		float GetVolume() const;

		// 设置音量
		void SetVolume(
			float volume		/* 1.0 为原始音量 */
		);

		// 暂停所有音乐
		void PauseAll();

		// 继续播放所有音乐
		void ResumeAll();

		// 停止所有音乐
		void StopAll();

		// 清除缓存
		static void ClearCache();

	protected:
		E2D_DISABLE_COPY(Player);

	protected:
		float volume_;

		static std::map<size_t, Music*> musics_;
	};


	// 数据管理工具
	class Data
	{
	public:
		Data(
			const std::wstring& key,				/* 键值 */
			const std::wstring& field = L"Defalut"	/* 字段名称 */
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

		// 保存 std::wstring 类型的值
		bool SaveString(
			const std::wstring& value
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
		std::wstring GetString();

	protected:
		std::wstring key_;
		std::wstring field_;
		const std::wstring& data_path_;
	};


	// 文件
	class File
	{
	public:
		File();

		File(
			const std::wstring& file_name
		);

		virtual ~File();

		// 打开文件
		bool Open(
			const std::wstring& file_name
		);

		// 文件是否存在
		bool Exists() const;

		// 删除文件
		bool Delete();

		// 获取文件路径
		const std::wstring& GetPath() const;

		// 获取文件扩展名
		std::wstring GetExtension() const;

		// 释放资源到临时文件目录
		static File Extract(
			Resource& res,						/* 资源 */
			const std::wstring& dest_file_name	/* 目标文件名 */
		);

		// 添加文件搜索路径
		static void AddSearchPath(
			const std::wstring& path
		);

		// 弹出打开文件对话框
		static File ShowOpenDialog(
			const std::wstring& title = L"打开",		/* 对话框标题 */
			const std::wstring& filter = L""		/* 筛选扩展名，例如 "*.jpg;*.jpeg" */
		);

		// 弹出保存文件对话框
		static File ShowSaveDialog(
			const std::wstring& title = L"保存",		/* 对话框标题 */
			const std::wstring& def_file = L"",		/* 默认保存的文件名 */
			const std::wstring& def_ext = L""		/* 默认追加的扩展名，例如 "txt" */
		);

	protected:
		std::wstring	file_path_;

		static std::list<std::wstring> search_paths_;
	};


	// 路径
	class Path
	{
	public:
		// 获取数据的默认保存路径
		static const std::wstring& GetDataPath();

		// 获取临时文件目录
		static const std::wstring& GetTemporaryPath();

		// 获取 LocalAppData 目录
		static const std::wstring& GetLocalAppDataPath();

		// 获取当前程序的运行路径
		static const std::wstring& GetExeFilePath();
	};

} // end of easy2d namespace


#endif // __E2D_TOOL_H__
