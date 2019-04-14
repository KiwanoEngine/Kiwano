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
		// ��������ͼƬ
		ImagePtr man_image = new Image(L"res/man.png");

		// ���������б�
		auto ease_functions = { Ease::Linear, Ease::EaseInOut, Ease::ExpoInOut, Ease::BounceInOut, Ease::BackInOut };

		float height = 100.f;
		for (auto& func : ease_functions)
		{
			SpritePtr man = new Sprite(man_image);
			man->SetPosition(100, height);
			man->SetScale(0.5f, 0.3f);
			this->AddChild(man);

			// ��������λ�ú���
			auto reset_pos = [ptr = man.Get()]() { ptr->Move(-350, 0); };

			// ִ�ж���
			man->AddAction(
				Tween::Group({							// Tween::Group ��϶���
					Tween::MoveBy(Point{ 350, 0 })		// Tween::MoveBy ����λ�� 350 ����
						.SetDuration(4000)				//     ����λ��ʱ��Ϊ 4 ��
						.SetEaseFunc(func),				//     ���û�������
					Tween::Delay(1000)					// Tween::Delay �ӳ� 1 ��
				})
				.SetLoops(-1)							// ����ѭ��ִ��
				.SetLoopDoneCallback(reset_pos)			// ����ÿ��ѭ����������������λ��
			);

			height += 60.f;
		}
	}
};
