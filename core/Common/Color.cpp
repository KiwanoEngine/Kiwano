#include "..\e2dcommon.h"

static const UINT32 sc_redShift = 16;
static const UINT32 sc_greenShift = 8;
static const UINT32 sc_blueShift = 0;

static const UINT32 sc_redMask = 0xff << sc_redShift;
static const UINT32 sc_greenMask = 0xff << sc_greenShift;
static const UINT32 sc_blueMask = 0xff << sc_blueShift;

e2d::Color::Color()
	: r(0)
	, g(0)
	, b(0)
	, a(1)
{
}

e2d::Color::Color(double r, double g, double b)
	: r(static_cast<float>(r))
	, g(static_cast<float>(g))
	, b(static_cast<float>(b))
	, a(static_cast<float>(1))
{
}

e2d::Color::Color(double r, double g, double b, double alpha)
	: r(static_cast<float>(r))
	, g(static_cast<float>(g))
	, b(static_cast<float>(b))
	, a(static_cast<float>(alpha))
{
}

e2d::Color::Color(UINT32 rgb)
{
	_init(rgb, 1);
}

e2d::Color::Color(UINT32 rgb, double alpha)
{
	_init(rgb, alpha);
}

void e2d::Color::_init(UINT32 rgb, double alpha)
{
	r = static_cast<float>((rgb & sc_redMask) >> sc_redShift) / 255.f;
	g = static_cast<float>((rgb & sc_greenMask) >> sc_greenShift) / 255.f;
	b = static_cast<float>((rgb & sc_blueMask) >> sc_blueShift) / 255.f;
	a = static_cast<float>(alpha);
}

D2D1_COLOR_F e2d::Color::toColorF() const
{
	return D2D1::ColorF(r, g, b, a);
}
