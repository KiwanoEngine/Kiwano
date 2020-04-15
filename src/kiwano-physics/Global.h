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
#include <kiwano/kiwano.h>

// Box2D
#include <Box2D/Box2D.h>

namespace kiwano
{
namespace physics
{

KGE_DECLARE_SMART_PTR(PhysicWorld);
KGE_DECLARE_SMART_PTR(PhysicBody);

namespace global
{

/// \~chinese
/// @brief 获取全局缩放比例
/// @details 缩放比例是指由物理世界的单位米转换到屏幕像素的比例，默认比例为1:100
float GetScale();

/// \~chinese
/// @brief 设置全局缩放比例
/// @details 缩放比例是指由物理世界的单位米转换到屏幕像素的比例，默认比例为1:100
void SetScale(float scale);

/// \~chinese
/// @brief 游戏世界单位转换为物理世界单位
/// @details 根据全局缩放比例将物理世界的单位米转换为像素单位
float WorldToLocal(float value);

/// \~chinese
/// @brief 游戏世界单位转换为物理世界单位
/// @details 根据全局缩放比例将物理世界的单位米转换为像素单位
Vec2 WorldToLocal(const b2Vec2& pos);

/// \~chinese
/// @brief 物理世界单位转换为游戏世界单位
/// @details 根据全局缩放比例将像素单位转换为物理世界的单位米
float LocalToWorld(float value);

/// \~chinese
/// @brief 物理世界单位转换为游戏世界单位
/// @details 根据全局缩放比例将像素单位转换为物理世界的单位米
b2Vec2 LocalToWorld(const Vec2& pos);

}  // namespace global
}  // namespace physics
}  // namespace kiwano
