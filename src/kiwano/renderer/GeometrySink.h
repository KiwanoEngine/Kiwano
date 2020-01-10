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
#include <kiwano/renderer/Geometry.h>

namespace kiwano
{
	class RenderContext;
	class Renderer;

	/**
	* \addtogroup Render
	* @{
	*/

	/// \~chinese
	/// @brief 几何形状组合方式
	enum class CombineMode
	{
		Union,		///< 并集 (A + B)
		Intersect,	///< 交集 (A + B)
		Xor,		///< 对称差集 ((A - B) + (B - A))
		Exclude		///< 差集 (A - B)
	};

	/// \~chinese
	/// @brief 几何形状生成器
	class KGE_API GeometrySink
		: protected Noncopyable
	{
		friend class Renderer;

	public:
		GeometrySink();

		~GeometrySink();

		/// \~chinese
		/// @brief 打开输入流
		/// @details 打开流后可以使用
		void Open();

		/// \~chinese
		/// @brief 关闭输入流
		void Close();

		/// \~chinese
		/// @brief 输入流是否已经打开
		bool IsOpened() const;

		/// \~chinese
		/// @brief 获取生成的几何形状
		/// @note 若还未关闭输入流，则自动关闭
		Geometry GetGeometry();

		/// \~chinese
		/// @brief 添加几何形状的轮廓
		/// @param input 输入的形状
		/// @param input_matrix 应用到输入形状上的二维变换
		/// @note 若还未打开输入流，则自动打开
		GeometrySink& AddGeometry(Geometry const& input, const Matrix3x2* input_matrix = nullptr);

		/// \~chinese
		/// @brief 开始添加路径
		/// @param begin_pos 路径起始点
		/// @note 若还未打开输入流，则自动打开
		GeometrySink& BeginPath(Point const& begin_pos = Point());

		/// \~chinese
		/// @brief 结束路径
		/// @param closed 路径是否闭合
		GeometrySink& EndPath(bool closed = false);

		/// \~chinese
		/// @brief 添加一条线段
		/// @param point 端点
		GeometrySink& AddLine(Point const& point);

		/// \~chinese
		/// @brief 添加多条线段
		/// @param points 端点集合
		GeometrySink& AddLines(Vector<Point> const& points);

		/// \~chinese
		/// @brief 添加多条线段
		/// @param points 端点数组
		/// @param count 端点数量
		GeometrySink& AddLines(const Point* points, size_t count);

		/// \~chinese
		/// @brief 添加一条三次方贝塞尔曲线
		/// @param point1 贝塞尔曲线的第一个控制点
		/// @param point2 贝塞尔曲线的第二个控制点
		/// @param point3 贝塞尔曲线的终点
		GeometrySink& AddBezier(Point const& point1, Point const& point2, Point const& point3);

		/// \~chinese
		/// @brief 添加弧线
		/// @param point 终点
		/// @param radius 椭圆半径
		/// @param rotation 椭圆旋转角度
		/// @param clockwise 顺时针 or 逆时针
		/// @param is_small 是否取小于 180° 的弧
		GeometrySink& AddArc(Point const& point, Size const& radius, float rotation, bool clockwise = true, bool is_small = true);

		/// \~chinese
		/// @brief 组合几何形状，并将结果输出到流中
		/// @param geo_a 输入的形状A
		/// @param geo_b 输入的形状B
		/// @param mode 组合方式
		/// @param matrix 应用到输入形状B上的二维变换
		/// @note 若还未打开输入流，则自动打开
		GeometrySink& Combine(Geometry const& geo_a, Geometry const& geo_b, CombineMode mode, const Matrix3x2* matrix = nullptr);

		/// \~chinese
		/// @brief 清空图形
		void Clear();

	private:
		ComPtr<ID2D1PathGeometry> GetPathGeometry() const;

		void SetPathGeometry(ComPtr<ID2D1PathGeometry> path);

		ComPtr<ID2D1GeometrySink> GetGeometrySink() const;

		void SetGeometrySink(ComPtr<ID2D1GeometrySink> sink);

	private:
		ComPtr<ID2D1PathGeometry> path_geo_;
		ComPtr<ID2D1GeometrySink> sink_;
	};

	/** @} */
	
	inline ComPtr<ID2D1PathGeometry> GeometrySink::GetPathGeometry() const		{ return path_geo_; }

	inline void GeometrySink::SetPathGeometry(ComPtr<ID2D1PathGeometry> path)	{ path_geo_ = path; }

	inline ComPtr<ID2D1GeometrySink> GeometrySink::GetGeometrySink() const		{ return sink_; }

	inline void GeometrySink::SetGeometrySink(ComPtr<ID2D1GeometrySink> sink)	{ sink_ = sink; }
}
