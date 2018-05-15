#pragma once
#include "e2dmacros.h"
#include "e2dcustom.h"

namespace e2d
{


class Size;

// 表示坐标的结构体
class Point
{
public:
	double x;	// X 坐标
	double y;	// Y 坐标

public:
	Point();

	Point(double x, double y);

	Point operator + (Point const & p) const;
	Point operator - (Point const & p) const;
	Point operator * (double const & value) const;
	Point operator / (double const & value) const;
	Point operator - () const;

	operator e2d::Size() const;
};

// 二维向量
typedef Point Vector;

// 表示大小的结构体
class Size
{
public:
	double width;	// 宽度
	double height;	// 高度

public:
	Size();

	Size(double width, double height);

	Size operator + (Size const & size) const;
	Size operator - (Size const & size) const;
	Size operator * (double const & value) const;
	Size operator / (double const & value) const;
	Size operator - () const;

	operator e2d::Point() const;
};


// 字符串
class String
{
public:
	String();
	String(const String &);
	String(const char *);
	String(const wchar_t *);
	String(String &&);

	~String();

	// 判断字符串是否为空
	bool isEmpty() const;

	// 获取字符串长度
	int getLength() const;

	// 获取该字符串的散列值
	unsigned int getHashCode() const;

	// 获取 Unicode 字符串
	std::wstring getWString() const;

	// 获取 ANSI 字符串
	std::string getCString() const;

	// 比较字符串
	int compare(
		const String & str
	) const;

	// 截取字符串
	e2d::String subtract(
		int offset,		/* 偏移量 */
		int count = -1	/* 截取字符数量 */
	) const;

	// 插入字符串
	void insert(
		const String & str,
		int pos
	);

	// 替换字符串中的指定内容
	void replace(
		const String & from,	/* 需替换内容 */
		const String & to		/* 替换成内容 */
	);

	// 删除字符串中的指定内容
	void erase(
		int offset,		/* 偏移量 */
		int count		/* 删除字符数量 */
	);

	// 搜索字符串
	int find(
		const String & str,	/* 查找内容 */
		int offset = 0		/* 偏移量 */
	) const;

	// 清空字符串
	void clear();

	// 获取大写字符串
	String toUpper() const;

	// 获取小写字符串
	String toLower() const;

	// 将字符串转化为 int 型
	int toInt() const;

	// 将字符串转化为 double 型
	double toDouble() const;

	// 将字符串转化为 bool 型
	bool toBool() const;

	// 数字类型转字符串
	static String parse(int value);
	static String parse(unsigned int value);
	static String parse(float value);
	static String parse(double value);

	// 格式化字符串
	static String format(const char * format, ...);
	static String format(const wchar_t * format, ...);

	// 交换两字符串
	static void swap(String &str1, String &str2);

	// 赋值运算符
	String& operator= (const String &);
	String& operator= (const char *);
	String& operator= (const wchar_t *);

	// 运算符
	String& operator+= (const String &);
	String& operator+= (const char *);
	String& operator+= (const wchar_t *);
	String operator+ (const String &);
	String operator+ (const char *);
	String operator+ (const wchar_t *);

	// 友元运算符
	friend String operator+ (const char *, const String &);
	friend String operator+ (const wchar_t*, const String &);

	// 类型转换操作符
	operator const wchar_t* () const;
	operator wchar_t* () const;

	// 比较运算符
	bool operator== (const String &);
	bool operator== (const char *);
	bool operator== (const wchar_t *);
	bool operator!= (const String &);
	bool operator!= (const char *);
	bool operator!= (const wchar_t *);
	bool operator> (const String &) const;
	bool operator>= (const String &) const;
	bool operator< (const String &) const;
	bool operator<= (const String &) const;

	// << 运算符（后接字符串）
	String& operator<< (const String &);
	String& operator<< (const char *);
	String& operator<< (char *);
	String& operator<< (const wchar_t *);
	String& operator<< (wchar_t *);
	String& operator<< (int value);
	String& operator<< (unsigned int value);
	String& operator<< (float value);
	String& operator<< (double value);

