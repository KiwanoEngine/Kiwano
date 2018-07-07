#pragma once
#include "e2dmacros.h"

namespace e2d
{


// 方向
enum class Direction : int
{
	Up,			/* 上 */
	Down,		/* 下 */
	Left,		/* 左 */
	Right		/* 右 */
};


// 线条相交样式
enum class LineJoin : int
{
	Miter = 0,	/* 斜切 */
	Bevel = 1,	/* 斜角 */
	Round = 2	/* 圆角 */
};


class Size;

// 坐标
class Point
{
public:
	double x;	// X 坐标
	double y;	// Y 坐标

public:
	Point();

	Point(double x, double y);

	Point(const Point& other);

	Point operator + (Point const & point) const;
	Point operator - (Point const & point) const;
	Point operator * (double const & point) const;
	Point operator / (double const & point) const;
	Point operator - () const;
	bool operator== (const Point& point) const;

	operator e2d::Size() const;

	// 判断两点间距离
	static double distance(const Point&, const Point&);
};


// 二维向量
typedef Point Vector2;


// 大小
class Size
{
public:
	double width;	// 宽度
	double height;	// 高度

public:
	Size();

	Size(double width, double height);

	Size(const Size& other);

	Size operator + (Size const & size) const;
	Size operator - (Size const & size) const;
	Size operator * (double const & size) const;
	Size operator / (double const & size) const;
	Size operator - () const;
	bool operator== (const Size& size) const;

	operator e2d::Point() const;
};


// 矩形
class Rect
{
public:
	Point origin;	// 原点坐标
	Size  size;		// 宽度和高度

public:
	Rect();
	
	Rect(double x, double y, double width, double height);
	
	Rect(const Point& pos, const Size& size);
	
	Rect(const Rect& other);
	
	Rect& operator= (const Rect& other);

	bool operator== (const Rect& rect) const;
	
	// 设置矩形
	void setRect(
		double x, 
		double y, 
		double width, 
		double height
	);
	
	// 判断点是否在矩形内
	bool containsPoint(
		const Point& point
	) const;
	
	// 判断两矩形是否相交
	bool intersects(
		const Rect& rect
	) const;
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

	// 获取指定位置字符
	wchar_t at(
		int index
	) const;

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
	E2D_OP_EXPLICIT operator const wchar_t* () const;
	E2D_OP_EXPLICIT operator wchar_t* () const;
	E2D_OP_EXPLICIT operator const char* () const;
	E2D_OP_EXPLICIT operator char* () const;

	// 比较运算符
	bool operator== (const String &) const;
	bool operator== (const char *) const;
	bool operator== (const wchar_t *) const;
	bool operator!= (const String &) const;
	bool operator!= (const char *) const;
	bool operator!= (const wchar_t *) const;
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
		UINT rgb
	);

	Color(
		UINT rgb,
		double alpha
	);

	D2D1_COLOR_F toD2DColorF() const;

public:
	enum Value : UINT
	{
		Black = 0x000000,
		Blue = 0x0000FF,
		BlueViolet = 0x8A2BE2,
		Brown = 0xA52A2A,
		Chocolate = 0xD2691E,
		DarkBlue = 0x00008B,
		DarkGray = 0xA9A9A9,
		DarkGreen = 0x006400,
		DarkOrange = 0xFF8C00,
		DarkRed = 0x8B0000,
		DarkViolet = 0x9400D3,
		ForestGreen = 0x228B22,
		Gold = 0xFFD700,
		Gray = 0x808080,
		Green = 0x008000,
		GreenYellow = 0xADFF2F,
		LightBlue = 0xADD8E6,
		LightCyan = 0xE0FFFF,
		LightGreen = 0x90EE90,
		LightGray = 0xD3D3D3,
		LightPink = 0xFFB6C1,
		LightSeaGreen = 0x20B2AA,
		LightSkyBlue = 0x87CEFA,
		LightYellow = 0xFFFFE0,
		Orange = 0xFFA500,
		OrangeRed = 0xFF4500,
		Pink = 0xFFC0CB,
		Purple = 0x800080,
		Red = 0xFF0000,
		Silver = 0xC0C0C0,
		SkyBlue = 0x87CEEB,
		Snow = 0xFFFAFA,
		Violet = 0xEE82EE,
		Wheat = 0xF5DEB3,
		White = 0xFFFFFF,
		WhiteSmoke = 0xF5F5F5,
		Wood = 0xDEB887,
		Yellow = 0xFFFF00,
		Yellow_Green = 0x9ACD32
	};

private:
	void _init(
		UINT rgb,
		double alpha
	);

private:
	float r;
	float g;
	float b;
	float a;
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

