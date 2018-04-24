#pragma once
#include "e2dnode.h"

namespace e2d
{


// 形状
class Shape :
	public Node
{
public:
	Shape();

	virtual ~Shape();

	// 获取样式
	ShapeStyle getStyle() const;

	// 获取填充颜色
	Color getFillColor() const;

	// 获取线条颜色
	Color getLineColor() const;

	// 获取线条宽度
	double getStrokeWidth() const;

	// 设置填充颜色
	void setFillColor(
		Color fillColor
	);

	// 设置线条颜色
	void setLineColor(
		Color lineColor
	);

	// 设置线条宽度
	void setStrokeWidth(
		double strokeWidth
	);

	// 设置样式
	void setStyle(ShapeStyle style);

	// 渲染形状
	virtual void onRender() override;

protected:
	// 渲染轮廓
	virtual void _renderLine() = 0;

	// 渲染填充色
	virtual void _renderFill() = 0;

protected:
	ShapeStyle	m_nStyle;
	float	m_fStrokeWidth;
	Color	m_nLineColor;
	Color	m_nFillColor;
};


// 矩形
class Rect :
	public Shape
{
public:
	Rect();

	Rect(
		double width,	/* 宽度 */
		double height	/* 高度 */
	);

	Rect(
		Size size		/* 宽度和高度 */
	);

	Rect(
		double top,		/* 左上角横坐标 */
		double left,	/* 左上角纵坐标 */
		double width,	/* 宽度 */
		double height	/* 高度 */
	);

	Rect(
		Point topLeft,	/* 左上角坐标 */
		Size size		/* 宽度和高度 */
	);

	virtual ~Rect();

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;
};


// 圆角矩形
class RoundRect :
	public Shape
{
public:
	RoundRect();

	RoundRect(
		double width,	/* 宽度 */
		double height,	/* 高度 */
		double radiusX,	/* 圆角半径 */
		double radiusY	/* 圆角半径 */
	);

	RoundRect(
		Size size,		/* 宽度和高度 */
		double radiusX,	/* 圆角半径 */
		double radiusY	/* 圆角半径 */
	);

	RoundRect(
		double top,		/* 左上角横坐标 */
		double left,	/* 左上角纵坐标 */
		double width,	/* 宽度 */
		double height,	/* 高度 */
		double radiusX,	/* 圆角半径 */
		double radiusY	/* 圆角半径 */
	);

	RoundRect(
		Point topLeft,	/* 左上角坐标 */
		Size size,		/* 宽度和高度 */
		double radiusX,	/* 圆角半径 */
		double radiusY	/* 圆角半径 */
	);

	virtual ~RoundRect();

	// 获取圆角半径
	double getRadiusX() const;

	// 获取圆角半径
	double getRadiusY() const;

	// 设置圆角半径
	virtual void setRadiusX(
		double radiusX
	);

	// 设置圆角半径
	virtual void setRadiusY(
		double radiusY
	);

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;

protected:
	float m_fRadiusX;
	float m_fRadiusY;
};


// 圆形
class Circle :
	public Shape
{
public:
	Circle();

	Circle(
		double radius	/* 半径 */
	);

	Circle(
		Point center,	/* 圆心坐标 */
		double radius	/* 半径 */
	);

	Circle(
		double centerX,	/* 圆心横坐标 */
		double centerY,	/* 圆心纵坐标 */
		double radius	/* 半径 */
	);

	virtual ~Circle();

	// 获取半径
	double getRadius() const;

	// 设置半径
	virtual void setRadius(
		double radius
	);

public:
	// 禁用的函数
	void setWidth() {}

	// 禁用的函数
	void setHeight() {}

	// 禁用的函数
	void setSize() {}

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;

protected:
	float m_fRadius;
};


// 椭圆形
class Ellipse :
	public Shape
{
public:
	Ellipse();

	Ellipse(
		double radiusX,	/* 横轴半径 */
		double radiusY	/* 纵轴半径 */
	);

	Ellipse(
		Point center,	/* 圆心坐标 */
		double radiusX,	/* 横轴半径 */
		double radiusY	/* 纵轴半径 */
	);

	Ellipse(
		double centerX,	/* 圆心横坐标 */
		double centerY,	/* 圆心纵坐标 */
		double radiusX,	/* 横轴半径 */
		double radiusY	/* 纵轴半径 */
	);

	virtual ~Ellipse();

	// 获取横轴半径
	double getRadiusX() const;

	// 获取纵轴半径
	double getRadiusY() const;

	// 设置横轴半径
	virtual void setRadiusX(
		double radiusX
	);

	// 设置纵轴半径
	virtual void setRadiusY(
		double radiusY
	);

public:
	// 禁用的函数
	void setWidth() {}

	// 禁用的函数
	void setHeight() {}

	// 禁用的函数
	void setSize() {}

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;

protected:
	float m_fRadiusX;
	float m_fRadiusY;
};

}