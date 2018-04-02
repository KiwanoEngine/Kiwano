#include "..\enode.h"

e2d::Font::Font() 
	: fontFamily("")
	, size(22)
	, color(Color::WHITE)
	, weight(FontWeight::NORMAL)
	, italic(false)
	, underline(false)
	, strikethrough(false)
{}

e2d::Font::Font(String fontFamily, double fontSize, UINT32 color, UINT32 fontWeight, bool italic, bool hasUnderline, bool hasStrikethrough)
	: fontFamily(fontFamily)
	, size(fontSize)
	, color(color)
	, weight(fontWeight)
	, italic(italic)
	, underline(hasUnderline)
	, strikethrough(hasStrikethrough)
{}