	E2D_OP_EXPLICIT operator bool() const;

protected:
	std::function<void()> _func;
};


// 字体
class Font
{
public:
	String	family;			// 字体族
	double	size;			// 字号
	UINT	weight;			// 粗细值
	bool	italic;			// 斜体

public:
	// 字体粗细值
	enum Weight : UINT
	{
		Thin = 100,
		ExtraLight = 200,
		Light = 300,
		Normal = 400,
		Medium = 500,
		Bold = 700,
		ExtraBold = 800,
		Black = 900,
		ExtraBlack = 950
	};

public:
	Font();

	explicit Font(
		const String& family,
		double size = 22,
		UINT weight = Font::Weight::Normal,
		bool italic = false
	);
};


class Node;
class ColliderManager;

// 碰撞体
class Collider
{
	friend class Node;
	friend class ColliderManager;

public:
	// 碰撞体类别
	enum class Type
	{
		None,		/* 无 */
		Rect,		/* 矩形 */
		Circle,		/* 圆形 */
		Ellipse		/* 椭圆形 */
	};

	// 碰撞体交集关系
	enum class Relation : int
	{
		Unknown = 0,		/* 关系不确定 */
		Disjoin = 1,		/* 没有交集 */
		IsContained = 2,	/* 完全被包含 */
		Contains = 3,		/* 完全包含 */
		Overlap = 4			/* 部分重叠 */
	};

public:
	// 启用或关闭该碰撞体
	virtual void setEnabled(
		bool enabled
	);

	// 设置碰撞体的可见性
	void setVisible(
		bool visible
	);

	// 设置绘制颜色
	void setColor(
		Color color
	);

	// 判断两碰撞体的交集关系
	virtual Relation getRelationWith(
		Collider * pCollider
	) const;

	// 获取绘制颜色
	Color getColor() const;

	// 获取 ID2D1Geometry* 对象
	ID2D1Geometry* getGeometry() const;

protected:
	Collider(
		Node * parent
	);

	virtual ~Collider();

	E2D_DISABLE_COPY(Collider);

	// 重新生成
	void _recreate();

	// 渲染碰撞体
	void _render();

protected:
	bool	_enabled;
	bool	_visible;
	Color	_color;
	Node *	_parentNode;
	Type	_type;
	ID2D1Geometry* _geometry;
};


// 资源
class Resource
{
public:
	Resource(
		const String& fileName		/* 文件路径 */
	);

	Resource(
		int resNameId,				/* 资源名称 */
		const String& resType		/* 资源类型 */
	);

	// 是否是资源类型
	bool isResource() const;

	const String& getFileName() const;

	int getResNameId() const;

	const String& getResType() const;

	int getKey() const;

	// 比较运算符
	bool operator> (const Resource &) const;
	bool operator>= (const Resource &) const;
	bool operator< (const Resource &) const;
	bool operator<= (const Resource &) const;

protected:
	bool _isResource;
	String _fileName;
	int _resNameId;
	String _resType;
};


// 引用计数对象
class Ref
{
public:
	Ref();

	virtual ~Ref();

	// 增加引用计数
	void retain();

	// 减少引用计数
	void release();

	// 获取引用计数
	int getRefCount() const;

protected:
	E2D_DISABLE_COPY(Ref);

private:
	int _refCount;
};


