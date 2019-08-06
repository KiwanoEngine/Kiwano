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
#include <functional>

namespace easy2d
{
    // ��ʱ����
	class Task
		: public RefCounter
	{
		friend class Node;

		typedef std::function<void()> Callback;

	public:
		explicit Task(
			const Callback& func,		/* ִ�к��� */
			const String& name = L""	/* �������� */
		);

		explicit Task(
			Callback const& func,		/* ִ�к��� */
			Duration const& delay,		/* ʱ�������룩 */
			int times = -1,				/* ִ�д������� -1 Ϊ����ִ�У� */
			const String& name = L""	/* �������� */
		);

		// ��������
		void Start();

		// ֹͣ����
		void Stop();

		// �����Ƿ�����ִ��
		bool IsRunning() const;

		// ��ȡ��������
		const String& GetName() const;

	protected:
		bool IsReady() const;

		void Update();

		void ResetTime();

	private:
		bool		running_;
		bool		stopped_;
		int			run_times_;
		int			total_times_;
		String		name_;
		Duration	delay_;
		TimePoint	last_time_;
		Callback	callback_;
	};
}
