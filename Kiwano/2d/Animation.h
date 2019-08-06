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
#include "ActionTween.h"

namespace kiwano
{
	// ֡����
	class KGE_API Animation
		: public ActionTween
	{
	public:
		Animation();

		Animation(
			Duration duration,			/* ����ʱ�� */
			FramesPtr frames,			/* ����֡ */
			EaseFunc func = nullptr		/* �ٶȱ仯 */
		);

		virtual ~Animation();

		// ��ȡ����
		FramesPtr GetFrames() const;

		// ���ö���
		void SetFrames(
			FramesPtr frames
		);

		// ��ȡ�ö����Ŀ�������
		ActionPtr Clone() const override;

		// ��ȡ�ö����ĵ�ת
		ActionPtr Reverse() const override;

	protected:
		void Init(NodePtr target) override;

		void UpdateTween(NodePtr target, float percent) override;

	protected:
		FramesPtr frames_;
	};
}
