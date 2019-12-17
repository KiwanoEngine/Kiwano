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

	static void			copy(value_type* const dest, const size_type count, const value_type& val)			{ for (size_type i = 0; i < count; ++i) std::memcpy(&dest[i], &val, sizeof(value_type)); }
	static void			copy_n(value_type* const dest, const value_type* src, const size_type count)		{ if (src == dest) return; std::memcpy(dest, src, count * sizeof(value_type)); }
	static void			move(value_type* const dest, const value_type* src, const size_type count)			{ if (src == dest) return; std::memmove(dest, src, count * sizeof(value_type)); }

	static void			construct(value_type* const ptr, const size_type count, const value_type& val)		{ copy(ptr, count, val); }
	static void			construct_n(value_type* const ptr, const value_type* src, const size_type count)	{ copy_n(ptr, src, count); }
	static void			destroy(value_type* const ptr, const size_type count)								{ }

	static value_type*	allocate(const size_type count)														{ return get_allocator().allocate(count); }
	static void			deallocate(value_type*& ptr, const size_type count)									{ if (ptr) { get_allocator().deallocate(ptr, count); ptr = nullptr; } }

private:
	static allocator_type& get_allocator()
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

	static void			copy(value_type* const dest, const size_type count, const value_type& val)			{ std::fill_n(dest, count, val); }
	static void			copy_n(value_type* const dest, const value_type* src, const size_type count)		{ if (src == dest) return; std::copy_n(src, count, dest); }
	static void			move(value_type* const dest, const value_type* src, const size_type count)			{ if (src == dest) return; if (dest > src && dest < src + count) std::copy_backward(src, src + count, dest); else copy_n(dest, src, count); }

	static void			construct(value_type* const ptr, const size_type count, const value_type& val)		{ for (size_type i = 0; i < count; ++i) get_allocator().construct(std::addressof(ptr[i]), val); }
	static void			construct_n(value_type* const ptr, const value_type* src, const size_type count)	{ for (size_type i = 0; i < count; ++i) get_allocator().construct(std::addressof(ptr[i]), src[i]); }
	static void			destroy(value_type* const ptr, const size_type count)								{ for (size_type i = 0; i < count; ++i) get_allocator().destroy(std::addressof(ptr[i])); }

	static value_type*	allocate(const size_type count)														{ return get_allocator().allocate(count); }
	static void			deallocate(value_type*& ptr, const size_type count)									{ if (ptr) { get_allocator().deallocate(ptr, count); ptr = nullptr; } }

private:
	static allocator_type& get_allocator()
	{
		static allocator_type allocator_;
		return allocator_;
	}
};

}  // namespace __vector_details
}  // namespace oc
