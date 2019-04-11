// Copyright (C) 2019 Nomango

#pragma once

#include "Circle.h"
#include "Rect.h"
#include "Board.h"

KGE_DECLARE_SMART_PTR(MainScene);
class MainScene
	: public Scene
{
	b2World* world_;

public:
	MainScene()
	{
		// ���ÿ���Ӧ״̬, ʹ�������Խ��յ���� Click ��Ϣ
		SetResponsible(true);

		// �����Ϣ����
		AddListener(Event::Click, MakeClosure(this, &MainScene::Click));

		// ������������
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
		// ���� Box2D ����
		world_->Step(dt.Seconds(), 6, 2);

		// ����ÿһ����������λ�ú���ת�Ƕ�
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

				// �Ƴ����䵽�����������
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
