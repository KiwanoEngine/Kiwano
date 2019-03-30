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
#include "Object.h"
#include "Component.h"
#include "../common/Singleton.hpp"
#include <functional>
#include <mutex>

namespace easy2d
{
	E2D_DECLARE_SMART_PTR(AsyncTask);

	typedef std::function<void()> AsyncTaskFunc;
	typedef std::function<void()> AsyncTaskCallback;

	class AsyncTask
		: public Object
	{
	public:
		AsyncTask();

		AsyncTask(
			AsyncTaskFunc func
		);

		AsyncTask& Then(
			AsyncTaskFunc func
		);

		AsyncTask& SetCallback(
			AsyncTaskCallback callback
		);

		void Start();

	protected:
		void TaskThread();

		void Complete();

	protected:
		Queue<AsyncTaskFunc> thread_func_queue_;
		AsyncTaskCallback thread_cb_;
		std::mutex func_mutex_;
	};

	class AsyncTaskThread
		: public Singleton<AsyncTaskThread>
		, public Component
	{
		E2D_DECLARE_SINGLETON(AsyncTaskThread);

	public:
		virtual void SetupComponent(Application*);

		virtual void DestroyComponent();

		void PerformTaskCallback(std::function<void()> func);

	private:
		Application* app_;
	};
}
