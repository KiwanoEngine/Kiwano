#pragma once
#include "ebase.h"


namespace e2d
{

class EShapeManager;
class ENode;


class EShape :
	public EObject
{
	friend EShapeManager;
	friend ENode;

public:
	EShape();

	virtual ~EShape();

	// 判断两形状的交集关系
	virtual int getRelationWith(
		EShape * pShape
	) const;

	// 获取父节点
	ENode * getParentNode() const;

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
	ENode * m_pParentNode;
	ID2D1TransformedGeometry * m_pTransformedShape;
};


class ERectangle :
	public EShape
{
public:
	// 创建一个空矩形
	ERectangle();

	// 根据左上角坐标和宽高创建矩形
	ERectangle(
		float x,
		float y,
		float width,
		float height
	);

	// 创建一个和节点位置大小相同的矩形
	ERectangle(
		ENode * node
	);

	virtual ~ERectangle();

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


class ECircle :
	public EShape
{
public:
	// 创建一个空的圆形
	ECircle();

	// 根据圆心和半径创建圆形
	ECircle(
		EPoint center,
		float radius
	);

	// 创建一个和节点位置大小相同的圆形
	ECircle(
		ENode * node
	);

	virtual ~ECircle();

protected:
	void _setCircle(
		EPoint center,
		float radius
	);

	virtual ID2D1EllipseGeometry * _getD2dGeometry() const override;

protected:
	ID2D1EllipseGeometry * m_pD2dCircle;
};


class EEllipse :
	public EShape
{
public:
	// 创建一个空的椭圆
	EEllipse();

	// 根据圆心和半径创建椭圆
	EEllipse(
		EPoint center,
		float radiusX,
		float radiusY
	);

	// 创建一个和节点位置大小相同的椭圆
	EEllipse(
		ENode * node
	);

	virtual ~EEllipse();

protected:
	void _setEllipse(
		EPoint center,
		float radiusX,
		float radiusY
	);

	virtual ID2D1EllipseGeometry * _getD2dGeometry() const override;

protected:
	ID2D1EllipseGeometry * m_pD2dEllipse;
};

}