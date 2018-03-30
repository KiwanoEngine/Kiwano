#pragma once
#include "ebase.h"

namespace e2d 
{


class Action;
class Shape;
class Transition;

class Node :
	public Object
{
	friend Scene;
	friend Shape;
	friend Transition;

public:
	Node();

	virtual ~Node();

	// 进入场景时执行
	virtual void onEnter() {}

	// 离开场景时执行
	virtual void onExit() {}

	// 更新节点
	virtual void onUpdate() {}

	// 固定地更新（游戏暂停时仍然运行）
	virtual void onFixedUpdate() {}

	// 渲染节点
	virtual void onRender() {}

	// 碰撞处理
	virtual void onCollide(
		Node* pCollisionNode,	/* 发生碰撞的节点 */
		int nRelation			/* 碰撞关系，取值为 Relation::VALUE 中的一种 */
	) {}

	// 获取节点显示状态
	virtual bool isVisiable() const;

	// 判断点是否在节点内
	virtual bool isPointIn(
		Point point
	) const;

	// 判断两节点是否相交
	virtual bool isIntersectWith(
		const Node * pNode
	) const;

	// 获取节点名称
	virtual String getName() const;

	// 获取节点绘图顺序
	virtual int getOrder() const;

	// 获取节点横坐标
	virtual double getPosX() const;

	// 获取节点纵坐标
	virtual double getPosY() const;

	// 获取节点坐标
	virtual Point getPos() const;

	// 获取节点宽度
	virtual double getWidth() const;

	// 获取节点高度
	virtual double getHeight() const;

	// 获取节点宽度（不考虑缩放）
	virtual double getRealWidth() const;

	// 获取节点高度（不考虑缩放）
	virtual double getRealHeight() const;

	// 获取节点大小（不考虑缩放）
	virtual Size getRealSize() const;

	// 获取节点的中心点
	virtual double getPivotX() const;

	// 获取节点的中心点
	virtual double getPivotY() const;

	// 获取节点大小
	virtual Size getSize() const;

	// 获取节点横向缩放比例
	virtual double getScaleX() const;

	// 获取节点纵向缩放比例
	virtual double getScaleY() const;

	// 获取节点横向倾斜角度
	virtual double getSkewX() const;

	// 获取节点纵向倾斜角度
	virtual double getSkewY() const;

	// 获取节点旋转角度
	virtual double getRotation() const;

	// 获取节点透明度
	virtual double getOpacity() const;

	// 获取节点形状
	virtual Shape * getShape() const;

	// 获取父节点
	virtual Node * getParent() const;

	// 获取节点所在场景
	virtual Scene * getParentScene() const;

	// 获取所有名称相同的子节点
	virtual std::vector<Node*> getChildren(
		String name
	);

	// 获取所有子节点
	virtual std::vector<Node*> getChildren();

	// 获取子节点数量
	virtual int getChildrenCount() const;

	// 移除子节点
	virtual bool removeChild(
		Node * child
	);

	// 移除所有名称相同的子节点
	virtual void removeChildren(
		String childName
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
		String name
	);

	// 设置节点横坐标
	virtual void setPosX(
		double x
	);

	// 设置节点纵坐标
	virtual void setPosY(
		double y
	);

	// 设置节点坐标
	virtual void setPos(
		const Point & point
	);

	// 设置节点坐标
	virtual void setPos(
		double x,
		double y
	);

	// 移动节点
	virtual void movePosX(
		double x
	);

	// 移动节点
	virtual void movePosY(
		double y
	);

	// 移动节点
	virtual void movePos(
		double x,
		double y
	);

	// 移动节点
	virtual void movePos(
		const Vector & v
	);

	// 设置节点绘图顺序
	// 默认为 0
	virtual void setOrder(
		int order
	);

	// 设置横向缩放比例
	// 默认为 1.0f
	virtual void setScaleX(
		double scaleX
	);

	// 设置纵向缩放比例
	// 默认为 1.0f
	virtual void setScaleY(
		double scaleY
	);

	// 设置缩放比例
	// 默认为 (1.0f, 1.0f)
	virtual void setScale(
		double scaleX,
		double scaleY
	);

	// 设置缩放比例
	// 默认为 1.0f
	virtual void setScale(
		double scale
	);

	// 设置横向倾斜角度
	// 默认为 0
	virtual void setSkewX(
		double angleX
	);

	// 设置纵向倾斜角度
	// 默认为 0
	virtual void setSkewY(
		double angleY
	);

	// 设置倾斜角度
	// 默认为 (0, 0)
	virtual void setSkew(
		double angleX,
		double angleY
	);

	// 设置旋转角度
	// 默认为 0
	virtual void setRotation(
		double rotation
	);

	// 设置透明度
	// 默认为 1.0f, 范围 [0, 1]
	virtual void setOpacity(
		double opacity
	);

	// 设置中心点的横向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setPivotX(
		double pivotX
	);

