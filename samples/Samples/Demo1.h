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
		// 创建人物图片
		ImagePtr man_image = new Image(L"res/man.png");

		// 缓动方程
		auto ease_functions = {
			Ease::Linear,		// 线性变化
			Ease::EaseInOut,	// 变化过程中有缓冲
			Ease::ExpoInOut,	// 在开始和结束阶段非常慢，但过程非常快
			Ease::BounceInOut,	// 在开始和结束阶段均赋予弹性
			Ease::BackInOut		// 开始和结束阶段均有一个短暂的反方向运动
		};

		float height = 100.f;
		for (auto& func : ease_functions)
		{
			SpritePtr man = new Sprite(man_image);
			man->SetPosition(100, height);
			man->SetScale(0.5f, 0.3f);
			this->AddChild(man);

			// 重置人物位置函数
			auto reset_pos = [ptr = man.Get()]() { ptr->Move(-350, 0); };

			// 执行动画
			man->AddAction(
				Tween::Group({							// Tween::Group 组合动画
					Tween::MoveBy(Point{ 350, 0 })		// Tween::MoveBy 横向位移 350 像素
						.SetDuration(4000)				//     设置位移时间为 4 秒
						.SetEaseFunc(func),				//     设置缓动函数
					Tween::Delay(1000)					// Tween::Delay 延迟 1 秒
				})
				.SetLoops(-1)							// 无限循环执行
				.SetLoopDoneCallback(reset_pos)			// 设置每次循环结束都重置人物位置
			);

			height += 60.f;
		}
	}
};
