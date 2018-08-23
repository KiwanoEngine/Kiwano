#pragma once
#include "e2dbase.h"
#include "e2devent.h"

namespace e2d 
{


class Action;


// 绘图接口
class Drawable
{
public:
	// 渲染图形
	virtual void draw(Renderer * renderer) const = 0;
};


// 更新接口
class Updatable
{
public:
	// 渲染图形
	virtual void update() = 0;
};


// 按键消息处理接口
class KeyEventHandler
{
public:
	// 处理按键消息
	virtual void handle(KeyEvent e) = 0;
};


// 鼠标消息处理接口
class MouseEventHandler
{
public:
	// 处理鼠标消息
	virtual void handle(MouseEvent e) = 0;
};


// 碰撞消息处理接口
class CollisionHandler
{
public:
	// 处理碰撞消息
	virtual void handle(Collision collision) = 0;
};


// 节点
class Node :
	public Ref
{
	friend class Collider;

public:
	// 节点属性
	struct Property
	{
		float posX;			// X 坐标
		float posY;			// Y 坐标
		float width;		// 宽度
		float height;		// 高度
		float anchorX;		// 锚点 X 坐标
		float anchorY;		// 锚点 Y 坐标
		float scaleX;		// 横向缩放
		float scaleY;		// 纵向缩放
		float rotation;		// 旋转角度
		float skewAngleX;	// 横向倾斜角度
		float skewAngleY;	// 纵向倾斜角度

		Property operator+ (Property const & prop) const;
		Property operator- (Property const & prop) const;

		static const Property Origin;
	};

public:
	Node();

	virtual ~Node();

	// 获取节点显示状态
	bool isVisible() const;

	// 获取节点名称
	const String& getName() const;

	// 获取节点名称的 Hash 值
	size_t getHashName() const;

	// 获取节点绘图顺序
	int getOrder() const;

	// 获取节点横坐标
	float getPosX() const;

	// 获取节点纵坐标
	float getPosY() const;

	// 获取节点坐标
	Point getPos() const;

	// 获取节点宽度
	float getWidth() const;

	// 获取节点高度
	float getHeight() const;

	// 获取节点宽度（不考虑缩放）
	float getRealWidth() const;

	// 获取节点高度（不考虑缩放）
	float getRealHeight() const;

	// 获取节点大小（不考虑缩放）
	Size getRealSize() const;

	// 获取节点的锚点
	float getAnchorX() const;

	// 获取节点的锚点
	float getAnchorY() const;

	// 获取节点大小
	Size getSize() const;

	// 获取节点横向缩放比例
	float getScaleX() const;

	// 获取节点纵向缩放比例
	float getScaleY() const;

	// 获取节点横向倾斜角度
	float getSkewX() const;

	// 获取节点纵向倾斜角度
	float getSkewY() const;

	// 获取节点旋转角度
	float getRotation() const;

	// 获取节点透明度
	float getOpacity() const;

	// 获取节点属性
	Property getProperty() const;

	// 获取差别属性
	Property getExtrapolate() const;

	// 获取节点碰撞体
	Collider * getCollider();

	// 获取父节点
	Node * getParent() const;

	// 获取节点所在场景
	Scene * getParentScene() const;

	// 设置节点是否显示
	void setVisible(
		bool value
	);

	// 设置节点名称
	void setName(
		const String& name
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
		const Point & point
	);

	// 设置节点坐标
	virtual void setPos(
		float x,
		float y
	);

	// 节点坐标固定
	virtual void setPosFixed(
		bool fixed
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
		const Vector2 & v
	);

	// 设置节点绘图顺序
	// 默认为 0
	virtual void setOrder(
		int order
	);

	// 设置横向缩放比例
	// 默认为 1.0
	virtual void setScaleX(
		float scaleX
	);

	// 设置纵向缩放比例
	// 默认为 1.0
	virtual void setScaleY(
		float scaleY
	);

	// 设置缩放比例
	// 默认为 (1.0, 1.0)
	virtual void setScale(
		float scaleX,
		float scaleY
	);

	// 设置缩放比例
	// 默认为 1.0
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
	// 默认为 1.0, 范围 [0, 1]
	virtual void setOpacity(
		float opacity
	);

	// 设置锚点的横向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setAnchorX(
		float anchorX
	);

	// 设置锚点的纵向位置
	// 默认为 0, 范围 [0, 1]
	virtual void setAnchorY(
		float anchorY
	);

