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
		// �� JSON �ļ�������Դ��Ϣ
		bool LoadFromJsonFile(String const& file_path);

		// �� JSON ������Դ��Ϣ
		bool LoadFromJson(Json const& json_data);

		// ���ͼƬ
		bool AddImage(String const& id, Resource const& image);

		// ���ͼƬ
		bool AddImage(String const& id, ImagePtr image);

		// ��� GIF ͼƬ
		bool AddGifImage(String const& id, Resource const& image);

		// ��� GIF ͼƬ
		bool AddGifImage(String const& id, GifImagePtr image);

		// ���֡����
		size_t AddFrames(String const& id, Array<Resource> const& images);

		// ���֡����
		size_t AddFrames(String const& id, Array<ImagePtr> const& images);

		// ���֡����
		// ���������ü�ͼƬ
		size_t AddFrames(String const& id, Resource const& image, int cols, int rows = 1);

		// ���֡����
		// ��ָ���ü����βü�ͼƬ
		size_t AddFrames(String const& id, Resource const& image, Array<Rect> const& crop_rects);

		// ���֡����
		bool AddFrames(String const& id, FramesPtr frames);

		// ��Ӷ���
		bool AddObj(String const& id, ObjectPtr obj);

		ImagePtr GetImage(String const& id) const;

		GifImagePtr GetGifImage(String const& id) const;

		FramesPtr GetFrames(String const& id) const;

		ObjectPtr GetObj(String const& id) const;

		// ɾ��ָ����Դ
		void Delete(String const& id);

		// ����������Դ
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