	// 设置中心点的纵向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setPivotY(
		double pivotY
	);

	// 设置中心点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual void setPivot(
		double pivotX,
		double pivotY
	);

	// 修改节点宽度
	void setWidth(
		double width
	);

	// 修改节点高度
	void setHeight(
		double height
	);

	// 修改节点大小
	void setSize(
		double width,
		double height
	);

	// 修改节点大小
	void setSize(
		Size size
	);

	// 设置节点形状
	virtual void setShape(
		Shape * pShape
	);

	// 添加子节点
	virtual void addChild(
		Node * child,
		int order = 0
	);

	// 执行动画
	virtual void runAction(
		Action * action
	);

	// 继续动画
	virtual void resumeAction(
		String strActionName
	);

	// 暂停动画
	virtual void pauseAction(
		String strActionName
	);

	// 停止动画
	virtual void stopAction(
		String strActionName
	);

	// 获取名称相同的动画
	virtual Action * getAction(
		String strActionName
	);

	// 获取所有名称相同的动画
	virtual std::vector<Action*> getActions(
		String strActionName
	);

	// 继续所有暂停动画
	virtual void resumeAllActions();

	// 暂停所有动画
	virtual void pauseAllActions();

	// 停止所有动画
	virtual void stopAllActions();

	// 修改节点的默认中心点位置
	static void setDefaultPiovt(
		double defaultPiovtX,
		double defaultPiovtY
	);

	// 设置节点是否包含默认形状（默认打开）
	static void setDefaultShapeEnable(
		bool bEnable
	);

protected:
	// 更新节点
	void _update();

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
		Scene * scene
	);

	// 对自身进行二维矩阵变换
	void _updateTransform();

	// 更新所有子节点矩阵
	void _updateChildrenTransform();

	// 更新所有子节点透明度
	void _updateChildrenOpacity();

	// 更新节点二维矩阵
	static void _updateTransform(Node * node);

	// 更新节点透明度
	static void _updateOpacity(Node * node);

protected:
	String		m_sName;
	unsigned	m_nHashName;
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
	float		m_fPivotX;
	float		m_fPivotY;
	int			m_nOrder;
	bool		m_bVisiable;
	bool		m_bAutoUpdate;
	bool		m_bDisplayedInScene;
	bool		m_bSortChildrenNeeded;
	bool		m_bTransformNeeded;
	Shape *	m_pShape;
	Scene *		m_pParentScene;
	Node *		m_pParent;
	D2D1::Matrix3x2F	m_MatriInitial;
	D2D1::Matrix3x2F	m_MatriFinal;
	std::vector<Node*>	m_vChildren;
};


class Sprite :
	public Node
{
public:
	// 创建一个空精灵
	Sprite();

	// 从 EImage 对象创建精灵
	Sprite(
		Image * image
	);

	// 从文件图片创建精灵
	Sprite(
		String imageFileName
	);

	// 从文件图片创建精灵并裁剪
	Sprite(
		String imageFileName,
		double x,
		double y,
		double width,
		double height
	);

	virtual ~Sprite();

	// 从本地文件加载图片
	virtual void open(
		String imageFileName
	);
	
	// 加载图片
	virtual void open(
		Image * image
	);

	// 裁剪图片
	virtual void clip(
		double x,
		double y,
		double width,
		double height
	);

	// 获取 EImage 对象
	virtual Image * getImage() const;

	// 渲染精灵
	virtual void onRender() override;

protected:
	Image * m_pImage;
};


