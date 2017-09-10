#include "..\Easy2d.h"

// 常用字体粗细值的定义
const LONG FontWeight::dontcare = 0;
const LONG FontWeight::thin = 100;
const LONG FontWeight::extraLight = 200;
const LONG FontWeight::light = 300;
const LONG FontWeight::normal = 400;
const LONG FontWeight::regular = 400;
const LONG FontWeight::medium = 500;
const LONG FontWeight::demiBlod = 600;
const LONG FontWeight::blod = 700;
const LONG FontWeight::extraBold = 800;
const LONG FontWeight::black = 900;
const LONG FontWeight::heavy = 900;

// 默认字体
static const FontStyle s_defaultFont(_T(""), 18, FontWeight::normal);


FontStyle::FontStyle()
{
	m_font = s_defaultFont.m_font;
}

FontStyle::FontStyle(LPCTSTR fontfamily, LONG height, LONG weight, LONG width, bool italic, bool underline, bool strikeout, LONG escapement, LONG orientation, bool quality)
{
	setFontFamily(fontfamily);
	m_font.lfWeight = weight;
	m_font.lfHeight = height;
	m_font.lfWidth = width;
	m_font.lfItalic = italic;
	m_font.lfUnderline = underline;
	m_font.lfStrikeOut = strikeout;
	m_font.lfEscapement = escapement;
	m_font.lfOrientation = orientation;
	setQuality(quality);
}

FontStyle::~FontStyle()
{
}

FontStyle * FontStyle::getDefault()
{
	return new FontStyle(s_defaultFont);
}

void FontStyle::setHeight(LONG value)
{
	m_font.lfHeight = value;
}

void FontStyle::setWidth(LONG value)
{
	m_font.lfWidth = value;
}

void FontStyle::setFontFamily(LPCTSTR value)
{
	_tcscpy_s(m_font.lfFaceName, 32, value);
}

void FontStyle::setEscapement(LONG value)
{
	m_font.lfEscapement = value;
}

void FontStyle::setOrientation(LONG value)
{
	m_font.lfOrientation = value;
}

void FontStyle::setQuality(bool value)
{
	m_font.lfQuality = value ? ANTIALIASED_QUALITY : DEFAULT_QUALITY;
}

void FontStyle::setWeight(LONG value)
{
	m_font.lfWeight = value;
}

void FontStyle::setItalic(bool value)
{
	m_font.lfItalic = value;
}

void FontStyle::setUnderline(bool value)
{
	m_font.lfUnderline = value;
}

void FontStyle::setStrikeOut(bool value)
{
	m_font.lfStrikeOut = value;
}