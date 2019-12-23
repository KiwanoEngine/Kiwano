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
#include <kiwano/2d/Frame.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(Sprite);

	/**
	* \addtogroup Actors
	* @{
	*/

	/**
	* \~chinese
	* @brief 精灵
	*/
	class KGE_API Sprite
		: public Actor
	{
	public:
		Sprite();

		/// \~chinese
		/// @brief 从本地图片构造精灵
		/// @param file_path 本地图片路径
		explicit Sprite(String const& file_path);

		/// \~chinese
		/// @brief 从图片资源构造精灵
		/// @param res 图片资源
		explicit Sprite(Resource const& res);

		/// \~chinese
		/// @brief 从图像帧构造精灵
		/// @param[in] frame 图像帧
		explicit Sprite(FramePtr frame);

		/// \~chinese
		/// @brief 从本地图片构造精灵并裁剪
		/// @param file_path 本地图片路径
		/// @param crop_rect 裁剪矩形
		Sprite(String const& file_path, Rect const& crop_rect);

		/// \~chinese
		/// @brief 从图片资源构造精灵并裁剪
		/// @param res 图片资源
		/// @param crop_rect 裁剪矩形
		Sprite(Resource const& res, Rect const& crop_rect);

		virtual ~Sprite();

		/// \~chinese
		/// @brief 加载本地图片
		/// @param file_path 本地图片路径
		bool Load(String const& file_path);

		/// \~chinese
		/// @brief 加载图像资源
		/// @param res 图片资源
		bool Load(Resource const& res);

		/// \~chinese
		/// @brief 使用矩形区域裁剪精灵
		/// @param crop_rect 裁剪矩形
		void SetCropRect(const Rect& crop_rect);

		/// \~chinese
		/// @brief 获取帧图像
		FramePtr GetFrame() const;

		/// \~chinese
		/// @brief 设置图像帧
		/// @param[in] frame 图像帧
		void SetFrame(FramePtr frame);

		void OnRender(RenderTarget* rt) override;

	private:
		FramePtr frame_;
	};

	/** @} */

	inline FramePtr Sprite::GetFrame() const { return frame_; }

}
