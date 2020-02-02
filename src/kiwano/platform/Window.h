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
 * @brief 鼠标指针类型
 */
enum class CursorType
{
    Arrow,      ///< 指针
    TextInput,  ///< 文本
    Hand,       ///< 手
    SizeAll,    ///< 指向四个方向的箭头
    SizeWE,     ///< 指向左右方向的箭头
    SizeNS,     ///< 指向上下方向的箭头
    SizeNESW,   ///< 指向左下到右上方向的箭头
    SizeNWSE,   ///< 指向左上到右下方向的箭头
};


#if defined(KGE_WIN32)

/**
 * \~chinese
 * @brief 窗口句柄
 */
typedef HWND WindowHandle;

#endif

/**
 * \~chinese
 * @brief 窗口类，控制窗口标题、大小、图标等
 */
class KGE_API Window : protected Noncopyable
{
public:
    /**
     * \~chinese
     * @brief 获取窗口实例
     */
    static Window& Instance();

    /**
     * \~chinese
     * @brief 初始化窗口
     * @param title 标题
     * @param width 宽度
     * @param height 高度
     * @param icon 图标资源ID
     * @param resizable 窗口大小可拉伸
     * @param fullscreen 全屏模式
     * @throw std::runtime_error 窗口创建失败时抛出
     */
    virtual void Create(String const& title, uint32_t width, uint32_t height, uint32_t icon = 0, bool resizable = false,
                        bool fullscreen = false) = 0;

    /**
     * \~chinese
     * @brief 获取窗口标题
     * @return 窗口标题
     */
    String GetTitle() const;

    /**
     * \~chinese
     * @brief 获取窗口大小
     * @return 窗口大小
     */
    Size GetSize() const;

    /**
     * \~chinese
     * @brief 获取窗口宽度
     * @return 窗口宽度
     */
    uint32_t GetWidth() const;

    /**
     * \~chinese
     * @brief 获取窗口高度
     * @return 窗口高度
     */
    uint32_t GetHeight() const;

    /**
     * \~chinese
     * @brief 获取窗口句柄
     */
    virtual WindowHandle GetHandle() const = 0;

    /**
     * \~chinese
     * @brief 设置标题
     * @param title 标题
     */
    virtual void SetTitle(String const& title) = 0;

    /**
     * \~chinese
     * @brief 设置窗口图标
     * @param icon_resource 图标资源ID
     */
    virtual void SetIcon(uint32_t icon_resource) = 0;

    /**
     * \~chinese
     * @brief 重设窗口大小
     * @param width 窗口宽度
     * @param height 窗口高度
     */
    virtual void Resize(uint32_t width, uint32_t height) = 0;

    /**
     * \~chinese
     * @brief 设置全屏模式
     * @param fullscreen 是否全屏
     * @param width 窗口宽度
     * @param height 窗口高度
     */
    virtual void SetFullscreen(bool fullscreen) = 0;

    /**
     * \~chinese
     * @brief 设置鼠标指针类型
     * @param cursor 鼠标指针类型
     */
    virtual void SetCursor(CursorType cursor) = 0;

    /**
     * \~chinese
     * @brief 轮询窗口事件
     * @return 返回事件队列中的第一个事件并将其从队列中移除\n
     *         若事件队列为空则返回空指针
     */
    EventPtr PollEvent();

    /**
     * \~chinese
     * @brief 将窗口事件放入队列
     * @param evt 窗口事件
     */
    void PushEvent(EventPtr evt);

    /**
     * \~chinese
     * @brief 窗口是否需要关闭
     */
    bool ShouldClose();

    /**
     * \~chinese
     * @brief 销毁窗口
     */
    virtual void Destroy();

protected:
    Window();

    ~Window();

    virtual void PumpEvents() = 0;

protected:
    bool                 should_close_;
    uint32_t             width_;
    uint32_t             height_;
    String               title_;
    std::queue<EventPtr> event_queue_;
};

}  // namespace kiwano
