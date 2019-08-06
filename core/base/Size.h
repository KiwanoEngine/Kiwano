// Copyright (c) 2016-2018 Easy2D - Nomango
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

namespace easy2d
{
    // ��С
	//
	// Usage:
	//     ��ʾһ����ά��������Ĵ�С: Size s(10, 5);  // ��Ϊ 10, ��Ϊ 5
	//     ��С������Ӽ�: Size s = Size(10, 10) + Size(20, 20);  // s �Ĵ�С�ǿ�߾�Ϊ 30
	//
	class Size
	{
	public:
		float width;	// ���
		float height;	// �߶�

	public:
		Size();

		Size(
			float width,
			float height
		);

		Size(
			const Size& other
		);

		Size operator + (const Size & other) const;
		Size operator - (const Size & other) const;
		Size operator * (float val) const;
		Size operator / (float val) const;
		Size operator - () const;
		bool operator== (const Size& other) const;
	};
}
