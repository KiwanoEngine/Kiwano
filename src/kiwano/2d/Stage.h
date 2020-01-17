// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include <kiwano/2d/Actor.h>
#include <kiwano/render/Brush.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Stage);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief 舞台
	* @details 舞台是所有角色的载体，是导演直接控制的对象
	* @see kiwano::Actor kiwano::Director
	*/
	class KGE_API Stage
		: public Actor
	{
		friend class Transition;
		friend class Director;

	public:
		Stage();

		virtual ~Stage();

		/// \~chinese
		/// @brief 进入舞台时
		/// @details 重载该函数以处理进入舞台前的行为
		virtual void OnEnter();

		/// \~chinese
		/// @brief 退出舞台时
		/// @details 重载该函数以处理退出舞台前的行为
		virtual void OnExit();

		/// \~chinese
		/// @brief 获取角色边界填充画刷
		BrushPtr GetBorderFillBrush() const;

		/// \~chinese
		/// @brief 获取角色边界轮廓画刷
		BrushPtr GetBorderStrokeBrush() const;

		/// \~chinese
		/// @brief 设置角色边界填充画刷
		void SetBorderFillBrush(BrushPtr brush);

		/// \~chinese
		/// @brief 设置角色边界轮廓画刷
		void SetBorderStrokeBrush(BrushPtr brush);

	protected:
		/// \~chinese
		/// @brief 绘制所有子角色的边界
		void RenderBorder(RenderContext& ctx) override;

	private:
		BrushPtr border_fill_brush_;
		BrushPtr border_stroke_brush_;
	};

	/** @} */

	inline BrushPtr Stage::GetBorderFillBrush() const
	{
		return border_fill_brush_;
	}

	inline BrushPtr Stage::GetBorderStrokeBrush() const
	{
		return border_stroke_brush_;
	}

	inline void Stage::SetBorderFillBrush(BrushPtr brush)
	{
		border_fill_brush_ = brush;
	}

	inline void Stage::SetBorderStrokeBrush(BrushPtr brush)
	{
		border_stroke_brush_ = brush;
	}

}
