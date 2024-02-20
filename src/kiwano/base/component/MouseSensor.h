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
#include <kiwano/base/component/Component.h>
#include <kiwano/event/MouseEvent.h>

namespace kiwano
{

/**
 * \addtogroup Component
 * @{
 */

/**
 * \~chinese
 * @brief ��괫����
 * @details �����ʹ��ɫ���յ����� Hover | Out | Click ��Ϣ
 */
class KGE_API MouseSensor : public Component
{
public:
    MouseSensor();

    virtual ~MouseSensor();

    /// \~chinese
    /// @brief ����Ƿ������ڽ�ɫ��
    bool IsHovering() const;

    /// \~chinese
    /// @brief ����Ƿ������ڽ�ɫ��
    bool IsPressing() const;

protected:
    /// \~chinese
    /// @brief �����ɫ�¼�
    void HandleEvent(Event* evt) override;

private:
    bool hover_;
    bool pressed_;
};

/** @} */

inline bool MouseSensor::IsHovering() const
{
    return hover_;
}

inline bool MouseSensor::IsPressing() const
{
    return pressed_;
}

}  // namespace kiwano
