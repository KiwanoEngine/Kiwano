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
#include <kiwano/2d/Layer.h>

namespace kiwano
{
	namespace imgui
	{
		KGE_DECLARE_SMART_PTR(ImGuiLayer);

		/// \~chinese
		/// @brief ImGui�ܵ�
		using ImGuiPipeline = Function<void()>;

		/**
		* \~chinese
		* @brief ImGuiͼ��
		*/
		class ImGuiLayer
			: public Layer
		{
		public:
			ImGuiLayer();

			virtual ~ImGuiLayer();

			/// \~chinese
			/// @brief ��� ImGui Ԫ��
			/// @param item �ܵ�
			/// @param name Ԫ������
			void AddItem(ImGuiPipeline const& item, String const& name);

			/// \~chinese
			/// @brief �Ƴ� ImGui Ԫ��
			/// @param name Ԫ������
			void RemoveItem(String const& name);

			// �Ƴ�����Ԫ��
			/// \~chinese
			/// @brief �Ƴ�����Ԫ��
			void RemoveAllItems();

		public:
			void OnRender(RenderTarget* rt) override;

		private:
			Map<String, ImGuiPipeline> pipelines_;
		};
	}
}
