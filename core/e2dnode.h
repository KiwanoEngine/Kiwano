#pragma once
#include "e2dbase.h"
#include "e2dcollider.h"

namespace e2d 
{


class Action;
class Transition;
class ColliderManager;

class Node :
	public Object
{
	friend class Scene;
	friend class Collider;
	friend class Transition;
	friend class ColliderManager;

public:
	// 节点属性
	struct Property
	{
		bool visable;		// 可见性
		double posX;		// X 坐标
		double posY;		// Y 坐标
		double width;		// 宽度
		double height;		// 高度
		double opacity;		// 透明度
		double pivotX;		// 中心点 X 坐标
		double pivotY;		// 中心点 Y 坐标
		double scaleX;		// 横向缩放
		double scaleY;		// 纵向缩放
		double rotation;	// 旋转角度
		double skewAngleX;	// 横向倾斜角度
		double skewAngleY;	// 纵向倾斜角度
	};

public:
	Node();

	virtual ~Node();

	// 更新节点
	virtual void onUpdate() {}

	// 渲染节点
	virtual void onRender() {}

	// 获取节点显示状态
	virtual bool isVisiable() const;

	// 判断点是否在节点内
	virtual bool containsPoint(
		const Point& point
	) const;

	// 判断两物体是否相交
	virtual bool intersects(
		Node * node
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
	virtual Property getProperty() const;

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
	virtual const std::vector<Node*>& getAllChildren() const;

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
		Property prop
	);

	// 设置碰撞体
	virtual void setCollider(
		Collider::Type type
	);

	// 设置碰撞体
	virtual void setCollider(
		Collider * pCollider
	);

	// 添加子节点
	virtual void addChild(
		Node * child,
		int order = 0	/* 渲染顺序 */
	);

	// 添加多个子节点
	virtual void addChild(
		const std::vector<Node*>& nodes,	/* 节点数组 */
		int order = 0						/* 渲染顺序 */
	);

	// 执行动作
	virtual void runAction(
		Action * action
	);

	// 继续动作
	virtual void resumeAction(
		const String& name
	);

	// 暂停动作
	virtual void pauseAction(
		const String& name
	);

	// 停止动作
	virtual void stopAction(
		const String& name
	);

	// 继续所有暂停动作
	virtual void resumeAllActions();

	// 暂停所有动作
	virtual void pauseAllActions();

	// 停止所有动作
	virtual void stopAllActions();

	// 修改节点的默认中心点位置
	static void setDefaultPiovt(
		double defaultPiovtX,
		double defaultPiovtY
	);

	// 设置节点的默认碰撞体类型（默认无）
	static void setDefaultCollider(
		Collider::Type type
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

	// 更新节点二维矩阵
	void _updateTransform();

	// 子节点排序
	void _sortChildren();

	// 更新节点透明度
	void _updateOpacity();

	// 提供给子类的更新函数
	virtual void _fixedUpdate() {}

protected:
	String		_name;
	unsigned	_hashName;
	float		_posX;
	float		_posY;
	float		_width;
	float		_height;
	float		_scaleX;
	float		_scaleY;
	float		_rotation;
	float		_skewAngleX;
	float		_skewAngleY;
	float		_displayOpacity;
	float		_realOpacity;
	float		_pivotX;
	float		_pivotY;
	int			_nOrder;
	bool		_visiable;
	bool		_autoUpdate;
	bool		_needSort;
	bool		_needTransform;
	bool		_positionFixed;
	Collider *	_collider;
	Scene *		_parentScene;
	Node *		_parent;
	D2D1::Matrix3x2F	_initialMatri;
	D2D1::Matrix3x2F	_finalMatri;
	std::vector<Node*>	_children;
};


class Sprite :
	public Node
{
public:
	Sprite();

	explicit Sprite(
		Image * image
	);

	explicit Sprite(
		const String& filePath	/* 图片文件路径 */
	);

	explicit Sprite(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	explicit Sprite(
		const String& filePath,	/* 图片文件路径 */
		const Rect& cropRect	/* 裁剪矩形 */
	);

	explicit Sprite(
		int resNameId,			/* 图片资源名称 */
		const String& resType,	/* 图片资源类型 */
		const Rect& cropRect	/* 裁剪矩形 */
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
		const Rect& cropRect	/* 裁剪矩形 */
	);

	// 获取 Image 对象
	virtual Image * getImage() const;

	// 渲染精灵
	virtual void onRender() override;

	// 销毁对象
	virtual void onDestroy() override;

protected:
	Image * _image;
};


// 文本
class Text :
	public Node
{
public:
	// 文本对齐方式
	enum class Align
	{
		Left,		/* 左对齐 */
		Right,		/* 右对齐 */
		Center		/* 居中对齐 */
	};

	// 文本样式
	class Style
	{
	public:
		Color		color;				// 颜色
		Align		alignment;			// 对齐方式
		bool		wrapping;			// 打开自动换行
		double		wrappingWidth;		// 自动换行宽度
		double		lineSpacing;		// 行间距
		bool		hasUnderline;		// 下划线
		bool		hasStrikethrough;	// 删除线
		bool		hasOutline;			// 显示描边
		Color		outlineColor;		// 描边颜色
		double		outlineWidth;		// 描边线宽
		LineJoin	outlineJoin;		// 描边线相交样式

	public:
		Style();

		Style(
			Color color,
			Align alignment = Align::Left,
			bool wrapping = false,
			double wrappingWidth = 0.0,
			double lineSpacing = 0.0,
			bool hasUnderline = false,
			bool hasStrikethrough = false,
			bool hasOutline = true,
			Color outlineColor = Color(Color::Black, 0.5),
			double outlineWidth = 1.0,
			LineJoin outlineJoin = LineJoin::Round
		);
	};

public:
	Text();

	explicit Text(
		const String& text,						/* 文字内容 */
		const Font& font = Font(),				/* 字体 */
		const Style& style = Style()			/* 文本样式 */
	);

	virtual ~Text();

	// 获取文本
	String getText() const;

	// 获取字体
	Font getFont() const;

	// 获取文本样式
	Style getStyle() const;

	// 获取字体族
	String getFontFamily() const;

	// 获取当前字号
	double getFontSize() const;

	// 获取当前字体粗细值
	UINT getFontWeight() const;

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
	void setStyle(
		const Style& style
	);

	// 设置字体
	void setFont(
		const Font& font
	);

	// 设置字体族
	void setFontFamily(
		const String& family
	);

	// 设置字号（默认值为 22）
	void setFontSize(
		double size
	);

	// 设置字体粗细值（默认值为 Text::Font::Weight::Normal）
	void setFontWeight(
		UINT weight
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
		double wrappingWidth
	);

	// 设置行间距（默认为 0）
	void setLineSpacing(
		double lineSpacing
	);

	// 设置对齐方式（默认为 Align::Left）
	void setAlignment(
		Align align
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
	String	_text;
	Font	_font;
	Style	_style;
	IDWriteTextFormat * _textFormat;
	IDWriteTextLayout * _textLayout;
};


class Button :
	public Node
{
public:
	Button();

	explicit Button(
		Node * normal,					/* 普通状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit Button(
		Node * normal,					/* 普通状态 */
		Node * selected,				/* 鼠标按下状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit Button(
		Node * normal,					/* 普通状态 */
		Node * mouseover,				/* 鼠标移入状态 */
		Node * selected,				/* 鼠标按下状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit Button(
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

protected:
	// 按钮状态枚举
	enum class ButtonState { Normal, Mouseover, Selected };

	// 设置按钮状态
	virtual void _setState(ButtonState state);

	// 刷新按钮显示
	virtual void _updateVisiable();

	// 更新按钮状态
	virtual void _fixedUpdate() override;

	// 执行按钮函数对象
	virtual void _runCallback();

protected:
	Node *		_normal;
	Node *		_mouseover;
	Node *		_selected;
	Node *		_disabled;
	bool		_enable;
	bool		_isSelected;
	ButtonState	_state;
	Function	_func;
};


class ToggleButton :
	public Button
{
public:
	ToggleButton();

	explicit ToggleButton(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit ToggleButton(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		Node * onSelected,				/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,				/* 按钮关闭时，鼠标按下状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit ToggleButton(
		Node * onNormal,				/* 按钮打开时，普通状态 */
		Node * offNormal,				/* 按钮关闭时，普通状态 */
		Node * onMouseOver,				/* 按钮打开时，鼠标移入状态 */
		Node * offMouseOver,			/* 按钮关闭时，鼠标移入状态 */
		Node * onSelected,				/* 按钮打开时，鼠标按下状态 */
		Node * offSelected,				/* 按钮关闭时，鼠标按下状态 */
		const Function& func = nullptr	/* 按钮点击后的执行函数 */
	);

	explicit ToggleButton(
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
	Node *	_normalOff;
	Node *	_mouseoverOff;
	Node *	_selectedOff;
	Node *	_disabledOff;
	bool	_toggle;
};


class Menu :
	public Node
{
public:
	Menu();

	explicit Menu(
		const std::vector<Button*>& buttons	/* 按钮数组 */
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

	// 获取所有按钮
	const std::vector<Button*>& getAllButtons() const;

protected:
	bool _enable;
	std::vector<Button*> _buttons;
};

}