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
#include <memory>
#include <kiwano-physics/Global.h>

namespace kiwano
{
namespace physics
{

KGE_DECLARE_SMART_PTR(Fixture);

/**
 * \addtogroup Physics
 * @{
 */

/// \~chinese
/// @brief ����о�
class KGE_API Fixture : public ObjectBase
{
public:
    /// \~chinese
    /// @brief �о߲���
    struct Param
    {
        float density     = 0.0f;   ///< �ܶ�
        float friction    = 0.2f;   ///< Ħ����
        float restitution = 0.0f;   ///< ���Իָ�
        bool  is_sensor   = false;  ///< �Ƿ��ǽӴ�������

        Param() {}

        Param(float density, float friction = 0.2f, float restitution = 0.f, bool is_sensor = false)
            : density(density)
            , friction(friction)
            , restitution(restitution)
            , is_sensor(is_sensor)
        {
        }
    };

    /// \~chinese
    /// @brief ����Բ�μо�
    /// @param param �о߲���
    /// @param radius Բ�ΰ뾶
    /// @param offset ƫ����
    static FixturePtr CreateCircle(const Param& param, float radius, const Point& offset = Point());

    /// \~chinese
    /// @brief �������μо�
    /// @param param �о߲���
    /// @param size ���δ�С
    /// @param offset ƫ����
    /// @param rotation ��ת�Ƕ�
    static FixturePtr CreateRect(const Param& param, const Size& size, const Point& offset = Point(),
                                 float rotation = 0.f);

    /// \~chinese
    /// @brief ��������μо�
    /// @param param �о߲���
    /// @param vertexs ����ζ���
    static FixturePtr CreatePolygon(const Param& param, const Vector<Point>& vertexs);

    /// \~chinese
    /// @brief �����߼о�
    /// @param param �о߲���
    /// @param p1 �ߵ����
    /// @param p2 �ߵ��յ�
    static FixturePtr CreateEdge(const Param& param, const Point& p1, const Point& p2);

    /// \~chinese
    /// @brief ���������о�
    /// @param param �о߲���
    /// @param vertexs ��������
    /// @param loop �Ƿ����������������յ�
    static FixturePtr CreateChain(const Param& param, const Vector<Point>& vertices, bool loop = false);

    Fixture();

    virtual ~Fixture();

    /// \~chinese
    /// @brief ��ʼ���о�
    bool Init(PhysicBodyPtr body);

    /// \~chinese
    /// @brief ��ʼ���о�
    bool Init(PhysicBody* body);

    /// \~chinese
    /// @brief ��ȡ�о����ڵ�����
    PhysicBody* GetBody() const;

    /// \~chinese
    /// @brief �Ƿ��ǽӴ�������
    bool IsSensor() const;

    /// \~chinese
    /// @brief ���üо��Ƿ��ǽӴ�������
    /// @details �Ӵ�������ֻ���������Ӵ���������Ӱ�������˶�
    void SetSensor(bool sensor);

    /// \~chinese
    /// @brief ��ȡ�оߵ���������
    void GetMassData(float* mass, Point* center, float* inertia) const;

    /// \~chinese
    /// @brief ��ȡ�ܶ�
    float GetDensity() const;

    /// \~chinese
    /// @brief �����ܶ�
    void SetDensity(float density);

    /// \~chinese
    /// @brief ��ȡĦ���� [N]
    float GetFriction() const;

    /// \~chinese
    /// @brief ����Ħ���� [N]
    void SetFriction(float friction);

    /// \~chinese
    /// @brief ��ȡ���Իָ�
    float GetRestitution() const;

    /// \~chinese
    /// @brief ���õ��Իָ�
    void SetRestitution(float restitution);

    /// \~chinese
    /// @brief �����
    bool TestPoint(const Point& p) const;

    /// \~chinese
    /// @brief ���ټо�
    void Destroy();

    /// \~chinese
    /// @brief ��ȡb2Fixture
    b2Fixture* GetB2Fixture() const;

    /// \~chinese
    /// @brief ����b2Fixture
    void SetB2Fixture(b2Fixture* fixture);

