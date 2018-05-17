#include "..\e2dcommon.h"

static const UINT sc_redShift = 16;
static const UINT sc_greenShift = 8;
static const UINT sc_blueShift = 0;

static const UINT sc_redMask = 0xff << sc_redShift;
static const UINT sc_greenMask = 0xff << sc_greenShift;
static const UINT sc_blueMask = 0xff << sc_blueShift;

e2d::Color::Color()
	: r(0)
	, g(0)
	, b(0)
	, a(1)
{
}

e2d::Color::Color(double r, double g, double b)
	: r(float(r))
	, g(float(g))
	, b(float(b))
	, a(float(1))
{
}

e2d::Color::Color(double r, double g, double b, double alpha)
	: r(float(r))
	, g(float(g))
	, b(float(b))
	, a(float(alpha))
{
}

e2d::Color::Color(UINT rgb)
{
	_init(rgb, 1);
}

e2d::Color::Color(UINT rgb, double alpha)
{
	_init(rgb, alpha);
}

void e2d::Color::_init(UINT rgb, double alpha)
{
	r = float((rgb & sc_redMask) >> sc_redShift) / 255.f;
	g = float((rgb & sc_greenMask) >> sc_greenShift) / 255.f;
	b = float((rgb & sc_blueMask) >> sc_blueShift) / 255.f;
	a = float(alpha);
}

D2D1_COLOR_F e2d::Color::toD2DColorF() const
{
	return D2D1::ColorF(r, g, b, a);
}
