// Copyright (c) 2016-2018 Kiwano - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include "FontCollection.h"

namespace kiwano
{
	// �����ϸֵ
	enum FontWeight : UInt32
	{
		Thin		= 100,
		ExtraLight	= 200,
		Light		= 300,
		Normal		= 400,
		Medium		= 500,
		Bold		= 700,
		ExtraBold	= 800,
		Black		= 900,
		ExtraBlack	= 950
	};

	// ����
	class Font
	{
	public:
		String			family;		// ������
		Float32			size;		// �ֺ�
		UInt32			weight;		// ��ϸֵ
		bool			italic;		// �Ƿ�б��
		FontCollection	collection;	// ���弯

	public:
		Font(
			const String& family		= L"",
			Float32 size				= 18,
			UInt32 weight				= FontWeight::Normal,
			bool italic					= false,
			FontCollection collection	= FontCollection()
		);
	};
}