// 图片
class Image :
	public Ref
{
public:
	Image();

	explicit Image(
		const Resource& res
	);

	explicit Image(
		const Resource& res,
		const Rect& cropRect	/* 裁剪矩形 */
	);

	explicit Image(
		const String& fileName
	);

	explicit Image(
		const String& fileName,
		const Rect& cropRect	/* 裁剪矩形 */
	);

	virtual ~Image();

	// 加载图片资源
	bool open(
		const Resource& res
	);

	// 加载图片资源
	bool open(
		const String& fileName
	);

	// 将图片裁剪为矩形
	void crop(
		const Rect& cropRect	/* 裁剪矩形 */
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

	// 预加载图片资源
	static bool preload(
		const Resource& res
	);

	// 清空缓存
	static void clearCache();

protected:
	E2D_DISABLE_COPY(Image);

	// 设置 Bitmap
	void _setBitmap(
		ID2D1Bitmap * bitmap
	);

protected:
	Rect _cropRect;
	ID2D1Bitmap * _bitmap;
};


// 场景
class Scene :
	public Ref
{
public:
	Scene();

	virtual ~Scene();

	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter() {}

	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit() {}

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

	// 获取名称相同的子节点
	Node* getChild(
		const String& name
	) const;

	// 获取所有名称相同的子节点
	std::vector<Node*> getChildren(
		const String& name
	) const;

	// 获取所有子节点
	const std::vector<Node*>& getAllChildren() const;

	// 获取根节点
	Node * getRoot() const;

	// 渲染场景画面
	void render();

	// 更新场景内容
	void update();

protected:
	E2D_DISABLE_COPY(Scene);

protected:
	bool _autoUpdate;
	Node * _root;
};


class Game;

// 游戏配置
class Config :
	public Ref
{
	friend class Game;

public:
	Config();

	virtual ~Config();

	// 修改游戏名称
	// 默认：空
	void setGameName(
		const String& name
	);

	// 显示或隐藏节点轮廓
	// 默认：隐藏
	void setOutlineVisible(
		bool visible
	);

	// 打开或关闭声音
	// 默认：打开
	void setSoundEnabled(
		bool enabled
	);

	// 打开或关闭碰撞监听
	// 默认：关闭
	void setCollisionEnabled(
		bool enabled
	);

	// 设置节点的默认中心点位置
	// 默认：(0,0)
	void setNodeDefaultPivot(
		Point pivot
	);

	// 设置节点的默认碰撞体类型
	// 默认：Collider::Type::None
	void setDefaultColliderType(
		Collider::Type type
	);

	// 打开或关闭碰撞体可视化
	// 默认：关闭
	void setColliderVisible(
		bool visible
	);

	// 获取游戏名称
	String getGameName() const;

	// 获取声音打开状态
	bool isSoundEnabled() const;

	// 获取节点轮廓显示状态
	bool isOutlineVisible() const;

	// 获取碰撞监听状态
	bool isCollisionEnabled() const;

	// 获取节点的默认中心点位置
	Point getNodeDefaultPivot() const;

	// 获取节点的默认碰撞体类型
	Collider::Type getDefaultColliderType() const;

	// 获取碰撞体可视化状态
	bool isColliderVisible() const;

protected:
	virtual void _update();

protected:
	bool			_unconfigured;
	bool			_soundEnabled;
	bool			_outlineVisible;
	bool			_collisionEnabled;
	bool			_colliderVisible;
	String			_gameName;
	Point			_nodeDefPivot;
	Collider::Type	_nodeDefColliderType;
};


}


namespace e2d
{
	struct autorelease_t { };

	extern autorelease_t const autorelease;
}

void* operator new(
	size_t size,
	e2d::autorelease_t const&
	) E2D_NOEXCEPT;

void* operator new[](
	size_t size,
	e2d::autorelease_t const&
	) E2D_NOEXCEPT;

void operator delete(
	void* block,
	e2d::autorelease_t const&
	) E2D_NOEXCEPT;

void operator delete[](
	void* block,
	e2d::autorelease_t const&
	) E2D_NOEXCEPT;
