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
	virtual void handle(const KeyEvent& e) = 0;
};


// 鼠标消息处理接口
class MouseEventHandler
{
public:
	// 处理鼠标消息
	virtual void handle(const MouseEvent& e) = 0;
};


// 碰撞消息处理接口
class CollisionHandler
{
public:
	// 处理碰撞消息
	virtual void handle(const Collision& collision) = 0;
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
		Point position;		// 坐标
		Size size;			// 大小
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
	virtual bool visible() const;

	// 获取节点名称
	const String& name() const;

	// 获取节点名称的 Hash 值
	size_t hashName() const;

	// 获取节点绘图顺序
	int order() const;

	// 获取节点横坐标
	float positionX() const;

	// 获取节点纵坐标
	float positionY() const;

	// 获取节点坐标
	const Point& position() const;

	// 获取节点宽度
	float width() const;

	// 获取节点高度
	float height() const;

	// 获取节点大小
	Size size() const;

	// 获取节点实际宽度（不考虑缩放）
	float realWidth() const;

	// 获取节点实际高度（不考虑缩放）
	float realHeight() const;

	// 获取节点实际大小（不考虑缩放）
	const Size& realSize() const;

	// 获取节点的锚点
	float anchorX() const;

	// 获取节点的锚点
	float anchorY() const;

	// 获取节点横向缩放比例
	float scaleX() const;

	// 获取节点纵向缩放比例
	float scaleY() const;

	// 获取节点横向倾斜角度
	float skewX() const;

	// 获取节点纵向倾斜角度
	float skewY() const;

	// 获取节点旋转角度
	float rotation() const;

	// 获取节点透明度
	float opacity() const;

	// 获取节点属性
	Property properties() const;

	// 获取差别属性
	Property extrapolate() const;

	// 设置节点是否显示
	Node& visible(
		bool value
	);

	// 设置节点名称
	Node& name(
		const String& name
	);

	// 设置节点横坐标
	virtual Node& positionX(
		float x
	);

	// 设置节点纵坐标
	virtual Node& positionY(
		float y
	);

	// 设置节点坐标
	virtual Node& position(
		const Point & point
	);

	// 设置节点坐标
	virtual Node& position(
		float x,
		float y
	);

	// 节点坐标固定
	virtual Node& positionFixed(
		bool fixed
	);

	// 移动节点
	virtual Node& move(
		float x,
		float y
	);

	// 移动节点
	virtual Node& move(
		const Vector2 & v
	);

	// 设置节点绘图顺序
	// 默认为 0
	virtual Node& order(
		int order
	);

	// 设置横向缩放比例
	// 默认为 1.0
	virtual Node& scaleX(
		float scaleX
	);

	// 设置纵向缩放比例
	// 默认为 1.0
	virtual Node& scaleY(
		float scaleY
	);

	// 设置缩放比例
	// 默认为 (1.0, 1.0)
	virtual Node& scale(
		float scaleX,
		float scaleY
	);

	// 设置缩放比例
	// 默认为 1.0
	virtual Node& scale(
		float scale
	);

	// 设置横向倾斜角度
	// 默认为 0
	virtual Node& skewX(
		float angleX
	);

	// 设置纵向倾斜角度
	// 默认为 0
	virtual Node& skewY(
		float angleY
	);

	// 设置倾斜角度
	// 默认为 (0, 0)
	virtual Node& skew(
		float angleX,
		float angleY
	);

	// 设置旋转角度
	// 默认为 0
	virtual Node& rotation(
		float rotation
	);

	// 设置透明度
	// 默认为 1.0, 范围 [0, 1]
	virtual Node& opacity(
		float opacity
	);

	// 设置锚点的横向位置
	// 默认为 0, 范围 [0, 1]
	virtual Node& anchorX(
		float anchorX
	);

	// 设置锚点的纵向位置
	// 默认为 0, 范围 [0, 1]
	virtual Node& anchorY(
		float anchorY
	);