	// 设置锚点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual void setAnchor(
		float anchorX,
		float anchorY
	);

	// 修改节点宽度
	virtual void setWidth(
		float width
	);

	// 修改节点高度
	virtual void setHeight(
		float height
	);

	// 修改节点大小
	virtual void setSize(
		float width,
		float height
	);

	// 修改节点大小
	virtual void setSize(
		Size size
	);

	// 设置节点属性
	virtual void setProperty(
		Property prop
	);

	// 启用或关闭渲染区域裁剪
	virtual void setClipEnabled(
		bool enabled
	);

	// 判断点是否在节点内
	bool containsPoint(
		const Point& point
	);

	// 判断两物体是否相交
	bool intersects(
		Node * node
	);

	// 添加子节点
	void addChild(
		Node * child,
		int order = 0	/* 渲染顺序 */
	);

	// 添加多个子节点
	void addChild(
		const std::vector<Node*>& nodes,	/* 节点数组 */
		int order = 0						/* 渲染顺序 */
	);

	// 获取所有名称相同的子节点
	std::vector<Node*> getChildren(
		const String& name
	) const;

	// 获取名称相同的子节点
	Node* getChild(
		const String& name
	) const;

	// 获取所有子节点
	const std::vector<Node*>& getAllChildren() const;

	// 获取子节点数量
	int getChildrenCount() const;

	// 移除子节点
	bool removeChild(
		Node * child
	);

	// 移除所有名称相同的子节点
	void removeChildren(
		const String& childName
	);

	// 移除所有节点
	void removeAllChildren();

	// 从父节点移除
	void removeFromParent();

	// 执行动作
	void runAction(
		Action * action
	);

	// 继续动作
	void resumeAction(
		const String& name
	);

	// 暂停动作
	void pauseAction(
		const String& name
	);

	// 停止动作
	void stopAction(
		const String& name
	);

	// 继续所有暂停动作
	void resumeAllActions();

	// 暂停所有动作
	void pauseAllActions();

	// 停止所有动作
	void stopAllActions();

	// 分发鼠标消息
	virtual bool dispatch(
		const MouseEvent& e,
		bool handled
	);

	// 分发按键消息
	virtual bool dispatch(
		const KeyEvent& e,
		bool handled
	);

	// 遍历节点
	virtual void visit(
		Game * game
	);

	// 渲染节点轮廓
	virtual void drawOutline(
		Renderer * renderer
	);

	// 渲染碰撞体轮廓
	virtual void drawCollider();

protected:
	E2D_DISABLE_COPY(Node);

	// 设置节点所在场景
	void _setParentScene(
		Scene * scene
	);

	// 子节点排序
	void _sortChildren();

	// 更新转换矩阵
	void _updateTransform();

	// 更新节点透明度
	void _updateOpacity();

protected:
	String		_name;
	size_t		_hashName;
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
	float		_anchorX;
	float		_anchorY;
	int			_order;
	bool		_visible;
	bool		_clipEnabled;
	bool		_needSort;
	bool		_needTransform;
	bool		_positionFixed;
	Collider	_collider;
	Scene *		_parentScene;
	Node *		_parent;
	Property	_extrapolate;
	std::vector<Node*>	_children;
	D2D1::Matrix3x2F	_initialMatri;
	D2D1::Matrix3x2F	_finalMatri;
};


// 场景
class Scene :
	public Node
{
public:
	Scene();

	virtual ~Scene();

	// 进入场景
	virtual void onEnter() {}

	// 退出场景
	virtual void onExit() {}

	// 关闭窗口
	// 说明：返回 false 将阻止窗口关闭
	virtual bool onCloseWindow() { return true; }

	// 显示或隐藏节点轮廓
	// 默认：隐藏
	void setOutlineVisible(
		bool visible
	);

	// 打开或关闭碰撞体可视化
	// 默认：关闭
	void setColliderVisible(
		bool visible
	);

	// 遍历节点
	virtual void visit(
		Game * game
	) override;

protected:
	E2D_DISABLE_COPY(Scene);

protected:
	bool _outlineVisible;
	bool _colliderVisible;
};


