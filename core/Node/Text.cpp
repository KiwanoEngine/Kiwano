#include "..\enodes.h"

e2d::Text::Text()
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setFont(new Font());
}

e2d::Text::Text(const String & text)
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setText(text);
	this->setFont(new Font());
}

e2d::Text::Text(Font * font)
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setFont(font);
}

e2d::Text::Text(const String & text, Font * font)
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setText(text);
	this->setFont(font);
}

e2d::Text::Text(const String & text, String fontFamily, float fontSize, UINT32 color, UINT32 fontWeight, bool italic)
	: m_bWordWrapping(false)
	, m_pFont(nullptr)
	, m_fWordWrappingWidth(0)
{
	this->setText(text);
	this->setFont(new Font(fontFamily, fontSize, color, fontWeight, italic));
}

e2d::Text::~Text()
{
	SafeRelease(&m_pFont);
}

e2d::String e2d::Text::getText() const
{
	return m_sText;
}

float e2d::Text::getWidth() const
{
	return m_fWordWrappingWidth * m_fScaleX;
}

float e2d::Text::getRealWidth() const
{
	return m_fWordWrappingWidth;
}

e2d::Font * e2d::Text::getFont() const
{
	return m_pFont;
}

void e2d::Text::setText(const String & text)
{
	m_sText = text;
	_initTextLayout();
}

void e2d::Text::setFont(Font * font)
{
	if (font)
	{
		SafeRelease(&m_pFont);
		m_pFont = font;
		font->retain();

		_initTextLayout();
	}
}

void e2d::Text::setWordWrappingEnable(bool value)
{
	m_bWordWrapping = value;
	_initTextLayout();
}

void e2d::Text::setWordWrappingWidth(float wordWrapWidth)
{
	m_fWordWrappingWidth = max(wordWrapWidth, 0);
	_initTextLayout();
}

void e2d::Text::onRender()
{
	Renderer::getSolidColorBrush()->SetColor(D2D1::ColorF(m_pFont->m_Color, m_fDisplayOpacity));
	Renderer::getRenderTarget()->DrawTextW(
		m_sText,
		static_cast<UINT32>(m_sText.getLength()),
		m_pFont->_getTextFormat(),
		D2D1::RectF(
			0,
			0,
			m_bWordWrapping ? m_fWordWrappingWidth : m_Size.width,
			getRealHeight()
		),
		Renderer::getSolidColorBrush()
	);
}

void e2d::Text::_initTextLayout()
{
	// 未设置字体或空字符串时，文本宽高为 0
	if (!m_pFont || m_sText.isEmpty())
	{
		this->_setSize(0, 0);
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

	HRESULT hr = Renderer::getIDWriteFactory()->CreateTextLayout(
		m_sText,
		static_cast<UINT32>(m_sText.getLength()),
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
