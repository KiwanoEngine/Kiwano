// Copyright (C) 2019 Nomango

#include "easy2d.h"
#include "Box2D/Box2D.h"

using namespace easy2d;

//
// Box2D �ǳ��õ���Ӧ�� ��/ǧ��/�� �ĵ�λ, ����
// ��������������Ϊ��λ���м���.
// ������ƽ�� Box2D �� Easy2D ����Ķ���ʱ, ��Ҫ
// ����һЩת��.
//
namespace
{
	const float GLOBAL_SCALE = 100.0f;

	b2Vec2 Vec2Convert(const Point& pos)
	{
		return b2Vec2(pos.x / GLOBAL_SCALE, pos.y / GLOBAL_SCALE);
	}

	Point Vec2Convert(const b2Vec2& pos)
	{
		return Point(pos.x * GLOBAL_SCALE, pos.y * GLOBAL_SCALE);
	}
}

// СԲ��
E2D_DECLARE_SMART_PTR(Circle);
class Circle
	: public Sprite
{
public:
	Circle(b2World* world, const Point& pos)
	{
		Load(L"circle.png");
		SetAnchor(0.5f, 0.5f);
		SetScale(0.7f);

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Vec2Convert(pos);

		b2Body* body = world->CreateBody(&bodyDef);

		SetUserData(body);

		b2CircleShape shape;
		shape.m_radius = GetWidth() / GLOBAL_SCALE / 2 * 0.7f;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		body->CreateFixture(&fixtureDef);
		body->SetUserData(this);
	}
};

// С����
E2D_DECLARE_SMART_PTR(Square);
class Square
	: public Sprite
{
public:
	Square(b2World* world, const Point& pos)
	{
		Load(L"square.png");
		SetAnchor(0.5f, 0.5f);
		SetScale(0.7f);

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = Vec2Convert(pos);

		b2Body* body = world->CreateBody(&bodyDef);

		SetUserData(body);

		b2PolygonShape shape;
		b2Vec2 sz = Vec2Convert(GetSize() / 2 * 0.7f);
		shape.SetAsBox(sz.x, sz.y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		body->CreateFixture(&fixtureDef);
		body->SetUserData(this);
	}
};

// �̶���ľ��
E2D_DECLARE_SMART_PTR(Board);
class Board
	: public GeometryNode
{
public:
	Board(b2World* world, const Size& size, const Point& pos)
	{
		GeometryPtr geo = new RectangleGeometry(Point(), size);
		SetGeometry(geo);
		SetStrokeColor(Color::White);
		SetFillColor(Color(0, 0, 0, 0));

		SetSize(size);
		SetAnchor(0.5f, 0.5f);
		SetRotation(10);
		SetPosition(pos);

		b2BodyDef groundBodyDef;
		groundBodyDef.position = Vec2Convert(GetPosition());
		groundBodyDef.angle = 10 * math::constants::PI_F / 180.f;

		b2Body* groundBody = world->CreateBody(&groundBodyDef);

		b2PolygonShape groundBox;
		b2Vec2 sz = Vec2Convert(Point{ size.x / 2, size.y / 2 });
		groundBox.SetAsBox(sz.x, sz.y);
		groundBody->CreateFixture(&groundBox, 0.0f);
	}
};

class MainScene
	: public Scene
{
	b2World* world_;

public:
	MainScene()
	{
		// �޸ĳ�����С, �����ÿ���Ӧ״̬, ʹ��������
		// ���յ���� Click ��Ϣ
		auto size = Window::Instance().GetSize();
		SetSize(size);
		SetResponsible(true);

		// �����Ϣ����
		AddListener(Event::Click, Closure(this, &MainScene::Click));

		// ������������
		world_ = new b2World(b2Vec2(0, 10));

		BoardPtr board = new Board(world_, Size(GetWidth() - 100, 20), Point(size.x / 2, size.y - 50));
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

int main()
{
	try
	{
		Application app;
		app.Init();

		ScenePtr scene = new MainScene;
		app.EnterScene(scene);
		app.Run();
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_TASKMODAL);
	}
	return 0;
}
