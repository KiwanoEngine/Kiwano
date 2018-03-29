#include "..\enodes.h"

e2d::Text::Text()
	: m_bWrappingEnable(false)
	, m_pFont(nullptr)
	, m_fWrappingWidth(0)
	, m_bHasUnderline(false)
	, m_bHasStrikethrough(false)
	, m_pDWriteTextLayout(nullptr)
{
	this->setFont(new Font());
}

e2d::Text::Text(String text)
	: m_bWrappingEnable(false)
	, m_pFont(nullptr)
	, m_fWrappingWidth(0)
	, m_bHasUnderline(false)
	, m_bHasStrikethrough(false)
	, m_pDWriteTextLayout(nullptr)
{
	this->setText(text);
	this->setFont(new Font());
}

e2d::Text::Text(Font * font)
	: m_bWrappingEnable(false)
	, m_pFont(nullptr)
	, m_fWrappingWidth(0)
	, m_bHasUnderline(false)
	, m_bHasStrikethrough(false)
	, m_pDWriteTextLayout(nullptr)
{
	this->setFont(font);
}

e2d::Text::Text(String text, Font * font)
	: m_bWrappingEnable(false)
	, m_pFont(nullptr)
	, m_fWrappingWidth(0)
	, m_bHasUnderline(false)
	, m_bHasStrikethrough(false)
	, m_pDWriteTextLayout(nullptr)
{
	this->setText(text);
	this->setFont(font);
}

e2d::Text::Text(String text, String fontFamily, double fontSize, UINT32 color, UINT32 fontWeight, bool italic)
	: m_bWrappingEnable(false)
	, m_pFont(nullptr)
	, m_fWrappingWidth(0)
	, m_bHasUnderline(false)
	, m_bHasStrikethrough(false)
	, m_pDWriteTextLayout(nullptr)
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

e2d::Font * e2d::Text::getFont() const
{
	return m_pFont;
}

void e2d::Text::setText(String text)
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

void e2d::Text::setWrappingWidth(double wordWrapWidth)
{
	m_fWrappingWidth = max(static_cast<float>(wordWrapWidth), 0);
	m_bWrappingEnable = (abs(m_fWrappingWidth) >= 1e-7);
	_initTextLayout();
}

void e2d::Text::setLineSpacing(double fLineSpacing)
{
	if (m_pFont)
	{
		if (fLineSpacing == 0.0f)
		{
			m_pFont->getDWriteTextFormat()->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
		}
		else
		{
			m_pFont->getDWriteTextFormat()->SetLineSpacing(
				DWRITE_LINE_SPACING_METHOD_UNIFORM,
				static_cast<float>(fLineSpacing),
				static_cast<float>(fLineSpacing) * 0.8f
			);
		}
		
		_initTextLayout();
	}
}

void e2d::Text::setAlignment(UINT32 nAlign)
{
	if (m_pFont)
	{
		m_pFont->getDWriteTextFormat()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(nAlign));
		_initTextLayout();
	}
}

void e2d::Text::setUnderline(bool hasUnderline)
{
	if (m_bHasUnderline != hasUnderline)
	{
		m_bHasUnderline = hasUnderline;
		_initTextLayout();
	}
}

void e2d::Text::setStrikethrough(bool hasStrikethrough)
{
	if (m_bHasStrikethrough != hasStrikethrough)
	{
		m_bHasStrikethrough = hasStrikethrough;
		_initTextLayout();
	}
}

void e2d::Text::onRender()
{
	// 创建文本区域
	D2D1_RECT_F textLayoutRect = D2D1::RectF(0, 0, m_bWrappingEnable ? m_fWrappingWidth : m_fWidth, m_fHeight);
	// 设置画刷颜色和透明度
	Renderer::getSolidColorBrush()->SetColor(D2D1::ColorF(m_pFont->m_Color, m_fDisplayOpacity));
	// 渲染文字内容
	if (m_pDWriteTextLayout)
	{
		Renderer::getRenderTarget()->DrawTextLayout(
			{ 0, 0 },
			m_pDWriteTextLayout,
			Renderer::getSolidColorBrush()
		);
	}
}

void e2d::Text::_initTextLayout()
{
	// 未设置字体或空字符串时，文本宽高为 0
	if (!m_pFont || m_sText.isEmpty())
	{
		this->setSize(0, 0);
		return;
	}

	// 未打开文本自动换行时，设置 TextFormat 属性为不换行
	if (!m_bWrappingEnable)
	{
		m_pFont->getDWriteTextFormat()->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	}
	else
	{
		m_pFont->getDWriteTextFormat()->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
	}
	
	// 获取 TextLayout
	SafeReleaseInterface(&m_pDWriteTextLayout);
	UINT32 length = static_cast<UINT32>(m_sText.getLength());

	HRESULT hr = Renderer::getIDWriteFactory()->CreateTextLayout(
		m_sText,
		length,
		m_pFont->getDWriteTextFormat(),
		m_bWrappingEnable ? m_fWrappingWidth : 0,
		0,
		&m_pDWriteTextLayout
	);

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");

	// 添加下划线和删除线
	if (m_bHasUnderline)
	{
		m_pDWriteTextLayout->SetUnderline(true, { 0, length });
	}
	if (m_bHasStrikethrough)
	{
		m_pDWriteTextLayout->SetStrikethrough(true, { 0, length });
	}

	// 获取文本布局的宽度和高度
	DWRITE_TEXT_METRICS metrics;
	m_pDWriteTextLayout->GetMetrics(&metrics);

	this->setSize(metrics.widthIncludingTrailingWhitespace, metrics.height);
	m_fWrappingWidth = metrics.widthIncludingTrailingWhitespace;
}
