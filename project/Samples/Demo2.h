// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Man
	: public Sprite
{
public:
	Man()
	{
		// ����ͼƬ
		Load(L"res/man.png");
		// ��СͼƬ
		SetScale(0.5f);
	}

	// ÿ֡����ʱ
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

		// ������������˳ʱ����תС��
		if (input->IsDown(MouseButton::Left))
		{
			// ��ȡ��ǰ��ת�Ƕ�
			float rotation = this->GetRotation();
			// ���ýǶ�ֵ+2
			this->SetRotation(rotation + 2);
		}

		// �������Ҽ������ػ���ʾС��
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
		// ��������
		auto man = E_NEW Man;
		// ����Ļ�Ͼ�����ʾ
		man->SetAnchor(0.5f, 0.5f);
		man->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// ����˵������
		auto text = E_NEW Text(L"���������Ҽ��ƶ�\n����������ת\n�������Ҽ�����");
		// ��������λ��
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
		text->SetAlignment(TextAlign::Center);

		// ��ӵ�����
		this->AddChild(man);
		this->AddChild(text);
	}
};