class Text :
	public Node
{
public:
	Text();

	Text(
		String text		/* 文字内容 */
	);

	Text(
		Font font		/* 字体样式 */
	);

	Text(
		String text,	/* 文字内容 */
		Font font		/* 字体样式 */
	);

	Text(
		String text,							/* 文字内容*/
		String fontFamily,						/* 字体 */
		double fontSize = 22,					/* 字号 */
		UINT32 color = Color::WHITE,			/* 颜色 */
		UINT32 fontWeight = FontWeight::NORMAL,	/* 粗细值 */
		bool italic = false,					/* 斜体 */
		bool hasUnderline = false,				/* 下划线 */
		bool hasStrikethrough = false			/* 删除线 */
	);

	virtual ~Text();

	// 获取文本
	String getText() const;

	// 获取文本样式
	Font getFont() const;

	// 获取字体
	String getFontFamily() const;

	// 获取当前字号
	double getFontSize() const;

	// 获取当前字体粗细值
	UINT32 getFontWeight() const;

	// 获取文字颜色
	UINT32 getColor() const;

	// 是否是斜体
	bool isItalic() const;

	// 设置文本
	void setText(
		String text
	);

	// 设置文本样式
	void setFont(
		Font pFont
	);

	// 设置字体
	void setFontFamily(
		String fontFamily
	);

	// 设置字号（默认值为 22）
	void setFontSize(
		double fontSize
	);

	// 设置字体粗细值（默认值为 FontWeight::NORMAL）
	void setFontWeight(
		UINT32 fontWeight
	);

	// 设置文字颜色（默认值为 Color::WHITE）
	void setColor(
		UINT32 color
	);

	// 设置文字斜体（默认值为 false）
	void setItalic(
		bool value
	);

	// 设置文本自动换行（根据 WrappingWidth 进行自动换行，默认为 false）
	void setWrappingEnable(
		bool bWrappingEnable
	);

	// 设置文本换行宽度（WrappingEnable 开启时生效，默认为 0）
	void setWrappingWidth(
		double fWrappingWidth
	);

	// 设置行间距（默认为 0）
	void setLineSpacing(
		double fLineSpacing
	);

	// 设置对齐方式（默认为 TextAlign::LEFT）
	void setAlignment(
		UINT32 nAlign
	);

	// 设置下划线（默认值为 false）
	void setUnderline(
		bool hasUnderline
	);

	// 设置删除线（默认值为 false）
	void setStrikethrough(
		bool hasStrikethrough
	);

	// 渲染文字
	virtual void onRender() override;

protected:
	// 重新排版文字
	void _reset();

	// 创建文字格式化
	void _createFormat();

	// 创建文字布局
	void _createLayout();

protected:
	String	m_sText;
	bool	m_bWrappingEnable;
	float	m_fWrappingWidth;
	Font	m_Font;
	UINT32	m_nAlign;
	float	m_fLineSpacing;
	IDWriteTextFormat * m_pDWriteTextFormat;
	IDWriteTextLayout * m_pDWriteTextLayout;
};


