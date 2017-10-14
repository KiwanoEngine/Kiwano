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
		EString name
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
	virtual ESize getSize() const;

	// 获取节点所在的矩形
	virtual ERect getRect() const;

	// 获取父节点
	virtual ENode * &getParent();

	// 获取节点所在场景
	virtual EScene * &getParentScene();

	// 获取所有子节点
	virtual std::vector<ENode*> &getChildren();

	// 获取子节点数量
	virtual int getChildrenCount() const;

	// 根据名字获取子节点
	virtual ENode * getChild(EString name);

	// 设置节点是否显示
	virtual void setVisiable(
		bool value
	);

	// 设置节点名称
	virtual void setName(EString name);

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
		ESize size
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
		ERect rect
	);

	// 设置节点绘图顺序（0为最先绘制，显示在最底层）
	virtual void setZOrder(
		int z
	);

	// 设置节点所在场景
	virtual void setParentScene(
		EScene * scene
	);

	// 设置父节点
	virtual void setParent(
		ENode* parent
	);

	// 添加子节点
	virtual void addChild(ENode * child);

	// 从父节点移除
	virtual void removeFromParent(bool release = false);

	// 移除子节点
	virtual void removeChild(ENode * child, bool release = false);

	// 移除子节点
	virtual void removeChild(EString childName, bool release = false);

protected:
	// 渲染节点
	virtual void _onRender();

protected:
	EString		m_sName;
	size_t		m_nHashName;
	int			m_nZOrder;
	bool		m_bVisiable;
	ERect		m_Rect;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	std::vector<ENode*> m_vChildren;
};

}