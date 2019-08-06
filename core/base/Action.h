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
#include "base.h"
#include "time.h"
#include "RefCounter.h"

namespace easy2d
{
	class Node;
	class Loop;
	class Sequence;
	class Spawn;

	// ��������
	class Action
		: public RefCounter
	{
		friend class Loop;
		friend class Sequence;
		friend class Spawn;

		E2D_DISABLE_COPY(Action);

	public:
		Action();

		virtual ~Action();

		// ��ȡ��������״̬
		virtual bool IsRunning();

		// ��������
		virtual void Resume();

		// ��ͣ����
		virtual void Pause();

		// ֹͣ����
		virtual void Stop();

		// ��ȡ��������
		virtual const String& GetName() const;

		// ���ö�������
		virtual void SetName(
			const String& name
		);

		// ��ȡ�����Ŀ���
		virtual Action * Clone() const = 0;

		// ��ȡ�����ĵ�ת
		virtual Action * Reverse() const = 0;

		// ���ö���
		virtual void Reset();

		// ��ȡ�ö�����ִ��Ŀ��
		virtual Node * GetTarget();

		// ��ʼ����
		virtual void StartWithTarget(
			Node* target
		);

		// ��ʼ������
		virtual void Initialize();

		// ���¶���
		virtual void Update();

		// ���ö���ʱ��
		virtual void ResetTime();

		// ��ȡ��������״̬
		virtual bool IsDone() const;

	protected:
		String			name_;
		bool			running_;
		bool			done_;
		bool			initialized_;
		Node*			target_;
		time::TimePoint	started_;
	};
}
