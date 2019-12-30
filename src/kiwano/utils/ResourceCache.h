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
	/// @brief ��Դ����
	/// @details ��Դ����
	class KGE_API ResourceCache
		: public Singleton<ResourceCache>
	{
		friend Singleton<ResourceCache>;

	public:
		/// \~chinese
		/// @brief �� JSON �ļ�������Դ��Ϣ
		/// @param file_path JSON�ļ�·��
		bool LoadFromJsonFile(String const& file_path);

		/// \~chinese
		/// @brief �� JSON ������Դ��Ϣ
		/// @param json_data JSON����
		bool LoadFromJson(Json const& json_data);

		/// \~chinese
		/// @brief �� XML �ļ�������Դ��Ϣ
		/// @param file_path XML�ļ�·��
		bool LoadFromXmlFile(String const& file_path);

		/// \~chinese
		/// @brief �� XML �ĵ����������Դ��Ϣ
		/// @param doc XML�ĵ�����
		bool LoadFromXml(const tinyxml2::XMLDocument* doc);

		/// \~chinese
		/// @brief ��ȡ��Դ
		/// @param id ����ID
		ObjectBasePtr Get(String const& id) const;

		/// \~chinese
		/// @brief ��ȡ��Դ
		/// @tparam _Ty ��������
		/// @param id ����ID
		/// @return ָ���������͵�ָ��
		template<typename _Ty>
		SmartPtr<_Ty> Get(String const& id) const
		{
			return dynamic_cast<_Ty*>(Get(id).get());
		}

		/// \~chinese
		/// @brief ��������뻺��
		/// @param id ����ID
		/// @param obj ����
		bool AddObject(String const& id, ObjectBasePtr obj);

		/// \~chinese
		/// @brief �Ӷ������ͼƬ��������֡�������뻺��
		/// @param id ����ID
		/// @param files ����ͼƬ·������
		/// @return ����֡��֡����
		size_t AddFrameSequence(String const& id, Vector<String> const& files);

		/// \~chinese
		/// @brief �Ӷ��ͼƬ��Դ��������֡�������뻺��
		/// @param id ����ID
		/// @param resources ͼƬ��Դ����
		/// @return ����֡��֡����
		size_t AddFrameSequence(String const& id, Vector<Resource> const& resources);

		/// \~chinese
		/// @brief �Ӷ��ͼ��֡��������֡�������뻺��
		/// @param id ����ID
		/// @param frames ͼ��֡����
		/// @return ����֡��֡����
		size_t AddFrameSequence(String const& id, Vector<FramePtr> const& frames);

		/// \~chinese
		/// @brief ��ͼ��֡�����зָ������֡�������뻺��
		/// @brief ���������ü�ͼƬ
		/// @param id ����ID
		/// @param frame ͼ��֡
		/// @param cols ��
		/// @param rows ��
		/// @param padding_x X������
		/// @param padding_y Y������
		/// @return ����֡��֡����
		size_t AddFrameSequence(String const& id, FramePtr frame, int cols, int rows = 1, float padding_x = 0, float padding_y = 0);

		/// \~chinese
		/// @brief ɾ��ָ����Դ
		/// @param id ����ID
		void Remove(String const& id);

		/// \~chinese
		/// @brief ���������Դ
		void Clear();

	private:
		ResourceCache();

		virtual ~ResourceCache();

	private:
		UnorderedMap<String, ObjectBasePtr> object_cache_;
	};
}
