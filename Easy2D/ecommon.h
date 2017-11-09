#pragma once
#include "emacros.h"
#include "etypedef.h"

namespace e2d
{


class EColor
{
public:
	enum COMMON_VALUE
	{
		ALICE_BLUE = 0xF0F8FF,
		ANTIQUE_WHITE = 0xFAEBD7,
		AQUA = 0x00FFFF,
		AQUAMARINE = 0x7FFFD4,
		AZURE = 0xF0FFFF,
		BEIGE = 0xF5F5DC,
		BISQUE = 0xFFE4C4,
		BLACK = 0x000000,
		BLANCHED_ALMOND = 0xFFEBCD,
		BLUE = 0x0000FF,
		BLUE_VIOLET = 0x8A2BE2,
		BROWN = 0xA52A2A,
		BURLY_WOOD = 0xDEB887,
		CADET_BLUE = 0x5F9EA0,
		CHARTREUSE = 0x7FFF00,
		CHOCOLATE = 0xD2691E,
		CORAL = 0xFF7F50,
		CORNFLOWER_BLUE = 0x6495ED,
		CORNSILK = 0xFFF8DC,
		CRIMSON = 0xDC143C,
		CYAN = 0x00FFFF,
		DARK_BLUE = 0x00008B,
		DARK_CYAN = 0x008B8B,
		DARK_GOLDENROD = 0xB8860B,
		DARK_GRAY = 0xA9A9A9,
		DARK_GREEN = 0x006400,
		DARK_KHAKI = 0xBDB76B,
		DARK_MAGENTA = 0x8B008B,
		DARK_OLIVE_GREEN = 0x556B2F,
		DARK_ORANGE = 0xFF8C00,
		DARK_ORCHID = 0x9932CC,
		DARK_RED = 0x8B0000,
		DARK_SALMON = 0xE9967A,
		DARK_SEA_GREEN = 0x8FBC8F,
		DARK_SLATE_BLUE = 0x483D8B,
		DARK_SLATE_GRAY = 0x2F4F4F,
		DARK_TURQUOISE = 0x00CED1,
		DARK_VIOLET = 0x9400D3,
		DEEP_PINK = 0xFF1493,
		DEEP_SKY_BLUE = 0x00BFFF,
		DIM_GRAY = 0x696969,
		DODGER_BLUE = 0x1E90FF,
		FIREBRICK = 0xB22222,
		FLORAL_WHITE = 0xFFFAF0,
		FOREST_GREEN = 0x228B22,
		FUCHSIA = 0xFF00FF,
		GAINSCORO = 0xDCDCDC,
		GHOST_WHITE = 0xF8F8FF,
		GOLD = 0xFFD700,
		GOLDENROD = 0xDAA520,
		GRAY = 0x808080,
		GREEN = 0x008000,
		GREEN_YELLOW = 0xADFF2F,
		HONEYDEW = 0xF0FFF0,
		HOT_PINK = 0xFF69B4,
		INDIAN_RED = 0xCD5C5C,
		INDIGO = 0x4B0082,
		IVORY = 0xFFFFF0,
		KHAKI = 0xF0E68C,
		LAVENDER = 0xE6E6FA,
		LAVENDER_BLUSH = 0xFFF0F5,
		LAWN_GREEN = 0x7CFC00,
		LEMON_CHIFFON = 0xFFFACD,
		LIGHT_BLUE = 0xADD8E6,
		LIGHT_CORAL = 0xF08080,
		LIGHT_CYAN = 0xE0FFFF,
		LIGHT_GOLDENROD_YELLOW = 0xFAFAD2,
		LIGHT_GREEN = 0x90EE90,
		LIGHT_GRAY = 0xD3D3D3,
		LIGHT_PINK = 0xFFB6C1,
		LIGHT_SALMON = 0xFFA07A,
		LIGHT_SEA_GREEN = 0x20B2AA,
		LIGHT_SKY_BLUE = 0x87CEFA,
		LIGHT_SLATE_GRAY = 0x778899,
		LIGHT_STEEL_BLUE = 0xB0C4DE,
		LIGHT_YELLOW = 0xFFFFE0,
		LIME = 0x00FF00,
		LIME_GREEN = 0x32CD32,
		LINEN = 0xFAF0E6,
		MAGENTA = 0xFF00FF,
		MAROON = 0x800000,
		MEDIUM_AQUAMARINE = 0x66CDAA,
		MEDIUM_BLUE = 0x0000CD,
		MEDIUM_ORCHID = 0xBA55D3,
		MEDIUM_PURPLE = 0x9370DB,
		MEDIUM_SEA_GREEN = 0x3CB371,
		MEDIUM_SLATE_BLUE = 0x7B68EE,
		MEDIUM_SPRING_GREEN = 0x00FA9A,
		MEDIUM_TURQUOISE = 0x48D1CC,
		MEDUIM_VIOLET_RED = 0xC71585,
		MIDNIGHT_BLUE = 0x191970,
		MINT_CREAM = 0xF5FFFA,
		MISTY_ROSE = 0xFFE4E1,
		MOCCASIN = 0xFFE4B5,
		NAVAJO_WHITE = 0xFFDEAD,
		NAVY = 0x000080,
		OLD_LACE = 0xFDF5E6,
		OLIVE = 0x808000,
		OLIVE_DRAB = 0x6B8E23,
		ORANGE = 0xFFA500,
		ORANGE_RED = 0xFF4500,
		ORCHID = 0xDA70D6,
		PALE_GOLDENROD = 0xEEE8AA,
		PALE_GREEN = 0x98FB98,
		PALE_TURQUOISE = 0xAFEEEE,
		PALE_VIOLET_RED = 0xDB7093,
		PAPAYA_WHIP = 0xFFEFD5,
		PEACH_PUFF = 0xFFDAB9,
		PERU = 0xCD853F,
		PINK = 0xFFC0CB,
		PLUM = 0xDDA0DD,
		POWDER_BLUE = 0xB0E0E6,
		PURPLE = 0x800080,
		RED = 0xFF0000,
		ROSY_BROWN = 0xBC8F8F,
		ROYAL_BLUE = 0x4169E1,
		SADDLE_BROWN = 0x8B4513,
		SALMON = 0xFA8072,
		SANDY_BROWN = 0xF4A460,
		SEA_GREEN = 0x2E8B57,
		SEA_SHELL = 0xFFF5EE,
		SIENNA = 0xA0522D,
		SILVER = 0xC0C0C0,
		SKY_BLUE = 0x87CEEB,
		SLATE_BLUE = 0x6A5ACD,
		SLATE_GRAY = 0x708090,
		SNOW = 0xFFFAFA,
		SPRING_GREEN = 0x00FF7F,
		STEEL_BLUE = 0x4682B4,
		TAN = 0xD2B48C,
		TEAL = 0x008080,
		THISTLE = 0xD8BFD8,
		TOMATO = 0xFF6347,
		TURQUOISE = 0x40E0D0,
		VIOLET = 0xEE82EE,
		WHEAT = 0xF5DEB3,
		WHITE = 0xFFFFFF,
		WHITE_SMOKE = 0xF5F5F5,
		YELLOW = 0xFFFF00,
		YELLOW_GREEN = 0x9ACD32
	};
};


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


// 鼠标消息
class EMouseMsg
{
public:
	// 鼠标消息集合
	enum MOUSE_MSG
	{
		MOVE = 0x0200,	// 鼠标移动
		LBUTTON_DOWN,	// 鼠标左键按下
		LBUTTON_UP,		// 鼠标左键抬起
		LBUTTON_DBLCLK,	// 鼠标左键双击
		RBUTTON_DOWN,	// 鼠标右键按下
		RBUTTON_UP,		// 鼠标右键抬起
		RBUTTON_DBLCLK,	// 鼠标右键双击
		MBUTTON_DOWN,	// 鼠标中键按下
		MBUTTON_UP,		// 鼠标中键抬起
		MBUTTON_DBLCLK,	// 鼠标中键双击
		WHEEL			// 滑动滚轮
	};

