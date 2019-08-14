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

#include "AsyncTask.h"
#include "../platform/Application.h"

namespace kiwano
{
	AsyncTask::AsyncTask()
		: thread_(bind_func(this, &AsyncTask::TaskThread))
	{
	}

	AsyncTask::AsyncTask(AsyncTaskFunc func)
		: AsyncTask()
	{
		Then(func);
	}

	AsyncTask::~AsyncTask()
	{
	}

	void AsyncTask::Start()
	{
		thread_.detach();

		// retain this object until finished
		Retain();
	}

	AsyncTask& AsyncTask::Then(AsyncTaskFunc func)
	{
		std::lock_guard<std::mutex> lock(func_mutex_);
		thread_func_queue_.push(func);
		return (*this);
	}

	AsyncTask& AsyncTask::SetCallback(AsyncTaskCallback callback)
	{
		thread_cb_ = callback;
		return (*this);
	}

	void AsyncTask::TaskThread()
	{
		while (!thread_func_queue_.empty())
		{
			auto& func = thread_func_queue_.front();
			if (func)
			{
				func();
			}

			func_mutex_.lock();
			thread_func_queue_.pop();
			func_mutex_.unlock();
		}

		Application::PreformInMainThread(bind_func(this, &AsyncTask::Complete));
	}

	void AsyncTask::Complete()
	{
		if (thread_cb_)
		{
			thread_cb_();
		}

		// Release this object
		Release();
	}

}