	// 其他运算符
	wchar_t& operator[] (int);

	friend std::ostream& operator<< (std::ostream &, const String &);
	friend std::wostream& operator<< (std::wostream &, const String &);

	friend std::istream& operator>> (std::istream &, String &);
	friend std::wistream& operator>> (std::wistream &, String &);

private:
	std::wstring _str;
};


// 颜色
class Color
{
public:
	Color();

	Color(
		double r,
		double g,
		double b
	);

	Color(
		double r,
		double g,
		double b,
		double alpha
	);

	Color(
		UINT32 rgb
	);

	Color(
		UINT32 rgb,
		double alpha
	);

	D2D1_COLOR_F toColorF() const;

public:
	enum RGB_VALUE : UINT32
	{
		ALICE_BLUE = 0xF0F8FF,
		AQUA = 0x00FFFF,
		AZURE = 0xF0FFFF,
		BEIGE = 0xF5F5DC,
		BLACK = 0x000000,
		BLUE = 0x0000FF,
		BLUE_VIOLET = 0x8A2BE2,
		BROWN = 0xA52A2A,
		CHOCOLATE = 0xD2691E,
		CYAN = 0x00FFFF,
		DARK_BLUE = 0x00008B,
		DARK_CYAN = 0x008B8B,
		DARK_GOLDENROD = 0xB8860B,
		DARK_GRAY = 0xA9A9A9,
		DARK_GREEN = 0x006400,
		DARK_ORANGE = 0xFF8C00,
		DARK_RED = 0x8B0000,
		DARK_SEA_GREEN = 0x8FBC8F,
		DARK_VIOLET = 0x9400D3,
		DEEP_PINK = 0xFF1493,
		DEEP_SKY_BLUE = 0x00BFFF,
		FOREST_GREEN = 0x228B22,
		GOLD = 0xFFD700,
		GOLDENROD = 0xDAA520,
		GRAY = 0x808080,
		GREEN = 0x008000,
		GREEN_YELLOW = 0xADFF2F,
		LIGHT_BLUE = 0xADD8E6,
		LIGHT_CYAN = 0xE0FFFF,
		LIGHT_GOLDENROD_YELLOW = 0xFAFAD2,
		LIGHT_GREEN = 0x90EE90,
		LIGHT_GRAY = 0xD3D3D3,
		LIGHT_PINK = 0xFFB6C1,
		LIGHT_SEA_GREEN = 0x20B2AA,
		LIGHT_SKY_BLUE = 0x87CEFA,
		LIGHT_SLATE_GRAY = 0x778899,
		LIGHT_YELLOW = 0xFFFFE0,
		MEDIUM_BLUE = 0x0000CD,
		MEDIUM_PURPLE = 0x9370DB,
		MEDIUM_SEA_GREEN = 0x3CB371,
		MEDIUM_SPRING_GREEN = 0x00FA9A,
		MEDUIM_VIOLET_RED = 0xC71585,
		MIDNIGHT_BLUE = 0x191970,
		ORANGE = 0xFFA500,
		ORANGE_RED = 0xFF4500,
		PINK = 0xFFC0CB,
		PURPLE = 0x800080,
		RED = 0xFF0000,
		SEA_GREEN = 0x2E8B57,
		SEA_SHELL = 0xFFF5EE,
		SILVER = 0xC0C0C0,
		SKY_BLUE = 0x87CEEB,
		SNOW = 0xFFFAFA,
		SPRING_GREEN = 0x00FF7F,
		TOMATO = 0xFF6347,
		VIOLET = 0xEE82EE,
		WHEAT = 0xF5DEB3,
		WHITE = 0xFFFFFF,
		WHITE_SMOKE = 0xF5F5F5,
		WOOD = 0xDEB887,
		YELLOW = 0xFFFF00,
		YELLOW_GREEN = 0x9ACD32
	};

protected:
	void _init(
		UINT32 rgb,
		double alpha
	);

public:
	float r;
	float g;
	float b;
	float a;
};


// 字体粗细值
class FontWeight
{
public:
	enum : UINT32
	{
		THIN = 100,
		EXTRA_LIGHT = 200,
		ULTRA_LIGHT = 200,
		LIGHT = 300,
		SEMI_LIGHT = 350,
		NORMAL = 400,
		REGULAR = 400,
		MEDIUM = 500,
		DEMI_BOLD = 600,
		SEMI_BOLD = 600,
		BOLD = 700,
		EXTRA_BOLD = 800,
		ULTRA_BOLD = 800,
		BLACK = 900,
		HEAVY = 900,
		EXTRA_BLACK = 950,
		ULTRA_BLACK = 950
	};
};


// 文本对齐方式
enum class TextAlign : int
{
	LEFT,		/* 左对齐 */
	RIGHT,		/* 右对齐 */
	CENTER		/* 居中对齐 */
};


// 键值集合
enum class KeyCode : int
{
	UP = 0xC8,
	LEFT = 0xCB,
	RIGHT = 0xCD,
	DOWN = 0xD0,
	ENTER = 0x1C,
	SPACE = 0x39,
	ESC = 0x01,
	BACK = 0x0E,
	TAB = 0x0F,
	PAUSE = 0xC5,
	Q = 0x10,
	W = 0x11,
	E = 0x12,
	R = 0x13,
	T = 0x14,
	Y = 0x15,
	U = 0x16,
	I = 0x17,
	O = 0x18,
	P = 0x19,
	A = 0x1E,
	S = 0x1F,
	D = 0x20,
	F = 0x21,
	G = 0x22,
	H = 0x23,
	J = 0x24,
	K = 0x25,
	L = 0x26,
	Z = 0x2C,
	X = 0x2D,
	C = 0x2E,
	V = 0x2F,
	B = 0x30,
	N = 0x31,
	M = 0x32,
	NUM1 = 0x02,
	NUM2 = 0x03,
	NUM3 = 0x04,
	NUM4 = 0x05,
	NUM5 = 0x06,
	NUM6 = 0x07,
	NUM7 = 0x08,
	NUM8 = 0x09,
	NUM9 = 0x0A,
	NUM0 = 0x0B,
	NUMPAD7 = 0x47,
	NUMPAD8 = 0x48,
	NUMPAD9 = 0x49,
	NUMPAD4 = 0x4B,
	NUMPAD5 = 0x4C,
	NUMPAD6 = 0x4D,
	NUMPAD1 = 0x4F,
	NUMPAD2 = 0x50,
	NUMPAD3 = 0x51,
	NUMPAD0 = 0x52
};


// 鼠标指针样式
enum class Cursor : int
{
	NORMAL,		/* 默认指针样式 */
	HAND,		/* 手状指针 */
	NO,			/* 禁止指针 */
	WAIT,		/* 沙漏指针 */
	ARROW_WAIT	/* 默认指针和小沙漏 */
};


// 方向
enum class Direction : int
{
	UP,			/* 上 */
	DOWN,		/* 下 */
	LEFT,		/* 左 */
	RIGHT		/* 右 */
};


// 物体交集关系
enum class Relation : int
{
	UNKNOWN = 0,		/* 关系不确定 */
	DISJOINT = 1,		/* 没有交集 */
	IS_CONTAINED = 2,	/* 完全被包含 */
	CONTAINS = 3,		/* 完全包含 */
	OVERLAP = 4			/* 部分重叠 */
};


// 线条相交样式
enum class LineJoin : int
{
	MITER = 0,
	BEVEL = 1,
	ROUND = 2
};


// 形状样式
enum class ShapeStyle : int
{
	SOLID,		/* 填充 */
	ROUND,		/* 轮廓 */
	FILL,		/* 轮廓 + 填充 */
};


// 碰撞体类别
enum class ColliderType : int
{
	RECT,		/* 矩形 */
	CIRCLE,		/* 圆形 */
	ELLIPSE		/* 椭圆形 */
};


// 文本样式
class TextStyle
{
public:
	String		fontFamily;			// 字体
	double		fontSize;			// 字号
	Color		color;				// 颜色
	UINT32		fontWeight;			// 粗细值
	bool		italic;				// 斜体
	TextAlign	alignment;			// 对齐方式
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
	TextStyle();

