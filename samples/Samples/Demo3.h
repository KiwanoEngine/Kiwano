// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Demo3
	: public Scene
{
	SoundPtr bgmusic;		// ���ֶ���
	TextPtr volume_text;	// ��������
	TextPtr state_text;		// ����״̬����

public:
	static ScenePtr Create()
	{
		return new Demo3;
	}

	Demo3()
	{
		// ��������
		bgmusic = new Sound;
		if (!bgmusic->Load(L"res/splash.mp3"))
		{
			bgmusic = nullptr;

			TextPtr err = new Text(L"��Ƶ�ļ�����ʧ��");
			this->AddChild(err);
		}

		// �������֣������������ò���ѭ��������-1 ��ʾѭ�����ţ�
		bgmusic->Play(-1);

		// ����˵������
		TextPtr intro_text = new Text(L"�����¼���������\n���ո����ͣ�����");
		intro_text->SetPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 50);

		// ������������
		volume_text = new Text(L"��ǰ������");
		volume_text->SetPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 30);

		// ����״̬����
		state_text = new Text(L"��ǰ״̬��");
		state_text->SetPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 60);

		// ��ӵ�����
		this->AddChild(intro_text);
		this->AddChild(volume_text);
		this->AddChild(state_text);
	}

	void OnUpdate(Duration dt) override
	{
		if (bgmusic == nullptr)
			return;

		// ��ȡ�����Ͳ���״̬
		float volume = bgmusic->GetVolume();
		bool playing = bgmusic->IsPlaying();

		// �޸��ı�
		volume_text->SetText(L"��ǰ������" + std::to_wstring(volume));
		state_text->SetText(playing ? L"��ǰ״̬�����ڲ���" : L"��ǰ״̬��ֹͣ����");

		// ��ȡ�����豸
		auto input = Input::Instance();

		// ���ո����ͣ�����
		if (input->WasPressed(KeyCode::Space))
		{
			bgmusic->IsPlaying() ? bgmusic->Pause() : bgmusic->Resume();
		}

		// �����¼���������
		if (input->WasPressed(KeyCode::Up))
		{
			bgmusic->SetVolume(volume + 0.1f);
		}
		else if (input->WasPressed(KeyCode::Down))
		{
			bgmusic->SetVolume(volume - 0.1f);
		}
	}
};
