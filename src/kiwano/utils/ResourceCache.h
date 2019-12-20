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
#include <kiwano/core/Resource.h>
#include <kiwano/2d/include-forwards.h>
#include <kiwano/renderer/GifImage.h>
#include <3rd-party/tinyxml2/tinyxml2.h>

namespace kiwano
{
	// 资源缓存
	class KGE_API ResourceCache
		: public Singleton<ResourceCache>
	{
		friend Singleton<ResourceCache>;

	public:
		// 从 JSON 文件加载资源信息
		bool LoadFromJsonFile(String const& file_path);

		// 从 JSON 加载资源信息
		bool LoadFromJson(Json const& json_data);

		// 从 XML 文件加载资源信息
		bool LoadFromXmlFile(String const& file_path);

		// 从 XML 文档对象加载资源信息
		bool LoadFromXml(const tinyxml2::XMLDocument* doc);

		// 添加帧图像
		bool AddFrame(String const& id, String const& file_path);

		// 添加帧图像
		bool AddFrame(String const& id, FramePtr frame);

		// 添加序列帧
		size_t AddFrameSequence(String const& id, Vector<String> const& files);

		// 添加序列帧
		// 按行列数裁剪图片
		size_t AddFrameSequence(String const& id, String const& file_path, int cols, int rows = 1, float padding_x = 0, float padding_y = 0);

		// 添加序列帧
		size_t AddFrameSequence(String const& id, FrameSequencePtr frames);

		// 添加对象
		bool AddObjectBase(String const& id, ObjectBasePtr obj);

		// 添加 GIF 图像
		bool AddGifImage(String const& id, GifImage const& gif);

		// 添加 GIF 图像
		bool AddGifImage(String const& id, String const& file_path);

		// 添加字体集
		bool AddFontCollection(String const& id, FontCollection const& collection);

		// 获取帧图像
		FramePtr GetFrame(String const& id) const;

		// 获取序列帧
		FrameSequencePtr GetFrameSequence(String const& id) const;

		// 获取 GIF 图像
		GifImage GetGifImage(String const& id) const;

		// 获取字体集
		FontCollection GetFontCollection(String const& id) const;

		// 删除指定资源
		void Remove(String const& id);

		// 清空所有资源
		void Clear();

		template<typename _Ty>
		_Ty* Get(String const& id) const
		{
			auto iter = object_cache_.find(id);
			if (iter == object_cache_.end())
				return nullptr;
			return const_cast<_Ty*>(dynamic_cast<const _Ty*>((*iter).second.get()));
		}

	protected:
		ResourceCache();

		virtual ~ResourceCache();

	protected:
		UnorderedMap<String, ObjectBasePtr>		object_cache_;
		UnorderedMap<String, GifImage>			gif_cache_;
		UnorderedMap<String, FontCollection>	font_collection_cache_;
	};
}