	// 设置锚点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual Node& anchor(
		float anchorX,
		float anchorY
	);

	// 修改节点宽度
	virtual Node& width(
		float width
	);

	// 修改节点高度
	virtual Node& height(
		float height
	);

	// 修改节点大小
	virtual Node& size(
		float width,
		float height
	);

	// 修改节点大小
	virtual Node& size(
		Size size
	);

	// 设置节点属性
	virtual Node& properties(
		Property prop
	);

	// 启用或关闭渲染区域裁剪
	virtual Node& clipEnabled(
		bool enabled
	);

	// 添加子节点
	Node& addChild(
		Node * child,
		int order = 0	/* 渲染顺序 */
	);

	// 添加多个子节点
	Node& addChild(
		const std::vector<Node*>& nodes,	/* 节点数组 */
		int order = 0						/* 渲染顺序 */
	);

	// 获取节点碰撞体
	Collider * collider();

	// 获取父节点
	Node * parent() const;

	// 获取节点所在场景
	Scene * parentScene() const;

	// 获取名称相同的子节点
	Node* child(
		const String& name
	) const;

	// 获取所有子节点
	const std::vector<Node*>& children() const;

	// 获取所有名称相同的子节点
	std::vector<Node*> children(
		const String& name
	) const;

	// 获取子节点数量
	int childrenCount() const;

	// 移除子节点
	Node& removeChild(
		Node * child
	);

	// 移除所有名称相同的子节点
	Node& removeChildren(
		const String& childName
	);

	// 移除所有节点
	Node& removeAllChildren();

	// 从父节点移除
	Node& removeFromParent();

	// 判断点是否在节点内
	bool containsPoint(
		const Point& point
	);

	// 判断两物体是否相交
	bool intersects(
		Node * node
	);

	// 执行动作
	Node& runAction(
		Action * action
	);

	// 继续动作
	Node& resumeAction(
		const String& name
	);

	// 暂停动作
	Node& pauseAction(
		const String& name
	);

	// 停止动作
	Node& stopAction(
		const String& name
	);

	// 继续所有暂停动作
	Node& resumeAllActions();

	// 暂停所有动作
	Node& pauseAllActions();

	// 停止所有动作
	Node& stopAllActions();

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
	Point		_position;
	Size		_size;
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
	Scene& outlineVisible(
		bool visible
	);

	// 打开或关闭碰撞体可视化
	// 默认：关闭
	Scene& colliderVisible(
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
	Sprite& crop(
		const Rect& cropRect	/* 裁剪矩形 */
	);

	// 获取 Image 对象
	Image * image() const;

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
	const String& text() const;

	// 获取字体
	const Font& font() const;

	// 获取文本样式
	const Style& style() const;

	// 获取字体族
	const String& fontFamily() const;

	// 获取当前字号
	float fontSize() const;

	// 获取当前字体粗细值
	UINT fontWeight() const;

	// 获取文字颜色
	const Color& color() const;

	// 获取描边颜色
	const Color& outlineColor() const;

	// 获取描边线宽
	float outlineWidth() const;

	// 获取描边线相交样式
	Stroke outlineStroke() const;

	// 获取文本显示行数
	int lineCount() const;

	// 是否是斜体
	bool italic() const;

	// 是否显示删除线
	bool hasStrikethrough() const;

	// 是否显示下划线
	bool hasUnderline() const;

	// 是否显示描边
	bool hasOutline() const;

	// 设置文本
	Text& text(
		const String& text
	);

	// 设置文本样式
	Text& style(
		const Style& style
	);

	// 设置字体
	Text& font(
		const Font& font
	);

	// 设置字体族
	Text& fontFamily(
		const String& family
	);

	// 设置字号（默认值为 22）
	Text& fontSize(
		float size
	);

	// 设置字体粗细值（默认值为 Text::Font::Weight::Normal）
	Text& fontWeight(
		UINT weight
	);

	// 设置文字颜色（默认值为 Color::WHITE）
	Text& color(
		Color color
	);

	// 设置文字斜体（默认值为 false）
	Text& italic(
		bool value
	);

	// 打开或关闭文本自动换行（默认为关闭）
	Text& wrapping(
		bool wrapping
	);

	// 设置文本自动换行的宽度（默认为 0）
	Text& wrappingWidth(
		float wrappingWidth
	);

	// 设置行间距（默认为 0）
	Text& lineSpacing(
		float lineSpacing
	);

	// 设置对齐方式（默认为 Align::Left）
	Text& alignment(
		Align align
	);

	// 设置下划线（默认值为 false）
	Text& underline(
		bool hasUnderline
	);

	// 设置删除线（默认值为 false）
	Text& strikethrough(
		bool hasStrikethrough
	);

	// 设置是否显示描边
	Text& outline(
		bool hasOutline
	);

	// 设置描边颜色
	Text& outlineColor(
		Color outlineColor
	);

	// 设置描边线宽
	Text& outlineWidth(
		float outlineWidth
	);

	// 设置描边线相交样式
	Text& outlineStroke(
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
	bool enabled() const;

	// 设置按钮启用或禁用
	Button& enabled(
		bool enabled
	);

	// 设置一般情况下显示的按钮
	virtual Button& normal(
		Node * normal
	);

	// 设置鼠标移入按钮时显示的按钮
	virtual Button& mouseover(
		Node * mouseover
	);

	// 设置鼠标按下按钮时显示的按钮
	virtual Button& selected(
		Node * selected
	);

	// 设置按钮被禁用时显示的按钮
	virtual Button& disabled(
		Node * disabled
	);

	// 设置点击回调函数
	Button& clickCallback(
		const Function& func
	);

	// 设置锚点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual Button& anchor(
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
	bool checked() const;

	// 设置开关按钮的状态
	ToggleButton& checked(
		bool checked
	);

	// 设置按钮打开状态下显示的按钮
	virtual ToggleButton& normal(
		Node * normal
	) override;

	// 设置按钮打开状态下，鼠标移入按钮时显示的按钮
	virtual ToggleButton& mouseover(
		Node * mouseover
	) override;

	// 设置按钮打开状态下，鼠标按下按钮时显示的按钮
	virtual ToggleButton& selected(
		Node * selected
	) override;

	// 设置按钮打开状态下，被禁用时显示的按钮
	virtual ToggleButton& disabled(
		Node * disabled
	) override;

	// 设置按钮关闭状态下显示的按钮
	ToggleButton& normalOff(
		Node * normal
	);

	// 设置按钮关闭状态下，鼠标移入按钮时显示的按钮
	ToggleButton& mouseOverOff(
		Node * mouseover
	);

	// 设置按钮关闭状态下，鼠标按下按钮时显示的按钮
	ToggleButton& selectedOff(
		Node * selected
	);

	// 设置按钮关闭状态下，按钮被禁用时显示的按钮
	ToggleButton& disabledOff(
		Node * disabled
	);

	// 设置锚点位置
	// 默认为 (0, 0), 范围 [0, 1]
	virtual ToggleButton& anchor(
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
	bool enabled() const;

	// 获取菜单中的按钮数量
	int buttonCount() const;

	// 设置菜单启用或禁用
	Menu& enabled(
		bool enabled
	);

	// 添加按钮
	Menu& addButton(
		Button * button
	);

	// 移除按钮
	bool removeButton(
		Button * button
	);

	// 获取所有按钮
	const std::vector<Button*>& buttons() const;

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
	Canvas& lineColor(
		const Color& color
	);

	// 设置填充颜色
	Canvas& fillColor(
		const Color& color
	);

	// 设置线条宽度
	Canvas& strokeWidth(
		float width
	);

	// 设置线条相交样式
	Canvas& strokeStyle(
		Stroke strokeStyle
	);

	// 获取线条颜色
	Color lineColor() const;

	// 获取填充颜色
	Color fillColor() const;

	// 获取线条宽度
	float strokeWidth() const;

	// 获取线条相交样式
	Stroke strokeStyle() const;

	// 画直线
	Canvas& drawLine(
		const Point& begin,
		const Point& end
	);

	// 画圆形边框
	Canvas& drawCircle(
		const Point& center,
		float radius
	);

	// 画椭圆形边框
	Canvas& drawEllipse(
		const Point& center,
		float radiusX,
		float radiusY
	);

	// 画矩形边框
	Canvas& drawRect(
		const Rect& rect
	);

	// 画圆角矩形边框
	Canvas& drawRoundedRect(
		const Rect& rect,
		float radiusX,
		float radiusY
	);

	// 填充圆形
	Canvas& fillCircle(
		const Point& center,
		float radius
	);

	// 填充椭圆形
	Canvas& fillEllipse(
		const Point& center,
		float radiusX,
		float radiusY
	);

	// 填充矩形
	Canvas& fillRect(
		const Rect& rect
	);

	// 填充圆角矩形
	Canvas& fillRoundedRect(
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