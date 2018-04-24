#include "..\e2dnode.h"


e2d::Text::Text()
	: m_TextStyle()
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
{
}

e2d::Text::Text(String text)
	: m_TextStyle()
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
	, m_sText(text)
{
	_reset();
}

e2d::Text::Text(TextStyle textStyle)
	: m_TextStyle(textStyle)
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
{
	_reset();
}

e2d::Text::Text(String text, TextStyle textStyle)
	: m_TextStyle(textStyle)
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
	, m_sText(text)
{
	_reset();
}

e2d::Text::Text(
	String text, 
	String fontFamily, 
	double fontSize, 
	UINT32 color, 
	UINT32 fontWeight, 
	bool italic, 
	TextAlign alignment,
	bool wrapping,
	double wrappingWidth,
	double lineSpacing,
	bool hasUnderline, 
	bool hasStrikethrough,
	bool hasOutline,
	UINT32 outlineColor,
	UINT32 outlineWidth
)
	: m_TextStyle(
		fontFamily, 
		fontSize, 
		color, 
		fontWeight, 
		italic, 
		alignment,
		wrapping,
		wrappingWidth,
		lineSpacing,
		hasUnderline, 
		hasStrikethrough, 
		hasOutline, 
		outlineColor,
		outlineWidth
	)
	, m_pDWriteTextLayout(nullptr)
	, m_pDWriteTextFormat(nullptr)
	, m_sText(text)
{
	_reset();
}

e2d::Text::~Text()
{
	SafeReleaseInterface(&m_pDWriteTextFormat);
	SafeReleaseInterface(&m_pDWriteTextLayout);
}

e2d::String e2d::Text::getText() const
{
	return m_sText;
}

e2d::TextStyle e2d::Text::getTextStyle() const
{
	return m_TextStyle;
}

e2d::String e2d::Text::getFontFamily() const
{
	return m_TextStyle.fontFamily;
}

double e2d::Text::getFontSize() const
{
	return m_TextStyle.fontSize;
}

UINT32 e2d::Text::getFontWeight() const
{
	return m_TextStyle.fontWeight;
}

e2d::Color e2d::Text::getColor() const
{
	return m_TextStyle.color;
}

e2d::Color e2d::Text::getOutlineColor() const
{
	return m_TextStyle.outlineColor;
}

double e2d::Text::getOutlineWidth() const
{
	return m_TextStyle.outlineWidth;
}

e2d::LineJoin e2d::Text::getOutlineJoin() const
{
	return m_TextStyle.outlineJoin;
}

int e2d::Text::getLineCount() const
{
	if (m_pDWriteTextLayout)
	{
		DWRITE_TEXT_METRICS metrics;
		m_pDWriteTextLayout->GetMetrics(&metrics);
		return static_cast<int>(metrics.lineCount);
	}
	else
	{
		return 0;
	}
}

bool e2d::Text::isItalic() const
{
	return m_TextStyle.italic;
}

bool e2d::Text::hasStrikethrough() const
{
	return m_TextStyle.hasStrikethrough;
}

bool e2d::Text::hasUnderline() const
{
	return m_TextStyle.hasUnderline;
}

bool e2d::Text::hasOutline() const
{
	return m_TextStyle.hasOutline;
}

void e2d::Text::setText(String text)
{
	m_sText = text;
	_reset();
}

void e2d::Text::setTextStyle(TextStyle textStyle)
{
	m_TextStyle = textStyle;
	_reset();
}

void e2d::Text::setFontFamily(String fontFamily)
{
	m_TextStyle.fontFamily = fontFamily;
	_reset();
}

void e2d::Text::setFontSize(double fontSize)
{
	m_TextStyle.fontSize = fontSize;
	_reset();
}

void e2d::Text::setFontWeight(UINT32 fontWeight)
{
	m_TextStyle.fontWeight = fontWeight;
	_reset();
}

void e2d::Text::setColor(Color color)
{
	m_TextStyle.color = color;
}

void e2d::Text::setItalic(bool value)
{
	m_TextStyle.italic = value;
	_reset();
}

void e2d::Text::setWrapping(bool wrapping)
{
	if (m_TextStyle.wrapping != wrapping)
	{
		m_TextStyle.wrapping = wrapping;
		_reset();
	}
}

void e2d::Text::setWrappingWidth(double fWrappingWidth)
{
	if (m_TextStyle.wrappingWidth != fWrappingWidth)
	{
		m_TextStyle.wrappingWidth = max(fWrappingWidth, 0);

		if (m_TextStyle.wrapping)
		{
			_reset();
		}
	}
}

void e2d::Text::setLineSpacing(double fLineSpacing)
{
	if (m_TextStyle.lineSpacing != fLineSpacing)
	{
		m_TextStyle.lineSpacing = fLineSpacing;
		_reset();
	}
}

void e2d::Text::setAlignment(TextAlign align)
{
	if (m_TextStyle.alignment != align)
	{
		m_TextStyle.alignment = align;
		_reset();
	}
}

void e2d::Text::setUnderline(bool hasUnderline)
{
	if (m_TextStyle.hasUnderline != hasUnderline)
	{
		m_TextStyle.hasUnderline = hasUnderline;
		if (!m_pDWriteTextFormat)
			_createFormat();
		_createLayout();
	}
}

