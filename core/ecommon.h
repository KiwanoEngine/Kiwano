#pragma once
#include "emacros.h"
#include <vector>
#include <functional>
#include <sstream>

namespace e2d
{


// 表示坐标的结构体
struct EPoint
{
	float x;
	float y;

	EPoint()
	{
		x = 0;
		y = 0;
	}

	EPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	EPoint operator + (EPoint const & p)
	{
		return EPoint(x + p.x, y + p.y);
	}

	EPoint operator - (EPoint const & p)
	{
		return EPoint(x - p.x, y - p.y);
	}

	EPoint operator * (float const & value)
	{
		return EPoint(x * value, y * value);
	}

	EPoint operator / (float const & value)
	{
		return EPoint(x / value, y / value);
	}
};

// 表示大小的结构体
struct ESize
{
	float width;
	float height;

	ESize()
	{
		width = 0;
		height = 0;
	}

	ESize(float width, float height)
	{
		this->width = width;
		this->height = height;
	}

	ESize operator + (ESize const & size)
	{
		return ESize(width + size.width, height + size.height);
	}

	ESize operator - (ESize const & size)
	{
		return ESize(width - size.width, height - size.height);
	}

	ESize operator * (float const & value)
	{
		return ESize(width * value, height * value);
	}

	ESize operator / (float const & value)
	{
		return ESize(width / value, height / value);
	}
};

// 字符串
class EString
{
public:
	EString();
	EString(const wchar_t);
	EString(const wchar_t *);
	EString(const EString &);
	EString(const std::wstring &);
	EString(EString &&);

	~EString();

	EString& operator=(const wchar_t *);
	EString& operator=(const EString &);
	EString& operator=(const std::wstring &);

	bool operator==(const wchar_t *);
	bool operator==(const EString &);
	bool operator==(const std::wstring &);

	bool operator!=(const wchar_t *);
	bool operator!=(const EString &);
	bool operator!=(const std::wstring &);

	wchar_t &operator[](int);

	EString operator+(const wchar_t);
	EString operator+(const wchar_t *);
	EString operator+(const EString &);
	EString operator+(const std::wstring &);

	template<typename T>
	EString operator+(const T value)
	{
		EString str_temp(*this);

		str_temp += value;
		return std::move(str_temp);
	}

	EString &operator +=(const wchar_t);
	EString &operator +=(const wchar_t *);
	EString &operator +=(const EString &);
	EString &operator +=(const std::wstring &);

	template<typename T>
	EString &operator +=(const T value)
	{
		std::wostringstream ss;
		ss << value;
		return (*this) += ss.str();
	}

	bool operator < (EString const&) const;
	bool operator <= (EString const&) const;
	bool operator > (EString const&) const;
	bool operator >= (EString const&) const;

	operator wchar_t*() const { return _string; }
	operator bool() const { return _size != 0; }

	friend EString operator+(const wchar_t, const EString &);
	friend EString operator+(const wchar_t*, const EString &);
	friend EString operator+(const EString &, const EString &);
	friend EString operator+(const std::wstring &, const EString &);
	template<typename T>
	friend EString operator+(const T &value, const EString &str)
	{
		return std::move((EString::parse(value) + str2));
	}

	friend std::wistream &operator>>(std::wistream &, EString &);

	// 判断字符串是否为空
	bool isEmpty() const { return _size == 0; }

	// 获取字符串长度
	int length() const { return _size; }

	// 获取大写字符串
	EString upper() const;

	// 获取小写字符串
	EString lower() const;

	// 获取裁剪字符串
	EString sub(int offset, int count = -1) const;

	// 获取字符串中第一个特定字符的下标
	int findFirstOf(const wchar_t ch) const;

	// 获取字符串中最后一个特定字符的下标
	int findLastOf(const wchar_t ch) const;

	// 后接字符
	EString &append(const wchar_t ch);

	// 后接字符串
	EString &append(const wchar_t *str);

	// 后接字符串
	EString &append(const EString &str);

	// 后接字符串
	template<typename T>
	EString &append(const T &value)
	{
		return (*this) += value;
	}

	// 获取该字符串的散列值
	unsigned int hash() const;

	// 将模板类型转化为字符串
	template<typename T>
	static EString parse(const T value)
	{
		EString str;

		std::wostringstream ss;
		ss << value;
		str += ss.str();

		return std::move(str);
	}

private:
	wchar_t *_string;
	int _size;
};


// 二维向量
typedef EPoint EVector2;

// 定时器回调函数（参数为该定时器被调用的次数，从 0 开始）
typedef std::function<void(int)> TimerCallback;

// 按钮点击回调函数
typedef std::function<void()> BtnClkCallback;

// 物理世界消息监听器回调函数
typedef std::function<void()> PhysLsnrCallback;

// 碰撞消息监听器回调函数
typedef PhysLsnrCallback  ClsLsnrCallback;

template<typename T>
inline void SafeDelete(T** p) { if (*p) { delete *p; *p = nullptr; } }

template<typename Obj>
inline void SafeRelease(Obj** p) { if (*p) { (*p)->release(); *p = nullptr; } }

template<class Interface>
inline void SafeReleaseInterface(Interface **pp) { if (*pp != nullptr) { (*pp)->Release(); (*pp) = nullptr; } }


// 颜色
class EColor
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
class EFontWeight
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
class EKeyCode
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


class EGeometry;

// 物理消息
class EPhysicsEvent
{
public:
	enum INTERSECT_RELATION
	{
		UNKNOWN = 0,		/* 关系不确定 */
		DISJOINT = 1,		/* 没有交集 */
		IS_CONTAINED = 2,	/* 完全被包含 */
		CONTAINS = 3,		/* 完全包含 */
		OVERLAP = 4			/* 部分重叠 */
	};