// 精灵
class Sprite :
	public Node,
	public Drawable
{
public:
	Sprite();

	explicit Sprite(
		Image * image
	);

	explicit Sprite(
		const Resource& res
	);

	explicit Sprite(
		const Resource& res,
		const Rect& cropRect	/* 裁剪矩形 */
	);

	explicit Sprite(
		const String& fileName
	);

	explicit Sprite(
		const String& fileName,
		const Rect& cropRect	/* 裁剪矩形 */
	);

	virtual ~Sprite();

	// 加载图片文件
	bool open(
		const Resource& res
	);

	// 加载图片文件
	bool open(
		const String& fileName
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
	Image * getImage() const;

	// 渲染精灵
	virtual void draw(
		Renderer * renderer
	) const override;

protected:
	E2D_DISABLE_COPY(Sprite);

protected:
	Image * _image;
};


// 文本
class Text :
	public Node,
	public Drawable
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
		float		wrappingWidth;		// 自动换行宽度
		float		lineSpacing;		// 行间距
		bool		hasUnderline;		// 下划线
		bool		hasStrikethrough;	// 删除线
		bool		hasOutline;			// 显示描边
		Color		outlineColor;		// 描边颜色
		float		outlineWidth;		// 描边线宽
		Stroke	outlineStroke;		// 描边线相交样式

	public:
		Style();

		Style(
			Color color,
			Align alignment = Align::Left,
			bool wrapping = false,
			float wrappingWidth = 0.f,
			float lineSpacing = 0.f,
			bool hasUnderline = false,
			bool hasStrikethrough = false,
			bool hasOutline = true,
			Color outlineColor = Color(Color::Black, 0.5),
			float outlineWidth = 1.f,
			Stroke outlineStroke = Stroke::Round
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
	const String& getText() const;

	// 获取字体
	const Font& getFont() const;

	// 获取文本样式
	const Style& getStyle() const;

	// 获取字体族
	const String& getFontFamily() const;

	// 获取当前字号
	float getFontSize() const;

	// 获取当前字体粗细值
	UINT getFontWeight() const;

	// 获取文字颜色
	const Color& getColor() const;

	// 获取描边颜色
	const Color& getOutlineColor() const;

	// 获取描边线宽
	float getOutlineWidth() const;

	// 获取描边线相交样式
	Stroke getOutlineStroke() const;

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
		float size
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
		float wrappingWidth
	);

	// 设置行间距（默认为 0）
	void setLineSpacing(
		float lineSpacing
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
		float outlineWidth
	);

	// 设置描边线相交样式
	void setOutlineStroke(
		Stroke outlineStroke
	);

	// 渲染文字
	virtual void draw(
		Renderer * renderer
	) const override;

protected:
	E2D_DISABLE_COPY(Text);

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
	void setEnabled(
		bool enabled
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

	// 设置锚点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual void setAnchor(
		float anchorX,
		float anchorY
	) override;

	// 分发鼠标消息
	virtual bool dispatch(
		const MouseEvent& e,
		bool handled
	) override;

	// 遍历节点
	virtual void visit(
		Game * game
	) override;

protected:
	E2D_DISABLE_COPY(Button);

	// 按钮状态枚举
	enum class Status { Normal, Mouseover, Selected };

	// 设置按钮状态
	virtual void _setStatus(Status status);

	// 刷新按钮显示
	virtual void _updateVisible();

	// 执行按钮函数对象
	virtual void _runCallback();

protected:
	Node *		_normal;
	Node *		_mouseover;
	Node *		_selected;
	Node *		_disabled;
	bool		_enabled;
	bool		_isSelected;
	Status		_status;
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

	// 获取开关状态
	bool isChecked() const;

	// 设置开关按钮的状态
	void setChecked(
		bool checked
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

	// 设置锚点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual void setAnchor(
		float anchorX,
		float anchorY
	) override;

protected:
	E2D_DISABLE_COPY(ToggleButton);

	// 刷新按钮开关
	virtual void _updateStatus();

	// 执行按钮函数对象
	virtual void _runCallback() override;

protected:
	Node*	_normalOn;
	Node*	_mouseoverOn;
	Node*	_selectedOn;
	Node*	_disabledOn;
	Node*	_normalOff;
	Node*	_mouseoverOff;
	Node*	_selectedOff;
	Node*	_disabledOff;
	bool	_checked;
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
	void setEnabled(
		bool enabled
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
	E2D_DISABLE_COPY(Menu);

protected:
	bool _enabled;
	std::vector<Button*> _buttons;
};


// 画布
class Canvas :
	public Node,
	public Drawable
{
public:
	Canvas(
		float width,
		float height
	);

	virtual ~Canvas();

	// 设置线条颜色
	void setLineColor(
		const Color& color
	);

	// 设置填充颜色
	void setFillColor(
		const Color& color
	);

	// 设置线条宽度
	void setStrokeWidth(
		float width
	);

	// 设置线条相交样式
	void setStrokeStyle(
		Stroke strokeStyle
	);

	// 获取线条颜色
	Color getLineColor() const;

	// 获取填充颜色
	Color getFillColor() const;

	// 获取线条宽度
	float getStrokeWidth() const;

	// 获取线条相交样式
	Stroke getStrokeStyle() const;

	// 画直线
	void drawLine(
		const Point& begin,
		const Point& end
	);

	// 画圆形边框
	void drawCircle(
		const Point& center,
		float radius
	);

	// 画椭圆形边框
	void drawEllipse(
		const Point& center,
		float radiusX,
		float radiusY
	);

	// 画矩形边框
	void drawRect(
		const Rect& rect
	);

	// 画圆角矩形边框
	void drawRoundedRect(
		const Rect& rect,
		float radiusX,
		float radiusY
	);

	// 填充圆形
	void fillCircle(
		const Point& center,
		float radius
	);

	// 填充椭圆形
	void fillEllipse(
		const Point& center,
		float radiusX,
		float radiusY
	);

	// 填充矩形
	void fillRect(
		const Rect& rect
	);

	// 填充圆角矩形
	void fillRoundedRect(
		const Rect& rect,
		float radiusX,
		float radiusY
	);

protected:
	E2D_DISABLE_COPY(Canvas);

protected:
	float _strokeWidth;
	Stroke _stroke;
	Renderer * _renderer;
	ID2D1RenderTarget * _renderTarget;
	ID2D1SolidColorBrush * _fillBrush;
	ID2D1SolidColorBrush * _lineBrush;
	ID2D1StrokeStyle * _strokeStyle;
};

}