// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <type_traits>
#include <iterator>
#include <algorithm>
#include <cstring>

namespace oc
{

namespace __vector_details
{

// vector_memory_manager<> with memory operations
template<typename _Ty, typename _Alloc, bool _IsTrivial = std::is_trivial<_Ty>::value>
struct vector_memory_manager;


//
// vector_memory_manager for common type
//
template<typename _Ty, typename _Alloc>
struct vector_memory_manager<_Ty, _Alloc, true>
{
	using value_type		= _Ty;
	using size_type			= size_t;
	using allocator_type	= typename _Alloc;

	static void fill_n(value_type* const dest, ptrdiff_t count, const value_type& val)
	{
		for (ptrdiff_t i = 0; i < count; ++i)
			std::memcpy(std::addressof(dest[i]), std::addressof(val), sizeof(value_type));
	}

	static void copy_n(value_type* const dest, const value_type* src, ptrdiff_t count)
	{
		if (src == dest)
			return;
		std::memcpy(dest, src, count * sizeof(value_type));
	}

	static void move(value_type* const dest, const value_type* src, ptrdiff_t count)
	{
		if (src == dest)
			return;
		std::memmove(dest, src, count * sizeof(value_type));
	}

	static void construct(value_type* const ptr, ptrdiff_t count, const value_type& val)
	{
		fill_n(ptr, count, val);
	}

	static void construct_n(value_type* const ptr, const value_type* src, ptrdiff_t count)
	{
		copy_n(ptr, src, count);
	}

	static void destroy(value_type* const ptr, ptrdiff_t count)
	{
	}

	static value_type* allocate(ptrdiff_t count)
	{
		return get_allocator().allocate(count);
	}

	static void deallocate(value_type*& ptr, ptrdiff_t count)
	{
		if (ptr)
		{
			get_allocator().deallocate(ptr, count);
			ptr = nullptr;
		}
	}

private:
	static inline allocator_type& get_allocator()
	{
		static allocator_type allocator_;
		return allocator_;
	}
};

//
// vector_memory_manager for classes
//
template<typename _Ty, typename _Alloc>
struct vector_memory_manager<_Ty, _Alloc, false>
{
	using value_type		= _Ty;
	using size_type			= size_t;
	using allocator_type	= typename _Alloc;

	static void fill_n(value_type* const dest, ptrdiff_t count, const value_type& val)
	{
		// Avoid warning C4996
		// std::fill_n(dest, count, val);
		for (ptrdiff_t i = 0; i < count; ++i)
			dest[i] = val;
	}

	static void copy_n(value_type* const dest, const value_type* src, ptrdiff_t count)
	{
		if (src == dest)
			return;

		// Avoid warning C4996
		// std::copy_n(src, count, dest);
		for (ptrdiff_t i = 0; i < count; ++i)
			dest[i] = src[i];
	}

	static void move(value_type* const dest, const value_type* src, ptrdiff_t count)
	{
		if (src == dest)
			return;

		if (dest > src && dest < src + count)
		{
			// Avoid warning C4996
			// std::copy_backward(src, src + count, dest);
			for (ptrdiff_t i = 0; i < count; ++i)
				dest[count - i - 1] = src[count - i - 1];
		}
		else
			copy_n(dest, src, count);
	}

	static void construct(value_type* const ptr, ptrdiff_t count, const value_type& val)
	{
		for (ptrdiff_t i = 0; i < count; ++i)
			get_allocator().construct(std::addressof(ptr[i]), val);
	}

	static void construct_n(value_type* const ptr, const value_type* src, ptrdiff_t count)
	{
		for (ptrdiff_t i = 0; i < count; ++i)
			get_allocator().construct(std::addressof(ptr[i]), src[i]);
	}

	static void destroy(value_type* const ptr, ptrdiff_t count)
	{
		for (ptrdiff_t i = 0; i < count; ++i)
			get_allocator().destroy(std::addressof(ptr[i]));
	}

	static value_type* allocate(ptrdiff_t count)
	{
		return get_allocator().allocate(count);
	}

	static void deallocate(value_type*& ptr, ptrdiff_t count)
	{
		if (ptr)
		{
			get_allocator().deallocate(ptr, count);
			ptr = nullptr;
		}
	}

private:
	static inline allocator_type& get_allocator()
	{
		static allocator_type allocator_;
		return allocator_;
	}
};

}  // namespace __vector_details
}  // namespace oc
