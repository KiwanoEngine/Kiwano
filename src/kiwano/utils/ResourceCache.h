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
	/// \~chinese
	/// @brief 资源缓存
	/// @details 资源缓存
	class KGE_API ResourceCache
		: public Singleton<ResourceCache>
	{
		friend Singleton<ResourceCache>;

	public:
		/// \~chinese
		/// @brief 从 JSON 文件加载资源信息
		/// @param file_path JSON文件路径
		bool LoadFromJsonFile(String const& file_path);

		/// \~chinese
		/// @brief 从 JSON 加载资源信息
		/// @param json_data JSON对象
		bool LoadFromJson(Json const& json_data);

		/// \~chinese
		/// @brief 从 XML 文件加载资源信息
		/// @param file_path XML文件路径
		bool LoadFromXmlFile(String const& file_path);

		/// \~chinese
		/// @brief 从 XML 文档对象加载资源信息
		/// @param doc XML文档对象
		bool LoadFromXml(const tinyxml2::XMLDocument* doc);

		/// \~chinese
		/// @brief 获取资源
		/// @param id 对象ID
		ObjectBasePtr Get(String const& id) const;

		/// \~chinese
		/// @brief 获取资源
		/// @tparam _Ty 对象类型
		/// @param id 对象ID
		/// @return 指定对象类型的指针
		template<typename _Ty>
		SmartPtr<_Ty> Get(String const& id) const
		{
			return dynamic_cast<_Ty*>(Get(id).get());
		}

		/// \~chinese
		/// @brief 将对象放入缓存
		/// @param id 对象ID
		/// @param obj 对象
		bool AddObject(String const& id, ObjectBasePtr obj);

		/// \~chinese
		/// @brief 从多个本地图片构建序列帧，并放入缓存
		/// @param id 对象ID
		/// @param files 本地图片路径集合
		/// @return 序列帧的帧数量
		size_t AddFrameSequence(String const& id, Vector<String> const& files);

		/// \~chinese
		/// @brief 从多个图片资源构建序列帧，并放入缓存
		/// @param id 对象ID
		/// @param resources 图片资源集合
		/// @return 序列帧的帧数量
		size_t AddFrameSequence(String const& id, Vector<Resource> const& resources);

		/// \~chinese
		/// @brief 从多个图像帧构建序列帧，并放入缓存
		/// @param id 对象ID
		/// @param frames 图像帧集合
		/// @return 序列帧的帧数量
		size_t AddFrameSequence(String const& id, Vector<FramePtr> const& frames);

		/// \~chinese
		/// @brief 将图像帧按行列分割构建序列帧，并放入缓存
		/// @brief 按行列数裁剪图片
		/// @param id 对象ID
		/// @param frame 图像帧
		/// @param cols 列
		/// @param rows 行
		/// @param padding_x X方向间隔
		/// @param padding_y Y方向间隔
		/// @return 序列帧的帧数量
		size_t AddFrameSequence(String const& id, FramePtr frame, int cols, int rows = 1, float padding_x = 0, float padding_y = 0);

		/// \~chinese
		/// @brief 删除指定资源
		/// @param id 对象ID
		void Remove(String const& id);

		/// \~chinese
		/// @brief 清空所有资源
		void Clear();

	private:
		ResourceCache();

		virtual ~ResourceCache();

	private:
		UnorderedMap<String, ObjectBasePtr> object_cache_;
	};
}
