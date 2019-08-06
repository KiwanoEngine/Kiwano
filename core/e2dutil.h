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

	// ����
	//
	// Usage:
	//     ��ʾһ����ά�ռ������: Point origin(0, 0);
	//     ������������: float distance = Point::Distance(p1, p2);
	//     ���������Ӽ�: Point p = Point(10, 10) + Point(20, 20);  // p �������� (30, 30)
	//
	class Point
	{
	public:
		float x;	// X ����
		float y;	// Y ����

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

		// �ж���������
		static float Distance(
			const Point& p1,
			const Point& p2
		);
	};


	// ��С
	//
	// Usage:
	//     ��ʾһ����ά��������Ĵ�С: Size s(10, 5);  // ��Ϊ 10, ��Ϊ 5
	//     ��С������Ӽ�: Size s = Size(10, 10) + Size(20, 20);  // s �Ĵ�С�ǿ�߾�Ϊ 30
	//
	class Size
	{
	public:
		float width;	// ���
		float height;	// �߶�

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


	// ����
	//
	// Usage:
	//     ��ʾһ����ά����: Rect rect = Rect(10, 20, 30, 40);  // ���Ͻ�����Ϊ (10, 20), ��Ϊ 30, ��Ϊ 40
	//     ���ο���ͨ�� Point + Size ����, Point ��ʾ�������Ͻ�����, Size ��ʾ���ο��
	//     �ж�һ�����Ƿ��ھ�����: bool contains = rect.ContainsPoint(p);
	//     �ж��������Ƿ��ཻ: bool intersects = rect1.Intersects(rect2);
	//
	class Rect
	{
	public:
		Point origin;	// ���Ͻ�����
		Size  size;		// ��Ⱥ͸߶�

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

		// �жϵ��Ƿ��ھ�����
		bool ContainsPoint(
			const Point& point
		) const;

		// �ж��������Ƿ��ཻ
		bool Intersects(
			const Rect& rect
		) const;
	};


	// ��ɫ
	//
	// Usage:
	//     ʹ��ö�ٱ�ʾ��ɫ: Color blue = Color::Blue;
	//     ʹ�� RGB ��ʾһ����ɫ: Color red(1.0f, 0.0f, 0.0f);
	//     ʹ�� RGBA ��ʾһ����͸���ȵ���ɫ: Color not_black(1.0f, 1.0f, 1.0f, 0.5f);
	//     ʹ��һ�� UINT ���͵�ֵ��ʾ RGB: Color black(0x000000);
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


	// ����
	enum class Direction : int
	{
		Up,			/* �� */
		Down,		/* �� */
		Left,		/* �� */
		Right		/* �� */
	};


	// �����ཻ��ʽ
	enum class Stroke : int
	{
		Miter = 0,	/* б�� */
		Bevel = 1,	/* б�� */
		Round = 2	/* Բ�� */
	};


	// ���̼�ֵ
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


	// ����ֵ
	enum class MouseCode : int
	{
		Left,		/* ������ */
		Right,		/* ����Ҽ� */
		Middle		/* ����м� */
	};


	// ��������
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


	// ʱ���
	//
	// Usage:
	//     5 ��: Duration::Second * 5
	//     1.5 Сʱ: Duration::Hour * 1.5
	//     3 Сʱ 45 �� 15 ��: Duration::Hour * 3 + Duration::Minute * 45 + Duration::Second * 15
	//
	class Duration
	{
	public:
		static const Duration Millisecond;	// ����
		static const Duration Second;		// ��
		static const Duration Minute;		// ����
		static const Duration Hour;			// Сʱ

	public:
		Duration();

		explicit Duration(
			int milliseconds
		);

		// ��ȡ������
		int Milliseconds() const;

		// ��ȡ����
		float Seconds() const;

		// ��ȡ������
		float Minutes() const;

		// ��ȡСʱ��
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


	// ʱ���
	//
	// Usage:
	//     ʹ�� Time::Now ������ȡ��ǰʱ��: Time now = Time::Now();
	//     ��ʱ�����, �õ��Ľ����һ�� Duration ����, ����:
	//     Time t1 = Time::Now();
	//     ...  // ��Щʲô
	//     Time t2 = Time::Now();
	//     Ȼ���ȡ��ʱ�����ĺ�����:
	//     int ms = (t2 - t1).Milliseconds();
	// 
	class Time
	{
	public:
		Time();

		// ��ȡʱ���
		time_t GetTimeStamp() const;

		// �Ƿ�����ʱ
		bool IsZero() const;

		Time operator + (Duration const &) const;
		Time operator - (Duration const &) const;

		Time& operator += (Duration const &);
		Time& operator -= (Duration const &);

		Duration operator - (Time const &) const;

		// ��ȡ��ǰʱ��
		static Time Now();

	private:
		std::chrono::steady_clock::time_point time_;
	};


	// ����
	class Font
	{
	public:
		std::wstring	family;		// ������
		float			size;		// �ֺ�
		UINT			weight;		// ��ϸֵ
		bool			italic;		// �Ƿ�б��

	public:
		// �����ϸֵ
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


	// ��Դ
	// 
	// Usage:
	//     Resource ���ڻ�ȡ��ִ���ļ� (exe) �е���Դ, �����ڹ��캯����ָ������
	//     ��Դ���ͺ����Ʊ�ʶ����
	//     ����, һ����Ƶ��Դ������Ϊ L"WAVE", ���Ʊ�ʶ��Ϊ IDR_WAVE_1, ��ô��������ָ������Դ:
	//     Resource res(MAKEINTRESOURCE(IDR_WAVE_1), L"WAVE");
	//     �����Ҫ�ֶ����������Դ, ����ͨ�� Load ������ȡ��Դ����
	//     if (res.Load()) {
	//         LPVOID data = res.GetData();
	//         DWORD size = res.GetDataSize();
	//     }
	//     �˽���Դ�ĸ�����Ϣ: https://docs.microsoft.com/en-us/windows/desktop/menurc/resources
	//
	class Resource
	{
	public:
		Resource(
			LPCWSTR name,	/* ��Դ���� */
			LPCWSTR type	/* ��Դ���� */
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


	// ��άת��
	class Transform
	{
	public:
		Point position;		// ����
		Size size;			// ��С
		float scale_x;		// ��������
		float scale_y;		// ��������
		float rotation;		// ��ת
		float skew_x;		// ������б�Ƕ�
		float skew_y;		// ������б�Ƕ�
		float pivot_x;		// ֧�������
		float pivot_y;		// ֧��������

	public:
		Transform();

		E2D_OP_EXPLICIT operator D2D1::Matrix3x2F() const;

		bool operator== (const Transform& other) const;
	};


	// �����������
	// 
	// Usage:
	//     ʹ�þ�̬���� Range ��ȡָ����Χ�ڵ�һ�������, ��:
	//     int n = Random::Range(1, 5);  // ��ȡ 1~6 �ڵ��������, ���� 1 �� 6
	//     ����ͬ�������ڸ�����������, ��:
	//     double d = Random::Range(1.2, 1.5);
	//     ע��, �����������ȡ���ڲ��������͡�
	//
	class Random
	{
	public:
		// ȡ�÷�Χ�ڵ�һ�����������
		template<typename T>
		static inline T Range(T min, T max)
		{
			return easy2d::Random::RandomInt(min, max);
		}

		// ȡ�÷�Χ�ڵ�һ�������������
		static inline float Range(float min, float max)
		{
			return easy2d::Random::RandomReal(min, max);
		}

		// ȡ�÷�Χ�ڵ�һ�������������
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


	// ���ü�������
	class Ref
	{
	public:
		Ref();

		virtual ~Ref();

		// �������ü���
		LONG Retain();

		// �������ü���
		LONG Release();

		// ��ȡ���ü���
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
			// �ڴ˴����öϵ��Բ���ϵͳ�쳣.
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(hr));
			throw std::runtime_error(s_str);
		}
	}

} // end of easy2d namespace


#endif // __E2D_UTIL_H__