	// 获取鼠标横坐标
	static DWORD getPosX();

	// 获取鼠标纵坐标
	static DWORD getPosY();

	// 获取鼠标坐标
	static EPoint getPos();

	// 获取鼠标左键按下状态
	static bool isLButtonDown();

	// 获取鼠标中键按下状态
	static bool isMButtonDown();

	// 获取鼠标右键按下状态
	static bool isRButtonDown();

	// 获取 Shift 按键状态
	static bool isShiftDown();

	// 获取 Ctrl 按键状态
	static bool isCtrlDown();

	// 获取鼠标滚轮值
	static DWORD getWheelDelta();

	// 获取当前鼠标消息类型
	static MOUSE_MSG getMsg();

public:
	static UINT s_nMsg;
	static WPARAM s_wParam;
	static LPARAM s_lParam;
};


// 按键消息
class EKeyboardMsg
{
public:
	// 按键消息类型集合
	enum KEYBOARD_MSG
	{
		KEY_DOWN = 0x0100,	// 按下
		KEY_UP				// 抬起
	};

	// 按键键值集合
	enum class KEY
	{
		A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,					// 字母键值
		NUM0 = '0', NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,									// 数字键值
		NUMPAD0 = 0x60, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,	// 数字小键盘键值
		F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,											// F键键值
		MULTIPLY,		// 乘号键键值
		ADD,			// 加号键键值
		SEPARATOR,		// 分割键键值
		SUBTRACT,		// 减号键键值
		DECIMAL,		// 小数点键键值
		DIVIDE,			// 除号键键值
		TAB = 0x09,		// TAB 键键值
		ENTER = 0x0D,	// 回车键键值
		SHIFT, CTRL, 	// SHIFT 键键值
		ESC = 0x1B, 	// ESCAPE 键键值
		SPACE = 0x20, 	// 空格键键值
		PAGE_UP, 		// PageUp 键键值
		PAGE_DOWN, 		// PageDown 键键值
		END, 			// End 键键值
		HOME, 			// Home 键键值
		LEFT, 			// 左键键值
		UP, 			// 上键键值
		RIGHT, 			// 右键键值
		DOWN			// 下键键值
	};

