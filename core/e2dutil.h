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

	class Size;

	// 坐标
	//
	// Usage:
	//     表示一个二维空间的坐标: Point origin(0, 0);
	//     计算两点间距离: float distance = Point::Distance(p1, p2);
	//     坐标可以相加减: Point p = Point(10, 10) + Point(20, 20);  // p 的坐标是 (30, 30)
	//
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
	//
	// Usage:
	//     表示一个二维矩形区域的大小: Size s(10, 5);  // 宽为 10, 高为 5
	//     大小可以相加减: Size s = Size(10, 10) + Size(20, 20);  // s 的大小是宽高均为 30
	//
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
	//
	// Usage:
	//     表示一个二维矩形: Rect rect = Rect(10, 20, 30, 40);  // 左上角坐标为 (10, 20), 宽为 30, 高为 40
	//     矩形可以通过 Point + Size 定义, Point 表示矩形左上角坐标, Size 表示矩形宽高
	//     判断一个点是否在矩形内: bool contains = rect.ContainsPoint(p);
	//     判断两矩形是否相交: bool intersects = rect1.Intersects(rect2);
	//
	class Rect
	{
	public:
		Point origin;	// 左上角坐标
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


	// 颜色
	//
	// Usage:
	//     使用枚举表示颜色: Color blue = Color::Blue;
	//     使用 RGB 表示一个颜色: Color red(1.0f, 0.0f, 0.0f);
	//     使用 RGBA 表示一个带透明度的颜色: Color not_black(1.0f, 1.0f, 1.0f, 0.5f);
	//     使用一个 UINT 类型的值表示 RGB: Color black(0x000000);
	//
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
			Black			= 0x000000,
			Blue			= 0x0000FF,
			BlueViolet		= 0x8A2BE2,
			Brown			= 0xA52A2A,
			Chocolate		= 0xD2691E,
			DarkBlue		= 0x00008B,
			DarkGray		= 0xA9A9A9,
			DarkGreen		= 0x006400,
			DarkOrange		= 0xFF8C00,
			DarkRed			= 0x8B0000,
			DarkViolet		= 0x9400D3,
			ForestGreen		= 0x228B22,
			Gold			= 0xFFD700,
			Gray			= 0x808080,
			Green			= 0x008000,
			GreenYellow		= 0xADFF2F,
			LightBlue		= 0xADD8E6,
			LightCyan		= 0xE0FFFF,
			LightGreen		= 0x90EE90,
			LightGray		= 0xD3D3D3,
			LightPink		= 0xFFB6C1,
			LightSeaGreen	= 0x20B2AA,
			LightSkyBlue	= 0x87CEFA,
			LightYellow		= 0xFFFFE0,
			Orange			= 0xFFA500,
			OrangeRed		= 0xFF4500,
			Pink			= 0xFFC0CB,
			Purple			= 0x800080,
			Red				= 0xFF0000,
			Silver			= 0xC0C0C0,
			SkyBlue			= 0x87CEEB,
			Snow			= 0xFFFAFA,
			Violet			= 0xEE82EE,
			Wheat			= 0xF5DEB3,
			White			= 0xFFFFFF,
			WhiteSmoke		= 0xF5F5F5,
			Wood			= 0xDEB887,
			Yellow			= 0xFFFF00,
			YellowGreen		= 0x9ACD32
		};

	public:
		float r;
		float g;
		float b;
		float a;
	};


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


	// 键盘键值
	enum class KeyCode : int
	{
		Unknown	= 0,
		Up		= 0xC8,
		Left	= 0xCB,
		Right	= 0xCD,
		Down	= 0xD0,
		Enter	= 0x1C,
		Space	= 0x39,
		Esc		= 0x01,
		Q		= 0x10,
		W		= 0x11,
		E		= 0x12,
		R		= 0x13,
		T		= 0x14,
		Y		= 0x15,
		U		= 0x16,
		I		= 0x17,
		O		= 0x18,
		P		= 0x19,
		A		= 0x1E,
		S		= 0x1F,
		D		= 0x20,
		F		= 0x21,
		G		= 0x22,
		H		= 0x23,
		J		= 0x24,
		K		= 0x25,
		L		= 0x26,
		Z		= 0x2C,
		X		= 0x2D,
		C		= 0x2E,
		V		= 0x2F,
		B		= 0x30,
		N		= 0x31,
		M		= 0x32,
		Num1	= 0x02,
		Num2	= 0x03,
		Num3	= 0x04,
		Num4	= 0x05,
		Num5	= 0x06,
		Num6	= 0x07,
		Num7	= 0x08,
		Num8	= 0x09,
		Num9	= 0x0A,
		Num0	= 0x0B,
		Numpad7	= 0x47,
		Numpad8	= 0x48,
		Numpad9	= 0x49,
		Numpad4	= 0x4B,
		Numpad5	= 0x4C,
		Numpad6	= 0x4D,
		Numpad1	= 0x4F,
		Numpad2	= 0x50,
		Numpad3	= 0x51,
		Numpad0	= 0x52,
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
	//
	// Usage:
	//     5 秒: Duration::Second * 5
	//     1.5 小时: Duration::Hour * 1.5
	//     3 小时 45 分 15 秒: Duration::Hour * 3 + Duration::Minute * 45 + Duration::Second * 15
	//
	class Duration
	{
	public:
		static const Duration Millisecond;	// 毫秒
		static const Duration Second;		// 秒
		static const Duration Minute;		// 分钟
		static const Duration Hour;			// 小时

	public:
		Duration();

		explicit Duration(
			int milliseconds
		);

		// 获取毫秒数
		int Milliseconds() const;

		// 获取秒数
		float Seconds() const;

		// 获取分钟数
		float Minutes() const;

		// 获取小时数
		float Hours() const;

		bool operator== (const Duration &) const;
		bool operator!= (const Duration &) const;
		bool operator> (const Duration &) const;
		bool operator>= (const Duration &) const;
		bool operator< (const Duration &) const;
		bool operator<= (const Duration &) const;

		Duration operator + (Duration const &) const;
		Duration operator - (Duration const &) const;
		Duration operator * (int) const;
		Duration operator * (float) const;
		Duration operator * (double) const;
		Duration operator / (int) const;
		Duration operator / (float) const;
		Duration operator / (double) const;

		Duration& operator += (Duration const &);
		Duration& operator -= (Duration const &);
		Duration& operator *= (int);
		Duration& operator *= (float);
		Duration& operator *= (double);
		Duration& operator /= (int);
		Duration& operator /= (float);
		Duration& operator /= (double);

		friend Duration operator* (int, const Duration &);
		friend Duration operator* (float, const Duration &);
		friend Duration operator* (double, const Duration &);
		friend Duration operator/ (int, const Duration &);
		friend Duration operator/ (float, const Duration &);
		friend Duration operator/ (double, const Duration &);

	private:
		int milliseconds_;
	};


	// 时间点
	//
	// Usage:
	//     使用 Time::Now 方法获取当前时间: Time now = Time::Now();
	//     两时间相减, 得到的结果是一个 Duration 对象, 例如:
	//     Time t1 = Time::Now();
	//     ...  // 做些什么
	//     Time t2 = Time::Now();
	//     然后获取两时间相差的毫秒数:
	//     int ms = (t2 - t1).Milliseconds();
	// 
	class Time
	{
	public:
		Time();

		// 获取时间戳
		time_t GetTimeStamp() const;

		// 是否是零时
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
		std::wstring	family;		// 字体族
		float			size;		// 字号
		UINT			weight;		// 粗细值
		bool			italic;		// 是否斜体

	public:
		// 字体粗细值
		enum Weight : UINT
		{
			Thin		= 100,
			ExtraLight	= 200,
			Light		= 300,
			Normal		= 400,
			Medium		= 500,
			Bold		= 700,
			ExtraBold	= 800,
			Black		= 900,
			ExtraBlack	= 950
		};

	public:
		explicit Font(
			const std::wstring& family = L"",
			float size	= 22,
			UINT weight	= Font::Weight::Normal,
			bool italic	= false
		);
	};


	// 资源
	// 
	// Usage:
	//     Resource 用于获取可执行文件 (exe) 中的资源, 必须在构造函数中指定它的
	//     资源类型和名称标识符。
	//     例如, 一份音频资源的类型为 L"WAVE", 名称标识符为 IDR_WAVE_1, 那么可以这样指定该资源:
	//     Resource res(MAKEINTRESOURCE(IDR_WAVE_1), L"WAVE");
	//     如果需要手动加载这份资源, 可以通过 Load 方法获取资源内容
	//     if (res.Load()) {
	//         LPVOID data = res.GetData();
	//         DWORD size = res.GetDataSize();
	//     }
	//     了解资源的更多信息: https://docs.microsoft.com/en-us/windows/desktop/menurc/resources
	//
	class Resource
	{
	public:
		Resource(
			LPCWSTR name,	/* 资源名称 */
			LPCWSTR type	/* 资源类型 */
		);

		bool Load();

		LPCWSTR	GetName() const;

		LPCWSTR	GetType() const;

		LPVOID GetData() const;

		DWORD GetDataSize() const;

		size_t GetHashCode() const;

	private:
		bool	loaded_;
		LPCWSTR	name_;
		LPCWSTR	type_;
		LPVOID	data_;
		DWORD	data_size_;
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


	// 随机数产生器
	// 
	// Usage:
	//     使用静态方法 Range 获取指定范围内的一个随机数, 如:
	//     int n = Random::Range(1, 5);  // 获取 1~6 内的随机整数, 包含 1 和 6
	//     方法同样适用于浮点数的生成, 如:
	//     double d = Random::Range(1.2, 1.5);
	//     注意, 随机数的类型取决于参数的类型。
	//
	class Random
	{
	public:
		// 取得范围内的一个整型随机数
		template<typename T>
		static inline T Range(T min, T max)
		{
			return easy2d::Random::RandomInt(min, max);
		}

		// 取得范围内的一个浮点数随机数
		static inline float Range(float min, float max)
		{
			return easy2d::Random::RandomReal(min, max);
		}

		// 取得范围内的一个浮点数随机数
		static inline double Range(double min, double max)
		{
			return easy2d::Random::RandomReal(min, max);
		}

	private:
		template<typename T>
		static T RandomInt(T min, T max)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(Random::GetEngine());
		}

		template<typename T>
		static T RandomReal(T min, T max)
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(Random::GetEngine());
		}

		static std::default_random_engine &GetEngine();
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


	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// 在此处设置断点以捕获系统异常.
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(hr));
			throw std::runtime_error(s_str);
		}
	}

} // end of easy2d namespace


#endif // __E2D_UTIL_H__
