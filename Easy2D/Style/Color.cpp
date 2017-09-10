#include "..\Easy2d.h"
#include "..\EasyX\easyx.h"

// 常用颜色值的定义
const COLORREF Color::black = BLACK;
const COLORREF Color::blue = BLUE;
const COLORREF Color::green = GREEN;
const COLORREF Color::cyan = CYAN;
const COLORREF Color::red = RED;
const COLORREF Color::magenta = MAGENTA;
const COLORREF Color::brown = BROWN;
const COLORREF Color::lightgray = LIGHTGRAY;
const COLORREF Color::darkgray = DARKGRAY;
const COLORREF Color::lightblue = LIGHTBLUE;
const COLORREF Color::lightgreen = LIGHTGREEN;
const COLORREF Color::lightcyan = LIGHTCYAN;
const COLORREF Color::lightred = LIGHTRED;
const COLORREF Color::lightmagenta = LIGHTMAGENTA;
const COLORREF Color::yellow = YELLOW;
const COLORREF Color::white = WHITE;


COLORREF Color::getFromRGB(BYTE r, BYTE g, BYTE b)
{
	return RGB(r, g, b);	// 从 (r, g, b) 三色值转化为颜色
}

COLORREF Color::getFromHSL(float H, float S, float L)
{
	return HSLtoRGB(H, S, L);
}

COLORREF Color::getFromHSV(float H, float S, float V)
{
	return HSVtoRGB(H, S, V);
}

BYTE Color::getRValue(COLORREF color)
{
	return GetRValue(color);	// 返回颜色中的红色值
}

BYTE Color::getGValue(COLORREF color)
{
	return GetGValue(color);	// 返回颜色中的绿色值
}

BYTE Color::getBValue(COLORREF color)
{
	return GetBValue(color);	// 返回颜色中的蓝色值
}

COLORREF Color::getGray(COLORREF color)
{
	return RGBtoGRAY(color);	// 获取颜色中的灰度值
}
