#pragma once
#include "ebase.h"


namespace e2d
{

class EPhysicsManager;

class EGeometry :
	public EObject
{
	friend EPhysicsManager;
public:
	EGeometry();

protected:
	virtual bool _isCollisionWith(
		EGeometry * pGeometry
	);

	virtual ID2D1Geometry * _getD2dGeometry() const = 0;

protected:
	bool	m_bTransformed;
	ENode * m_pParentNode;
};


class ERectangle :
	public EGeometry
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
	public EGeometry
{
public:
	ECircle();

	ECircle(
		EPoint center,
		float radius
	);

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
	public EGeometry
{
public:
	EEllipse();

	EEllipse(
		EPoint center,
		float radiusX,
		float radiusY
	);

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