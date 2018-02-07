#pragma once
#include "ebase.h"


namespace e2d
{

class ShapeManager;
class Node;


class Shape :
	public Obj
{
	friend ShapeManager;
	friend Node;

public:
	Shape();

	virtual ~Shape();

	// 判断两形状的交集关系
	virtual int getRelationWith(
		Shape * pShape
	) const;

	// 获取父节点
	Node * getParentNode() const;

	// 获取类别掩码
	UINT32 getCategoryBitmask() const;

	// 获取冲突掩码
	UINT32 getCollisionBitmask() const;

	// 设置类别掩码
	void setCategoryBitmask(
		UINT32 mask
	);

	// 设置冲突掩码
	void setCollisionBitmask(
		UINT32 mask
	);

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
		float opacity
	);

protected:
	// 转换形状
	virtual void _transform();

	// 渲染形状
	virtual void _render();

	// 获取 ID2D1Geometry 对象
	virtual ID2D1Geometry * _getD2dGeometry() const = 0;

protected:
	bool	m_bEnable;
	bool	m_bIsVisiable;
	UINT32	m_nCategoryBitmask;
	UINT32	m_nCollisionBitmask;
	UINT32	m_nColor;
	float	m_fOpacity;
	Node * m_pParentNode;
	ID2D1TransformedGeometry * m_pTransformedShape;
};


class Rect :
	public Shape
{
public:
	// 创建一个空矩形
	Rect();

	// 根据左上角坐标和宽高创建矩形
	Rect(
		float x,
		float y,
		float width,
		float height
	);

	// 创建一个和节点位置大小相同的矩形
	Rect(
		Node * node
	);

	virtual ~Rect();

protected:
	void _setRect(
		float left,
		float top,
		float right,
		float bottom
	);

	virtual ID2D1RectangleGeometry * _getD2dGeometry() const override;

protected:
	ID2D1RectangleGeometry * m_pD2dRectangle;
};


class Circle :
	public Shape
{
public:
	// 创建一个空的圆形
	Circle();

	// 根据圆心和半径创建圆形
	Circle(
		Point center,
		float radius
	);

	// 创建一个和节点位置大小相同的圆形
	Circle(
		Node * node
	);

	virtual ~Circle();

protected:
	void _setCircle(
		Point center,
		float radius
	);

	virtual ID2D1EllipseGeometry * _getD2dGeometry() const override;

protected:
	ID2D1EllipseGeometry * m_pD2dCircle;
};


class Ellipse :
	public Shape
{
public:
	// 创建一个空的椭圆
	Ellipse();

	// 根据圆心和半径创建椭圆
	Ellipse(
		Point center,
		float radiusX,
		float radiusY
	);

	// 创建一个和节点位置大小相同的椭圆
	Ellipse(
		Node * node
	);

	virtual ~Ellipse();

protected:
	void _setEllipse(
		Point center,
		float radiusX,
		float radiusY
	);

	virtual ID2D1EllipseGeometry * _getD2dGeometry() const override;

protected:
	ID2D1EllipseGeometry * m_pD2dEllipse;
};

}