class Button :
	public Node
{
public:
	// 创建一个空按钮
	Button();

	// 创建按钮
	Button(
		Node * normal,			/* 普通状态 */
		Function func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建按钮
	Button(
		Node * normal,			/* 普通状态 */
		Node * selected,		/* 鼠标按下状态 */
		Function func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建按钮
	Button(
		Node * normal,			/* 普通状态 */
		Node * mouseover,		/* 鼠标移入状态 */
		Node * selected,		/* 鼠标按下状态 */
		Function func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建按钮
	Button(
		Node * normal,			/* 普通状态 */
		Node * mouseover,		/* 鼠标移入状态 */
		Node * selected,		/* 鼠标移入状态 */
		Node * disabled,		/* 按钮禁用状态 */
		Function func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 获取按钮状态是启用还是禁用
	bool isEnable() const;

	// 设置按钮启用或禁用
	void setEnable(
		bool bEnable
	);

	// 设置一般情况下显示的按钮
	virtual void setNormal(
		Node * normal
	);

	// 设置鼠标移入按钮时显示的按钮
	virtual void setMouseOver(
		Node * mouseover
	);

	// 设置鼠标按下按钮时显示的按钮
	virtual void setSelected(
		Node * selected
	);

	// 设置按钮被禁用时显示的按钮
	virtual void setDisabled(
		Node * disabled
	);

	// 设置按钮点击后的执行函数
	void setFunction(
		Function func
	);

	// 更新按钮状态
	virtual void onFixedUpdate() override;

protected:
	enum BTN_STATE { NORMAL, MOUSEOVER, SELECTED };

	// 设置按钮状态
	virtual void _setState(BTN_STATE state);

	// 刷新按钮显示
	virtual void _updateVisiable();

	// 执行按钮函数对象
	virtual void _runCallback();

protected:
	Node *		m_pNormal;
	Node *		m_pMouseover;
	Node *		m_pSelected;
	Node *		m_pDisabled;
	bool		m_bEnable;
	bool		m_bIsSelected;
	BTN_STATE	m_eBtnState;
	Function	m_Callback;
};


class ButtonToggle :
	public Button
{
public:
	// 创建一个空的开关按钮
	ButtonToggle();

	// 创建开关按钮
	ButtonToggle(
		Node * onNormal,		/* 按钮打开时，普通状态 */
		Node * offNormal,		/* 按钮关闭时，普通状态 */
		Function func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建开关按钮
	ButtonToggle(
		Node * onNormal,		/* 按钮打开时，普通状态 */
		Node * offNormal,		/* 按钮关闭时，普通状态 */
		Node * onSelected,		/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,		/* 按钮关闭时，鼠标按下状态 */
		Function func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建开关按钮
	ButtonToggle(
		Node * onNormal,		/* 按钮打开时，普通状态 */
		Node * offNormal,		/* 按钮关闭时，普通状态 */
		Node * onMouseOver,		/* 按钮打开时，鼠标移入状态 */
		Node * offMouseOver,	/* 按钮关闭时，鼠标移入状态 */
		Node * onSelected,		/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,		/* 按钮关闭时，鼠标按下状态 */
		Function func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建开关按钮
	ButtonToggle(
		Node * onNormal,		/* 按钮打开时，普通状态 */
		Node * offNormal,		/* 按钮关闭时，普通状态 */
		Node * onMouseOver,		/* 按钮打开时，鼠标移入状态 */
		Node * offMouseOver,	/* 按钮关闭时，鼠标移入状态 */
		Node * onSelected,		/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,		/* 按钮关闭时，鼠标按下状态 */
		Node * onDisabled,		/* 按钮打开时，禁用状态 */
		Node * offDisabled,		/* 按钮关闭时，禁用状态 */
		Function func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 获取开关状态（打开或关闭）
	bool getState() const;

	// 设置开关按钮的状态（打开或关闭）
	void setState(
		bool bState
	);

	// 设置按钮打开状态下显示的按钮
	virtual void setNormal(
		Node * normal
	) override;

	// 设置按钮打开状态下，鼠标移入按钮时显示的按钮
	virtual void setMouseOver(
		Node * mouseover
	) override;

	// 设置按钮打开状态下，鼠标按下按钮时显示的按钮
	virtual void setSelected(
		Node * selected
	) override;

	// 设置按钮打开状态下，被禁用时显示的按钮
	virtual void setDisabled(
		Node * disabled
	) override;

	// 设置按钮关闭状态下显示的按钮
	void setNormalOff(
		Node * normal
	);

	// 设置按钮关闭状态下，鼠标移入按钮时显示的按钮
	void setMouseOverOff(
		Node * mouseover
	);

	// 设置按钮关闭状态下，鼠标按下按钮时显示的按钮
	void setSelectedOff(
		Node * selected
	);

	// 设置按钮关闭状态下，按钮被禁用时显示的按钮
	void setDisabledOff(
		Node * disabled
	);

protected:
	// 刷新按钮开关
	virtual void _updateState();

	// 执行按钮函数对象
	virtual void _runCallback() override;

protected:
	Node *	m_pNormalOn;
	Node *	m_pNormalOff;
	Node *	m_pMouseoverOn;
	Node *	m_pMouseoverOff;
	Node *	m_pSelectedOn;
	Node *	m_pSelectedOff;
	Node *	m_pDisabledOn;
	Node *	m_pDisabledOff;
	bool	m_bState;
};


class Menu :
	public Node
{
public:
	// 创建空菜单
	Menu();

	// 创建菜单
	Menu(
		int number,			/* 菜单中按钮的数量 */
		Button * button1,	/* 第一个按钮 */
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
		Button * button
	);

	// 移除按钮
	bool removeButton(
		Button * button
	);

protected:
	bool m_bEnable;
	std::vector<Button*> m_vButtons;
};

}