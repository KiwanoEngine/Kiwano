// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once

// Class that will implement the singleton mode must use the macro in its delare file

#ifndef OC_DECLARE_SINGLETON
#define OC_DECLARE_SINGLETON( CLASS ) \
	friend ::oc::singleton< CLASS >;
#endif

namespace oc
{

template <typename _Ty>
struct singleton
{
protected:
	singleton() = default;
	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

private:
	struct object_creator
	{
		object_creator()
		{
			(void)singleton<_Ty>::instance();
		}

		inline void dummy() const {}
	};
	static object_creator creator_;

public:
	using object_type = _Ty;

	static inline object_type& instance()
	{
		static object_type instance;
		creator_.dummy();
		return instance;
	}
};

template <typename _Ty>
typename singleton<_Ty>::object_creator singleton<_Ty>::creator_;


}  // namespace oc
