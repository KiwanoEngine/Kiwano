#include "..\e2dcommon.h"

e2d::Font::Font()
	: family("")
	, size(22)
	, weight(Font::Weight::NORMAL)
	, italic(false)
{
}

e2d::Font::Font(const String & family, double size, UINT weight, bool italic)
	: family(family)
	, size(size)
	, weight(weight)
	, italic(italic)
{
}