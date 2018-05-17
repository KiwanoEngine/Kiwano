#pragma once
#include "e2dnode.h"

namespace e2d
{


// 形状
class Shape :
	public Node
{
public:
	// 形状样式
	enum class Style : int
	{
		SOLID,		/* 填充 */
		ROUND,		/* 轮廓 */
		FILL,		/* 轮廓 + 填充 */
	};

public:
	Shape();

	virtual ~Shape();

	// 获取样式
	Style getStyle() const;

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
	void setStyle(Style style);

	// 渲染形状
	virtual void onRender() override;

protected:
	// 渲染轮廓
	virtual void _renderLine() = 0;

	// 渲染填充色
	virtual void _renderFill() = 0;

protected:
	Style	_style;
	float	_strokeWidth;
	Color	_lineColor;
	Color	_fillColor;
};


// 矩形
class Rect :
	public Shape
{
public:
	Rect();

	Rect(
		Size size		/* 宽度和高度 */
	);

	Rect(
		Point topLeft,	/* 左上角坐标 */
		Size size		/* 宽度和高度 */
	);

	static Rect * create();

	static Rect * create(
		Size size		/* 宽度和高度 */
	);

	static Rect * create(
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
		Size size,		/* 宽度和高度 */
		double radiusX,	/* 圆角半径 */
		double radiusY	/* 圆角半径 */
	);

	RoundRect(
		Point topLeft,	/* 左上角坐标 */
		Size size,		/* 宽度和高度 */
		double radiusX,	/* 圆角半径 */
		double radiusY	/* 圆角半径 */
	);

	static RoundRect * create();

	static RoundRect * create(
		Size size,		/* 宽度和高度 */
		double radiusX,	/* 圆角半径 */
		double radiusY	/* 圆角半径 */
	);

	static RoundRect * create(
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
	float _radiusX;
	float _radiusY;
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

	static Circle * create();

	static Circle * create(
		double radius	/* 半径 */
	);

	static Circle * create(
		Point center,	/* 圆心坐标 */
		double radius	/* 半径 */
	);

	virtual ~Circle();

	// 获取半径
	double getRadius() const;

	// 设置半径
	virtual void setRadius(
		double radius
	);

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;

protected:
	float _radius;
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

	static Ellipse * create();

	static Ellipse * create(
		double radiusX,	/* 横轴半径 */
		double radiusY	/* 纵轴半径 */
	);

	static Ellipse * create(
		Point center,	/* 圆心坐标 */
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

protected:
	// 渲染轮廓
	virtual void _renderLine() override;

	// 渲染填充色
	virtual void _renderFill() override;

protected:
	float _radiusX;
	float _radiusY;
};

}