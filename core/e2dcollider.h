#pragma once
#include "e2dbase.h"


namespace e2d
{



class ColliderManager;

// 碰撞事件
class Collision
{
	friend ColliderManager;

public:
	// 获取碰撞发生时的主动体
	static Node * getActiveNode();

	// 获取碰撞发生时的被动体
	static Node * getPassiveNode();

	// 判断碰撞是否由该节点引发
	// 如果是，返回与其相撞的节点指针，否则返回空
	static Node * isCausedBy(
		Node * node
	);

	// 判断发生碰撞的节点名称是否相同
	// 若相同，返回其指针，否则返回空
	static Node * isCausedBy(
		const String& name
	);

private:
	static Node * __activeNode;
	static Node * __passiveNode;
};


// 碰撞体
class Collider :
	public Object
{
	friend ColliderManager;
	friend Node;

public:
	// 碰撞体类别
	enum class Type : int
	{
		RECT,		/* 矩形 */
		CIRCLE,		/* 圆形 */
		ELLIPSE		/* 椭圆形 */
	};

	// 碰撞体交集关系
	enum class Relation : int
	{
		UNKNOWN = 0,		/* 关系不确定 */
		DISJOIN = 1,		/* 没有交集 */
		IS_CONTAINED = 2,	/* 完全被包含 */
		CONTAINS = 3,		/* 完全包含 */
		OVERLAP = 4			/* 部分重叠 */
	};

public:
	Collider();

	virtual ~Collider();

	// 判断两碰撞体的交集关系
	virtual Relation getRelationWith(
		Collider * pCollider
	) const;

	// 获取父节点
	Node * getParentNode() const;

	// 获取绘制颜色
	Color getColor() const;

	// 启用或关闭该碰撞体
	virtual void setEnable(
		bool enable
	);

	// 设置碰撞体的可见性
	void setVisiable(
		bool bVisiable
	);

	// 设置绘制颜色
	void setColor(
		Color color
	);

	// 设置大小跟随
	void setAutoResize(
		bool enable
	);

	// 获取 ID2D1Geometry 对象
	virtual ID2D1Geometry * getD2dGeometry() const = 0;

protected:
	// 转换碰撞体
	virtual void _transform();

	// 重设大小
	virtual void _resize() = 0;

	// 渲染碰撞体
	virtual void _render();

protected:
	bool	_enable;
	bool	_visiable;
	bool	_autoResize;
	Color	_color;
	Node *	_parentNode;
	ID2D1TransformedGeometry * _transformed;
};


// 矩形碰撞体
class ColliderRect :
	public Collider
{
public:
	ColliderRect();

	ColliderRect(
		double x,
		double y,
		double width,
		double height
	);

	ColliderRect(
		Node * node
	);

	virtual ~ColliderRect();

	// 修改矩形碰撞体大小
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
	ID2D1RectangleGeometry * _d2dRectangle;
};


// 圆形碰撞体
class ColliderCircle :
	public Collider
{
public:
	ColliderCircle();

	ColliderCircle(
		Point center,
		double radius
	);

	ColliderCircle(
		Node * node
	);

	virtual ~ColliderCircle();

	// 修改圆形碰撞体大小
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
	ID2D1EllipseGeometry * _d2dCircle;
};


// 椭圆形碰撞体
class ColliderEllipse :
	public Collider
{
public:
	ColliderEllipse();

	ColliderEllipse(
		Point center,
		double radiusX,
		double radiusY
	);

	ColliderEllipse(
		Node * node
	);

	virtual ~ColliderEllipse();

	// 修改椭圆碰撞体大小
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
	ID2D1EllipseGeometry * _d2dEllipse;
};

}