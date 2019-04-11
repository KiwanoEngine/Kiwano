// Copyright (C) 2019 Nomango

#pragma once

#include "kiwano.h"
#include <Box2D/Box2D.h>

using namespace kiwano;

//
// Box2D 非常好地适应了 米/千克/秒 的单位, 所以
// 它并不是以像素为单位进行计算.
// 我们在平衡 Box2D 和 Kiwano 世界的度量时, 需要
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
