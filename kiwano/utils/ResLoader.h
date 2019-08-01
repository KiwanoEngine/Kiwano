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
#include "../macros.h"
#include "../common/helper.h"
#include "../common/Json.h"
#include "../base/Resource.h"
#include "../2d/include-forwards.h"

namespace kiwano
{
	class KGE_API ResLoader
	{
	public:
		// 从 JSON 文件加载资源信息
		bool LoadFromJsonFile(String const& file_path);

		// 从 JSON 加载资源信息
		bool LoadFromJson(Json const& json_data);

		// 添加图片
		bool AddImage(String const& id, Resource const& image);

		// 添加图片
		bool AddImage(String const& id, ImagePtr image);

		// 添加 GIF 图片
		bool AddGifImage(String const& id, Resource const& image);

		// 添加 GIF 图片
		bool AddGifImage(String const& id, GifImagePtr image);

		// 添加帧集合
		size_t AddFrames(String const& id, Array<Resource> const& images);

		// 添加帧集合
		size_t AddFrames(String const& id, Array<ImagePtr> const& images);

		// 添加帧集合
		// 按行列数裁剪图片
		size_t AddFrames(String const& id, Resource const& image, int cols, int rows = 1);

		// 添加帧集合
		// 按指定裁剪矩形裁剪图片
		size_t AddFrames(String const& id, Resource const& image, Array<Rect> const& crop_rects);

		// 添加帧集合
		bool AddFrames(String const& id, FramesPtr frames);

		// 添加对象
		bool AddObj(String const& id, ObjectPtr obj);

		ImagePtr GetImage(String const& id) const;

		GifImagePtr GetGifImage(String const& id) const;

		FramesPtr GetFrames(String const& id) const;

		ObjectPtr GetObj(String const& id) const;

		// 删除指定资源
		void Delete(String const& id);

		// 销毁所有资源
		void Destroy();

		template<typename _Ty>
		_Ty* Get(String const& id) const
		{
			auto iter = res_.find(id);
			if (iter == res_.end())
				return nullptr;
			return dynamic_cast<_Ty*>((*iter).second.Get());
		}

	public:
		ResLoader();

		virtual ~ResLoader();

	protected:
		UnorderedMap<String, ObjectPtr> res_;
	};
}
