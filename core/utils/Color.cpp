#include "..\e2dutil.h"

static const UINT kRedShift = 16;
static const UINT kGreenShift = 8;
static const UINT kBlueShift = 0;

static const UINT kRedMask = 0xff << kRedShift;
static const UINT kGreenMask = 0xff << kGreenShift;
static const UINT kBlueMask = 0xff << kBlueShift;

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
	: r(((rgb & kRedMask) >> kRedShift) / 255.f)
	, g(((rgb & kGreenMask) >> kGreenShift) / 255.f)
	, b(((rgb & kBlueMask) >> kBlueShift) / 255.f)
	, a(1.f)
{
}

e2d::Color::Color(UINT rgb, float alpha)
	: r(((rgb & kRedMask) >> kRedShift) / 255.f)
	, g(((rgb & kGreenMask) >> kGreenShift) / 255.f)
	, b(((rgb & kBlueMask) >> kBlueShift) / 255.f)
	, a(alpha)
{
}

e2d::Color::Color(const D2D1_COLOR_F& color)
	: r(color.r)
	, g(color.g)
	, b(color.b)
	, a(color.a)
{
}

e2d::Color::operator D2D1_COLOR_F() const
{
	D2D1::ColorF color_f(r, g, b, a);
	return std::move(color_f);
}
