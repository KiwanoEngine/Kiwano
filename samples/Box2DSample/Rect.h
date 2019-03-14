// Copyright (C) 2019 Nomango

#pragma once

#include "common.h"

// ·½¿é
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
