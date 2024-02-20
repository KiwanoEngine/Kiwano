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
#include <typeinfo>
#include <type_traits>
#include <exception>

namespace kiwano
{

/// \~chinese
/// @brief 可储存单个任意对象的容器
class Any
{
public:
    Any()
        : storage_{}
    {
    }

    template <typename _Ty, typename _Decayed = typename std::decay<_Ty>::type,
              typename std::enable_if<std::is_copy_constructible<_Decayed>::value, int>::type = 0>
    Any(_Ty&& val)
        : storage_{}
    {
        Emplace<_Decayed>(std::forward<_Ty>(val));
    }

    template <typename _Ty, typename... _Args>
    Any(_Args&&... args)
        : storage_{}
    {
        using _Decayed = typename std::decay<_Ty>::type;

        Clear();
        EmplaceDecayed<_Decayed>(std::forward<_Args>(args)...);
    }

    Any(const Any& rhs)
        : storage_{}
    {
        CopyFrom(rhs);
    }

    Any(Any&& rhs) noexcept
        : storage_{}
    {
        MoveFrome(std::move(rhs));
    }

    ~Any()
    {
        Clear();
    }

    /// \~chinese
    /// @brief 获取含有对象类型
    inline const std::type_info& GetType() const noexcept
    {
        const std::type_info* const info = GetTypeinfo();
        if (info)
        {
            return *info;
        }
        return typeid(void);
    }

    /// \~chinese
    /// @brief 是否含有对象
    inline bool HasValue() const noexcept
    {
        return GetTypeinfo() != nullptr;
    }

    /// \~chinese
    /// @brief 从参数构造对象
    template <typename _Ty, typename... _Args>
    void Emplace(_Args&&... args)
    {
        using _Decayed = typename std::decay<_Ty>::type;

        Clear();
        EmplaceDecayed<_Decayed>(std::forward<_Args>(args)...);
    }

    /// \~chinese
    /// @brief 交换容器
    void Swap(Any& rhs) noexcept
    {
        Any old = std::move(rhs);
        rhs     = std::move(*this);
        *this   = std::move(old);
    }

    /// \~chinese
    /// @brief 销毁所含对象
    inline void Clear() noexcept
    {
        Tidy();
    }

    /// \~chinese
    /// @brief 转换为指定类型的指针
    template <typename _Ty>
    _Ty* CastPtr() noexcept
    {
        return const_cast<_Ty*>(const_cast<const Any*>(this)->CastPtr<_Ty>());
    }

    /// \~chinese
    /// @brief 转换为指定类型的指针
    template <typename _Ty>
    const _Ty* CastPtr() const noexcept
    {
        static_assert(!std::is_void<_Ty>::value, "oc::Any cannot contain void");

        const std::type_info* const info = GetTypeinfo();
        if (info && (*info == typeid(std::decay<_Ty>::type)))
        {
            if (HasSmallType())
            {
                return static_cast<const _Ty*>(GetSmallData());
            }
            else
            {
                return static_cast<const _Ty*>(GetBigData());
            }
        }
        return nullptr;
    }

    /// \~chinese
    /// @brief 转换为指定类型
    /// @throw std::bad_cast 转换失败时抛出
    template <typename _Ty>
    _Ty Cast()
    {
        using _Decayed = typename std::decay<_Ty>::type;

        const auto ptr = CastPtr<_Decayed>();
        if (!ptr)
        {
            throw std::bad_cast();
        }
        return static_cast<_Ty>(*ptr);
    }

    /// \~chinese
    /// @brief 转换为指定类型
    /// @throw std::bad_cast 转换失败时抛出
    template <typename _Ty>
    _Ty Cast() const
    {
        using _Decayed = typename std::decay<_Ty>::type;

        const auto ptr = CastPtr<_Decayed>();
        if (!ptr)
        {
            throw std::bad_cast();
        }
        return static_cast<_Ty>(*ptr);
    }

    Any& operator=(const Any& rhs)
    {
        *this = Any(rhs);
        return (*this);
    }

    Any& operator=(Any&& rhs) noexcept
    {
        Clear();
        MoveFrome(std::move(rhs));
        return (*this);
    }

private:
    const std::type_info*& GetTypeinfo()
    {
        return storage_.small_.info_;
    }

    const std::type_info* GetTypeinfo() const
    {
        return storage_.small_.info_;
    }

    template <typename _Decayed, typename... _Args>
    inline void EmplaceDecayed(_Args&&... args)
    {
        Store<_Decayed>(IsSmallSize<_Decayed>{}, std::forward<_Args>(args)...);
    }

    template <typename _Decayed, typename... _Args>
    void Store(std::true_type, _Args&&... args)
    {
        storage_.is_small_ = true;
        GetTypeinfo()      = &typeid(_Decayed);
        GetSmallRTTI()     = SmallStorageRTTI::make<_Decayed>();

        ::new (GetSmallData()) _Decayed(std::forward<_Args>(args)...);
    }

    template <typename _Decayed, typename... _Args>
    void Store(std::false_type, _Args&&... args)
    {
        storage_.is_small_ = false;
        GetTypeinfo()      = &typeid(_Decayed);
        GetBigRTTI()       = BigStorageRTTI::make<_Decayed>();

        GetBigData() = ::new _Decayed(std::forward<_Args>(args)...);
    }

    void Tidy() noexcept
    {
        if (HasValue())
        {
            if (HasSmallType())
            {
                GetSmallRTTI().destroy(GetSmallData());
            }
            else
            {
                GetBigRTTI().destroy(GetBigData());
                GetBigData() = nullptr;
            }
            GetTypeinfo() = nullptr;
        }
    }

