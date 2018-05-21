#pragma once
#include "e2dmacros.h"
#include "e2dcustom.h"

namespace e2d
{



// 方向
enum class Direction : int
{
	UP,			/* 上 */
	DOWN,		/* 下 */
	LEFT,		/* 左 */
	RIGHT		/* 右 */
};


// 线条相交样式
enum class LineJoin : int
{
	MITER = 0,	/* 斜切 */
	BEVEL = 1,	/* 斜角 */
	ROUND = 2	/* 圆角 */
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

	Point operator + (Point const & p) const;
	Point operator - (Point const & p) const;
	Point operator * (double const & value) const;
	Point operator / (double const & value) const;
	Point operator - () const;

	operator e2d::Size() const;
};

// 二维向量
typedef Point Vector;

// 大小
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
		UINT rgb
	);

	Color(
		UINT rgb,
		double alpha
	);

	D2D1_COLOR_F toD2DColorF() const;

public:
	enum RGB_VALUE : UINT
	{
		BLACK = 0x000000,
		BLUE = 0x0000FF,
		BLUE_VIOLET = 0x8A2BE2,
		BROWN = 0xA52A2A,
		CHOCOLATE = 0xD2691E,
		DARK_BLUE = 0x00008B,
		DARK_GRAY = 0xA9A9A9,
		DARK_GREEN = 0x006400,
		DARK_ORANGE = 0xFF8C00,
		DARK_RED = 0x8B0000,
		DARK_VIOLET = 0x9400D3,
		FOREST_GREEN = 0x228B22,
		GOLD = 0xFFD700,
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
		ORANGE = 0xFFA500,
		ORANGE_RED = 0xFF4500,
		PINK = 0xFFC0CB,
		PURPLE = 0x800080,
		RED = 0xFF0000,
		SILVER = 0xC0C0C0,
		SKY_BLUE = 0x87CEEB,
		SNOW = 0xFFFAFA,
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
		UINT rgb,
		double alpha
	);

public:
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
	Image();

	Image(
		const String& filePath	/* 图片文件路径 */
	);

	Image(
		int resNameId,			/* 图片资源名称 */
		const String& resType	/* 图片资源类型 */
	);

	Image(
		const String& filePath,	/* 图片文件路径 */
		double cropX,			/* 裁剪位置 X 坐标 */
		double cropY,			/* 裁剪位置 Y 坐标 */
		double cropWidth,		/* 裁剪宽度 */
		double cropHeight		/* 裁剪高度 */
	);

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


class Input;
class Collision;

// 监听器
class Listener
{
	friend Input;
	friend Collision;

public:
	Listener(
		const Function& func,
		const String& name,
		bool paused
	);

	// 更新监听器状态
	virtual void update();

	// 获取监听器运行状态
	bool isRunning() const;

	// 获取名称
	String getName() const;

	// 设置名称
	void setName(
		const String& name
	);

protected:
	bool _running;
	bool _stopped;
	String _name;
	Function _callback;
};


}