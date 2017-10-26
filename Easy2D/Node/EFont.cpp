#include "..\enodes.h"
#include "..\Win\winbase.h"

e2d::EFont::EFont()
	: m_pTextFormat(nullptr)
	, m_Color(EColor::WHITE)
	, m_fFontSize(22)
	, m_FontWeight(EFontWeight::REGULAR)
	, m_bItalic(false)
	, m_bRecreateNeeded(true)
{
}

e2d::EFont::EFont(EString fontFamily, float fontSize /* = 22 */, UINT32 color /* = EColor::WHITE */, UINT32 fontWeight, bool italic /* = false */)
{
	this->setFamily(fontFamily);
	this->setSize(fontSize);
	this->setColor(color);
	this->setWeight(fontWeight);
	this->setItalic(italic);
}

e2d::EFont::~EFont()
{
	SafeReleaseInterface(&m_pTextFormat);
}

float e2d::EFont::getFontSize() const
{
	return m_fFontSize;
}

UINT32 e2d::EFont::getFontWeight() const
{
	return m_FontWeight;
}

UINT32 e2d::EFont::getColor() const
{
	return m_Color;
}

bool e2d::EFont::isItalic() const
{
	return m_bItalic;
}

void e2d::EFont::setFamily(EString fontFamily)
{
	m_sFontFamily = fontFamily;
	m_bRecreateNeeded = true;
}

void e2d::EFont::setSize(float fontSize)
{
	m_fFontSize = fontSize;
	m_bRecreateNeeded = true;
}

void e2d::EFont::setWeight(UINT32 fontWeight)
{
	m_FontWeight = fontWeight;
	m_bRecreateNeeded = true;
}

void e2d::EFont::setColor(UINT32 color)
{
	m_Color = color;
}

void e2d::EFont::setItalic(bool value)
{
	m_bItalic = value;
	m_bRecreateNeeded = true;
}

void e2d::EFont::_initTextFormat()
{
	SafeReleaseInterface(&m_pTextFormat);

	HRESULT hr = GetDirectWriteFactory()->CreateTextFormat(
		m_sFontFamily.c_str(),
		NULL,                          // Font collection(NULL sets it to the system font collection)
		DWRITE_FONT_WEIGHT(m_FontWeight),
		m_bItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_fFontSize,
		L"en-us",                      // Local
		&m_pTextFormat
	);

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");
}

IDWriteTextFormat * e2d::EFont::_getTextFormat()
{
	if (m_bRecreateNeeded)
	{
		_initTextFormat();
		m_bRecreateNeeded = false;
	}
	return m_pTextFormat;
}
