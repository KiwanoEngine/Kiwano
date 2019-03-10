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
		// 创建文本
		TextPtr text = new Text(L"Hello Easy2D!");
		// 设置节点大小为文字布局大小
		text->SetSize(text->GetLayoutSize());
		// 让文本显示在屏幕中央
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		text->SetAnchor(0.5, 0.5);
		// 添加到场景
		this->AddChild(text);

		// 执行动画
		text->AddAction(
			Tween::Multiple({							// Action5: 同时执行 Action1 和 Action4, 并无限循环
				Tween::RotateBy(60).SetDuration(1000),	// Action1: 1秒旋转 60 度
				Tween::Group({							// Action4: 顺序执行 Action2 和 Action3
					Tween::FadeOut(500),				// Action2: 500毫秒淡出动画
					Tween::FadeIn(500)					// Action3: 500毫秒淡入动画
				})
			}).SetLoops(-1)
		);
	}
};