    bool operator==(const Fixture& rhs) const;
    bool operator!=(const Fixture& rhs) const;

private:
    b2Fixture*               fixture_;
    std::unique_ptr<b2Shape> shape_;
    Fixture::Param           param_;
};

/// \~chinese
/// @brief ����о��б�
class FixtureList
{
    template <typename _Ty>
    class IteratorImpl
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = _Ty;
        using pointer           = _Ty*;
        using reference         = _Ty&;
        using difference_type   = ptrdiff_t;

        IteratorImpl(pointer elem)
            : elem_(elem)
        {
        }

        inline reference operator*() const
        {
            return const_cast<reference>(*elem_);
        }

        inline pointer operator->() const
        {
            return std::pointer_traits<pointer>::pointer_to(**this);
        }

        inline IteratorImpl& operator++()
        {
            b2Fixture* next = elem_->GetB2Fixture()->GetNext();

            elem_ = static_cast<Fixture*>(next->GetUserData());
            return *this;
        }

        inline IteratorImpl operator++(int)
        {
            IteratorImpl old = *this;
            operator++();
            return old;
        }

        inline bool operator==(const IteratorImpl& rhs) const
        {
            return elem_ == rhs.elem_;
        }

        inline bool operator!=(const IteratorImpl& rhs) const
        {
            return !operator==(rhs);
        }

    private:
        pointer elem_;
    };

public:
    using value_type     = Fixture;
    using reference      = value_type&;
    using pointer        = value_type*;
    using iterator       = IteratorImpl<value_type>;
    using const_iterator = IteratorImpl<const value_type>;

    inline FixtureList()
        : first_(nullptr)
    {
    }

    inline FixtureList(pointer first)
        : first_(first)
    {
    }

    inline const value_type& front() const
    {
        return *first_;
    }

    inline value_type& front()
    {
        return *first_;
    }

    inline iterator begin()
    {
        return iterator(first_);
    }

    inline const_iterator begin() const
    {
        return cbegin();
    }

    inline const_iterator cbegin() const
    {
        return const_iterator(first_);
    }

    inline iterator end()
    {
        return iterator(nullptr);
    }

    inline const_iterator end() const
    {
        return cend();
    }

    inline const_iterator cend() const
    {
        return const_iterator(nullptr);
    }

private:
    pointer first_;
};

/** @} */

inline bool Fixture::IsSensor() const
{
    return param_.is_sensor;
}

inline void Fixture::SetSensor(bool sensor)
{
    if (param_.is_sensor != sensor)
    {
        param_.is_sensor = sensor;
        if (fixture_)
        {
            fixture_->SetSensor(sensor);
        }
    }
}

inline float Fixture::GetDensity() const
{
    return param_.density;
}

inline void Fixture::SetDensity(float density)
{
    if (param_.density != density)
    {
        param_.density = density;
        if (fixture_)
        {
            fixture_->SetDensity(density);
        }
    }
}

inline float Fixture::GetFriction() const
{
    return param_.friction;
}

inline void Fixture::SetFriction(float friction)
{
    if (param_.friction != friction)
    {
        param_.friction = friction;
        if (fixture_)
        {
            fixture_->SetFriction(friction);
        }
    }
}

inline float Fixture::GetRestitution() const
{
    return param_.restitution;
}

inline void Fixture::SetRestitution(float restitution)
{
    if (param_.restitution != restitution)
    {
        param_.restitution = restitution;
        if (fixture_)
        {
            fixture_->SetRestitution(restitution);
        }
    }
}

inline b2Fixture* Fixture::GetB2Fixture() const
{
    return fixture_;
}

inline void Fixture::SetB2Fixture(b2Fixture* fixture)
{
    fixture_ = fixture;
    if (fixture)
    {
        fixture->SetUserData(this);
    }
}

inline bool Fixture::operator==(const Fixture& rhs) const
{
    return fixture_ == rhs.fixture_;
}

inline bool Fixture::operator!=(const Fixture& rhs) const
{
    return fixture_ != rhs.fixture_;
}

}  // namespace physics
}  // namespace kiwano
