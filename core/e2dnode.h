#pragma once
#include "e2dbase.h"

namespace e2d 
{


class Action;
class TransitionBase;
class Collider;
class ColliderManager;

class Node :
	public Object
{
	friend Scene;
	friend Collider;
	friend TransitionBase;
	friend ColliderManager;

public:
	Node();

	virtual ~Node();

	// 更新节点
	virtual void onUpdate() {}

	// 固定地更新（游戏暂停时仍然运行）
	virtual void onFixedUpdate() {}

	// 渲染节点
	virtual void onRender() {}

	// 碰撞处理
	virtual void onCollide(
		Node* node	/* 发生碰撞的节点 */
	) {}

	// 获取节点显示状态
	virtual bool isVisiable() const;

	// 判断点是否在节点内
	virtual bool isPointIn(
		Point point
	) const;

	// 判断两节点是否相交
	virtual bool isIntersectWith(
		const Node * node
	) const;

	// 获取节点名称
	virtual String getName() const;

	// 获取节点名称的 Hash 值
	virtual unsigned int getHashName() const;

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

	// 获取节点属性
	virtual NodeProperty getProperty() const;

	// 获取节点碰撞体
	virtual Collider * getCollider() const;

	// 获取父节点
	virtual Node * getParent() const;

	// 获取节点所在场景
	virtual Scene * getParentScene() const;

	// 获取所有名称相同的子节点
	virtual std::vector<Node*> getChildren(
		const String& name
	) const;

	// 获取名称相同的子节点
	virtual Node* getChild(
		const String& name
	) const;

	// 获取所有子节点
	virtual std::vector<Node*> getAllChildren() const;

	// 获取子节点数量
	virtual int getChildrenCount() const;

	// 移除子节点
	virtual bool removeChild(
		Node * child
	);

	// 移除所有名称相同的子节点
	virtual void removeChildren(
		const String& childName
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
		const String& name
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

	// 节点坐标固定
	virtual void setPosFixed(
		bool fixed
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
	virtual void setWidth(
		double width
	);

	// 修改节点高度
	virtual void setHeight(
		double height
	);

	// 修改节点大小
	virtual void setSize(
		double width,
		double height
	);

	// 修改节点大小
	virtual void setSize(
		Size size
	);

	// 设置节点属性
	virtual void setProperty(
		NodeProperty prop
	);

	// 设置碰撞体
	virtual void setCollider(
		ColliderType nColliderType
	);

	// 设置碰撞体
	virtual void setCollider(
		Collider * pCollider
	);

	// 添加可碰撞节点的名称
	virtual void addColliableName(
		const String& collliderName
	);

#ifdef HIGHER_THAN_VS2012
	// 添加多个可碰撞节点的名称
	virtual void addColliableName(
		const std::initializer_list<String>& vCollliderName	/* 名称列表 */
	);
#endif

	// 移除可碰撞节点的名称
	virtual void removeColliableName(
		const String& collliderName
	);

	// 添加子节点
	virtual void addChild(
		Node * child,
		int order = 0	/* 渲染顺序 */
	);

#ifdef HIGHER_THAN_VS2012
	// 添加多个子节点
	virtual void addChild(
		const std::initializer_list<Node*>& vNodes,	/* 节点列表 */
		int order = 0								/* 渲染顺序 */
	);
#endif

	// 执行动画
	virtual void runAction(
		Action * action
	);

	// 继续动画
	virtual void resumeAction(
		const String& strActionName
	);

	// 暂停动画
	virtual void pauseAction(
		const String& strActionName
	);

	// 停止动画
	virtual void stopAction(
		const String& strActionName
	);

	// 获取名称相同的动画
	virtual Action * getAction(
		const String& strActionName
	);

	// 获取所有名称相同的动画
	virtual std::vector<Action*> getActions(
		const String& strActionName
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

	// 设置节点是否包含默认碰撞体（默认打开）
	static void setDefaultColliderEnable(
		bool enable
	);

	// 销毁对象
	virtual void onDestroy() override;

protected:
	// 更新节点
	void _update();

	// 渲染节点
	void _render();

	// 渲染图形
	void _drawCollider();

	// 设置节点所在场景
	void _setParentScene(
		Scene * scene
	);

	// 对自身进行二维矩阵变换
	void _updateSelfTransform();

	// 更新节点二维矩阵
	void _updateTransform();

	// 更新节点透明度
	void _updateOpacity();

protected:
	String		_name;
	unsigned	_nHashName;
	float		_fPosX;
	float		_fPosY;
	float		_fWidth;
	float		_fHeight;
	float		_fScaleX;
	float		_fScaleY;
	float		_fRotation;
	float		_fSkewAngleX;
	float		_fSkewAngleY;
	float		_fDisplayOpacity;
	float		_fRealOpacity;
	float		_fPivotX;
	float		_fPivotY;
	int			_nOrder;
	bool		_bVisiable;
	bool		_bAutoUpdate;
	bool		_bSortChildrenNeeded;
	bool		_bTransformNeeded;
	bool		_bPositionFixed;
	Collider *	_pCollider;
	Scene *		_pParentScene;
	Node *		_pParent;
	D2D1::Matrix3x2F		_MatriInitial;
	D2D1::Matrix3x2F		_MatriFinal;
	std::set<unsigned int>	_vColliders;
	std::vector<Node*>		_vChildren;
};


class Sprite :
	public Node
{
public:
	// 创建一个空精灵
	Sprite();

	// 从 Image 对象创建精灵
	Sprite(
		Image * image
	);

	// 加载图片文件
	Sprite(
		const String& filePath	/* 图片文件路径 */
	);

	// 加载图片资源
	Sprite(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	// 加载图片文件
	Sprite(
		const String& filePath,	/* 图片文件路径 */
		double x,
		double y,
		double width,
		double height
	);

	// 加载图片资源
	Sprite(
		int resNameId,			/* 图片资源名称 */
		const String& resType,	/* 图片资源类型 */
		double x,
		double y,
		double width,
		double height
	);

	virtual ~Sprite();

	// 加载图片文件
	bool open(
		const String& filePath
	);

	// 加载图片资源
	bool open(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);
	
	// 加载图片
	bool open(
		Image * image
	);

	// 将图片裁剪为矩形
	void crop(
		double x,
		double y,
		double width,
		double height
	);

	// 获取 Image 对象
	virtual Image * getImage() const;

	// 渲染精灵
	virtual void onRender() override;

	// 销毁对象
	virtual void onDestroy() override;

protected:
	Image * _pImage;
};


class Text :
	public Node
{
public:
	Text();

	Text(
		const String& text		/* 文字内容 */
	);

	Text(
		TextStyle textStyle		/* 文字样式 */
	);

	Text(
		const String& text,		/* 文字内容 */
		TextStyle textStyle		/* 文字样式 */
	);

	Text(
		const String& text,						/* 文字内容*/
		const String& fontFamily,				/* 字体 */
		double fontSize = 22,					/* 字号 */
		UINT32 color = Color::WHITE,			/* 颜色 */
		UINT32 fontWeight = FontWeight::NORMAL,	/* 粗细值 */
		bool italic = false,					/* 斜体 */
		TextAlign alignment = TextAlign::LEFT,	/* 对齐方式 */
		bool wrapping = false,					/* 打开自动换行 */
		double wrappingWidth = 0.0,				/* 自动换行宽度 */
		double lineSpacing = 0.0,				/* 行间距 */
		bool hasUnderline = false,				/* 下划线 */
		bool hasStrikethrough = false,			/* 删除线 */
		bool hasOutline = true,					/* 显示描边 */
		UINT32 outlineColor = Color::BLACK,		/* 描边颜色 */
		UINT32 outlineWidth = 1.0				/* 描边线宽 */
	);

	virtual ~Text();

	// 获取文本
	String getText() const;

	// 获取文本样式
	TextStyle getTextStyle() const;

	// 获取字体
	String getFontFamily() const;

	// 获取当前字号
	double getFontSize() const;

	// 获取当前字体粗细值
	UINT32 getFontWeight() const;

	// 获取文字颜色
	Color getColor() const;

	// 获取描边颜色
	Color getOutlineColor() const;

	// 获取描边线宽
	double getOutlineWidth() const;

	// 获取描边线相交样式
	LineJoin getOutlineJoin() const;

	// 获取文本显示行数
	int getLineCount() const;

	// 是否是斜体
	bool isItalic() const;

	// 是否显示删除线
	bool hasStrikethrough() const;

	// 是否显示下划线
	bool hasUnderline() const;

	// 是否显示描边
	bool hasOutline() const;

	// 设置文本
	void setText(
		const String& text
	);

	// 设置文本样式
	void setTextStyle(
		TextStyle pTextStyle
	);

	// 设置字体
	void setFontFamily(
		const String& fontFamily
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
		Color color
	);

	// 设置文字斜体（默认值为 false）
	void setItalic(
		bool value
	);

	// 打开或关闭文本自动换行（默认为关闭）
	void setWrapping(
		bool wrapping
	);

	// 设置文本自动换行的宽度（默认为 0）
	void setWrappingWidth(
		double fWrappingWidth
	);

	// 设置行间距（默认为 0）
	void setLineSpacing(
		double fLineSpacing
	);

	// 设置对齐方式（默认为 TextAlign::LEFT）
	void setAlignment(
		TextAlign align
	);

	// 设置下划线（默认值为 false）
	void setUnderline(
		bool hasUnderline
	);

	// 设置删除线（默认值为 false）
	void setStrikethrough(
		bool hasStrikethrough
	);

	// 设置是否显示描边
	void setOutline(
		bool hasOutline
	);

	// 设置描边颜色
	void setOutlineColor(
		Color outlineColor
	);

	// 设置描边线宽
	void setOutlineWidth(
		double outlineWidth
	);

	// 设置描边线相交样式
	void setOutlineJoin(
		LineJoin outlineJoin
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
	String	_sText;
	TextStyle _TextStyle;
	IDWriteTextFormat * _pDWriteTextFormat;
	IDWriteTextLayout * _pDWriteTextLayout;
};


class Button :
	public Node
{
public:
	// 创建一个空按钮
	Button();

	// 创建按钮
	Button(
		Node * normal,					/* 普通状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建按钮
	Button(
		Node * normal,					/* 普通状态 */
		Node * selected,				/* 鼠标按下状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建按钮
	Button(
		Node * normal,					/* 普通状态 */
		Node * mouseover,				/* 鼠标移入状态 */
		Node * selected,				/* 鼠标按下状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建按钮
	Button(
		Node * normal,					/* 普通状态 */
		Node * mouseover,				/* 鼠标移入状态 */
		Node * selected,				/* 鼠标移入状态 */
		Node * disabled,				/* 按钮禁用状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 获取按钮状态是启用还是禁用
	bool isEnable() const;

	// 设置按钮启用或禁用
	void setEnable(
		bool enable
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
	void setClickFunc(
		const Function& func
	);

	// 更新按钮状态
	virtual void onFixedUpdate() override;

protected:
	// 按钮状态枚举
	enum class ButtonState { NORMAL, MOUSEOVER, SELECTED };

	// 设置按钮状态
	virtual void _setState(ButtonState state);

	// 刷新按钮显示
	virtual void _updateVisiable();

	// 执行按钮函数对象
	virtual void _runCallback();

protected:
	Node *		_pNormal;
	Node *		_pMouseover;
	Node *		_pSelected;
	Node *		_pDisabled;
	bool		_bEnable;
	bool		_bIsSelected;
	ButtonState	_eBtnState;
	Function	_func;
};


class ButtonToggle :
	public Button
{
public:
	// 创建一个空的开关按钮
	ButtonToggle();

	// 创建开关按钮
	ButtonToggle(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建开关按钮
	ButtonToggle(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		Node * onSelected,				/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,				/* 按钮关闭时，鼠标按下状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建开关按钮
	ButtonToggle(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		Node * onMouseOver,				/* 按钮打开时，鼠标移入状态 */
		Node * offMouseOver,			/* 按钮关闭时，鼠标移入状态 */
		Node * onSelected,				/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,				/* 按钮关闭时，鼠标按下状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	// 创建开关按钮
	ButtonToggle(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		Node * onMouseOver,				/* 按钮打开时，鼠标移入状态 */
		Node * offMouseOver,			/* 按钮关闭时，鼠标移入状态 */
		Node * onSelected,				/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,				/* 按钮关闭时，鼠标按下状态 */
		Node * onDisabled,				/* 按钮打开时，禁用状态 */
		Node * offDisabled,				/* 按钮关闭时，禁用状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
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
	Node *	_pNormalOn;
	Node *	_pNormalOff;
	Node *	_pMouseoverOn;
	Node *	_pMouseoverOff;
	Node *	_pSelectedOn;
	Node *	_pSelectedOff;
	Node *	_pDisabledOn;
	Node *	_pDisabledOff;
	bool	_bState;
};


class Menu :
	public Node
{
public:
	// 创建空菜单
	Menu();

#ifdef HIGHER_THAN_VS2012
	// 创建菜单
	Menu(
		const std::initializer_list<Button*>& vButtons	/* 按钮列表 */
	);
#else
	// 创建菜单
	Menu(
		int number,			/* 菜单中按钮的数量 */
		Button * button1,	/* 第一个按钮 */
		...
	);
#endif

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
	bool _bEnable;
	std::vector<Button*> _vButtons;
};

}