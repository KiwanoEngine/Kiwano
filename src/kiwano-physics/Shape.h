// Copyright (c) 2018-2019 Kiwano - Nomango
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include <kiwano-physics/helper.h>

namespace kiwano
{
namespace physics
{
class World;
class Fixture;

/**
 * \addtogroup Physics
 * @{
 */

/// \~chinese
/// @brief 形状基类
class KGE_API Shape
{
    friend class Fixture;

public:
    Shape();
    Shape(b2Shape* shape);

    b2Shape* GetB2Shape() const;
    void     SetB2Shape(b2Shape* shape);

private:
    virtual void FitWorld(World* world) {}

private:
    b2Shape* shape_;
};

/// \~chinese
/// @brief 圆形形状
class KGE_API CircleShape : public Shape
{
public:
    CircleShape();

    CircleShape(float radius, Point const& offset = Point());

    void Set(float radius, Point const& offset = Point());

private:
    void FitWorld(World* world) override;

private:
    float         radius_;
    Point         offset_;
    b2CircleShape circle_;
};

/// \~chinese
/// @brief 盒子形状
class KGE_API BoxShape : public Shape
{
public:
    BoxShape();

    BoxShape(Vec2 const& size, Point const& offset = Point(), float rotation = 0.f);

    void Set(Vec2 const& size, Point const& offset = Point(), float rotation = 0.f);

private:
    void FitWorld(World* world) override;

private:
    float          rotation_;
    Vec2           box_size_;
    Point          offset_;
    b2PolygonShape polygon_;
};

/// \~chinese
/// @brief 多边形形状
class KGE_API PolygonShape : public Shape
{
public:
    PolygonShape();

    PolygonShape(Vector<Point> const& vertexs);

    void Set(Vector<Point> const& vertexs);

private:
    void FitWorld(World* world) override;

private:
    Vector<Point>  vertexs_;
    b2PolygonShape polygon_;
};

/// \~chinese
/// @brief 线段形状, 用于表示一条边
class KGE_API EdgeShape : public Shape
{
public:
    EdgeShape();

    EdgeShape(Point const& p1, Point const& p2);

    void Set(Point const& p1, Point const& p2);

private:
    void FitWorld(World* world) override;

private:
    Point       p_[2];
    b2EdgeShape edge_;
};

/// \~chinese
/// @brief 链式形状
class KGE_API ChainShape : public Shape
{
public:
    ChainShape();

    ChainShape(Vector<Point> const& vertexs, bool loop = false);

    void Set(Vector<Point> const& vertexs, bool loop = false);

private:
    void FitWorld(World* world) override;

private:
    bool          loop_;
    Vector<Point> vertexs_;
    b2ChainShape  chain_;
};

/** @} */
}  // namespace physics
}  // namespace kiwano
