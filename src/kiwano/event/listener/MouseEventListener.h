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
#include <kiwano/event/MouseEvent.h>
#include <kiwano/event/listener/EventListener.h>

namespace kiwano
{

/**
 * \addtogroup EventListener
 * @{
 */

/**
 * \~chinese
 * @brief ����¼�������
 */
class KGE_API MouseEventListener
    : public EventListener
{
public:
    /// \~chinese
    /// @brief ����ƶ�ʱ
    /// @param pos ���λ��
    virtual void OnMouseMoving(const Point& pos) {}

    /// \~chinese
    /// @brief ��갴��ʱ
    /// @param pos ���λ��
    /// @param button ����ֵ
    virtual void OnMouseDown(const Point& pos, MouseButton button) {}

    /// \~chinese
    /// @brief ���̧��ʱ
    /// @param pos ���λ��
    /// @param button ����ֵ
    virtual void OnMouseUp(const Point& pos, MouseButton button) {}

    /// \~chinese
    /// @brief �����ֹ���ʱ
    /// @param pos ���λ��
    /// @param wheel �����ֹ���ֵ
    virtual void OnMouseWheelScrolling(const Point& pos, float wheel) {}

    /// \~chinese
    /// @brief ������Ϣ
    void Handle(Event* evt) override;
};

/**
 * \~chinese
 * @brief ����϶��¼�������
 */
class KGE_API MouseDragEventListener : public EventListener
{
public:
    MouseDragEventListener();

    /// \~chinese
    /// @brief ����϶���ʼ
    /// @param pos ���λ��
    /// @param button ����ֵ
    /// @return �Ƿ���Ӧ����϶�
    virtual bool OnDragStart(const Point& pos, MouseButton button)
    {
        return true;
    }

    /// \~chinese
    /// @brief ����϶�ʱ
    /// @param offset ���λ��ƫ����
    /// @param button ����ֵ
    virtual void OnDragging(const Point& offset, MouseButton button) {}

    /// \~chinese
    /// @brief ����϶�����
    /// @param pos ���λ��
    /// @param button ����ֵ
    virtual void OnDragEnd(const Point& pos, MouseButton button) {}

    /// \~chinese
    /// @brief ��ȡ��갴�����϶�״̬
    bool IsDragging(MouseButton button);

    /// \~chinese
    /// @brief ��ȡ��갴������ʼ�϶�λ��
    Point GetDragPoint(MouseButton button);

    /// \~chinese
    /// @brief ������Ϣ
    void Handle(Event* evt) override;

private:
    void SetDragging(MouseButton button, bool dragging);

    void SetDragPoint(MouseButton button, Point pos);

    Point GetDragPrevPoint(MouseButton button);

    void SetDragPrevPoint(MouseButton button, Point pos);

private:
    enum : int
    {
        MouseButtonNum = 3
    };

    bool  is_dragging_[MouseButtonNum];
    Point drag_point_[MouseButtonNum];
    Point drag_prev_point_[MouseButtonNum];
};

/** @} */

}  // namespace kiwano
