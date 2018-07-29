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
	, a(1.f)
{
}

e2d::Color::Color(float r, float g, float b)
	: r(r)
	, g(g)
	, b(b)
	, a(1.f)
{
}

e2d::Color::Color(float r, float g, float b, float alpha)
	: r(r)
	, g(g)
	, b(b)
	, a(alpha)
{
}

e2d::Color::Color(UINT rgb)
{
	_init(rgb, 1);
}

e2d::Color::Color(UINT rgb, float alpha)
{
	_init(rgb, alpha);
}

e2d::Color::operator D2D1_COLOR_F() const
{
	return std::move(D2D1::ColorF(r, g, b, a));
}

void e2d::Color::_init(UINT rgb, float alpha)
{
	r = ((rgb & sc_redMask) >> sc_redShift) / 255.f;
	g = ((rgb & sc_greenMask) >> sc_greenShift) / 255.f;
	b = ((rgb & sc_blueMask) >> sc_blueShift) / 255.f;
	a = alpha;
}
