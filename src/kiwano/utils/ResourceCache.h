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
#include <kiwano/2d/Frame.h>
#include <kiwano/2d/FrameSequence.h>
#include <kiwano/renderer/GifImage.h>
#include <kiwano/renderer/Font.h>
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

		// 添加对象
		bool AddObject(String const& id, ObjectBasePtr obj);

		// 添加序列帧
		size_t AddFrameSequence(String const& id, Vector<String> const& files);

		// 添加序列帧
		// 按行列数裁剪图片
		size_t AddFrameSequence(String const& id, String const& file_path, int cols, int rows = 1, float padding_x = 0, float padding_y = 0);

		// 删除指定资源
		void Remove(String const& id);

		// 清空所有资源
		void Clear();

		ObjectBasePtr Get(String const& id) const;

		template<typename _Ty>
		SmartPtr<_Ty> Get(String const& id) const
		{
			return dynamic_cast<_Ty*>(Get(id).get());
		}

	private:
		ResourceCache();

		virtual ~ResourceCache();

	private:
		UnorderedMap<String, ObjectBasePtr> object_cache_;
	};
}
