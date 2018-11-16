// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining lhs copy
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
#include <utility>

namespace easy2d
{
	namespace intrusive
	{
		template <typename T>
		class SmartPointer
		{
			T* ptr_{ nullptr };

		public:
			using Type = T;

			SmartPointer() {}

			SmartPointer(nullptr_t) {}

			SmartPointer(Type* p) : ptr_(p)
			{
				IntrusivePtrAddRef(ptr_);
			}

			SmartPointer(const SmartPointer& other) : ptr_(other.ptr_)
			{
				IntrusivePtrAddRef(ptr_);
			}

			template <typename U>
			SmartPointer(const SmartPointer<U>& other) : ptr_(other.Get())
			{
				IntrusivePtrAddRef(ptr_);
			}

			SmartPointer(SmartPointer&& other)
			{
				ptr_ = other.ptr_;
				other.ptr_ = nullptr;
			}

			~SmartPointer()
			{
				IntrusivePtrRelease(ptr_);
			}

			inline Type* Get() const { return ptr_; }

			inline void Swap(SmartPointer& other)
			{
				std::swap(ptr_, other.ptr_);
			}

			inline Type* operator ->() const
			{
				return ptr_;
			}

			inline Type& operator *() const
			{
				return *ptr_;
			}

			inline operator bool() const { return ptr_ != nullptr; }

			inline bool operator !() const { return ptr_ == 0; }

			inline SmartPointer& operator =(const SmartPointer& other)
			{
				SmartPointer(other).Swap(*this);
				return *this;
			}

			inline SmartPointer& operator =(SmartPointer&& other)
			{
				IntrusivePtrRelease(ptr_);
				ptr_ = other.ptr_;
				other.ptr_ = nullptr;
				return *this;
			}

			inline SmartPointer& operator =(Type* p)
			{
				SmartPointer(p).Swap(*this);
				return *this;
			}

			inline SmartPointer& operator =(nullptr_t)
			{
				SmartPointer{}.Swap(*this);
				return *this;
			}
		};

		template<class T, class U>
		inline bool operator==(SmartPointer<T> const& lhs, SmartPointer<U> const& rhs)
		{
			return lhs.Get() == rhs.Get();
		}

		template<class T, class U>
		inline bool operator!=(SmartPointer<T> const& lhs, SmartPointer<U> const& rhs)
		{
			return lhs.Get() != rhs.Get();
		}

		template<class T, class U>
		inline bool operator<(SmartPointer<T> const& lhs, SmartPointer<U> const& rhs)
		{
			return lhs.Get() < rhs.Get();
		}

		template<class T>
		inline bool operator==(SmartPointer<T> const& lhs, T* rhs)
		{
			return lhs.Get() == rhs;
		}

		template<class T>
		inline bool operator!=(SmartPointer<T> const& lhs, T* rhs)
		{
			return lhs.Get() != rhs;
		}

		template<class T>
		inline bool operator==(T* lhs, SmartPointer<T> const& rhs)
		{
			return lhs == rhs.Get();
		}

		template<class T>
		inline bool operator!=(T* lhs, SmartPointer<T> const& rhs)
		{
			return lhs != rhs.Get();
		}

		template<class T>
		inline bool operator==(SmartPointer<T> const& lhs, nullptr_t)
		{
			return !static_cast<bool>(lhs);
		}

		template<class T>
		inline bool operator!=(SmartPointer<T> const& lhs, nullptr_t)
		{
			return static_cast<bool>(lhs);
		}

		template<class T>
		inline bool operator==(nullptr_t, SmartPointer<T> const& rhs)
		{
			return !static_cast<bool>(rhs);
		}

		template<class T>
		inline bool operator!=(nullptr_t, SmartPointer<T> const& rhs)
		{
			return static_cast<bool>(rhs);
		}

		template<class T>
		inline SmartPointer<T> make_intrusive(T* ptr)
		{
			return SmartPointer<T>(ptr);
		}

		// template class cannot support std::swap,
		// so implement a swap function in easy2d namespace
		template<class T>
		inline void swap(SmartPointer<T>& lhs, SmartPointer<T>& rhs)
		{
			lhs.Swap(rhs);
		}
	}
}
