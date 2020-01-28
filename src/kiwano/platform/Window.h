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
#include <kiwano/core/Common.h>
#include <kiwano/core/event/Event.h>
#include <kiwano/macros.h>
#include <kiwano/math/math.h>

namespace kiwano
{
/**
 * \~chinese
 * @brief ���ָ������
 */
enum class CursorType
{
    Arrow,      ///< ָ��
    TextInput,  ///< �ı�
    Hand,       ///< ��
    SizeAll,    ///< ָ���ĸ�����ļ�ͷ
    SizeWE,     ///< ָ�����ҷ���ļ�ͷ
    SizeNS,     ///< ָ�����·���ļ�ͷ
    SizeNESW,   ///< ָ�����µ����Ϸ���ļ�ͷ
    SizeNWSE,   ///< ָ�����ϵ����·���ļ�ͷ
};

/**
 * \~chinese
 * @brief ��������
 */
struct WindowConfig
{
    String   title;       ///< ����
    uint32_t width;       ///< ���
    uint32_t height;      ///< �߶�
    uint32_t icon;        ///< ͼ����Դ ID
    bool     resizable;   ///< ���ڴ�С������
    bool     fullscreen;  ///< ȫ��ģʽ

    /**
     * \~chinese
     * @brief ������������
     * @param title ����
     * @param width ���
     * @param height �߶�
     * @param icon ͼ����ԴID
     * @param resizable ���ڴ�С������
     * @param fullscreen ȫ��ģʽ
     */
    WindowConfig(String const& title = L"Kiwano Game", uint32_t width = 640, uint32_t height = 480, uint32_t icon = 0,
                 bool resizable = false, bool fullscreen = false);
};

#if defined(KGE_WIN32)
typedef HWND WindowHandle;
#endif

/**
 * \~chinese
 * @brief �����࣬���ƴ��ڱ��⡢��С��ͼ���
 */
class KGE_API Window : protected Noncopyable
{
public:
    /**
     * \~chinese
     * @brief ��ȡ����ʵ��
     */
    static Window& Instance();

    /**
     * \~chinese
     * @brief ��ʼ������
     * @param config ��������
     */
    virtual bool Create(WindowConfig const& config) = 0;

    /**
     * \~chinese
     * @brief ��ȡ���ڱ���
     * @return ���ڱ���
     */
    String GetTitle() const;

    /**
     * \~chinese
     * @brief ��ȡ���ڴ�С
     * @return ���ڴ�С
     */
    Size GetSize() const;

    /**
     * \~chinese
     * @brief ��ȡ���ڿ��
     * @return ���ڿ��
     */
    uint32_t GetWidth() const;

    /**
     * \~chinese
     * @brief ��ȡ���ڸ߶�
     * @return ���ڸ߶�
     */
    uint32_t GetHeight() const;

    /**
     * \~chinese
     * @brief ��ȡ���ھ��
     */
    virtual WindowHandle GetHandle() const = 0;

    /**
     * \~chinese
     * @brief ���ñ���
     * @param title ����
     */
    virtual void SetTitle(String const& title) = 0;

    /**
     * \~chinese
     * @brief ���ô���ͼ��
     * @param icon_resource ͼ����ԴID
     */
    virtual void SetIcon(uint32_t icon_resource) = 0;

    /**
     * \~chinese
     * @brief ���贰�ڴ�С
     * @param width ���ڿ��
     * @param height ���ڸ߶�
     */
    virtual void Resize(uint32_t width, uint32_t height) = 0;

    /**
     * \~chinese
     * @brief ����ȫ��ģʽ
     * @param fullscreen �Ƿ�ȫ��
     * @param width ���ڿ��
     * @param height ���ڸ߶�
     */
    virtual void SetFullscreen(bool fullscreen) = 0;

    /**
     * \~chinese
     * @brief �������ָ������
     * @param cursor ���ָ������
     */
    virtual void SetCursor(CursorType cursor) = 0;

    /**
     * \~chinese
     * @brief ��ѯ�����¼�
     * @return
     * �����¼������еĵ�һ���¼�������Ӷ������Ƴ������¼�����Ϊ���򷵻ؿ�ָ��
     */
    EventPtr PollEvent();

    /**
     * \~chinese
     * @brief �����Ƿ���Ҫ�ر�
     */
    bool ShouldClose();

    /**
     * \~chinese
     * @brief ���ٴ���
     */
    virtual void Destroy();

protected:
    Window();

    ~Window();

    void PushEvent(EventPtr evt);

    void SetInternalSize(uint32_t width, uint32_t height);

    void SetInternalTitle(String const& title);

    virtual void PumpEvents() = 0;

private:
    bool                 should_close_;
    uint32_t             width_;
    uint32_t             height_;
    String               title_;
    std::queue<EventPtr> event_queue_;
};

inline void Window::SetInternalSize(uint32_t width, uint32_t height)
{
    width_  = width;
    height_ = height;
}

inline void Window::SetInternalTitle(String const& title)
{
    title_ = title;
}
}  // namespace kiwano
