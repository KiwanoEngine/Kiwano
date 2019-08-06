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
#include "Action.hpp"

namespace easy2d
{
	// ֡����
	class Animation
		: public RefCounter
	{
		using Images = std::vector< spImage >;

	public:
		Animation();

		explicit Animation(
			const Images& frames		/* �ؼ�֡���� */
		);

		explicit Animation(
			Duration const& interval	/* ֡������룩 */
		);

		explicit Animation(
			Duration const& interval,	/* ֡������룩 */
			const Images& frames		/* �ؼ�֡���� */
		);

		virtual ~Animation();

		// ��ӹؼ�֡
		void Add(
			spImage const& frame	/* �ؼ�֡ */
		);

		// ��Ӷ���ؼ�֡
		void Add(
			const Images& frames	/* �ؼ�֡���� */
		);

		// ��ȡ֡���
		Duration const& GetInterval() const;

		// ��ȡ�ؼ�֡
		Images const& GetFrames() const;

		// ����ÿһ֡��ʱ����
		void SetInterval(
			Duration const& interval	/* ֡������룩 */
		);

		// ��ȡ֡�����Ŀ�������
		spAnimation Clone() const;

		// ��ȡ֡�����ĵ�ת
		spAnimation Reverse() const;

	protected:
		Duration	interval_;
		Images		frames_;
	};


	// ���鶯��
	class Animate
		: public Action
	{
	public:
		Animate();

		explicit Animate(
			spAnimation const& animation
		);

		virtual ~Animate();

		// ��ȡ����
		spAnimation GetAnimation() const;

		// ���ö���
		void SetAnimation(
			spAnimation const& animation
		);

		// ��ȡ�ö����Ŀ�������
		virtual spAction Clone() const override;

		// ��ȡ�ö����ĵ�ת
		virtual spAction Reverse() const override;

		// ���ö���
		virtual void Reset() override;

	protected:
		// ��ʼ������
		virtual void Init(Node* target) override;

		// ���¶���
		virtual void Update(Node* target, Duration const& dt) override;

	protected:
		size_t frame_index_;
		Duration delta_;
		spAnimation animation_;
	};
}
