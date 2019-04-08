// Copyright (C) 2019 Nomango

#pragma once

#include "Circle.h"
#include "Rect.h"
#include "Board.h"

E2D_DECLARE_SMART_PTR(MainScene);
class MainScene
	: public Scene
{
	b2World* world_;

public:
	MainScene()
	{
		// 设置可响应状态, 使场景可以接收到鼠标 Click 消息
		SetResponsible(true);

		// 添加消息监听
		AddListener(Event::Click, MakeClosure(this, &MainScene::Click));

		// 创建物理世界
		world_ = new b2World(b2Vec2(0, 10));

		BoardPtr board = new Board(world_, Size(GetWidth() - 100, 20), Point(GetWidth() / 2, GetHeight() - 50));
		AddChild(board);

		CirclePtr circle = new Circle(world_, Point(320, 240));
		AddChild(circle);
	}

	~MainScene()
	{
		if (world_)
			delete world_;
	}

	void OnUpdate(Duration dt) override
	{
		// 更新 Box2D 世界
		world_->Step(dt.Seconds(), 6, 2);

		// 更新每一个物理对象的位置和旋转角度
		b2Body* body = world_->GetBodyList();
		while (body)
		{
			Node* node = (Node*)body->GetUserData();
			b2Body* next = body->GetNext();
			if (node)
			{
				const b2Vec2& pos = body->GetPosition();
				node->SetPosition(Vec2Convert(pos));
				node->SetRotation(body->GetAngle() * 180.f / math::constants::PI_F);

				// 移除掉落到场景外的物体
				if (node->GetPosition().y > GetHeight() + 50)
				{
					body->SetUserData(0);
					world_->DestroyBody(body);

					node->RemoveFromParent();
				}
			}

			body = next;
		}
	}

	void Click(Event const& evt)
	{
		if (evt.mouse.button == MouseButton::Left)
		{
			CirclePtr circle = new Circle(world_, Point{ evt.mouse.x, evt.mouse.y });
			AddChild(circle);
		}
		else if (evt.mouse.button == MouseButton::Right)
		{
			SquarePtr rect = new Square(world_, Point{ evt.mouse.x, evt.mouse.y });
			AddChild(rect);
		}
	}
};
