// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

E2D_DECLARE_SMART_PTR(Man);
class Man
	: public Sprite
{
public:
	Man()
	{
		// 加载图片
		Load(L"res/man.png");
		// 缩小图片
		SetScale(0.5f);
	}

	// 每帧更新时
	void OnUpdate(Duration dt) override
	{
		// 获取输入设备
		auto input = Input::Instance();
		// 按下左右键
		if (input->IsDown(KeyCode::Left))
		{
			this->Move(-2, 0);
		}
		else if (input->IsDown(KeyCode::Right))
		{
			this->Move(2, 0);
		}

		// 按下上下键
		if (input->IsDown(KeyCode::Up))
		{
			this->Move(0, -2);
		}
		else if (input->IsDown(KeyCode::Down))
		{
			this->Move(0, 2);
		}

		// 按下鼠标左键，顺时针旋转小人
		if (input->IsDown(MouseButton::Left))
		{
			// 获取当前旋转角度
			float rotation = this->GetRotation();
			// 设置角度值+2
			this->SetRotation(rotation + 2);
		}

		// 点击鼠标右键，隐藏或显示小人
		if (input->WasPressed(MouseButton::Right))
		{
			// 获取当前显示状态
			bool visible = this->IsVisible();
			// 设置相反的显示状态
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
		// 创建人物
		ManPtr man = new Man;
		// 在屏幕上居中显示
		man->SetAnchor(0.5f, 0.5f);
		man->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// 创建说明文字
		TextPtr text = new Text(L"按上下左右键移动\n按鼠标左键旋转\n点击鼠标右键隐藏");
		// 设置文字位置
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
		text->SetAlignment(TextAlign::Center);

		// 添加到场景
		this->AddChild(man);
		this->AddChild(text);
	}
};
