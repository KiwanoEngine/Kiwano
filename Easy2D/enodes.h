#pragma once
#include "ebase.h"
#include "Win\winbase.h"

namespace e2d 
{

class ENode :
	public EObject
{
	friend EScene;

public:
	ENode();

	ENode(
		const EString & name
	);

	virtual ~ENode();

	// 重写这个函数，它将在节点进入场景时自动执行
	virtual void onEnter();

	// 重写这个函数，它将在节点离开场景时自动执行
	virtual void onExit();

	// 获取节点显示状态
	virtual bool isVisiable() const;

	// 获取节点名称
	virtual EString getName() const;

	// 获取节点绘图顺序
	virtual int getOrder() const;

	// 获取节点横坐标
	virtual float getPosX() const;

	// 获取节点纵坐标
	virtual float getPosY() const;

	// 获取节点坐标
	virtual EPoint getPos() const;

	// 获取节点宽度
	virtual float getWidth() const;

	// 获取节点高度
	virtual float getHeight() const;

	// 获取节点宽度（不考虑缩放）
	virtual float getRealWidth() const;

	// 获取节点高度（不考虑缩放）
	virtual float getRealHeight() const;

	// 获取节点大小（不考虑缩放）
	virtual ESize getRealSize() const;

	// 获取节点大小
	virtual ESize getSize() const;

	// 获取节点横向缩放比例
	virtual float getScaleX() const;

	// 获取节点纵向缩放比例
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
	virtual EVector<ENode*> &getChildren();

	// 获取子节点数量
	virtual size_t getChildrenCount() const;

	// 根据名字获取子节点
	virtual ENode * getChild(
		const EString & name
	);

	// 设置节点是否显示
	virtual void setVisiable(
		bool value
	);

	// 设置节点名称
	virtual void setName(
		const EString & name
	);

	// 设置节点横坐标
	virtual void setPosX(
		float x
	);

	// 设置节点纵坐标
	virtual void setPosY(
		float y
	);

	// 设置节点坐标
	virtual void setPos(
		const EPoint & point
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

	// 移动节点
	virtual void move(
		const EVec & v
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
		const ESize & size
	);

	// 设置节点大小
	virtual void setSize(
		float width,
		float height
	);

	// 设置节点绘图顺序
	// 默认为 0
	virtual void setOrder(
		int order
	);

	// 设置横向缩放比例
	// 默认为 1.0f
	virtual void setScaleX(
		float scaleX
	);

	// 设置纵向缩放比例
	// 默认为 1.0f
	virtual void setScaleY(
		float scaleY
	);

	// 设置缩放比例
	// 默认为 (1.0f, 1.0f)
	virtual void setScale(
		float scaleX,
		float scaleY
	);

	// 设置缩放比例
	// 默认为 1.0f
	virtual void setScale(
		float scale
	);

	// 设置横向倾斜角度
	// 默认为 0
	virtual void setSkewX(
		float angleX
	);

	// 设置纵向倾斜角度
	// 默认为 0
	virtual void setSkewY(
		float angleY
	);

	// 设置倾斜角度
	// 默认为 (0, 0)
	virtual void setSkew(
		float angleX,
		float angleY
	);

	// 设置旋转角度
	// 默认为 0
	virtual void setRotation(
		float rotation
	);

	// 设置透明度
	// 默认为 1.0f, 范围 [0, 1]
	virtual void setOpacity(
		float opacity
	);

	// 设置纵向锚点
	// 默认为 0, 范围 [0, 1]
	virtual void setAnchorX(
		float anchorX
	);

	// 设置横向锚点
	// 默认为 0, 范围 [0, 1]
	virtual void setAnchorY(
		float anchorY
	);

	// 设置锚点
	// 默认为 (0, 0), 范围 [0, 1]
	virtual void setAnchor(
		float anchorX,
		float anchorY
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
	virtual bool removeChild(
		ENode * child, 
		bool release = false
	);

	// 移除子节点
	virtual void removeChild(
		const EString & childName,
		bool release = false
	);

	// 移除所有节点
	virtual void clearAllChildren(
		bool release = false
	);

protected:
	// 访问节点
	virtual void _callOn();

	// 渲染节点
	virtual void _onRender();

	// 节点被添加到场景时的执行程序
	virtual void _onEnter();

	// 节点从场景中消失时的执行程序
	virtual void _onExit();

	// 节点清除时的执行程序
	virtual void _onClear();

	// 子节点排序
	void _sortChildren();

	// 设置节点所在场景
	virtual void _setParentScene(
		EScene * scene
	);

	// 只考虑自身进行二维矩阵变换
	void _updateTransformToReal();

	// 更新所有子节点矩阵
	void _updateChildrenTransform();

	// 更新所有子节点透明度
	void _updateChildrenOpacity();

	// 更新节点矩阵
	static void _updateTransform(ENode * node);

	// 更新节点透明度
	static void _updateOpacity(ENode * node);

protected:
	EString		m_sName;
	size_t		m_nHashName;
	EPoint		m_Pos;
	ESize		m_Size;
	float		m_fScaleX;
	float		m_fScaleY;
	float		m_fRotation;
	float		m_fSkewAngleX;
	float		m_fSkewAngleY;
	float		m_fDisplayOpacity;
	float		m_fRealOpacity;
	float		m_fAnchorX;
	float		m_fAnchorY;
	int			m_nOrder;
	bool		m_bVisiable;
	bool		m_bDisplayedInScene;
	bool		m_bSortChildrenNeeded;
	bool		m_bTransformChildrenNeeded;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	D2D1::Matrix3x2F	m_Matri;
	EVector<ENode*>	m_vChildren;
};


class ERectangle :
	public ENode
{
public:
	ERectangle();

	EColor getColor() const;

	void setColor(EColor color);

protected:
	virtual void _onRender() override;

protected:
	EColor m_Color;
};


class ESprite :
	public ENode
{
public:
	// 创建一个空精灵
	ESprite();

	// 从文件图片创建精灵
	ESprite(
		const EString & imageFileName
	);

	// 从资源图片创建精灵
	ESprite(
		const EString & resourceName,
		const EString & resourceType
	);
	
	// 从文件加载图片
	void setImage(
		const EString & fileName
	);

	// 从资源加载图片
	void setImage(
		const EString & resourceName,
		const EString & resourceType
	);

	// 预加载资源
	static bool preloadImage(
		const EString & fileName
	);

	// 预加载资源
	static bool preloadImage(
		const EString & resourceName,
		const EString & resourceType
	);

	// 清空图片缓存
	static void clearCache();

protected:
	// 渲染精灵
	virtual void _onRender() override;

protected:
	EString m_sFileName;
	EString m_sResourceName;
	EString m_sResourceType;
	ID2D1Bitmap * pBitmap;
};

}