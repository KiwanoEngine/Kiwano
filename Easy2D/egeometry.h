#pragma once
#include "ebase.h"


namespace e2d
{

class EPhysicsManager;
class ENode;
class EGeometry;

class EPhysicsMsg
{
	friend EPhysicsManager;

public:
	enum INTERSECT_RELATION
	{
		UNKNOWN = 0,		/* 关系不确定 */
		DISJOINT = 1,		/* 没有交集 */
		IS_CONTAINED = 2,	/* 完全被包含 */
		CONTAINS = 3,		/* 完全包含 */
		OVERLAP = 4			/* 部分重叠 */
	};

	// 获取当前物理碰撞消息类型
	static INTERSECT_RELATION getMsg();

	// 获取主动方
	static EGeometry * getActiveGeometry();

	// 获取被动方
	static EGeometry * getPassiveGeometry();

public:
	static INTERSECT_RELATION s_nRelation;
	static EGeometry * s_pActiveGeometry;
	static EGeometry * s_pPassiveGeometry;
};

class EGeometry :
	public EObject
{
	friend EPhysicsManager;
	friend ENode;

public:
	EGeometry();

	// 获取父节点
	ENode * getParentNode() const;

protected:
	// 判断两形状的交集关系
	virtual EPhysicsMsg::INTERSECT_RELATION _intersectWith(
		EGeometry * pGeometry
	);

	// 转换形状
	virtual void _transform();

	virtual ID2D1Geometry * _getD2dGeometry() const = 0;

protected:
	bool	m_bTransformNeeded;
	ENode * m_pParentNode;
	ID2D1TransformedGeometry * m_pTransformedGeometry;
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