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
#include <kiwano/core/RingBuffer.hpp>
#include <kiwano/event/Event.h>
#include <kiwano/math/Math.h>
#include <kiwano/module/Module.h>

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
    No,         ///< 禁止
};

/**
 * \~chinese
 * @brief 分辨率
 */
struct Resolution
{
    uint32_t width        = 0;  ///< 分辨率宽度
    uint32_t height       = 0;  ///< 分辨率高度
    uint32_t refresh_rate = 0;  ///< 刷新率

    Resolution() = default;

    Resolution(uint32_t width, uint32_t height, uint32_t refresh_rate)
        : width(width)
        , height(height)
        , refresh_rate(refresh_rate)
    {
    }
};

/**
 * \~chinese
 * @brief 图标
 */
struct Icon
{
    Icon() = default;

    Icon(StringView file_path)
        : file_path(file_path)
    {
    }

    String file_path;  ///< 文件路径

#if defined(KGE_PLATFORM_WINDOWS)
    uint32_t resource_id = 0;  ///< 资源ID，仅在windows上生效

    Icon(uint32_t resource_id)
        : resource_id(resource_id)
    {
    }
#endif
};

/**
 * \~chinese
 * @brief 窗口设置
 */
struct WindowConfig
{
    uint32_t width  = 640;            ///< 窗口宽度
    uint32_t height = 480;            ///< 窗口高度
    String   title  = "Kiwano Game";  ///< 窗口标题
    Icon     icon;                    ///< 窗口图标
    bool     resizable  = false;      ///< 窗口大小可调整
    bool     fullscreen = false;      ///< 窗口全屏
};

#if defined(KGE_PLATFORM_WINDOWS)
typedef HWND WindowHandle;
#endif

/**
 * \addtogroup Module
 * @{
 */

/**
 * \~chinese
 * @brief 窗口类，控制窗口标题、大小、图标等
 */
class KGE_API Window
    : public Module
    , public RefObject
{
public:
    /**
     * \~chinese
     * @brief 初始化窗口
     * @param config 窗口设置
     * @throw kiwano::SystemError 窗口创建失败时抛出
     */
    static RefPtr<Window> Create(const WindowConfig& config);

    /**
     * \~chinese
     * @brief 获取窗口标题
     * @return 窗口标题
     */
    String GetTitle() const;

    /**
     * \~chinese
     * @brief 获取窗口横向位置
     * @return 横向位置
     */
    int GetPosX() const;

    /**
     * \~chinese
     * @brief 获取窗口纵向位置
     * @return 纵向位置
     */
    int GetPosY() const;

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
     * @brief 获取当前分辨率
     */
    Resolution GetCurrentResolution() const;

    /**
     * \~chinese
     * @brief 获取窗口句柄
     */
    WindowHandle GetHandle() const;

    /**
     * \~chinese
     * @brief 获取支持的屏幕分辨率列表
     */
    virtual Vector<Resolution> GetResolutions() = 0;

    /**
     * \~chinese
     * @brief 设置标题
     * @param title 标题
     */
    virtual void SetTitle(StringView title) = 0;

    /**
     * \~chinese
     * @brief 设置窗口图标
     * @param icon 图标
     */
    virtual void SetIcon(Icon icon) = 0;

    /**
     * \~chinese
     * @brief 设置窗口分辨率
     * @param width 分辨率宽度
     * @param height 分辨率高度
     * @param fullscreen 是否全屏
     */
    virtual void SetResolution(uint32_t width, uint32_t height, bool fullscreen) = 0;

    /**
     * \~chinese
     * @brief 设置窗口最小大小
     * @param width 最小窗口宽度
     * @param height 最小窗口高度
     */
    virtual void SetMinimumSize(uint32_t width, uint32_t height) = 0;

    /**
     * \~chinese
     * @brief 设置窗口最大大小
     * @param width 最大窗口宽度
     * @param height 最大窗口高度
     */
    virtual void SetMaximumSize(uint32_t width, uint32_t height) = 0;

    /**
     * \~chinese
     * @brief 设置鼠标指针类型
     * @param cursor 鼠标指针类型
     */
    virtual void SetCursor(CursorType cursor) = 0;

    /**
     * \~chinese
     * @brief 将事件放入队列
     * @param evt 事件
     */
    void PushEvent(RefPtr<Event> evt);

    /**
     * \~chinese
     * @brief 窗口是否需要关闭
     */
    bool ShouldClose() const;

    /**
     * \~chinese
     * @brief 设置是否需要关闭
     */
    void SetShouldClose(bool should);

    /**
     * \~chinese
     * @brief 启用或禁用输入法（默认禁用）
     */
    virtual void SetImmEnabled(bool enable) = 0;

protected:
    Window();

    virtual ~Window();

    void OnUpdate(UpdateModuleContext& ctx) override;

    /**
     * \~chinese
     * @brief 抽取窗口事件
     */
    virtual void PumpEvents() = 0;

protected:
    bool                      should_close_;
    bool                      is_fullscreen_;
    int                       pos_x_;
    int                       pos_y_;
    uint32_t                  width_;
    uint32_t                  height_;
    uint32_t                  min_width_;
    uint32_t                  min_height_;
    uint32_t                  max_width_;
    uint32_t                  max_height_;
    Resolution                resolution_;
    WindowHandle              handle_;
    String                    title_;
    RingBuffer<RefPtr<Event>> event_buffer_;
};

/** @} */

}  // namespace kiwano
