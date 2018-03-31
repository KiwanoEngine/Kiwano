#pragma once
#include "ebase.h"


namespace e2d
{


class CollisionManager;

// 形状
class ShapeBase :
	public Object
{
	friend CollisionManager;
	friend Node;

public:
	ShapeBase();

	virtual ~ShapeBase();

	// 判断两形状的交集关系
	virtual Relation getRelationWith(
		ShapeBase * pShape
	) const;

	// 获取父节点
	Node * getParentNode() const;

	// 启用或关闭该形状
	virtual void setEnable(
		bool bEnable
	);

	// 设置形状的可见性
	void setVisiable(
		bool bVisiable
	);

	// 设置绘制颜色
	void setColor(
		UINT32 color
	);

	// 设置绘制透明度
	void setOpacity(
		double opacity
	);

	// 设置大小跟随
	void setAutoResize(
		bool bEnable
	);

	// 获取 ID2D1Geometry 对象
	virtual ID2D1Geometry * getD2dGeometry() const = 0;

protected:
	// 转换形状
	virtual void _transform();

	// 重设大小
	virtual void _resize() = 0;

	// 渲染形状
	virtual void _render();

protected:
	bool	m_bEnable;
	bool	m_bIsVisiable;
	bool	m_bAutoResize;
	UINT32	m_nColor;
	float	m_fOpacity;
	Node *	m_pParentNode;
	ID2D1TransformedGeometry * m_pTransformedShape;
};


// 矩形
class ShapeRectangle :
	public ShapeBase
{
public:
	// 创建一个默认矩形
	ShapeRectangle();

	// 根据左上角坐标和宽高创建矩形
	ShapeRectangle(
		double x,
		double y,
		double width,
		double height
	);

	// 创建一个和节点位置大小相同的矩形
	ShapeRectangle(
		Node * node
	);

	virtual ~ShapeRectangle();

	// 修改矩形大小
	void setRect(
		double left,
		double top,
		double right,
		double bottom
	);

	// 获取 ID2D1Geometry 对象
	virtual ID2D1RectangleGeometry * getD2dGeometry() const override;

protected:
	// 重设大小
	virtual void _resize();

protected:
	ID2D1RectangleGeometry * m_pD2dRectangle;
};


// 圆形
class ShapeCircle :
	public ShapeBase
{
public:
	// 创建一个默认圆形
	ShapeCircle();

	// 根据圆心和半径创建圆形
	ShapeCircle(
		Point center,
		double radius
	);

	// 创建一个和节点位置大小相同的圆形
	ShapeCircle(
		Node * node
	);

	virtual ~ShapeCircle();

	// 修改圆形大小
	void setCircle(
		Point center,
		double radius
	);

	// 获取 ID2D1Geometry 对象
	virtual ID2D1EllipseGeometry * getD2dGeometry() const override;

protected:
	// 重设大小
	virtual void _resize();

protected:
	ID2D1EllipseGeometry * m_pD2dCircle;
};


// 椭圆形
class ShapeEllipse :
	public ShapeBase
{
public:
	// 创建一个默认椭圆
	ShapeEllipse();

	// 根据圆心和半径创建椭圆
	ShapeEllipse(
		Point center,
		double radiusX,
		double radiusY
	);

	// 创建一个和节点位置大小相同的椭圆
	ShapeEllipse(
		Node * node
	);

	virtual ~ShapeEllipse();

	// 修改椭圆大小
	void setEllipse(
		Point center,
		double radiusX,
		double radiusY
	);

	// 获取 ID2D1Geometry 对象
	virtual ID2D1EllipseGeometry * getD2dGeometry() const override;

protected:
	// 重设大小
	virtual void _resize();

protected:
	ID2D1EllipseGeometry * m_pD2dEllipse;
};

}