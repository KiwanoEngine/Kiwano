// Copyright (c) 2016-2018 Easy2D - Nomango
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
#include "../core/include-forwards.h"
#include "../core/Resource.h"

namespace easy2d
{
	class ResLoader
	{
	public:
		void AddImage(String const& id, Resource const& image);

		void AddImage(String const& id, ImagePtr const& image);

		void AddFrames(String const& id, Array<Resource> const& images);

		void AddFrames(String const& id, Array<Pair<Resource, Rect>> const& images);

		void AddFrames(String const& id, Array<ImagePtr> const& images);

		void AddMusic(String const& id, Resource const& music);

		void AddObj(String const& id, ObjectPtr const& obj);

		ImagePtr GetImage(String const& id) const;

		FramesPtr GetFrames(String const& id) const;

		MusicPtr GetMusic(String const& id) const;

		ObjectPtr GetObj(String const& id) const;

		void Delete(String const& id);

		void Destroy();

		// 添加资源搜索路径
		void AddSearchPath(
			String const& path
		);

		template<typename T>
		auto Get(String const& id) const -> decltype(auto)
		{
			auto iter = res_.find(id);
			if (iter == res_.end())
				return T{};
			return dynamic_cast<T>((*iter).second.Get());
		}

	protected:
		UnorderedMap<String, ObjectPtr> res_;
		List<String> search_paths_;
	};
}
