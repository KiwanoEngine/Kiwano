#pragma once
#include "ebase.h"
#include "Win\winbase.h"

namespace e2d 
{

class ENode :
	public EObject
{
public:
	ENode();

	ENode(
		EString name
	);

	virtual ~ENode();

	// 节点是否显示
	virtual bool isVisiable() const;

	// 获取节点绘图顺序
	virtual int getOrder() const;

	// 获取节点横坐标
	virtual float getX() const;

	// 获取节点纵坐标
	virtual float getY() const;

	// 获取节点宽度
	virtual float getWidth() const;

	// 获取节点高度
	virtual float getHeight() const;

	// 获取节点横向缩放倍数
	virtual float getScaleX() const;

	// 获取节点纵向缩放倍数
	virtual float getScaleY() const;

	// 获取节点横向倾斜角度
	virtual float getSkewX() const;

	// 获取节点纵向倾斜角度
	virtual float getSkewY() const;

	// 获取节点旋转角度
	virtual float getRotation() const;

	// 获取节点透明度
	virtual float getOpacity() const;

	// 获取父节点
	virtual ENode * getParent() const;

	// 获取节点所在场景
	virtual EScene * getParentScene() const;

	// 获取所有子节点
	virtual std::vector<ENode*> &getChildren();

	// 获取子节点数量
	virtual size_t getChildrenCount() const;

	// 根据名字获取子节点
	virtual ENode * getChild(
		EString name
	) const;

	// 根据名字获取子节点
	static ENode * getChild(
		EString name,
		const std::vector<ENode*> &children
	);

	// 设置节点是否显示
	virtual void setVisiable(
		bool value
	);

	// 设置节点名称
	virtual void setName(
		EString name
	);

	// 设置节点横坐标
	virtual void setX(
		float x
	);

	// 设置节点纵坐标
	virtual void setY(
		float y
	);

	// 设置节点坐标
	virtual void setPos(
		float x,
		float y
	);

	// 移动节点
	virtual void move(
		float x,
		float y
	);

	// 设置节点宽度
	virtual void setWidth(
		float width
	);

	// 设置节点高度
	virtual void setHeight(
		float height
	);

	// 设置节点大小
	virtual void setSize(
		float width,
		float height
	);

	// 设置节点绘图顺序
	virtual void setOrder(
		int order
	);

	// 设置横向缩放
	virtual void setScaleX(
		float scaleX
	);

	// 设置纵向缩放
	virtual void setScaleY(
		float scaleY
	);

	// 设置缩放
	virtual void setScale(
		float scaleX,
		float scaleY
	);

	// 设置缩放
	virtual void setScale(
		float scale
	);

	// 设置横向倾斜角度
	virtual void setSkewX(
		float angleX
	);

	// 设置纵向倾斜角度
	virtual void setSkewY(
		float angleY
	);

	// 设置倾斜角度
	virtual void setSkew(
		float angleX,
		float angleY
	);

	// 设置旋转角度
	virtual void setRotation(
		float rotation
	);

	// 设置透明度
	virtual void setOpacity(
		float opacity
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
	virtual void addChild(
		ENode * child,
		int order = 0
	);

	// 从父节点移除
	virtual void removeFromParent(
		bool release = false
	);

	// 移除子节点
	virtual void removeChild(
		ENode * child, 
		bool release = false
	);

	// 移除子节点
	virtual void removeChild(
		EString childName, 
		bool release = false
	);

	// 访问节点
	virtual void callOn();

protected:
	// 渲染节点
	virtual void _onRender();

	// 子节点排序
	void _sortChildren();

	// 节点状态转换
	void _transfrom();

protected:
	EString		m_sName;
	size_t		m_nHashName;
	float		m_fPosX;
	float		m_fPosY;
	float		m_fWidth;
	float		m_fHeight;
	float		m_fScaleX;
	float		m_fScaleY;
	float		m_fRotation;
	float		m_fSkewAngleX;
	float		m_fSkewAngleY;
	float		m_fDisplayOpacity;
	float		m_fRealOpacity;
	D2D1::Matrix3x2F m_Matri;
	int			m_nOrder;
	bool		m_bVisiable;
	bool		m_bSortNeeded;
	bool		m_bTransformNeeded;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	std::vector<ENode*> m_vChildren;
};


class ERectangle :
	public ENode
{
public:
	ERectangle();

	EColor::Enum getColor() const;

	void setColor(EColor::Enum color);

protected:
	virtual void _onRender() override;

protected:
	EColor::Enum m_Color;
};

}