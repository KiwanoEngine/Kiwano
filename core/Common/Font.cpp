#include "..\enodes.h"

e2d::Font::Font()
	: m_pTextFormat(nullptr)
	, m_Color(Color::WHITE)
	, m_fFontSize(22)
	, m_FontWeight(FontWeight::REGULAR)
	, m_bItalic(false)
	, m_bRecreateNeeded(true)
{
}

e2d::Font::Font(String fontFamily, float fontSize /* = 22 */, UINT32 color /* = EColor::WHITE */, UINT32 fontWeight, bool italic /* = false */)
	: m_pTextFormat(nullptr)
	, m_Color(Color::WHITE)
	, m_fFontSize(22)
	, m_FontWeight(FontWeight::REGULAR)
	, m_bItalic(false)
	, m_bRecreateNeeded(true)
{
	this->setFamily(fontFamily);
	this->setSize(fontSize);
	this->setColor(color);
	this->setWeight(fontWeight);
	this->setItalic(italic);
}

e2d::Font::~Font()
{
	SafeReleaseInterface(&m_pTextFormat);
}

float e2d::Font::getFontSize() const
{
	return m_fFontSize;
}

UINT32 e2d::Font::getFontWeight() const
{
	return m_FontWeight;
}

UINT32 e2d::Font::getColor() const
{
	return m_Color;
}

bool e2d::Font::isItalic() const
{
	return m_bItalic;
}

void e2d::Font::setFamily(const String & fontFamily)
{
	m_sFontFamily = fontFamily;
	m_bRecreateNeeded = true;
}

void e2d::Font::setSize(float fontSize)
{
	m_fFontSize = fontSize;
	m_bRecreateNeeded = true;
}

void e2d::Font::setWeight(UINT32 fontWeight)
{
	m_FontWeight = fontWeight;
	m_bRecreateNeeded = true;
}

void e2d::Font::setColor(UINT32 color)
{
	m_Color = color;
}

void e2d::Font::setItalic(bool value)
{
	m_bItalic = value;
	m_bRecreateNeeded = true;
}

void e2d::Font::_initTextFormat()
{
	SafeReleaseInterface(&m_pTextFormat);

	HRESULT hr = Renderer::getIDWriteFactory()->CreateTextFormat(
		m_sFontFamily,
		NULL,
		DWRITE_FONT_WEIGHT(m_FontWeight),
		m_bItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_fFontSize,
		L"en-us",
		&m_pTextFormat
	);

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");
}

IDWriteTextFormat * e2d::Font::_getTextFormat()
{
	if (m_bRecreateNeeded)
	{
		_initTextFormat();
		m_bRecreateNeeded = false;
	}
	return m_pTextFormat;
}
