#pragma once
#include "e2dbase.h"


namespace e2d
{



class Listener;
class ColliderManager;

// 碰撞事件
class Collision
{
	friend class Game;
	friend class ColliderManager;

public:
	// 添加可互相碰撞物体的名称
	static void addName(
		const String& name1,
		const String& name2
	);

	// 添加可互相碰撞物体的名称
	static void addName(
		const std::vector<std::pair<String, String> >& names
	);

	// 判断两个物体是否是可碰撞的
	static bool isCollidable(
		Node * node1,
		Node * node2
	);

	// 判断两个物体是否是可碰撞的
	static bool isCollidable(
		const String& name1,
		const String& name2
	);

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

	// 开启或关闭物体碰撞监听功能（默认关闭）
	static void setEnable(
		bool enable
	);

	// 是否打开了物体碰撞监听
	static bool isEnable();

	// 添加碰撞监听
	static Listener * addListener(
		const Function& func,		/* 监听到碰撞时的执行函数 */
		const String& name = L"",	/* 监听器名称 */
		bool paused = false			/* 是否暂停 */
	);

	// 添加碰撞监听
	static void addListener(
		Listener * listener			/* 监听器 */
	);

	// 移除监听器
	static void removeListener(
		Listener * listener			/* 监听器 */
	);

	// 启动碰撞监听
	static void startListener(
		const String& name
	);

	// 停止碰撞监听
	static void stopListener(
		const String& name
	);

	// 移除碰撞监听
	static void removeListener(
		const String& name
	);

	// 启动所有监听器
	static void startAllListeners();

	// 停止所有监听器
	static void stopAllListeners();

	// 移除所有监听器
	static void removeAllListeners();

private:
	// 更新监听器
	static void __update(
		Node * active,
		Node * passive
	);

	// 清空监听器
	static void __clearListeners();
};


// 碰撞体
class Collider :
	public Object
{
	friend class ColliderManager;
	friend class Node;

public:
	// 碰撞体类别
	enum class Type
	{
		NONE,		/* 无 */
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
class RectCollider :
	public Collider
{
public:
	RectCollider();

	explicit RectCollider(
		double x,
		double y,
		double width,
		double height
	);

	explicit RectCollider(
		Node * node
	);

	virtual ~RectCollider();

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
class CircleCollider :
	public Collider
{
public:
	CircleCollider();

	explicit CircleCollider(
		Point center,
		double radius
	);

	explicit CircleCollider(
		Node * node
	);

	virtual ~CircleCollider();

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
class EllipseCollider :
	public Collider
{
public:
	EllipseCollider();

	explicit EllipseCollider(
		Point center,
		double radiusX,
		double radiusY
	);

	explicit EllipseCollider(
		Node * node
	);

	virtual ~EllipseCollider();

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