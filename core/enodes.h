#pragma once
#include "ebase.h"

namespace e2d 
{


class EAction;
class EShape;
class ETransition;

class ENode :
	public EObject
{
	friend EScene;
	friend EShape;
	friend ETransition;

public:
	ENode();

	virtual ~ENode();

	// 进入场景时执行
	virtual void onEnter() {}

	// 离开场景时执行
	virtual void onExit() {}

	// 更新节点
	virtual void onUpdate() {}

	// 渲染节点
	virtual void onRender() {}

	// 碰撞处理
	virtual void onCollide(
		ENode* pCollisionNode,	/* 发生碰撞的节点 */
		int nRelation			/* 碰撞关系，取值为 ERelation::VALUE 中的一种 */
	) {}

	// 游戏暂停时的处理
	virtual void onPause() {}

	// 节点被销毁时的处理
	virtual void onDestroy() {}

	// 获取节点显示状态
	virtual bool isVisiable() const;

	// 判断点是否在节点内
	virtual bool isPointIn(
		EPoint point
	);

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

	// 获取节点的中心点
	virtual float getPivotX() const;

	// 获取节点的中心点
	virtual float getPivotY() const;

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

	// 获取节点形状
	virtual EShape * getShape() const;

	// 获取父节点
	virtual ENode * getParent() const;

	// 获取节点所在场景
	virtual EScene * getParentScene() const;

	// 获取名称相同的子节点
	virtual ENode * getChild(
		const EString & name
	);

	// 获取所有名称相同的子节点
	virtual std::vector<ENode*> getChildren(
		const EString & name
	);

	// 获取所有子节点
	virtual std::vector<ENode*> &getChildren();

	// 获取子节点数量
	virtual int getChildrenCount() const;

	// 移除子节点
	virtual bool removeChild(
		ENode * child
	);

	// 移除所有名称相同的子节点
	virtual void removeChildren(
		const EString & childName
	);

	// 从父节点移除
	virtual void removeFromParent();

	// 移除所有节点
	virtual void clearAllChildren();

	// 设置节点是否显示
	virtual void setVisiable(
		bool value
	);

	// 开启或禁用 onUpdate 函数
	virtual void setAutoUpdate(
		bool bAutoUpdate
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
		const EVector2 & v
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

	// 设置中心点的横向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setPivotX(
		float pivotX
	);

	// 设置中心点的纵向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setPivotY(
		float pivotY
	);

	// 设置中心点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual void setPivot(
		float pivotX,
		float pivotY
	);

	// 设置节点形状
	virtual void setShape(
		EShape * pShape
	);

	// 添加子节点
	virtual void addChild(
		ENode * child,
		int order = 0
	);

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
	virtual void resumeAllActions();

	// 暂停所有动画
	virtual void pauseAllActions();

	// 停止所有动画
	virtual void stopAllActions();

	// 修改节点的默认中心点位置
	static void setDefaultPiovt(
		float defaultPiovtX,
		float defaultPiovtY
	);

protected:
	// 更新节点
	void _update(
		bool bPaused
	);

	// 渲染节点
	void _render();

	// 渲染图形
	void _drawShape();

	// 节点被添加到场景时的执行程序
	void _onEnter();

	// 节点从场景中消失时的执行程序
	void _onExit();

	// 设置节点所在场景
	void _setParentScene(
		EScene * scene
	);

	// 对自身进行二维矩阵变换
	void _updateTransform();

	// 更新所有子节点矩阵
	void _updateChildrenTransform();

	// 更新所有子节点透明度
	void _updateChildrenOpacity();

	// 修改节点大小
	void _setSize(
		float width,
		float height
	);

	// 更新节点二维矩阵
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
	float		m_fPivotX;
	float		m_fPivotY;
	int			m_nOrder;
	bool		m_bVisiable;
	bool		m_bAutoUpdate;
	bool		m_bDisplayedInScene;
	bool		m_bSortChildrenNeeded;
	bool		m_bTransformNeeded;
	EShape *	m_pShape;
	EScene *	m_pParentScene;
	ENode *		m_pParent;
	D2D1::Matrix3x2F	m_MatriInitial;
	D2D1::Matrix3x2F	m_MatriFinal;
	std::vector<ENode*>	m_vChildren;
};


class ESprite :
	public ENode
{
public:
	// 创建一个空精灵
	ESprite();

	// 从 EImage 对象创建精灵
	ESprite(
		EImage * image
	);

	// 从文件图片创建精灵
	ESprite(
		LPCTSTR imageFileName
	);

	// 从文件图片创建精灵并裁剪
	ESprite(
		LPCTSTR imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESprite();
	
	// 加载精灵图片
	virtual void loadFrom(
		EImage * texture
	);

	// 从本地文件加载图片
	virtual void loadFrom(
		LPCTSTR imageFileName
	);

	// 裁剪图片
	virtual void clip(
		float x,
		float y,
		float width,
		float height
	);

	// 获取 EImage 对象
	virtual EImage * getImage() const;

	// 渲染精灵
	virtual void onRender() override;

protected:
	EImage * m_pImage;
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

	// 渲染文字
	virtual void onRender() override;

protected:
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
		ENode * normal,		/* 普通状态 */
		const BtnClkCallback & callback = nullptr
	);

	// 创建按钮
	EButton(
		ENode * normal,		/* 普通状态 */
		ENode * selected,	/* 鼠标按下状态 */
		const BtnClkCallback & callback = nullptr
	);

	// 创建按钮
	EButton(
		ENode * normal,		/* 普通状态 */
		ENode * mouseover,	/* 鼠标移入状态 */
		ENode * selected,	/* 鼠标按下状态 */
		const BtnClkCallback & callback = nullptr
	);

	// 创建按钮
	EButton(
		ENode * normal,		/* 普通状态 */
		ENode * mouseover,	/* 鼠标移入状态 */
		ENode * selected,	/* 鼠标移入状态 */
		ENode * disabled,	/* 按钮禁用状态 */
		const BtnClkCallback & callback = nullptr
	);

	// 获取按钮状态是启用还是禁用
	bool isEnable() const;

	// 设置按钮启用或禁用
	void setEnable(
		bool bEnable
	);

	// 设置一般情况下显示的按钮
	virtual void setNormal(
		ENode * normal
	);

	// 设置鼠标移入按钮时显示的按钮
	virtual void setMouseOver(
		ENode * mouseover
	);

	// 设置鼠标选中按钮时显示的按钮
	virtual void setSelected(
		ENode * selected
	);

	// 设置按钮被禁用时显示的按钮
	virtual void setDisabled(
		ENode * disabled
	);

	// 设置回调函数
	void setCallback(
		const BtnClkCallback & callback
	);

	// 设置中心点的横向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setPivotX(
		float pivotX
	) override;

	// 设置中心点的纵向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setPivotY(
		float pivotY
	) override;

	// 设置中心点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual void setPivot(
		float pivotX,
		float pivotY
	) override;

	// 更新按钮状态
	virtual void onUpdate() override;

	// 更新游戏暂停时的按钮状态
	virtual void onPause() override;

