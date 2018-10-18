// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __E2D_UTIL_H__
#define __E2D_UTIL_H__


#include "e2dmacros.h"

namespace easy2d
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
	enum class Stroke : int
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
		float x;	// X 坐标
		float y;	// Y 坐标

	public:
		Point();

		Point(
			float x,
			float y
		);

		Point(
			const Point& other
		);

		Point operator + (const Point & other) const;
		Point operator - (const Point & other) const;
		Point operator * (float value) const;
		Point operator / (float value) const;
		Point operator - () const;
		bool operator== (const Point& other) const;

		E2D_OP_EXPLICIT operator easy2d::Size() const;

		// 判断两点间距离
		static float Distance(
			const Point& p1,
			const Point& p2
		);
	};


	// 大小
	class Size
	{
	public:
		float width;	// 宽度
		float height;	// 高度

	public:
		Size();

		Size(
			float width,
			float height
		);

		Size(
			const Size& other
		);

		Size operator + (const Size & other) const;
		Size operator - (const Size & other) const;
		Size operator * (float value) const;
		Size operator / (float value) const;
		Size operator - () const;
		bool operator== (const Size& other) const;

		E2D_OP_EXPLICIT operator easy2d::Point() const;
	};


	// 矩形
	class Rect
	{
	public:
		Point origin;	// 原点坐标
		Size  size;		// 宽度和高度

	public:
		Rect();

		Rect(
			float x,
			float y,
			float width,
			float height
		);

		Rect(
			const Point& pos,
			const Size& size
		);

		Rect(
			const Rect& other
		);

		Rect& operator= (const Rect& other);

		bool operator== (const Rect& rect) const;

		// 判断点是否在矩形内
		bool ContainsPoint(
			const Point& point
		) const;

		// 判断两矩形是否相交
		bool Intersects(
			const Rect& rect
		) const;
	};


	// 字符串
	class String
	{
	public:
		String();

		String(
			const String &
		);

		String(
			const char *
		);

		String(
			const wchar_t *
		);

		String(
			String &&
		);

		~String();

		// 获取字符串长度
		int Length() const;

		// 获取该字符串的 Hash 值
		size_t GetHash() const;

		// 判断字符串是否为空
		bool IsEmpty() const;

		// 获取指定位置字符
		const wchar_t& At(
			size_t index
		) const;

		// 比较字符串
		int Compare(
			const String & str
		) const;

		// 截取字符串
		String Subtract(
			int offset,		/* 偏移量 */
			int count = -1	/* 截取字符数量 */
		) const;

		// 插入字符串
		void Insert(
			const String & str,
			int pos
		);

		// 替换字符串中的指定内容
		void Replace(
			const String & from,	/* 需替换内容 */
			const String & to		/* 替换成内容 */
		);

		// 删除字符串中的指定内容
		void Erase(
			int offset,		/* 偏移量 */
			int count		/* 删除字符数量 */
		);

		// 搜索字符串
		int Find(
			const String & str,	/* 查找内容 */
			int offset = 0		/* 偏移量 */
		) const;

		// 清空字符串
		void Clear();

		// 获取大写字符串
		String ToUpper() const;

		// 获取小写字符串
		String ToLower() const;

		// 将字符串转化为 int 型
		int ToInt() const;

		// 将字符串转化为 float 型
		float ToFloat() const;

		// 将字符串转化为 double 型
		double ToDouble() const;

		// 将字符串转化为 bool 型
		bool ToBool() const;

		// 数字类型转字符串
		static String Parse(int value);
		static String Parse(unsigned int value);
		static String Parse(float value);
		static String Parse(double value);

		// 格式化字符串
		static String Format(const char * format, ...);
		static String Format(const wchar_t * format, ...);

		// 赋值运算符
		String& operator= (const String &);
		String& operator= (const char *);
		String& operator= (const wchar_t *);

		// 运算符
		String& operator+= (const String &);
		String& operator+= (const char *);
		String& operator+= (const wchar_t *);
		String operator+ (const String &) const;
		String operator+ (const char *) const;
		String operator+ (const wchar_t *) const;

		// 友元运算符
		friend String operator+ (const char *, const String &);
		friend String operator+ (const wchar_t*, const String &);

		// 类型转换操作符
		E2D_OP_EXPLICIT operator const wchar_t* () const;
		E2D_OP_EXPLICIT operator wchar_t* () const;
		E2D_OP_EXPLICIT operator std::wstring() const;
		E2D_OP_EXPLICIT operator std::string() const;

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
		wchar_t& operator[] (size_t);

		friend std::ostream& operator<< (std::ostream &, const String &);
		friend std::wostream& operator<< (std::wostream &, const String &);

		friend std::istream& operator>> (std::istream &, String &);
		friend std::wistream& operator>> (std::wistream &, String &);

	private:
		std::wstring string_;
	};


	// 颜色
	class Color
	{
	public:
		Color();

		Color(
			float r,
			float g,
			float b
		);

		Color(
			float r,
			float g,
			float b,
			float alpha
		);

		Color(
			UINT rgb
		);

		Color(
			UINT rgb,
			float alpha
		);

		Color(
			const D2D1_COLOR_F& color
		);

		E2D_OP_EXPLICIT operator D2D1_COLOR_F() const;

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

	public:
		float r;
		float g;
		float b;
		float a;
	};


	// 键盘键值
	enum class KeyCode : int
	{
		Unknown = 0,
		Up = 0xC8,
		Left = 0xCB,
		Right = 0xCD,
		Down = 0xD0,
		Enter = 0x1C,
		Space = 0x39,
		Esc = 0x01,
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
		Num1 = 0x02,
		Num2 = 0x03,
		Num3 = 0x04,
		Num4 = 0x05,
		Num5 = 0x06,
		Num6 = 0x07,
		Num7 = 0x08,
		Num8 = 0x09,
		Num9 = 0x0A,
		Num0 = 0x0B,
		Numpad7 = 0x47,
		Numpad8 = 0x48,
		Numpad9 = 0x49,
		Numpad4 = 0x4B,
		Numpad5 = 0x4C,
		Numpad6 = 0x4D,
		Numpad1 = 0x4F,
		Numpad2 = 0x50,
		Numpad3 = 0x51,
		Numpad0 = 0x52,
	};


	// 鼠标键值
	enum class MouseCode : int
	{
		Left,		/* 鼠标左键 */
		Right,		/* 鼠标右键 */
		Middle		/* 鼠标中键 */
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
		Function(Func func)
			: func_(func)
		{
		}

		template<typename Func, typename Object>
		Function(Func&& func, Object&& obj)
			: func_(std::bind(func, obj))
		{
		}

		void operator() (void) const;

		E2D_OP_EXPLICIT operator bool() const;

	private:
		std::function<void()> func_;
	};


	// 时间段
	class Duration
	{
	public:
		Duration();

		explicit Duration(
			float seconds
		);

		// 获取毫秒数
		int Milliseconds() const;

		// 获取秒数
		float Seconds() const;

		bool operator== (const Duration &) const;
		bool operator!= (const Duration &) const;
		bool operator> (const Duration &) const;
		bool operator>= (const Duration &) const;
		bool operator< (const Duration &) const;
		bool operator<= (const Duration &) const;

		Duration operator + (Duration const &) const;
		Duration operator - (Duration const &) const;

		Duration& operator += (Duration const &);
		Duration& operator -= (Duration const &);

	private:
		std::chrono::milliseconds duration_ms_;
	};


	// 时间点
	class Time
	{
	public:
		Time();

		// 获取时间戳
		time_t GetTimeStamp() const;

		// 是否是
		bool IsZero() const;

		Time operator + (Duration const &) const;
		Time operator - (Duration const &) const;

		Time& operator += (Duration const &);
		Time& operator -= (Duration const &);

		Duration operator - (Time const &) const;

		// 获取当前时间
		static Time Now();

	private:
		std::chrono::steady_clock::time_point time_;
	};


	// 字体
	class Font
	{
	public:
		String	family;			// 字体族
		float	size;			// 字号
		UINT	weight;			// 粗细值
		bool	italic;			// 是否斜体

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
			float size = 22,
			UINT weight = Font::Weight::Normal,
			bool italic = false
		);
	};


	// 资源
	class Resource
	{
	public:
		Resource(
			int resource_id,				/* 资源名称 */
			const String& resource_type		/* 资源类型 */
		);

	public:
		int		id;
		String	type;
	};


	// 二维转换
	class Transform
	{
	public:
		Point position;		// 坐标
		Size size;			// 大小
		float scale_x;		// 横向缩放
		float scale_y;		// 纵向缩放
		float rotation;		// 旋转
		float skew_x;		// 横向倾斜角度
		float skew_y;		// 纵向倾斜角度
		float pivot_x;		// 支点横坐标
		float pivot_y;		// 支点纵坐标

	public:
		Transform();

		E2D_OP_EXPLICIT operator D2D1::Matrix3x2F() const;

		bool operator== (const Transform& other) const;
	};


	// 引用计数对象
	class Ref
	{
	public:
		Ref();

		virtual ~Ref();

		// 增加引用计数
		LONG Retain();

		// 减少引用计数
		LONG Release();

		// 获取引用计数
		LONG GetRefCount() const;

	private:
		LONG ref_count_;
	};


	template<class Interface>
	inline void SafeRelease(Interface*& p)
	{
		if (p != nullptr)
		{
			p->Release();
			p = nullptr;
		}
	}

	// 运行时异常
	class RuntimeError
	{
	public:
		RuntimeError() E2D_NOEXCEPT
			: message_(nullptr)
		{
		}

		explicit RuntimeError(char const* const message) E2D_NOEXCEPT
			: message_(message)
		{
		}

		RuntimeError(RuntimeError const& other) E2D_NOEXCEPT
			: message_(other.message_)
		{
		}

		RuntimeError& operator=(RuntimeError const& other) E2D_NOEXCEPT
		{
			if (this == &other)
			{
				return *this;
			}

			message_ = other.message_;
			return *this;
		}

		virtual char const* Message() const
		{
			return message_ ? message_ : "Unknown runtime exception";
		}

	private:
		char const* message_;
	};


	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// 在此处设置断点以捕获系统异常.
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(hr));
			throw RuntimeError(s_str);
		}
	}

} // end of easy2d namespace


#endif // __E2D_UTIL_H__