	// 获取当前物理碰撞消息类型
	static INTERSECT_RELATION getMsg();

	// 获取主动方
	static EGeometry * getActiveGeometry();

	// 获取被动方
	static EGeometry * getPassiveGeometry();

public:
	static INTERSECT_RELATION s_nRelation;
	static EGeometry * s_pActiveGeometry;
	static EGeometry * s_pPassiveGeometry;
};


class EObjectManager;

class EObject
{
	friend EObjectManager;

public:
	EObject();

	virtual ~EObject();

	// 引用计数加一
	void retain();

	// 引用计数减一
	void release();

private:
	int m_nRefCount;
	bool m_bManaged;
};


class EText;

class EFont :
	public EObject
{
	friend EText;

public:
	EFont();

	EFont(
		EString fontFamily,
		float fontSize = 22,
		UINT32 color = EColor::WHITE,
		UINT32 fontWeight = EFontWeight::REGULAR,
		bool italic = false
	);

	virtual ~EFont();

	// 获取当前字号
	float getFontSize() const;

	// 获取当前字体粗细值
	UINT32 getFontWeight() const;

	// 获取文字颜色
	UINT32 getColor() const;

	// 是否是斜体
	bool isItalic() const;

	// 设置字体
	void setFamily(
		const EString & fontFamily
	);

	// 设置字号
	void setSize(
		float fontSize
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
	EString		m_sFontFamily;
	float		m_fFontSize;
	UINT32		m_FontWeight;
	UINT32		m_Color;
	bool		m_bItalic;
	bool		m_bRecreateNeeded;
	IDWriteTextFormat * m_pTextFormat;
};


class ESprite;

class EImage :
	public EObject
{
	friend ESprite;

public:
	// 创建一个空的图片
	EImage();

	// 从本地文件中读取资源
	EImage(
		LPCTSTR fileName
	);

	// 读取程序资源
	EImage(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	virtual ~EImage();

	// 从本地文件中读取资源
	void loadFromFile(
		const EString & fileName
	);

	// 读取程序资源
	void loadFromResource(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// 获取源图片宽度
	virtual float getSourceWidth() const;

	// 获取源图片高度
	virtual float getSourceHeight() const;

	// 获取源图片大小
	virtual ESize getSourceSize() const;

	// 预加载资源
	static bool preload(
		const EString & fileName
	);

	// 预加载资源
	static bool preload(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// 清空缓存
	static void clearCache();

protected:
	ID2D1Bitmap * _getBitmap();

protected:
	ID2D1Bitmap * m_pBitmap;
};


class EKeyframe :
	public EObject
{
	friend ESprite;

public:
	// 创建空的关键帧
	EKeyframe();

	// 创建空的关键帧
	EKeyframe(
		EImage * texture
	);

	// 创建空的关键帧
	EKeyframe(
		const EString & imageFileName
	);

	// 创建空的关键帧
	EKeyframe(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// 创建空的关键帧
	EKeyframe(
		EImage * texture,
		float x,
		float y,
		float width,
		float height
	);

	// 创建空的关键帧
	EKeyframe(
		const EString & imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	// 创建空的关键帧
	EKeyframe(
		LPCTSTR resourceName,
		LPCTSTR resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~EKeyframe();

	// 获取宽度
	float getWidth() const;

	// 获取高度
	float getHeight() const;

	// 获取图片
	EImage * getImage() const;

protected:
	// 修改图片
	void _setImage(
		EImage * texture
	);

	// 裁剪图片
	void _clipTexture(
		float x,
		float y,
		float width,
		float height
	);

protected:
	float	m_fSourceClipX;
	float	m_fSourceClipY;
	float	m_fSourceClipWidth;
	float	m_fSourceClipHeight;
	EImage * m_pImage;
};


class ESceneManager;
class ENode;
class EAction;

// 场景
class EScene :
	public EObject
{
	friend ESceneManager;

public:
	EScene();

	virtual ~EScene();

	// 重写这个函数，它将在进入这个场景时自动执行
	virtual void onEnter() {}

	// 重写这个函数，它将在离开这个场景时自动执行
	virtual void onExit() {}

	// 重写这个函数，它将在窗口激活时执行
	virtual bool onActivate() { return false; }

	// 重写这个函数，它将在窗口非激活时执行
	virtual bool onInactive() { return false; }

	// 重写这个函数，它将在关闭窗口时执行
	virtual bool onCloseWindow() { return true; }

	// 重写这个函数，它将在每一帧画面刷新时执行
	virtual void onUpdate() {}

	// 添加子节点到场景
	void add(
		ENode * child,
		int zOrder = 0
	);

	// 删除子节点
	bool remove(
		ENode * child
	);

	// 删除相同名称的子节点
	void remove(
		const EString &childName
	);

	// 获取所有子节点
	std::vector<e2d::ENode*> getChildren();

	// 获取子节点数量
	size_t getChildrenCount() const;

	// 根据名称获取子节点
	ENode * getChild(
		const EString &childName
	);

	// 获取根节点
	ENode * getRoot() const;

	// 清空所有子成员
	void clearAllChildren();

	// 执行动画
	void runAction(
		EAction * action
	);

	// 开启几何图形的渲染
	void setGeometryVisiable(
		bool visiable
	);

protected:
	// 渲染场景画面
	void _render();

	// 更新场景内容
	void _update();

protected:
	bool m_bSortNeeded;
	bool m_bWillSave;
	bool m_bGeometryVisiable;
	ENode * m_pRoot;
};

}