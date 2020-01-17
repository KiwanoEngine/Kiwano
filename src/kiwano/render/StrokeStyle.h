// Copyright (c) 2016-2019 Kiwano - Nomango
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
#include <kiwano/render/dx/D2DDeviceResources.h>

namespace kiwano
{
	class RenderContext;
	class Renderer;

	/**
	* \addtogroup Render
	* @{
	*/

	/// \~chinese
	/// @brief �����˵���ʽ
	/// @details �����˵���ʽ��ʾ�߶ζ˵㲿�ֵ���״
	enum class CapStyle
	{
		Flat,		///< ��˵�
		Square,		///< ���ζ˵㣬����ͻ�����ֵ����߶ο�ȵ�һ��
		Round,		///< Բ�ζ˵㣬Բֱ�������߶ο��
		Triangle,	///< ������ʽ������б�߳��ȵ����߶ο��
	};

	/// \~chinese
	/// @brief ����������ʽ
	/// @details ����������ʽ��ʾ�������ཻ���ֵ���״
	enum class LineJoinStyle
	{
		Miter,	///< б����ʽ
		Bevel,	///< б����ʽ
		Round	///< Բ����ʽ
	};

	/// \~chinese
	/// @brief ����������ʽ
	/// @details ����������ʽ��ʾ�߶εļ�϶
	enum class DashStyle
	{
		Solid,		///< �޼�ϵ�ʵ��
		Dash,		///< б����ʽ
		Dot,		///< Բ����ʽ
		DashDot,	///< Բ����ʽ
		DashDotDot,	///< Բ����ʽ
	};

	/// \~chinese
	/// @brief ������ʽ
	class StrokeStyle
	{
		friend class RenderContext;
		friend class Renderer;

	public:
		StrokeStyle();

		/// \~chinese
		/// @brief �Ƿ���Ч
		bool IsValid() const;

		/// \~chinese
		/// @brief ����������ʽ
		/// @param cap �����˵���ʽ
		/// @param line_join ����������ʽ
		/// @param dash ����������ʽ
		/// @param dash_offset ��������ƫ����
		static StrokeStyle Create(CapStyle cap, LineJoinStyle line_join = LineJoinStyle::Miter, DashStyle dash = DashStyle::Solid, float dash_offset = 0.0f);

		/// \~chinese
		/// @brief ����������ʽ
		/// @param cap �����˵���ʽ
		/// @param line_join ����������ʽ
		/// @param dash_array �������ߵĳ������϶����
		/// @param dash_size �������������С
		/// @param dash_offset ��������ƫ����
		static StrokeStyle Create(CapStyle cap, LineJoinStyle line_join = LineJoinStyle::Miter, const float* dash_array = nullptr, size_t dash_size = 0, float dash_offset = 0.0f);

		/// \~chinese
		/// @brief ����������ʽ
		/// @tparam _DashSize �������������С
		/// @param cap �����˵���ʽ
		/// @param line_join ����������ʽ
		/// @param dash_array �������ߵĳ������϶����
		/// @param dash_offset ��������ƫ����
		template <size_t _DashSize>
		static StrokeStyle Create(CapStyle cap, LineJoinStyle line_join = LineJoinStyle::Miter, float(&dash_array)[_DashSize] = nullptr, float dash_offset = 0.0f)
		{
			return StrokeStyle::Create(cap, line_join, dash_array, _DashSize, dash_offset);
		}

	private:
		ComPtr<ID2D1StrokeStyle> GetStrokeStyle() const;

		void SetStrokeStyle(ComPtr<ID2D1StrokeStyle> style);

	private:
		ComPtr<ID2D1StrokeStyle> style_;
	};

	/** @} */

	inline bool StrokeStyle::IsValid() const
	{
		return true;  // Always valid
	}

	inline ComPtr<ID2D1StrokeStyle> StrokeStyle::GetStrokeStyle() const
	{
		return style_;
	}

	inline void StrokeStyle::SetStrokeStyle(ComPtr<ID2D1StrokeStyle> style)
	{
		style_ = style;
	}

}