protected:
	enum BTN_STATE { NORMAL, MOUSEOVER, SELECTED };

	// 设置按钮状态
	virtual void _setState(BTN_STATE state);

	// 刷新按钮显示
	virtual void _updateVisiable();

	// 执行按钮回调函数
	virtual void _runCallback();

protected:
	ENode *			m_pNormal;
	ENode *			m_pMouseover;
	ENode *			m_pSelected;
	ENode *			m_pDisabled;
	bool			m_bEnable;
	bool			m_bIsSelected;
	BTN_STATE		m_eBtnState;
	BtnClkCallback	m_Callback;
};


class EButtonToggle :
	public EButton
{
public:
	// 创建一个空的开关按钮
	EButtonToggle();

	// 创建开关按钮
	EButtonToggle(
		ENode * onNormal,
		ENode * offNormal,
		const BtnClkCallback & callback = nullptr
	);

	// 创建开关按钮
	EButtonToggle(
		ENode * onNormal,
		ENode * offNormal,
		ENode * onSelected,
		ENode * offSelected,
		const BtnClkCallback & callback = nullptr
	);

	// 创建开关按钮
	EButtonToggle(
		ENode * onNormal,
		ENode * offNormal,
		ENode * onMouseOver,
		ENode * offMouseOver,
		ENode * onSelected,
		ENode * offSelected,
		const BtnClkCallback & callback = nullptr
	);

	// 创建开关按钮
	EButtonToggle(
		ENode * onNormal,
		ENode * offNormal,
		ENode * onMouseOver,
		ENode * offMouseOver,
		ENode * onSelected,
		ENode * offSelected,
		ENode * onDisabled,
		ENode * offDisabled,
		const BtnClkCallback & callback = nullptr
	);

	// 切换开关状态（执行回调函数）
	void toggle();

	// 获取开关状态
	bool getState() const;

	// 设置开关按钮的状态（打开或关闭）
	void setState(
		bool bState
	);

	// 设置按钮打开状态下显示的按钮
	virtual void setNormal(
		ENode * normal
	) override;

	// 设置按钮打开状态下，鼠标移入按钮时显示的按钮
	virtual void setMouseOver(
		ENode * mouseover
	) override;

	// 设置按钮打开状态下，鼠标选中按钮时显示的按钮
	virtual void setSelected(
		ENode * selected
	) override;

	// 设置按钮打开状态下，被禁用时显示的按钮
	virtual void setDisabled(
		ENode * disabled
	) override;

	// 设置按钮关闭状态下显示的按钮
	void setNormalOff(
		ENode * normal
	);

	// 设置按钮关闭状态下，鼠标移入按钮时显示的按钮
	void setMouseOverOff(
		ENode * mouseover
	);

	// 设置按钮关闭状态下，鼠标选中按钮时显示的按钮
	void setSelectedOff(
		ENode * selected
	);

	// 设置按钮关闭状态下，按钮被禁用时显示的按钮
	void setDisabledOff(
		ENode * disabled
	);

	// 设置中心点的横向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setPivotX(
		float pivotX
	) override;

	// 设置中心点的纵向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setPivotY(
		float pivotY
	) override;

	// 设置中心点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual void setPivot(
		float pivotX,
		float pivotY
	) override;

protected:
	// 刷新按钮开关
	virtual void _updateState();

	// 执行按钮回调函数
	virtual void _runCallback() override;

protected:
	ENode * m_pNormalOn;
	ENode * m_pNormalOff;
	ENode * m_pMouseoverOn;
	ENode * m_pMouseoverOff;
	ENode * m_pSelectedOn;
	ENode * m_pSelectedOff;
	ENode * m_pDisabledOn;
	ENode * m_pDisabledOff;
	bool	m_bState;
};


class EMenu :
	public ENode
{
public:
	// 创建空菜单
	EMenu();

	// 创建菜单
	EMenu(
		int number,			/* 菜单中按钮的数量 */
		EButton * button1,	/* 第一个按钮 */
		...
	);

	// 获取菜单是否禁用
	bool isEnable() const;

	// 获取菜单中的按钮数量
	size_t getButtonCount() const;

	// 设置菜单启用或禁用
	void setEnable(
		bool enable
	);

	// 添加按钮
	void addButton(
		EButton * button
	);

	// 移除按钮
	bool removeButton(
		EButton * button
	);

protected:
	bool m_bEnable;
	std::vector<EButton*> m_vButtons;
};

}