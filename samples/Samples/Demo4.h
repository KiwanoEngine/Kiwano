// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// 老虎
E2D_DECLARE_SMART_PTR(Tiger);
class Tiger
	: public Sprite
{
	FramesPtr run_frames;			// 跑步序列帧
	FramesPtr stand_frames;			// 站立序列帧
	bool facing_left;				// 面朝左或面朝右
	bool running;					// 是否正在跑步
	Direction running_direction;	// 跑步方向

public:
	Tiger()
	{
		// 获取图片原始大小
		ImagePtr image = new Image(L"res/tiger.png");
		Size source_size = image->GetSize();
		// 计算每帧图片大小
		Size frame_size = { source_size.x / 5, source_size.y / 3 };

		// 加载帧动画
		run_frames = new Frames;
		for (int i = 0; i < 6; ++i)
		{
			Point pos = { (i % 5) * frame_size.x, (i / 5) * frame_size.y };
			ImagePtr frame = new Image(L"res/tiger.png", Rect{ pos, frame_size });
			run_frames->Add(frame);
		}

		stand_frames = new Frames;
		for (int i = 0; i < 6; ++i)
		{
			Point pos = { ((i + 1) % 5) * frame_size.x, ((i + 1) / 5 + 1) * frame_size.y };
			ImagePtr frame = new Image(L"res/tiger.png", Rect{ pos, frame_size });
			stand_frames->Add(frame);
		}

		// 执行动画
		AddAction(
			Tween::Animation(stand_frames)
			.SetDuration(1000)
			.SetLoops(-1)
		);

		// 添加按键监听
		AddListener(Event::KeyDown, MakeClosure(this, &Tiger::OnKeyDown));
		AddListener(Event::KeyUp, MakeClosure(this, &Tiger::OnKeyUp));

		// 默认方向为 Left
		facing_left = true;
		running = false;

		// 设置锚点
		SetAnchor(0.5f, 0.5f);
	}

	void OnKeyDown(Event const& e)
	{
		if (e.key.code == KeyCode::Left)
			Run(Direction::Left);
		else if (e.key.code == KeyCode::Right)
			Run(Direction::Right);
		else if (e.key.code == KeyCode::Up)
			Run(Direction::Up);
		else if (e.key.code == KeyCode::Down)
			Run(Direction::Down);
	}

	void OnKeyUp(Event const& e)
	{
		switch (e.key.code)
		{
		case KeyCode::Left:
		case KeyCode::Right:
		case KeyCode::Up:
		case KeyCode::Down:
			StopRun();
			break;
		}
	}

	void Run(Direction d)
	{
		if (!running)
		{
			running = true;
			StopAllActions();

			// 执行跑步动画
			AddAction(
				Tween::Animation(run_frames)
				.SetDuration(500)
				.SetLoops(-1)
			);
		}

		running_direction = d;
		if (running_direction == Direction::Left)
		{
			facing_left = true;
		}
		else if (running_direction == Direction::Right)
		{
			facing_left = false;
		}

		// 缩放可以调整图片显示方向
		// 缩放至 -1 图片会反转
		SetScaleX(facing_left ? 1.0f : -1.0f);
	}

	void StopRun()
	{
		if (running)
		{
			running = false;
			StopAllActions();

			// 执行站立动画
			AddAction(
				Tween::Animation(stand_frames)
				.SetDuration(1000)
				.SetLoops(-1)
			);
		}
	}

	void OnUpdate(Duration dt)
	{
		if (running)
		{
			// 计算移动距离
			// OnUpdate 并不是一个稳定间隔执行的函数, 如果想实现稳定
			// 每秒移动 150 像素, 应根据 dt 参数计算移动距离
			const float moving_per_sec = 150;
			const float distance = moving_per_sec * dt.Seconds();

			switch (running_direction)
			{
			case Direction::Up:
				Move(0, -distance);
				break;
			case Direction::Down:
				Move(0, distance);
				break;
			case Direction::Left:
				Move(-distance, 0);
				break;
			case Direction::Right:
				Move(distance, 0);
				break;
			}
		}
	}
};

class Demo4
	: public Scene
{
public:
	static ScenePtr Create()
	{
		return new Demo4;
	}

	Demo4()
	{
		// 创建老虎
		TigerPtr tiger = new Tiger;
		// 在屏幕上居中显示
		tiger->SetAnchor(0.5f, 0.5f);
		tiger->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// 创建说明文字
		TextPtr text = new Text(L"按上下左右键移动");
		// 设置节点大小为文字布局大小
		text->SetSize(text->GetLayoutSize());
		// 设置文字位置
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 80);
		text->SetAlignment(TextAlign::Center);

		// 添加到场景
		this->AddChild(tiger);
		this->AddChild(text);
	}
};
