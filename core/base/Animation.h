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
#include "Action.h"
#include "Image.h"

namespace easy2d
{
	// ֡����
	class Animation
		: public RefCounter
	{
		E2D_DISABLE_COPY(Animation);

	public:
		typedef std::vector<Image*> Images;

		Animation();

		explicit Animation(
			const Images& frames	/* �ؼ�֡���� */
		);

		explicit Animation(
			float interval			/* ֡������룩 */
		);

		explicit Animation(
			float interval,			/* ֡������룩 */
			const Images& frames	/* �ؼ�֡���� */
		);

		virtual ~Animation();

		// ��ӹؼ�֡
		void Add(
			Image * frame			/* �ؼ�֡ */
		);

		// ��Ӷ���ؼ�֡
		void Add(
			const Images& frames	/* �ؼ�֡���� */
		);

		// ��ȡ֡���
		float GetInterval() const;

		// ��ȡ�ؼ�֡
		const Images& GetFrames() const;

		// ����ÿһ֡��ʱ����
		void SetInterval(
			float interval		/* ֡������룩 */
		);

		// ��ȡ֡�����Ŀ�������
		Animation * Clone() const;

		// ��ȡ֡�����ĵ�ת
		Animation * Reverse() const;

	protected:
		float	interval_;
		Images	frames_;
	};


	// ���鶯��
	class Animate
		: public Action
	{
		E2D_DISABLE_COPY(Animate);

	public:
		Animate();

		explicit Animate(
			Animation * animation
		);

		virtual ~Animate();

		// ��ȡ����
		virtual Animation * GetAnimation() const;

		// ���ö���
		virtual void SetAnimation(
			Animation * animation
		);

		// ��ȡ�ö����Ŀ�������
		virtual Animate * Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual Animate * Reverse() const override;

		// ���ö���
		virtual void Reset() override;

	protected:
		// ��ʼ������
		virtual void Initialize() override;

		// ���¶���
		virtual void Update() override;

		// ���ö���ʱ��
		virtual void ResetTime() override;

	protected:
		UINT frame_index_;
		Animation * animation_;
	};
}
