// Copyright (C) 2019 Nomango

#include "easy2d.h"
#include "Box2D/Box2D.h"

using namespace easy2d;

//
// Box2D 非常好地适应了 米/千克/秒 的单位, 所以
// 它并不是以像素为单位进行计算.
// 我们在平衡 Box2D 和 Easy2D 世界的度量时, 需要
// 进行一些转换.
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

// 小圆形
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

// 小方块
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

// 固定的木板
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
		// 修改场景大小, 并设置可响应状态, 使场景可以
		// 接收到鼠标 Click 消息
		auto size = Window::Instance().GetSize();
		SetSize(size);
		SetResponsible(true);

		// 添加消息监听
		AddListener(Event::Click, Closure(this, &MainScene::Click));

		// 创建物理世界
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
