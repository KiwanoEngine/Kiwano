// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// ��ɫ
KGE_DECLARE_SMART_PTR(Hero);
class Hero
	: public GifSprite
{
public:
	Hero(GifImagePtr image)
	{
		// ����ͼƬ
		Load(image);
		// ���� GIF ��ͼ����ѭ��
		SetLoopCount(-1);
	}

	// ÿ֡��Ⱦǰִ�� OnUpdate
	void OnUpdate(Duration dt) override
	{
		// ��ȡ�����豸
		auto input = Input::Instance();

		// �������Ҽ�
		if (input->IsDown(KeyCode::Left))
		{
			this->Move(-2, 0);
		}
		else if (input->IsDown(KeyCode::Right))
		{
			this->Move(2, 0);
		}

		// �������¼�
		if (input->IsDown(KeyCode::Up))
		{
			this->Move(0, -2);
		}
		else if (input->IsDown(KeyCode::Down))
		{
			this->Move(0, 2);
		}

		// ������������˳ʱ����ת��ɫ
		if (input->IsDown(MouseButton::Left))
		{
			// ��ȡ��ǰ��ת�Ƕ�
			float rotation = this->GetRotation();
			// ���ýǶ�ֵ+2
			this->SetRotation(rotation + 2);
		}

		// �������Ҽ������ػ���ʾ��ɫ
		if (input->WasPressed(MouseButton::Right))
		{
			// ��ȡ��ǰ��ʾ״̬
			bool visible = this->IsVisible();
			// �����෴����ʾ״̬
			this->SetVisible(!visible);
		}
	}
};

class Demo2
	: public Scene
{
public:
	static ScenePtr Create()
	{
		return new Demo2;
	}

	Demo2()
	{
		// ������ɫ
		HeroPtr hero = new Hero(g_Loader.GetGifImage(L"Kusanagi"));
		// ����Ļ�Ͼ�����ʾ
		hero->SetAnchor(0.5f, 0.5f);
		hero->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// ����˵������
		TextPtr text = new Text(L"���������Ҽ��ƶ�\n����������ת\n�������Ҽ�����");
		// ���ýڵ��СΪ���ֲ��ִ�С
		text->SetSize(text->GetLayoutSize());
		// ��������λ��
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
		text->SetAlignment(TextAlign::Center);

		// ��ӵ�����
		this->AddChild(hero);
		this->AddChild(text);
	}
};
