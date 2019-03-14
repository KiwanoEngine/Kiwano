// Copyright (C) 2019 Nomango

#pragma once

#include "common.h"

// т╡пн
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