void e2d::Text::setStrikethrough(bool hasStrikethrough)
{
	if (m_TextStyle.hasStrikethrough != hasStrikethrough)
	{
		m_TextStyle.hasStrikethrough = hasStrikethrough;
		if (!m_pDWriteTextFormat)
			_createFormat();
		_createLayout();
	}
}

void e2d::Text::setOutline(bool hasOutline)
{
	m_TextStyle.hasOutline = hasOutline;
}

void e2d::Text::setOutlineColor(Color outlineColor)
{
	m_TextStyle.outlineColor = outlineColor;
}

void e2d::Text::setOutlineWidth(double outlineWidth)
{
	m_TextStyle.outlineWidth = outlineWidth;
}

void e2d::Text::setOutlineJoin(LineJoin outlineJoin)
{
	m_TextStyle.outlineJoin = outlineJoin;
}

void e2d::Text::onRender()
{
	if (m_pDWriteTextLayout)
	{
		// 创建文本区域
		D2D1_RECT_F textLayoutRect = D2D1::RectF(0, 0, m_fWidth, m_fHeight);
		// 设置画刷颜色和透明度
		Renderer::getSolidColorBrush()->SetOpacity(m_fDisplayOpacity);
		// 获取文本渲染器
		auto pTextRenderer = Renderer::getCustomTextRenderer();
		pTextRenderer->SetTextStyle(
			m_TextStyle.color.toColorF(),
			m_TextStyle.hasOutline,
			m_TextStyle.outlineColor.toColorF(),
			static_cast<FLOAT>(m_TextStyle.outlineWidth),
			D2D1_LINE_JOIN(m_TextStyle.outlineJoin)
		);
		m_pDWriteTextLayout->Draw(NULL, pTextRenderer, 0, 0);
	}
}

void e2d::Text::_reset()
{
	// 创建文字格式化
	_createFormat();
	// 创建文字布局
	_createLayout();
}

void e2d::Text::_createFormat()
{
	SafeReleaseInterface(&m_pDWriteTextFormat);

	HRESULT hr = Renderer::getIDWriteFactory()->CreateTextFormat(
		m_TextStyle.fontFamily,
		NULL,
		DWRITE_FONT_WEIGHT(m_TextStyle.fontWeight),
		m_TextStyle.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		static_cast<float>(m_TextStyle.fontSize),
		L"",
		&m_pDWriteTextFormat
	);

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");

	if (m_pDWriteTextFormat)
	{
		// 设置文字对齐方式
		m_pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(m_TextStyle.alignment));
		// 设置行间距
		if (m_TextStyle.lineSpacing == 0.0)
		{
			m_pDWriteTextFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
		}
		else
		{
			m_pDWriteTextFormat->SetLineSpacing(
				DWRITE_LINE_SPACING_METHOD_UNIFORM,
				static_cast<FLOAT>(m_TextStyle.lineSpacing),
				static_cast<FLOAT>(m_TextStyle.lineSpacing) * 0.8f
			);
		}
		// 打开文本自动换行时，设置换行属性
		if (m_TextStyle.wrapping)
		{
			m_pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		}
		else
		{
			m_pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
	}
}

void e2d::Text::_createLayout()
{
	SafeReleaseInterface(&m_pDWriteTextLayout);

	// 文本为空字符串时，重置属性
	if (m_sText.isEmpty())
	{
		this->setSize(0, 0);
		return;
	}

	if (m_pDWriteTextFormat == nullptr)
	{
		WARN_IF(true, "Text::_createLayout failed! m_pDWriteTextFormat NULL pointer exception.");
		return;
	}
	
	UINT32 length = static_cast<UINT32>(m_sText.getLength());

	// 创建 TextLayout
	HRESULT hr;
	// 对文本自动换行情况下进行处理
	if (m_TextStyle.wrapping)
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(
			m_sText,
			length,
			m_pDWriteTextFormat,
			static_cast<FLOAT>(m_TextStyle.wrappingWidth),
			0,
			&m_pDWriteTextLayout
		);
		if (m_pDWriteTextLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			m_pDWriteTextLayout->GetMetrics(&metrics);
			// 重设文本宽高
			this->setSize(metrics.layoutWidth, metrics.height);
		}
	}
	else
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(m_sText, length, m_pDWriteTextFormat, 0, 0, &m_pDWriteTextLayout);
		// 为防止文本对齐问题，根据刚才创建的 layout 宽度重新创建它
		if (m_pDWriteTextLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			m_pDWriteTextLayout->GetMetrics(&metrics);
			// 重设文本宽高
			this->setSize(metrics.width, metrics.height);
			// 重新创建 layout
			SafeReleaseInterface(&m_pDWriteTextLayout);
			hr = Renderer::getIDWriteFactory()->CreateTextLayout(m_sText, length, m_pDWriteTextFormat, m_fWidth, 0, &m_pDWriteTextLayout);
		}
	}

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");

	// 添加下划线和删除线
	DWRITE_TEXT_RANGE range = { 0, length };
	if (m_TextStyle.hasUnderline)
	{
		m_pDWriteTextLayout->SetUnderline(true, range);
	}
	if (m_TextStyle.hasStrikethrough)
	{
		m_pDWriteTextLayout->SetStrikethrough(true, range);
	}
}
