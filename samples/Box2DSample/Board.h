// Copyright (C) 2019 Nomango

#pragma once

#include "common.h"

// ¹Ì¶¨µÄÄ¾°å
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
