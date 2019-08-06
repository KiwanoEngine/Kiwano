// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Demo1
	: public Scene
{
public:
	static ScenePtr Create()
	{
		return new Demo1;
	}

	Demo1()
	{
		// �����ı�
		auto text = E_NEW Text(L"Hello Easy2D!");
		// ���ı���ʾ����Ļ����
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		text->SetAnchor(0.5, 0.5);
		// ��ӵ�����
		this->AddChild(text);

		// ִ�ж���
		text->AddAction(
			Tween::Spawn({								// Action5: ͬʱִ�� Action1 �� Action4, ������ѭ��
				Tween::RotateBy(60).SetDuration(1000),	// Action1: 1����ת 60 ��
				Tween::Sequence({						// Action4: ˳��ִ�� Action2 �� Action3
					Tween::FadeOut(500),				// Action2: 500���뵭������
					Tween::FadeIn(500)					// Action3: 500���뵭�붯��
				})
			}).SetLoopCount(-1)
		);
	}
};