	// 获取按键消息类型
	static KEYBOARD_MSG getMsg();

	// 获取键值
	static KEY getKeyValue();

	// 获取按键消息的计数
	static DWORD getCount();

	// 获取特定按键的状态
	static bool isKeyDown(
		KEY key
	);

	// 获取大小写锁定状态
	static bool isCapitalLockOn();

	// 获取数字小键盘锁定状态
	static bool isNumpadLockOn();

	// 获取滑动锁定状态
	static bool isScrollLockOn();

public:
	static UINT s_nMsg;
	static WPARAM s_wParam;
	static LPARAM s_lParam;
};


class EGeometry;

// 物理消息
class EPhysicsMsg
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

class ETexture :
	public EObject
{
	friend ESprite;

public:
	// 创建一个空的纹理
	ETexture();

	// 从本地文件中读取资源
	ETexture(
		const EString & fileName
	);

	// 读取程序资源
	ETexture(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	virtual ~ETexture();

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


class ESpriteFrame :
	public EObject
{
	friend ESprite;

public:
	// 创建空的精灵帧
	ESpriteFrame();

	// 创建空的精灵帧
	ESpriteFrame(
		ETexture * texture
	);

	// 创建空的精灵帧
	ESpriteFrame(
		const EString & imageFileName
	);

	// 创建空的精灵帧
	ESpriteFrame(
		LPCTSTR resourceName,
		LPCTSTR resourceType
	);

	// 创建空的精灵帧
	ESpriteFrame(
		ETexture * texture,
		float x,
		float y,
		float width,
		float height
	);

	// 创建空的精灵帧
	ESpriteFrame(
		const EString & imageFileName,
		float x,
		float y,
		float width,
		float height
	);

	// 创建空的精灵帧
	ESpriteFrame(
		LPCTSTR resourceName,
		LPCTSTR resourceType,
		float x,
		float y,
		float width,
		float height
	);

	virtual ~ESpriteFrame();

	// 获取宽度
	float getWidth() const;

	// 获取高度
	float getHeight() const;

	// 获取纹理
	ETexture * getTexture() const;

protected:
	// 获取纹理
	void _setTexture(
		ETexture * texture
	);

	// 裁剪纹理
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
	ETexture * m_pTexture;
};

}