    void CopyFrom(const Any& rhs)
    {
        if (rhs.HasValue())
        {
            GetTypeinfo()      = rhs.GetTypeinfo();
            storage_.is_small_ = rhs.storage_.is_small_;

            if (rhs.HasSmallType())
            {
                GetSmallRTTI() = rhs.GetSmallRTTI();
                GetSmallRTTI().copy(GetSmallData(), rhs.GetSmallData());
            }
            else
            {
                GetBigRTTI() = rhs.GetBigRTTI();
                GetBigData() = GetBigRTTI().copy(rhs.GetBigData());
            }
        }
    }

    void MoveFrome(Any&& rhs) noexcept
    {
        if (rhs.HasValue())
        {
            GetTypeinfo()      = rhs.GetTypeinfo();
            storage_.is_small_ = rhs.storage_.is_small_;

            if (rhs.HasSmallType())
            {
                GetSmallRTTI() = rhs.GetSmallRTTI();
                GetSmallRTTI().move(GetSmallData(), rhs.GetSmallData());
            }
            else
            {
                GetBigRTTI()      = rhs.GetBigRTTI();
                GetBigData()      = rhs.GetBigData();
                rhs.GetTypeinfo() = nullptr;
            }
        }
    }

    inline void* GetSmallData()
    {
        return storage_.small_.buffer_;
    }

    inline const void* GetSmallData() const
    {
        return storage_.small_.buffer_;
    }

    inline void*& GetBigData()
    {
        return storage_.big_.ptr_;
    }

    inline void* GetBigData() const
    {
        return storage_.big_.ptr_;
    }

    inline bool HasSmallType() const
    {
        return storage_.is_small_;
    }

private:
    static const auto ANY_SMALL_SPACE_SIZE = 8U;

    template <typename _Ty>
    struct IsSmallSize : public std::bool_constant<sizeof(_Ty) <= ANY_SMALL_SPACE_SIZE>
    {
    };

    struct BigStorageRTTI
    {
        using DestroyFunc = void(void*);
        using CopyFunc    = void*(const void*);

        BigStorageRTTI()
        {
            destroy = nullptr;
            copy    = nullptr;
        }

        template <typename _Ty>
        static inline BigStorageRTTI make()
        {
            BigStorageRTTI rtti;
            rtti.destroy = &BigStorageRTTI::Destroy<_Ty>;
            rtti.copy    = &BigStorageRTTI::Copy<_Ty>;
            return rtti;
        }

        template <typename _Ty>
        static void Destroy(void* const ptr) noexcept
        {
            ::delete static_cast<_Ty*>(ptr);
        }

        template <typename _Ty>
        static void* Copy(const void* const ptr) noexcept
        {
            return ::new _Ty(*static_cast<const _Ty*>(ptr));
        }

        DestroyFunc* destroy;
        CopyFunc*    copy;
    };

    struct SmallStorageRTTI
    {
        using DestroyFunc = void(void*);
        using CopyFunc    = void*(void*, const void*);
        using MoveFunc    = void*(void*, void*);

        SmallStorageRTTI()
        {
            destroy = nullptr;
            copy    = nullptr;
            move    = nullptr;
        }

        template <typename _Ty>
        static inline SmallStorageRTTI make()
        {
            SmallStorageRTTI rtti;
            rtti.destroy = &SmallStorageRTTI::Destroy<_Ty>;
            rtti.copy    = &SmallStorageRTTI::Copy<_Ty>;
            rtti.move    = &SmallStorageRTTI::Move<_Ty>;
            return rtti;
        }

        template <typename _Ty>
        static void Destroy(void* const ptr) noexcept
        {
            if (ptr)
            {
                _Ty& obj = *(static_cast<_Ty* const>(ptr));
                obj.~_Ty();
            }
        }

        template <typename _Ty>
        static void* Copy(void* const target, const void* const ptr) noexcept
        {
            return ::new (static_cast<_Ty*>(target)) _Ty(*static_cast<const _Ty*>(ptr));
        }

        template <typename _Ty>
        static void* Move(void* const target, void* const ptr) noexcept
        {
            return ::new (static_cast<_Ty*>(target)) _Ty(std::move(*static_cast<_Ty*>(ptr)));
        }

        DestroyFunc* destroy;
        CopyFunc*    copy;
        MoveFunc*    move;
    };

private:
    inline SmallStorageRTTI& GetSmallRTTI()
    {
        return storage_.small_.rtti_;
    }

    inline const SmallStorageRTTI& GetSmallRTTI() const
    {
        return storage_.small_.rtti_;
    }

    inline BigStorageRTTI& GetBigRTTI()
    {
        return storage_.big_.rtti_;
    }

    inline const BigStorageRTTI& GetBigRTTI() const
    {
        return storage_.big_.rtti_;
    }

private:
    struct SmallStorage
    {
        const std::type_info* info_;
        SmallStorageRTTI      rtti_;
        char                  buffer_[ANY_SMALL_SPACE_SIZE];
    };

    struct BigStorage
    {
        const std::type_info* info_;
        BigStorageRTTI        rtti_;
        void*                 ptr_;
    };

    struct Storage
    {
        bool is_small_;
        union
        {
            SmallStorage small_;
            BigStorage   big_;
        };

        Storage()
            : is_small_(false)
            , small_()
        {
        }  // fix error C2280 for VisualStudio 2015
    };

    Storage storage_;
};

}  // namespace kiwano