	TextStyle(
		const String& fontFamily,
		double fontSize = 22,
		Color color = Color::WHITE,
		UINT32 fontWeight = FontWeight::NORMAL,
		bool italic = false,
		TextAlign alignment = TextAlign::LEFT,
		bool wrapping = false,
		double wrappingWidth = 0.0,
		double lineSpacing = 0.0,
		bool hasUnderline = false,
		bool hasStrikethrough = false,
		bool hasOutline = true,
		Color outlineColor = Color(Color::BLACK, 0.5),
		double outlineWidth = 1.0,
		LineJoin outlineJoin = LineJoin::ROUND
	);
};


// 节点属性
struct NodeProperty
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


// 函数对象
class Function
{
public:
	Function();

	Function(
		std::nullptr_t
	);

	Function(
		std::function<void()> func
	);

	template<typename Func>
	Function(Func func) : _func(func) {}

	template<typename Func, typename Object>
	Function(
		Func&& func,	/* 对象的成员函数 */
		Object&& obj	/* 对象指针 */
	)
	{
		_func = std::bind(func, obj);
	}

	void operator() (void) const;

	operator bool() const;

protected:
	std::function<void()> _func;
};


// 基础对象
class Object
{
public:
	Object();

	virtual ~Object();

	// 自动释放
	void autorelease();

