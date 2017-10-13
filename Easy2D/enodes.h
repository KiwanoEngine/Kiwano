#pragma once
#include "ebase.h"

namespace e2d 
{

class ENode :
	public EObject
{
	friend EScene;

public:
	ENode();

	ENode(
		EPoint p
	);

	ENode(
		int x, 
		int y
	);

	virtual ~ENode();

	// 节点是否显示
	virtual bool isVisiable() const;

	// 获取节点绘图顺序
	virtual int getZOrder() const;

	// 获取节点横坐标
	virtual int getX() const;

	// 获取节点纵坐标
	virtual int getY() const;

	// 获取节点坐标
	virtual EPoint getPos() const;

	// 获取节点宽度
	virtual UINT32 getWidth() const;

	// 获取节点高度
	virtual UINT32 getHeight() const;

	// 获取节点大小
	virtual e2d::ESize getSize() const;

	// 获取节点所在的矩形
	virtual e2d::ERect getRect() const;

	// 获取父节点
	virtual e2d::ENode* &getParent();

	// 获取节点所在场景
	EScene * &getParentScene();

	// 设置节点是否显示
	virtual void setVisiable(
		bool value
	);

	// 设置节点横坐标
	virtual void setX(
		int x
	);

	// 设置节点纵坐标
	virtual void setY(
		int y
	);

	// 设置节点坐标
	virtual void setPos(
		int x, 
		int y
	);

	// 设置节点坐标
	virtual void setPos(
		EPoint p
	);

	// 移动节点
	virtual void move(
		int x, 
		int y
	);

	// 移动节点
	virtual void move(
		EVector v
	);

	// 设置节点宽度
	virtual void setWidth(
		UINT32 width
	);

	// 设置节点高度
	virtual void setHeight(
		UINT32 height
	);

	// 设置节点大小
	virtual void setSize(
		UINT32 width, 
		UINT32 height
	);

	// 设置节点大小
	virtual void setSize(
		e2d::ESize size
	);

	// 设置节点所在的矩形
	virtual void setRect(
		int x1, 
		int y1, 
		int x2, 
		int y2
	);

	// 设置节点所在的矩形
	virtual void setRect(
		EPoint leftTop, 
		EPoint rightBottom
	);

	// 设置节点所在的矩形
	virtual void setRect(
		e2d::ERect rect
	);

	// 设置节点绘图顺序（0为最先绘制，显示在最底层）
	virtual void setZOrder(
		int z
	);

	// 设置父节点
	virtual void setParent(
		ENode* parent
	);

	// 设置节点所在场景
	void setParentScene(
		EScene * scene
	);

protected:
	int			m_nZOrder;
	bool		m_bVisiable;
	ERect		m_Rect;
	EScene *	m_pParentScene;
	ENode *		m_pParent;

protected:

	virtual bool _exec(bool active);

	virtual void _onRender();
};

}