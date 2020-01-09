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
#include <thread>
#include <mutex>

#include <kiwano/core/ObjectBase.h>

namespace kiwano
{
	KGE_DECLARE_SMART_PTR(AsyncTask);

	typedef Function<void()> AsyncTaskFunc;
	typedef Function<void()> AsyncTaskCallback;

	/// \~chinese
	/// @brief �첽����
	/// @details �ڶ��߳���ִ�����񲢷���
	///   @code
	///     AsyncTaskPtr task = new AsyncTask;
	///     task->Then(DoSomething);
	///     task->Start();
	///   @endcode
	class AsyncTask
		: public virtual ObjectBase
	{
	public:
		/// \~chinese
		/// @brief �����첽����
		AsyncTask();

		/// \~chinese
		/// @brief �����첽����
		/// @param func �첽�ص�����
		AsyncTask(
			AsyncTaskFunc func
		);

		virtual ~AsyncTask();

		/// \~chinese
		/// @brief ����첽������
		AsyncTask& Then(
			AsyncTaskFunc func
		);

		/// \~chinese
		/// @brief ��������ִ����ɺ�Ļص�����
		/// @note �ú����� Kiwano ���߳���ִ��
		AsyncTask& SetCallback(
			AsyncTaskCallback callback
		);

		/// \~chinese
		/// @brief �����첽����
		void Start();

	private:
		void TaskThread();

		void Complete();

	private:
		std::thread thread_;
		std::mutex func_mutex_;
		Queue<AsyncTaskFunc> thread_func_queue_;
		AsyncTaskCallback thread_cb_;
	};
}
