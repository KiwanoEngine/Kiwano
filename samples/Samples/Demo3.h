// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Demo3
	: public Scene
{
	SoundPtr bgmusic;		// 音乐对象
	TextPtr volume_text;	// 音量文字
	TextPtr state_text;		// 播放状态文字

public:
	static ScenePtr Create()
	{
		return new Demo3;
	}

	Demo3()
	{
		// 加载音乐
		bgmusic = new Sound;
		if (!bgmusic->Load(L"res/splash.mp3"))
		{
			bgmusic = nullptr;

			TextPtr err = new Text(L"音频文件加载失败");
			this->AddChild(err);
		}

		// 播放音乐（参数用来设置播放循环次数，-1 表示循环播放）
		bgmusic->Play(-1);

		// 创建说明文字
		TextPtr intro_text = new Text(L"按上下键调整音量\n按空格键暂停或继续");
		intro_text->SetPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 50);

		// 创建音量文字
		volume_text = new Text(L"当前音量：");
		volume_text->SetPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 30);

		// 创建状态文字
		state_text = new Text(L"当前状态：");
		state_text->SetPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 60);

		// 添加到场景
		this->AddChild(intro_text);
		this->AddChild(volume_text);
		this->AddChild(state_text);
	}

	void OnUpdate(Duration dt) override
	{
		if (bgmusic == nullptr)
			return;

		// 获取音量和播放状态
		float volume = bgmusic->GetVolume();
		bool playing = bgmusic->IsPlaying();

		// 修改文本
		volume_text->SetText(L"当前音量：" + std::to_wstring(volume));
		state_text->SetText(playing ? L"当前状态：正在播放" : L"当前状态：停止播放");

		// 获取输入设备
		auto& input = Input::Instance();

		// 按空格键暂停或继续
		if (input.WasPressed(KeyCode::Space))
		{
			bgmusic->IsPlaying() ? bgmusic->Pause() : bgmusic->Resume();
		}

		// 按上下键调整音量
		if (input.WasPressed(KeyCode::Up))
		{
			bgmusic->SetVolume(volume + 0.1f);
		}
		else if (input.WasPressed(KeyCode::Down))
		{
			bgmusic->SetVolume(volume - 0.1f);
		}
	}
};
