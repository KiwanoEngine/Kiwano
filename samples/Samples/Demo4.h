// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// �ϻ�
E2D_DECLARE_SMART_PTR(Tiger);
class Tiger
	: public Sprite
{
	FramesPtr run_frames;			// �ܲ�����֡
	FramesPtr stand_frames;			// վ������֡
	bool facing_left;				// �泯����泯��
	bool running;					// �Ƿ������ܲ�
	Direction running_direction;	// �ܲ�����

public:
	Tiger()
	{
		// ��ȡͼƬԭʼ��С
		ImagePtr image = new Image(L"res/tiger.png");
		Size source_size = image->GetSize();
		// ����ÿ֡ͼƬ��С
		Size frame_size = { source_size.x / 5, source_size.y / 3 };

		// ����֡����
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

		// ִ�ж���
		AddAction(
			Tween::Animation(stand_frames)
			.SetDuration(1000)
			.SetLoops(-1)
		);

		// ��Ӱ�������
		AddListener(Event::KeyDown, MakeClosure(this, &Tiger::OnKeyDown));
		AddListener(Event::KeyUp, MakeClosure(this, &Tiger::OnKeyUp));

		// Ĭ�Ϸ���Ϊ Left
		facing_left = true;
		running = false;

		// ����ê��
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

			// ִ���ܲ�����
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

		// ���ſ��Ե���ͼƬ��ʾ����
		// ������ -1 ͼƬ�ᷴת
		SetScaleX(facing_left ? 1.0f : -1.0f);
	}

	void StopRun()
	{
		if (running)
		{
			running = false;
			StopAllActions();

			// ִ��վ������
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
			// �����ƶ�����
			// OnUpdate ������һ���ȶ����ִ�еĺ���, �����ʵ���ȶ�
			// ÿ���ƶ� 150 ����, Ӧ���� dt ���������ƶ�����
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
		// �����ϻ�
		TigerPtr tiger = new Tiger;
		// ����Ļ�Ͼ�����ʾ
		tiger->SetAnchor(0.5f, 0.5f);
		tiger->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// ����˵������
		TextPtr text = new Text(L"���������Ҽ��ƶ�");
		// ���ýڵ��СΪ���ֲ��ִ�С
		text->SetSize(text->GetLayoutSize());
		// ��������λ��
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 80);
		text->SetAlignment(TextAlign::Center);

		// ��ӵ�����
		this->AddChild(tiger);
		this->AddChild(text);
	}
};
