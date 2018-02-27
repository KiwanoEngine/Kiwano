#pragma once
#include "emacros.h"
#include <vector>
#include <functional>
#include <sstream>

namespace e2d
{


// 表示坐标的结构体
struct Point
{
	double x;
	double y;

	Point()
	{
		x = 0;
		y = 0;
	}

	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	Point operator + (Point const & p)
	{
		return Point(x + p.x, y + p.y);
	}

	Point operator - (Point const & p)
	{
		return Point(x - p.x, y - p.y);
	}

	Point operator * (double const & value)
	{
		return Point(x * value, y * value);
	}

	Point operator / (double const & value)
	{
		return Point(x / value, y / value);
	}
};

// 表示大小的结构体
struct Size
{
	double width;
	double height;

	Size()
	{
		width = 0;
		height = 0;
	}

	Size(double width, double height)
	{
		this->width = width;
		this->height = height;
	}

	Size operator + (Size const & size)
	{
		return Size(width + size.width, height + size.height);
	}

	Size operator - (Size const & size)
	{
		return Size(width - size.width, height - size.height);
	}

	Size operator * (double const & value)
	{
		return Size(width * value, height * value);
	}

	Size operator / (double const & value)
	{
		return Size(width / value, height / value);
	}
};


// 字符串
class String
{
public:
	String();
	String(const wchar_t *);
	String(const String &);
	String(const std::wstring &);
	String(String &&);

	~String();

	String& operator= (const wchar_t *);
	String& operator= (const String &);
	String& operator= (const std::wstring &);

	bool operator== (const wchar_t *);
	bool operator== (const String &);
	bool operator== (const std::wstring &);

	bool operator!= (const wchar_t *);
	bool operator!= (const String &);
	bool operator!= (const std::wstring &);

	wchar_t &operator[] (int);

	String operator+ (const wchar_t);
	String operator+ (const wchar_t *);
	String operator+ (const String &);
	String operator+ (const std::wstring &);

	friend String operator+ (const wchar_t, const String &);
	friend String operator+ (const wchar_t*, const String &);
	friend String operator+ (const std::wstring &, const String &);

	String& operator+= (const wchar_t);
	String& operator+= (const wchar_t *);
	String& operator+= (const String &);
	String& operator+= (const std::wstring &);

	template<typename T>
	String& operator<< (const T value)
	{
		std::wostringstream ss;
		ss << value;
		return (*this) += ss.str();
	}

	operator const wchar_t* () const;
	operator bool () const;

	friend std::wistream& operator>> (std::wistream &, String &);

	// 判断字符串是否为空
	bool isEmpty() const;

	// 获取字符串长度
	int getLength() const;

	// 获取该字符串的散列值
	unsigned int getHash() const;

	// 后接字符
	String &append(const wchar_t ch);

	// 后接字符串
	String &append(const wchar_t *str);

	// 后接字符串
	String &append(const String &str);

	// 后接字符串
	template<typename T>
	String &append(const T &value)
	{
		return (*this) += value;
	}

	// 获取裁剪字符串
	String subtract(int offset, int count = -1) const;

	// 获取字符串中第一个特定字符的下标
	int findFirstOf(const wchar_t ch) const;

	// 获取字符串中最后一个特定字符的下标
	int findLastOf(const wchar_t ch) const;

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

	// 将任意类型转化为字符串
	template<typename T>
	static String toString(const T value)
	{
		std::wostringstream ss;
		ss << value;
		return ss.str();
	}

private:
	std::wstring m_str;
};


// 颜色
class Color
{
public:
	enum COMMON_VALUE
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
};

// 字体粗细值
class FontWeight
{
public:
	enum COMMON_VALUE
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


// 键值集合
class KeyCode
{
public:
	enum VALUE
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
		NUMPAD0 = 0x52,
		F1 = 0x3B,
		F2 = 0x3C,
		F3 = 0x3D,
		F4 = 0x3E,
		F5 = 0x3F,
		F6 = 0x40,
		F7 = 0x41,
		F8 = 0x42,
		F9 = 0x43,
		F10 = 0x44
	};
};


// 形状交集关系
class Relation
{
public:
	enum VALUE
	{
		UNKNOWN = 0,		/* 关系不确定 */
		DISJOINT = 1,		/* 没有交集 */
		IS_CONTAINED = 2,	/* 完全被包含 */
		CONTAINS = 3,		/* 完全包含 */
		OVERLAP = 4			/* 部分重叠 */
	};
};


class ObjectManager;

// 基础对象
class Obj
{
	friend ObjectManager;

public:
	Obj();

	virtual ~Obj();

	// 引用计数加一
	void retain();

