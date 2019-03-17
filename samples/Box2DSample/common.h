// Copyright (C) 2019 Nomango

#pragma once

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
