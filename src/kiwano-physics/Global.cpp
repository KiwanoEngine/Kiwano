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

#include <kiwano-physics/Global.h>

namespace kiwano
{
namespace physics
{

namespace
{
float global_scale = 100.f;  // 100 pixels per meters
}

float GetScale()
{
    return global_scale;
}

void SetScale(float scale)
{
    global_scale = scale;
}

float WorldToLocal(float value)
{
    return value * global_scale;
}

Vec2 WorldToLocal(const b2Vec2& pos)
{
    return Point(WorldToLocal(pos.x), WorldToLocal(pos.y));
}

Vector<Vec2> WorldToLocal(const Vector<b2Vec2>& b2vertexs)
{
    Vector<Vec2> vertexs;
    vertexs.reserve(b2vertexs.size());
    for (const auto& v : b2vertexs)
    {
        vertexs.push_back(WorldToLocal(v));
    }
    return vertexs;
}

float LocalToWorld(float value)
{
    return value / global_scale;
}

b2Vec2 LocalToWorld(const Vec2& pos)
{
    return b2Vec2(LocalToWorld(pos.x), LocalToWorld(pos.y));
}

Vector<b2Vec2> LocalToWorld(const Vector<Vec2>& vertexs)
{
    Vector<b2Vec2> b2vertexs;
    b2vertexs.reserve(vertexs.size());
    for (const auto& v : vertexs)
    {
        b2vertexs.push_back(LocalToWorld(v));
    }
    return b2vertexs;
}

}  // namespace physics
}  // namespace kiwano
