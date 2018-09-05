#include "..\e2dutil.h"

e2d::Font::Font()
	: family("")
	, size(22)
	, weight(Font::Weight::Normal)
	, italic(false)
{
}

e2d::Font::Font(const String & family, float size, UINT weight, bool italic)
	: family(family)
	, size(size)
	, weight(weight)
	, italic(italic)
{
}