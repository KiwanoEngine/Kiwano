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
#include "../base/Resource.h"
#include "../2d/include-forwards.h"
#include "../renderer/GifImage.h"
#include "../third-party/tinyxml2/tinyxml2.h"

namespace kiwano
{
	// 资源缓存
	class KGE_API ResourceCache
		: public Singleton<ResourceCache>
	{
		KGE_DECLARE_SINGLETON(ResourceCache);

	public:
		// 从 JSON 文件加载资源信息
		bool LoadFromJsonFile(String const& file_path);

		// 从 JSON 加载资源信息
		bool LoadFromJson(Json const& json_data);

		// 从 XML 文件加载资源信息
		bool LoadFromXmlFile(String const& file_path);

		// 从 XML 文档对象加载资源信息
		bool LoadFromXml(tinyxml2::XMLDocument* doc);

		// 添加帧图像
		bool AddFrame(String const& id, String const& file_path);

		// 添加帧图像
		bool AddFrame(String const& id, FramePtr frame);

		// 添加序列帧
		UInt32 AddFrameSequence(String const& id, Vector<String> const& files);

		// 添加序列帧
		// 按行列数裁剪图片
		UInt32 AddFrameSequence(String const& id, String const& file_path, Int32 cols, Int32 rows = 1, Float32 padding_x = 0, Float32 padding_y = 0);

		// 添加序列帧
		UInt32 AddFrameSequence(String const& id, FrameSequencePtr frames);

		// 添加对象
		bool AddObjectBase(String const& id, ObjectBasePtr obj);

		// 获取帧图像
		FramePtr GetFrame(String const& id) const;

		// 获取序列帧
		FrameSequencePtr GetFrameSequence(String const& id) const;

		// 删除指定资源
		void Delete(String const& id);

		// 清空所有资源
		void Clear();

		template<typename _Ty>
		_Ty* Get(String const& id) const
		{
			auto iter = cache_.find(id);
			if (iter == cache_.end())
				return nullptr;
			return dynamic_cast<_Ty*>((*iter).second.get());
		}

	protected:
		ResourceCache();

		virtual ~ResourceCache();

	protected:
		UnorderedMap<String, ObjectBasePtr> cache_;
	};
}
