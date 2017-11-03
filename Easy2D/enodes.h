#pragma once
#include "ebase.h"

namespace e2d 
{

class EText;
class ESprite;
class EAction;
class EButton;
class EGeometry;

class ENode :
	public EObject
{
	friend EScene;
	friend EButton;
	friend EGeometry;

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

	// 获取节点的锚点
	virtual float getAnchorX() const;

	// 获取节点的锚点
	virtual float getAnchorY() const;

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
	virtual void movePosX(
		float x
	);

	// 移动节点
	virtual void movePosY(
		float y
	);

	// 移动节点
	virtual void movePos(
		float x,
		float y
	);

	// 移动节点
	virtual void movePos(
		const EVec & v
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
	// 默认为 0.5f, 范围 [0, 1]
	virtual void setAnchorX(
		float anchorX
	);

	// 设置横向锚点
	// 默认为 0.5f, 范围 [0, 1]
	virtual void setAnchorY(
		float anchorY
	);

	// 设置锚点
	// 默认为 (0.5f, 0.5f), 范围 [0, 1]
	virtual void setAnchor(
		float anchorX,
		float anchorY
	);

	// 设置节点形状
	virtual void setGeometry(
		EGeometry * geometry
	);

	// 添加子节点
	virtual void addChild(
		ENode * child,
		int order = 0
	);

	// 从父节点移除
	virtual void removeFromParent();

	// 移除子节点
	virtual bool removeChild(
		ENode * child
	);

	// 移除子节点
	virtual void removeChild(
		const EString & childName
	);

	// 移除所有节点
	virtual void clearAllChildren();

	// 执行动画
	virtual void runAction(
		EAction * action
	);

	// 继续动画
	virtual void resumeAction(
		EAction * action
	);

	// 暂停动画
	virtual void pauseAction(
		EAction * action
	);

	// 停止动画
	virtual void stopAction(
		EAction * action
	);

	// 继续所有暂停动画
	virtual void startAllActions();

	// 暂停所有动画
	virtual void pauseAllActions();

	// 停止所有动画
	virtual void stopAllActions();

	// 判断点是否在节点内
	virtual bool isPointIn(
		EPoint point
	);

protected:
	// 访问节点
	virtual void _callOn();

	// 渲染节点
	virtual void _onRender();

	// 渲染几何图形
	virtual void _drawGeometry();

	// 节点被添加到场景时的执行程序
	virtual void _onEnter();

	// 节点从场景中消失时的执行程序
	virtual void _onExit();

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

	// 设置节点宽度
	virtual void _setWidth(
		float width
	);

	// 设置节点高度
	virtual void _setHeight(
		float height
	);

	// 设置节点大小
	virtual void _setSize(
		const ESize & size
	);

	// 设置节点大小
	virtual void _setSize(
		float width,
		float height
	);

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
	EGeometry * m_pGeometry;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	D2D1::Matrix3x2F	m_Matri;
	EVector<ENode*>		m_vChildren;
};


class ETexture :
	public EObject
{
	friend ESprite;

public:
	// 创建一个空的纹理
	ETexture();

	// 从本地文件中读取资源
	ETexture(
		const EString & fileName
	);

	// 读取程序资源
	ETexture(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	virtual ~ETexture();

	// 从本地文件中读取资源
	void loadFromFile(
		const EString & fileName
	);

	// 读取程序资源
	void loadFromResource(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// 获取源图片宽度
	virtual float getSourceWidth() const;

	// 获取源图片高度
	virtual float getSourceHeight() const;

	// 获取源图片大小
	virtual ESize getSourceSize() const;

	// 预加载资源
	static bool preload(
		const EString & fileName
	);

	// 预加载资源
	static bool preload(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// 清空缓存
	static void clearCache();

protected:
	ID2D1Bitmap * _getBitmap();

protected:
	ID2D1Bitmap * m_pBitmap;
};


class ESpriteFrame :
	public EObject
{
	friend ESprite;

public:
	// 创建空的精灵帧
	ESpriteFrame();

	// 创建空的精灵帧
	ESpriteFrame(
		ETexture * texture
	);

	// 创建空的精灵帧
	ESpriteFrame(
		const EString & imageFileName
	);

	// 创建空的精灵帧
	ESpriteFrame(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// 创建空的精灵帧
	ESpriteFrame(
		ETexture * texture,
		float x,
		float y,
		float width,
		float height
	);

	// 创建空的精灵帧
	ESpriteFrame(
		const EString & imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	// 创建空的精灵帧
	ESpriteFrame(
		LPCTSTR resourceName,
		LPCTSTR resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESpriteFrame();

	// 获取宽度
	float getWidth() const;

	// 获取高度
	float getHeight() const;

	// 获取纹理
	ETexture * getTexture() const;

protected:
	// 获取纹理
	void _setTexture(
		ETexture * texture
	);

	// 裁剪纹理
	void _clipTexture(
		float x,
		float y,
		float width,
		float height
	);

protected:
	float	m_fSourceClipX;
	float	m_fSourceClipY;
	float	m_fSourceClipWidth;
	float	m_fSourceClipHeight;
	ETexture * m_pTexture;
};


class ESprite :
	public ENode
{
public:
	// 创建一个空精灵
	ESprite();

	// 从文理资源创建精灵
	ESprite(
		ETexture * texture
	);

	// 从精灵帧创建精灵
	ESprite(
		ESpriteFrame * spriteFrame
	);

	// 从文件图片创建精灵
	ESprite(
		const EString & imageFileName
	);

	// 从文件图片创建精灵并裁剪
	ESprite(
		const EString & imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	// 从资源图片创建精灵
	ESprite(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// 从资源图片创建精灵并裁剪
	ESprite(
		LPCTSTR resourceName,
		LPCTSTR resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESprite();
	
	// 加载精灵纹理
	void loadFrom(
		ETexture * texture
	);

	// 从本地文件加载纹理
	void loadFrom(
		const EString & imageFileName
	);

	// 从资源加载纹理
	void loadFrom(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// 加载纹理并裁剪
	void loadFrom(
		ETexture * texture,
		float x,
		float y,
		float width,
		float height
	);

	// 从精灵帧加载资源
	void loadFrom(
		ESpriteFrame * frame
	);

	// 裁剪纹理
	void clip(
		float x,
		float y,
		float width,
		float height
	);

protected:
	// 渲染精灵
	virtual void _onRender() override;

protected:
	float	m_fSourceClipX;
	float	m_fSourceClipY;
	ETexture * m_pTexture;
};


class EFont :
	public EObject
{
	friend EText;

public:
	EFont();

	EFont(
		EString fontFamily,
		float fontSize = 22,
		UINT32 color = EColor::WHITE,
		UINT32 fontWeight = EFontWeight::REGULAR,
		bool italic = false
	);

	virtual ~EFont();

	// 获取当前字号
	float getFontSize() const;

	// 获取当前字体粗细值
	UINT32 getFontWeight() const;

	// 获取文字颜色
	UINT32 getColor() const;

	// 是否是斜体
	bool isItalic() const;

	// 设置字体
	void setFamily(
		EString fontFamily
	);

	// 设置字号
	void setSize(
		float fontSize
	);

	// 设置字体粗细值
	void setWeight(
		UINT32 fontWeight
	);

	// 设置文字颜色
	void setColor(
		UINT32 color
	);

	// 设置文字斜体
	void setItalic(
		bool value
	);

protected:
	// 创建文字格式
	void _initTextFormat();

	// 获取文字格式
	IDWriteTextFormat * _getTextFormat();

protected:
	EString		m_sFontFamily;
	float		m_fFontSize;
	UINT32		m_FontWeight;
	UINT32		m_Color;
	bool		m_bItalic;
	bool		m_bRecreateNeeded;
	IDWriteTextFormat * m_pTextFormat;
};


class EText :
	public ENode
{
public:
	EText();

	EText(
		const EString & text
	);

	EText(
		EFont * font
	);

	EText(
		const EString & text,
		EFont * font
	);

	EText(
		const EString & text,
		EString fontFamily,
		float fontSize = 22,
		UINT32 color = EColor::WHITE,
		UINT32 fontWeight = EFontWeight::REGULAR,
		bool italic = false
	);

	virtual ~EText();

	// 获取文本
	EString getText() const;

	// 获取文本宽度
	virtual float getWidth() const override;

	// 获取文本宽度（不考虑缩放）
	virtual float getRealWidth() const override;

	// 获取字体
	EFont * getFont() const;

	// 设置文本
	void setText(
		const EString & text
	);

	// 设置字体
	void setFont(
		EFont * font
	);

	// 设置文字自动换行
	void setWordWrapping(
		bool value
	);

	// 设置文字换行宽度（WordWrapping 打开时生效）
	void setWordWrappingWidth(
		float wordWrapWidth
	);

protected:
	// 渲染文字
	virtual void _onRender() override;

	// 创建文字布局
	void _initTextLayout();

protected:
	EString	m_sText;
	bool	m_bWordWrapping;
	float	m_fWordWrappingWidth;
	EFont * m_pFont;
};


class EButton :
	public ENode
{
public:
	// 创建一个空按钮
	EButton();

	// 创建按钮
	EButton(
		ENode * normal,
		const BUTTON_CLICK_CALLBACK & callback
	);

	// 创建按钮
	EButton(
		ENode * normal,
		ENode * selected,
		const BUTTON_CLICK_CALLBACK & callback
	);

	// 创建按钮
	EButton(
		ENode * normal,
		ENode * mouseover,
		ENode * selected,
		const BUTTON_CLICK_CALLBACK & callback
	);

	// 创建按钮
	EButton(
		ENode * normal,
		ENode * mouseover,
		ENode * selected,
		ENode * disabled,
		const BUTTON_CLICK_CALLBACK & callback
	);

	// 设置一般情况下显示的按钮
	void setNormal(
		ENode * normal
	);

	// 设置鼠标移入按钮时显示的按钮
	void setMouseOver(
		ENode * mouseover
	);

	// 设置鼠标选中按钮时显示的按钮
	void setSelected(
		ENode * selected
	);

	// 设置按钮被禁用时显示的按钮
	void setDisabled(
		ENode * disabled
	);

	// 设置按钮禁用
	void setDisable(
		bool disable
	);

	// 设置回调函数
	void setCallback(
		const BUTTON_CLICK_CALLBACK & callback
	);

protected:
	// 渲染按钮
	virtual void _callOn() override;

	// 鼠标消息监听
	void _listenerCallback();

protected:
	enum STATUS { NORMAL, MOUSEOVER, SELECTED, DISABLED };
	STATUS	m_eStatus;
	ENode * m_pNormal;
	ENode * m_pMouseover;
	ENode * m_pSelected;
	ENode * m_pDisabled;
	ENode * m_pDisplayed;
	bool	m_bIsDisable;
	bool	m_bIsSelected;
	BUTTON_CLICK_CALLBACK m_Callback;
};

}