	// 引用计数加一
	void retain();

	// 引用计数减一
	void release();

	// 获取引用计数
	int getRefCount() const;

	// 销毁对象
	virtual void onDestroy() {}

private:
	int _refCount;
};


// 图片
class Image :
	public Object
{
public:
	// 创建一个空的图片对象
	Image();

	// 加载图片文件
	Image(
		const String& filePath	/* 图片文件路径 */
	);

	// 加载图片资源
	Image(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	// 加载位图
	Image(
		ID2D1Bitmap * bitmap	/* 位图资源 */
	);

	// 加载图片文件并裁剪
	Image(
		const String& filePath,	/* 图片文件路径 */
		double cropX,			/* 裁剪位置 X 坐标 */
		double cropY,			/* 裁剪位置 Y 坐标 */
		double cropWidth,		/* 裁剪宽度 */
		double cropHeight		/* 裁剪高度 */
	);

	// 加载图片资源并裁剪
	Image(
		int resNameId,			/* 图片资源名称 */
		const String& resType,	/* 图片资源类型 */
		double cropX,			/* 裁剪位置 X 坐标 */
		double cropY,			/* 裁剪位置 Y 坐标 */
		double cropWidth,		/* 裁剪宽度 */
		double cropHeight		/* 裁剪高度 */
	);

	virtual ~Image();

	// 加载图片文件
	bool open(
		const String& filePath	/* 图片文件路径 */
	);

	// 加载图片资源
	bool open(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	// 加载位图
	bool open(
		ID2D1Bitmap * bitmap	/* 位图资源 */
	);

	// 将图片裁剪为矩形
	void crop(
		double cropX,			/* 裁剪位置 X 坐标 */
		double cropY,			/* 裁剪位置 Y 坐标 */
		double cropWidth,		/* 裁剪宽度 */
		double cropHeight		/* 裁剪高度 */
	);

	// 获取宽度
	virtual double getWidth() const;

	// 获取高度
	virtual double getHeight() const;

	// 获取大小
	virtual Size getSize() const;

	// 获取源图片宽度
	virtual double getSourceWidth() const;

	// 获取源图片高度
	virtual double getSourceHeight() const;

	// 获取源图片大小
	virtual Size getSourceSize() const;
	
	// 获取裁剪位置 X 坐标
	virtual double getCropX() const;

	// 获取裁剪位置 Y 坐标
	virtual double getCropY() const;

	// 获取裁剪位置
	virtual Point getCropPos() const;

	// 获取 ID2D1Bitmap 对象
	ID2D1Bitmap * getBitmap();

	// 预加载图片文件
	static bool preload(
		const String& filePath	/* 图片文件路径 */
	);

	// 预加载图片资源
	static bool preload(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	// 清空缓存
	static void clearCache();

protected:
	// 设置 Bitmap
	void _setBitmap(
		ID2D1Bitmap * bitmap
	);

protected:
	double	_cropX;
	double	_cropY;
	double	_cropWidth;
	double	_cropHeight;
	ID2D1Bitmap * _bitmap;
};


// 帧动画
class Animation :
	public Object
{
public:
	// 创建帧动画
	Animation();

	// 创建帧动画
	Animation(
		const std::vector<Image*>& frames	/* 关键帧数组 */
	);

	// 创建特定帧间隔的帧动画
	Animation(
		double interval						/* 帧间隔（秒） */
	);

	// 创建特定帧间隔的帧动画
	Animation(
		double interval,					/* 帧间隔（秒） */
		const std::vector<Image*>& frames	/* 关键帧数组 */
	);

	virtual ~Animation();

	// 添加关键帧
	void add(
		Image * frame	/* 关键帧 */
	);

	// 添加多个关键帧
	void add(
		const std::vector<Image*>& frames	/* 关键帧列表 */
	);

	// 获取帧间隔
	double getInterval() const;

	// 获取关键帧
	const std::vector<Image*>& getFrames() const;

	// 设置每一帧的时间间隔
	void setInterval(
		double interval		/* 帧间隔（秒） */
	);

	// 获取动画的拷贝对象
	virtual Animation * clone() const;

	// 销毁对象
	virtual void onDestroy() override;

protected:
	double	_interval;
	std::vector<Image*> _frames;
};


class Node;
class SceneManager;
class Transition;

// 场景
class Scene :
	public Object
{
	friend SceneManager;
	friend Transition;

public:
	Scene();

	virtual ~Scene();

	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter() {}

	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit() {}

	// 重写这个函数，它将在碰撞发生时自动执行
	virtual void onCollide(
		Node* pActiveNode,	/* 碰撞发生时的主动体 */
		Node* pPassiveNode	/* 碰撞发生时的被动体 */
	) {}

	// 重写这个函数，它将在关闭窗口时执行（返回 false 将阻止窗口关闭）
	virtual bool onCloseWindow() { return true; }

	// 重写这个函数，它将在每一帧画面刷新时执行
	virtual void onUpdate() {}

	// 开启或禁用 onUpdate 函数
	void setAutoUpdate(
		bool bAutoUpdate
	);

	// 添加节点到场景
	void add(
		Node * child,	/* 要添加的节点 */
		int zOrder = 0	/* 渲染顺序 */
	);

	// 添加多个节点到场景
	virtual void add(
		const std::vector<Node*>& nodes,	/* 节点数组 */
		int order = 0						/* 渲染顺序 */
	);

	// 删除子节点
	bool remove(
		Node * child
	);

	// 获取所有名称相同的子节点
	std::vector<Node*> get(
		const String& name
	) const;

	// 获取名称相同的子节点
	Node* getOne(
		const String& name
	) const;

	// 获取所有子节点
	const std::vector<Node*>& getAll() const;

	// 获取根节点
	Node * getRoot() const;

	// 开启或关闭节点轮廓渲染
	void showCollider(
		bool visiable = true
	);

	// 销毁对象
	virtual void onDestroy() override;

protected:
	// 渲染场景画面
	void _render();

	// 更新场景内容
	void _update();

protected:
	bool _autoUpdate;
	bool _colliderVisiable;
	Node * _root;
};


template <class Type>
inline void SafeRelease(Type*& p)
{ 
	if (p != nullptr)
	{ 
		p->release(); 
		p = nullptr; 
	}
}


}