	// 引用计数减一
	void release();

private:
	int m_nRefCount;
	bool m_bManaged;
};


class Text;

class Font :
	public Obj
{
	friend Text;

public:
	Font();

	Font(
		String fontFamily,
		double fontSize = 22,
		UINT32 color = Color::WHITE,
		UINT32 fontWeight = FontWeight::REGULAR,
		bool italic = false
	);

	virtual ~Font();

	// 获取当前字号
	double getFontSize() const;

	// 获取当前字体粗细值
	UINT32 getFontWeight() const;

	// 获取文字颜色
	UINT32 getColor() const;

	// 是否是斜体
	bool isItalic() const;

	// 设置字体
	void setFamily(
		const String & fontFamily
	);

	// 设置字号
	void setSize(
		double fontSize
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
	String		m_sFontFamily;
	float		m_fFontSize;
	UINT32		m_FontWeight;
	UINT32		m_Color;
	bool		m_bItalic;
	bool		m_bRecreateNeeded;
	IDWriteTextFormat * m_pTextFormat;
};


// 图片
class Image :
	public Obj
{
public:
	// 创建一个空的图片
	Image();

	// 从本地文件中读取资源
	Image(
		LPCTSTR strFilePath	/* 图片文件路径 */
	);

	// 从本地文件中读取资源
	Image(
		LPCTSTR strFilePath,/* 图片文件路径 */
		double nClipX,		/* 裁剪位置 X 坐标 */
		double nClipY,		/* 裁剪位置 Y 坐标 */
		double nClipWidth,	/* 裁剪宽度 */
		double nClipHeight	/* 裁剪高度 */
	);

	virtual ~Image();

	// 裁剪图片
	void clip(
		double nClipX,		/* 裁剪位置 X 坐标 */
		double nClipY,		/* 裁剪位置 Y 坐标 */
		double nClipWidth,	/* 裁剪宽度 */
		double nClipHeight	/* 裁剪高度 */
	);

	// 从本地文件中读取图片
	void loadFrom(
		const String & strFilePath
	);

	// 从本地文件中读取图片并裁剪
	void loadFrom(
		const String & strFilePath,/* 图片文件路径 */
		double nClipX,				/* 裁剪位置 X 坐标 */
		double nClipY,				/* 裁剪位置 Y 坐标 */
		double nClipWidth,			/* 裁剪宽度 */
		double nClipHeight			/* 裁剪高度 */
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
	virtual double getClipX() const;

	// 获取裁剪位置 Y 坐标
	virtual double getClipY() const;

	// 获取裁剪位置
	virtual Point getClipPos() const;

	// 获取 ID2D1Bitmap 对象
	ID2D1Bitmap * getBitmap();

	// 预加载资源
	static bool preload(
		const String & sFileName	/* 图片文件路径 */
	);

	// 清空缓存
	static void clearCache();

protected:
	double	m_fSourceClipX;
	double	m_fSourceClipY;
	double	m_fSourceClipWidth;
	double	m_fSourceClipHeight;
	ID2D1Bitmap * m_pBitmap;
};


class SceneManager;
class Node;
class Action;

// 场景
class Scene :
	public Obj
{
	friend SceneManager;

public:
	Scene();

	virtual ~Scene();

	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter() {}

	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit() {}

	// 重写这个函数，它将在碰撞发生时自动执行
	virtual void onCollide(
		Node * pActiveNode,	/* 主动发生碰撞的节点 */
		Node * pPassiveNode	/* 被动发生碰撞的节点 */
	) {}

	// 重写这个函数，它将在关闭窗口时执行
	virtual bool onCloseWindow() { return true; }

	// 重写这个函数，它将在每一帧画面刷新时执行
	virtual void onUpdate() {}

	// 重写这个函数，它将在游戏暂停时执行
	virtual void onPause() {}

	// 开启或禁用 onUpdate 函数
	void setAutoUpdate(
		bool bAutoUpdate
	);

	// 添加节点到场景
	void add(
		Node * child,
		int zOrder = 0
	);

	// 删除子节点
	bool remove(
		Node * child
	);

	// 获取根节点
	Node * getRoot() const;

	// 开启几何图形的渲染
	void setShapeVisiable(
		bool visiable
	);

protected:
	// 渲染场景画面
	void _render();

	// 更新场景内容
	void _update();

protected:
	bool m_bAutoUpdate;
	bool m_bSortNeeded;
	bool m_bWillSave;
	bool m_bShapeVisiable;
	Node * m_pRoot;
};


// 二维向量
typedef Point Vector;

// 返回值和参数列表都为空的函数
typedef std::function<void(void)> VoidFunction;

// 定时器回调函数
typedef VoidFunction TimerCallback;

// 按钮点击回调函数
typedef VoidFunction ButtonCallback;

template<typename T>
inline void SafeDelete(T** p) { if (*p) { delete *p; *p = nullptr; } }

template<typename Obj>
inline void SafeRelease(Obj** p) { if (*p) { (*p)->release(); *p = nullptr; } }

template<class Interface>
inline void SafeReleaseInterface(Interface **pp) { if (*pp != nullptr) { (*pp)->Release(); (*pp) = nullptr; } }

}