// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// ����
KGE_DECLARE_SMART_PTR(Monster);
class Monster
	: public Sprite
{
public:
	Monster()
	{
		// ����ͼƬ
		Load(L"res/akushu.png");
		// ��СͼƬ
		SetScale(0.7f);
	}

	// ÿ֡��Ⱦǰִ�� OnUpdate
	void OnUpdate(Duration dt) override
	{
		// ��ȡ�����豸
		auto& input = Input::Instance();
		// �������Ҽ�
		if (input.IsDown(KeyCode::Left))
		{
			this->Move(-2, 0);
		}
		else if (input.IsDown(KeyCode::Right))
		{
			this->Move(2, 0);
		}

		// �������¼�
		if (input.IsDown(KeyCode::Up))
		{
			this->Move(0, -2);
		}
		else if (input.IsDown(KeyCode::Down))
		{
			this->Move(0, 2);
		}

		// ������������˳ʱ����ת����
		if (input.IsDown(MouseButton::Left))
		{
			// ��ȡ��ǰ��ת�Ƕ�
			float rotation = this->GetRotation();
			// ���ýǶ�ֵ+2
			this->SetRotation(rotation + 2);
		}

		// �������Ҽ������ػ���ʾ����
		if (input.WasPressed(MouseButton::Right))
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
		MonsterPtr monster = new Monster;
		// ����Ļ�Ͼ�����ʾ
		monster->SetAnchor(0.5f, 0.5f);
		monster->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// ����˵������
		TextPtr text = new Text(L"���������Ҽ��ƶ�\n����������ת\n�������Ҽ�����");
		// ���ýڵ��СΪ���ֲ��ִ�С
		text->SetSize(text->GetLayoutSize());
		// ��������λ��
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
		text->SetAlignment(TextAlign::Center);

		// ��ӵ�����
		this->AddChild(monster);
		this->AddChild(text);
	}
};
