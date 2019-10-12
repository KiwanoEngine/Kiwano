// Copyright (c) 2018-2019 Kiwano - Nomango
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
#include <typeinfo>
#include <type_traits>

namespace kiwano
{

inline namespace core
{

class any
{
public:
	any() : storage_{}
	{
	}

	template <
		typename _Ty,
		typename _Decayed = typename std::decay<_Ty>::type,
		typename = typename std::enable_if<std::is_copy_constructible<_Decayed>::value>::type
	>
	any(_Ty&& val) : storage_{}
	{
		emplace<_Decayed>(std::forward<_Ty&&>(val));
	}

	template <
		typename _Ty,
		typename _Decayed = typename std::decay<_Ty>::type,
		typename... _Args
	>
	any(_Args&&... args) : storage_{}
	{
		emplace<_Decayed>(std::forward<_Args>(args)...);
	}

	any(const any& rhs) : storage_{}
	{
		if (rhs.has_value())
		{
			typeinfo() = rhs.typeinfo();
			storage_.is_small_ = rhs.storage_.is_small_;

			if (rhs.has_small_type())
			{
				small_rtti() = rhs.small_rtti();
				small_rtti().copy(small_data(), rhs.small_data());
			}
			else
			{
				big_rtti() = rhs.big_rtti();
				big_data() = big_rtti().copy(rhs.big_data());
			}
		}
	}

	any(any&& rhs) noexcept : storage_{}
	{
		move_from(std::move(rhs));
	}

	~any()
	{
		reset();
	}

	inline const type_info& type() const noexcept
	{
		const type_info* const info = typeinfo();
		if (info)
		{
			return *info;
		}
		return typeid(void);
	}

	inline bool has_value() const noexcept
	{
		return typeinfo() != nullptr;
	}

	template <typename _Decayed, typename... _Args>
	void emplace(_Args&&... args)
	{
		reset();
		store<_Decayed>(decayed_is_small<_Decayed>{}, std::forward<_Args>(args)...);
	}

	void swap(any& rhs) noexcept
	{
		any old	= static_cast<any&&>(rhs);
		rhs		= static_cast<any&&>(*this);
		*this	= static_cast<any&&>(old);
	}

	inline void reset() noexcept
	{
		tidy();
	}

	any& operator=(const any& rhs)
	{
		*this = any(rhs);
		return (*this);
	}

	any& operator=(any&& rhs) noexcept
	{
		reset();
		move_from(std::move(rhs));
		return (*this);
	}

protected:
	const type_info*& typeinfo()
	{
		return storage_.small_.info_;
	}

	const type_info* typeinfo() const
	{
		return storage_.small_.info_;
	}

	template <typename _Decayed, typename... _Args>
	void store(std::true_type, _Args&&... args)
	{
		storage_.is_small_ = true;
		typeinfo() = &typeid(_Decayed);
		small_rtti() = small_storage_rtti::make<_Decayed>();

		::new (small_data()) _Decayed(std::forward<_Args>(args)...);
	}

	template <typename _Decayed, typename... _Args>
	void store(std::false_type, _Args&&... args)
	{
		storage_.is_small_ = false;
		typeinfo() = &typeid(_Decayed);
		big_rtti() = big_storage_rtti::make<_Decayed>();

		big_data() = ::new _Decayed(std::forward<_Args>(args)...);
	}

	void tidy() noexcept
	{
		if (has_value())
		{
			if (has_small_type())
			{
				small_rtti().destroy(small_data());
			}
			else
			{
				big_rtti().destroy(big_data());
				big_data() = nullptr;
			}
			typeinfo() = nullptr;
		}
	}

	void move_from(any&& rhs) noexcept
	{
		if (rhs.has_value())
		{
			typeinfo() = rhs.typeinfo();
			storage_.is_small_ = rhs.storage_.is_small_;

			if (rhs.has_small_type())
			{
				small_rtti() = rhs.small_rtti();
				small_rtti().move(small_data(), rhs.small_data());
			}
			else
			{
				big_rtti() = rhs.big_rtti();
				big_data() = rhs.big_data();
			}
			rhs.typeinfo() = nullptr;
		}
	}

	inline void* small_data()
	{
		return storage_.small_.buffer_;
	}

	inline const void* small_data() const
	{
		return storage_.small_.buffer_;
	}

	inline void*& big_data()
	{
		return storage_.big_.ptr_;
	}

	inline void* big_data() const
	{
		return storage_.big_.ptr_;
	}

	inline bool has_small_type() const
	{
		return storage_.is_small_;
	}

protected:
	static const auto ANY_SPACE_SIZE = 16U;

	template <typename _Ty>
	struct decayed_is_small : public std::bool_constant<sizeof(_Ty) <= ANY_SPACE_SIZE>
	{
	};

	struct big_storage_rtti
	{
		using destroy_func	= void(void*);
		using copy_func		= void*(const void*);

		big_storage_rtti()
		{
			destroy = nullptr;
			copy = nullptr;
		}

		template <typename _Ty>
		static inline big_storage_rtti make()
		{
			big_storage_rtti rtti;
			rtti.destroy = &big_storage_rtti::destroy_impl<_Ty>;
			rtti.copy = &big_storage_rtti::copy_impl<_Ty>;
			return rtti;
		}

		template <typename _Ty>
		static void destroy_impl(void* const ptr) noexcept
		{
			::delete static_cast<_Ty*>(ptr);
		}

		template <typename _Ty>
		static void* copy_impl(void* const ptr) noexcept
		{
			return ::new _Ty(*static_cast<_Ty*>(ptr));
		}

		destroy_func* destroy;
		copy_func* copy;
	};

	struct small_storage_rtti
	{
		using destroy_func	= void(void*);
		using copy_func		= void* (void*, const void*);
		using move_func		= void*(void*, const void*);

		small_storage_rtti()
		{
			destroy = nullptr;
			copy = nullptr;
			move = nullptr;
		}

		template <typename _Ty>
		static inline small_storage_rtti make()
		{
			small_storage_rtti rtti;
			rtti.destroy = &small_storage_rtti::destroy_impl<_Ty>;
			rtti.copy = &small_storage_rtti::copy_impl<_Ty>;
			rtti.move = &small_storage_rtti::move_impl<_Ty>;
			return rtti;
		}

		template <typename _Ty>
		static void destroy_impl(void* const ptr) noexcept
		{
			if (ptr)
			{
				_Ty& obj = *(static_cast<_Ty* const>(ptr));
				obj.~_Ty();
			}
		}

		template <typename _Ty>
		static void* copy_impl(void* const target, const void* const ptr) noexcept
		{
			return ::new (static_cast<_Ty*>(target)) _Ty(*static_cast<const _Ty*>(ptr));
		}

		template <typename _Ty>
		static void* move_impl(void* const target, const void* const ptr) noexcept
		{
			return ::new (static_cast<_Ty*>(target)) _Ty(std::move(*static_cast<const _Ty*>(ptr)));
		}

		destroy_func* destroy;
		copy_func* copy;
		move_func* move;
	};

protected:
	inline small_storage_rtti& small_rtti()
	{
		return storage_.small_.rtti_;
	}

	inline const small_storage_rtti& small_rtti() const
	{
		return storage_.small_.rtti_;
	}

	inline big_storage_rtti& big_rtti()
	{
		return storage_.big_.rtti_;
	}

	inline const big_storage_rtti& big_rtti() const
	{
		return storage_.big_.rtti_;
	}

protected:
	struct small_storage
	{
		const type_info* info_;
		small_storage_rtti rtti_;
		char buffer_[ANY_SPACE_SIZE];
	};

	struct big_storage
	{
		const type_info* info_;
		big_storage_rtti rtti_;
		void* ptr_;
	};

	struct storage
	{
		bool is_small_;
		union
		{
			small_storage small_;
			big_storage big_;
		};
	};

	storage storage_;
};

}

}
