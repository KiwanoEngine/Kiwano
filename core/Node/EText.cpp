#include "..\enodes.h"
#include "..\Win\winbase.h"

e2d::EText::EText()
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setFont(new EFont());
}

e2d::EText::EText(const EString & text)
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setText(text);
	this->setFont(new EFont());
}

e2d::EText::EText(EFont * font)
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setFont(font);
}

e2d::EText::EText(const EString & text, EFont * font)
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setText(text);
	this->setFont(font);
}

e2d::EText::EText(const EString & text, EString fontFamily, float fontSize, UINT32 color, UINT32 fontWeight, bool italic)
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setText(text);
	this->setFont(new EFont(fontFamily, fontSize, color, fontWeight, italic));
}

e2d::EText::~EText()
{
	SafeRelease(&m_pFont);
}

e2d::EString e2d::EText::getText() const
{
	return m_sText;
}

float e2d::EText::getWidth() const
{
	return m_fWordWrappingWidth * m_fScaleX;
}

float e2d::EText::getRealWidth() const
{
	return m_fWordWrappingWidth;
}

e2d::EFont * e2d::EText::getFont() const
{
	return m_pFont;
}

void e2d::EText::setText(const EString & text)
{
	m_sText = text;
	_initTextLayout();
}

void e2d::EText::setFont(EFont * font)
{
	if (font)
	{
		SafeRelease(&m_pFont);
		m_pFont = font;
		font->retain();

		_initTextLayout();
	}
}

void e2d::EText::setWordWrapping(bool value)
{
	m_bWordWrapping = value;
	_initTextLayout();
}

void e2d::EText::setWordWrappingWidth(float wordWrapWidth)
{
	m_fWordWrappingWidth = max(wordWrapWidth, 0);
	_initTextLayout();
}

void e2d::EText::_render()
{
	GetSolidColorBrush()->SetColor(D2D1::ColorF(m_pFont->m_Color, m_fDisplayOpacity));
	GetRenderTarget()->DrawTextW(
		m_sText,
		UINT32(m_sText.length()),
		m_pFont->_getTextFormat(),
		D2D1::RectF(
			0,
			0,
			m_bWordWrapping ? m_fWordWrappingWidth : m_Size.width,
			getRealHeight()
		),
		GetSolidColorBrush()
	);
}

void e2d::EText::_initTextLayout()
{
	// 未设置字体或空字符串时，文本宽高为 0
	if (!m_pFont || m_sText.isEmpty())
	{
		this->_setHeight(0);
		m_fWordWrappingWidth = 0;
		return;
	}

	// 未打开文本自动换行时，设置 TextFormat 属性为不换行
	if (!m_bWordWrapping)
	{
		m_pFont->_getTextFormat()->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	}
	else
	{
		m_pFont->_getTextFormat()->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
	}
	
	// 获取 TextLayout
	IDWriteTextLayout * pDWriteTextLayout = nullptr;

	HRESULT hr = GetDirectWriteFactory()->CreateTextLayout(
		m_sText,
		UINT32(m_sText.length()),
		m_pFont->_getTextFormat(),
		m_bWordWrapping ? m_fWordWrappingWidth : 0,
		0,
		&pDWriteTextLayout
	);

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");

	// 获取文本布局的宽度和高度
	DWRITE_TEXT_METRICS metrics;
	pDWriteTextLayout->GetMetrics(&metrics);

	this->_setSize(metrics.widthIncludingTrailingWhitespace, metrics.height);
	m_fWordWrappingWidth = metrics.widthIncludingTrailingWhitespace;

	// 删除相关资源
	SafeReleaseInterface(&pDWriteTextLayout);
}
