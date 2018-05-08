#include "..\e2dnode.h"


e2d::Text::Text()
	: _TextStyle()
	, _pDWriteTextLayout(nullptr)
	, _pDWriteTextFormat(nullptr)
{
}

e2d::Text::Text(const String& text)
	: _TextStyle()
	, _pDWriteTextLayout(nullptr)
	, _pDWriteTextFormat(nullptr)
	, _sText(text)
{
	_reset();
}

e2d::Text::Text(TextStyle textStyle)
	: _TextStyle(textStyle)
	, _pDWriteTextLayout(nullptr)
	, _pDWriteTextFormat(nullptr)
{
	_reset();
}

e2d::Text::Text(const String& text, TextStyle textStyle)
	: _TextStyle(textStyle)
	, _pDWriteTextLayout(nullptr)
	, _pDWriteTextFormat(nullptr)
	, _sText(text)
{
	_reset();
}

e2d::Text::Text(
	const String& text, 
	const String& fontFamily, 
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
	: _TextStyle(
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
	, _pDWriteTextLayout(nullptr)
	, _pDWriteTextFormat(nullptr)
	, _sText(text)
{
	_reset();
}

e2d::Text::~Text()
{
	SafeReleaseInterface(&_pDWriteTextFormat);
	SafeReleaseInterface(&_pDWriteTextLayout);
}

e2d::String e2d::Text::getText() const
{
	return _sText;
}

e2d::TextStyle e2d::Text::getTextStyle() const
{
	return _TextStyle;
}

e2d::String e2d::Text::getFontFamily() const
{
	return _TextStyle.fontFamily;
}

double e2d::Text::getFontSize() const
{
	return _TextStyle.fontSize;
}

UINT32 e2d::Text::getFontWeight() const
{
	return _TextStyle.fontWeight;
}

e2d::Color e2d::Text::getColor() const
{
	return _TextStyle.color;
}

e2d::Color e2d::Text::getOutlineColor() const
{
	return _TextStyle.outlineColor;
}

double e2d::Text::getOutlineWidth() const
{
	return _TextStyle.outlineWidth;
}

e2d::LineJoin e2d::Text::getOutlineJoin() const
{
	return _TextStyle.outlineJoin;
}

int e2d::Text::getLineCount() const
{
	if (_pDWriteTextLayout)
	{
		DWRITE_TEXT_METRICS metrics;
		_pDWriteTextLayout->GetMetrics(&metrics);
		return static_cast<int>(metrics.lineCount);
	}
	else
	{
		return 0;
	}
}

bool e2d::Text::isItalic() const
{
	return _TextStyle.italic;
}

bool e2d::Text::hasStrikethrough() const
{
	return _TextStyle.hasStrikethrough;
}

bool e2d::Text::hasUnderline() const
{
	return _TextStyle.hasUnderline;
}

bool e2d::Text::hasOutline() const
{
	return _TextStyle.hasOutline;
}

void e2d::Text::setText(const String& text)
{
	_sText = text;
	_reset();
}

void e2d::Text::setTextStyle(TextStyle textStyle)
{
	_TextStyle = textStyle;
	_reset();
}

void e2d::Text::setFontFamily(const String& fontFamily)
{
	_TextStyle.fontFamily = fontFamily;
	_reset();
}

void e2d::Text::setFontSize(double fontSize)
{
	_TextStyle.fontSize = fontSize;
	_reset();
}

void e2d::Text::setFontWeight(UINT32 fontWeight)
{
	_TextStyle.fontWeight = fontWeight;
	_reset();
}

void e2d::Text::setColor(Color color)
{
	_TextStyle.color = color;
}

void e2d::Text::setItalic(bool value)
{
	_TextStyle.italic = value;
	_reset();
}

void e2d::Text::setWrapping(bool wrapping)
{
	if (_TextStyle.wrapping != wrapping)
	{
		_TextStyle.wrapping = wrapping;
		_reset();
	}
}

void e2d::Text::setWrappingWidth(double fWrappingWidth)
{
	if (_TextStyle.wrappingWidth != fWrappingWidth)
	{
		_TextStyle.wrappingWidth = max(fWrappingWidth, 0);

		if (_TextStyle.wrapping)
		{
			_reset();
		}
	}
}

void e2d::Text::setLineSpacing(double fLineSpacing)
{
	if (_TextStyle.lineSpacing != fLineSpacing)
	{
		_TextStyle.lineSpacing = fLineSpacing;
		_reset();
	}
}

void e2d::Text::setAlignment(TextAlign align)
{
	if (_TextStyle.alignment != align)
	{
		_TextStyle.alignment = align;
		_reset();
	}
}

void e2d::Text::setUnderline(bool hasUnderline)
{
	if (_TextStyle.hasUnderline != hasUnderline)
	{
		_TextStyle.hasUnderline = hasUnderline;
		if (!_pDWriteTextFormat)
			_createFormat();
		_createLayout();
	}
}

void e2d::Text::setStrikethrough(bool hasStrikethrough)
{
	if (_TextStyle.hasStrikethrough != hasStrikethrough)
	{
		_TextStyle.hasStrikethrough = hasStrikethrough;
		if (!_pDWriteTextFormat)
			_createFormat();
		_createLayout();
	}
}

void e2d::Text::setOutline(bool hasOutline)
{
	_TextStyle.hasOutline = hasOutline;
}

void e2d::Text::setOutlineColor(Color outlineColor)
{
	_TextStyle.outlineColor = outlineColor;
}

void e2d::Text::setOutlineWidth(double outlineWidth)
{
	_TextStyle.outlineWidth = outlineWidth;
}

void e2d::Text::setOutlineJoin(LineJoin outlineJoin)
{
	_TextStyle.outlineJoin = outlineJoin;
}

void e2d::Text::onRender()
{
	if (_pDWriteTextLayout)
	{
		// 创建文本区域
		D2D1_RECT_F textLayoutRect = D2D1::RectF(0, 0, _fWidth, _fHeight);
		// 设置画刷颜色和透明度
		Renderer::getSolidColorBrush()->SetOpacity(_fDisplayOpacity);
		// 获取文本渲染器
		auto pTextRenderer = Renderer::getCustomTextRenderer();
		pTextRenderer->SetTextStyle(
			_TextStyle.color.toColorF(),
			_TextStyle.hasOutline,
			_TextStyle.outlineColor.toColorF(),
			static_cast<FLOAT>(_TextStyle.outlineWidth),
			D2D1_LINE_JOIN(_TextStyle.outlineJoin)
		);
		_pDWriteTextLayout->Draw(NULL, pTextRenderer, 0, 0);
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
	SafeReleaseInterface(&_pDWriteTextFormat);

	HRESULT hr = Renderer::getIDWriteFactory()->CreateTextFormat(
		_TextStyle.fontFamily,
		NULL,
		DWRITE_FONT_WEIGHT(_TextStyle.fontWeight),
		_TextStyle.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		static_cast<float>(_TextStyle.fontSize),
		L"",
		&_pDWriteTextFormat
	);

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");

	if (_pDWriteTextFormat)
	{
		// 设置文字对齐方式
		_pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(_TextStyle.alignment));
		// 设置行间距
		if (_TextStyle.lineSpacing == 0.0)
		{
			_pDWriteTextFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0, 0);
		}
		else
		{
			_pDWriteTextFormat->SetLineSpacing(
				DWRITE_LINE_SPACING_METHOD_UNIFORM,
				static_cast<FLOAT>(_TextStyle.lineSpacing),
				static_cast<FLOAT>(_TextStyle.lineSpacing) * 0.8f
			);
		}
		// 打开文本自动换行时，设置换行属性
		if (_TextStyle.wrapping)
		{
			_pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
		}
		else
		{
			_pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
	}
}

void e2d::Text::_createLayout()
{
	SafeReleaseInterface(&_pDWriteTextLayout);

	// 文本为空字符串时，重置属性
	if (_sText.isEmpty())
	{
		this->setSize(0, 0);
		return;
	}

	if (_pDWriteTextFormat == nullptr)
	{
		WARN_IF(true, "Text::_createLayout failed! _pDWriteTextFormat NULL pointer exception.");
		return;
	}
	
	UINT32 length = static_cast<UINT32>(_sText.getLength());

	// 创建 TextLayout
	HRESULT hr;
	// 对文本自动换行情况下进行处理
	if (_TextStyle.wrapping)
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(
			_sText,
			length,
			_pDWriteTextFormat,
			static_cast<FLOAT>(_TextStyle.wrappingWidth),
			0,
			&_pDWriteTextLayout
		);
		if (_pDWriteTextLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			_pDWriteTextLayout->GetMetrics(&metrics);
			// 重设文本宽高
			this->setSize(metrics.layoutWidth, metrics.height);
		}
	}
	else
	{
		hr = Renderer::getIDWriteFactory()->CreateTextLayout(_sText, length, _pDWriteTextFormat, 0, 0, &_pDWriteTextLayout);
		// 为防止文本对齐问题，根据刚才创建的 layout 宽度重新创建它
		if (_pDWriteTextLayout)
		{
			// 获取文本布局的宽度和高度
			DWRITE_TEXT_METRICS metrics;
			_pDWriteTextLayout->GetMetrics(&metrics);
			// 重设文本宽高
			this->setSize(metrics.width, metrics.height);
			// 重新创建 layout
			SafeReleaseInterface(&_pDWriteTextLayout);
			hr = Renderer::getIDWriteFactory()->CreateTextLayout(_sText, length, _pDWriteTextFormat, _fWidth, 0, &_pDWriteTextLayout);
		}
	}

	ASSERT(SUCCEEDED(hr), "Create IDWriteTextFormat Failed!");

	// 添加下划线和删除线
	DWRITE_TEXT_RANGE range = { 0, length };
	if (_TextStyle.hasUnderline)
	{
		_pDWriteTextLayout->SetUnderline(true, range);
	}
	if (_TextStyle.hasStrikethrough)
	{
		_pDWriteTextLayout->SetStrikethrough(true, range);
	}
}
