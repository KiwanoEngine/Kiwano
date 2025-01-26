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
    No,         ///< ��ֹ
};

/**
 * \~chinese
 * @brief �ֱ���
 */
struct Resolution
{
    uint32_t width        = 0;  ///< �ֱ��ʿ��
    uint32_t height       = 0;  ///< �ֱ��ʸ߶�
    uint32_t refresh_rate = 0;  ///< ˢ����

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
 * @brief ͼ��
 */
struct Icon
{
    Icon() = default;

    Icon(StringView file_path)
        : file_path(file_path)
    {
    }

    String file_path;  ///< �ļ�·��

#if defined(KGE_PLATFORM_WINDOWS)
    uint32_t resource_id = 0;  ///< ��ԴID������windows����Ч

    Icon(uint32_t resource_id)
        : resource_id(resource_id)
    {
    }
#endif
};

/**
 * \~chinese
 * @brief ��������
 */
struct WindowConfig
{
    uint32_t width  = 640;            ///< ���ڿ��
    uint32_t height = 480;            ///< ���ڸ߶�
    String   title  = "Kiwano Game";  ///< ���ڱ���
    Icon     icon;                    ///< ����ͼ��
    bool     resizable  = false;      ///< ���ڴ�С�ɵ���
    bool     fullscreen = false;      ///< ����ȫ��
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
 * @brief �����࣬���ƴ��ڱ��⡢��С��ͼ���
 */
class KGE_API Window
    : public Module
    , public RefObject
{
public:
    /**
     * \~chinese
     * @brief ��ʼ������
     * @param config ��������
     * @throw kiwano::SystemError ���ڴ���ʧ��ʱ�׳�
     */
    static RefPtr<Window> Create(const WindowConfig& config);

    /**
     * \~chinese
     * @brief ��ȡ���ڱ���
     * @return ���ڱ���
     */
    String GetTitle() const;

    /**
     * \~chinese
     * @brief ��ȡ���ں���λ��
     * @return ����λ��
     */
    int GetPosX() const;

    /**
     * \~chinese
     * @brief ��ȡ��������λ��
     * @return ����λ��
     */
    int GetPosY() const;

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
     * @brief ��ȡ��ǰ�ֱ���
     */
    Resolution GetCurrentResolution() const;

    /**
     * \~chinese
     * @brief ��ȡ���ھ��
     */
    WindowHandle GetHandle() const;

    /**
     * \~chinese
     * @brief ��ȡ֧�ֵ���Ļ�ֱ����б�
     */
    virtual Vector<Resolution> GetResolutions() = 0;

    /**
     * \~chinese
     * @brief ���ñ���
     * @param title ����
     */
    virtual void SetTitle(StringView title) = 0;

    /**
     * \~chinese
     * @brief ���ô���ͼ��
     * @param icon ͼ��
     */
    virtual void SetIcon(Icon icon) = 0;

    /**
     * \~chinese
     * @brief ���ô��ڷֱ���
     * @param width �ֱ��ʿ��
     * @param height �ֱ��ʸ߶�
     * @param fullscreen �Ƿ�ȫ��
     */
    virtual void SetResolution(uint32_t width, uint32_t height, bool fullscreen) = 0;

    /**
     * \~chinese
     * @brief ���ô�����С��С
     * @param width ��С���ڿ��
     * @param height ��С���ڸ߶�
     */
    virtual void SetMinimumSize(uint32_t width, uint32_t height) = 0;

    /**
     * \~chinese
     * @brief ���ô�������С
     * @param width ��󴰿ڿ��
     * @param height ��󴰿ڸ߶�
     */
    virtual void SetMaximumSize(uint32_t width, uint32_t height) = 0;

    /**
     * \~chinese
     * @brief �������ָ������
     * @param cursor ���ָ������
     */
    virtual void SetCursor(CursorType cursor) = 0;

    /**
     * \~chinese
     * @brief ���¼��������
     * @param evt �¼�
     */
    void PushEvent(RefPtr<Event> evt);

    /**
     * \~chinese
     * @brief �����Ƿ���Ҫ�ر�
     */
    bool ShouldClose() const;

    /**
     * \~chinese
     * @brief �����Ƿ���Ҫ�ر�
     */
    void SetShouldClose(bool should);

    /**
     * \~chinese
     * @brief ���û�������뷨��Ĭ�Ͻ��ã�
     */
    virtual void SetImmEnabled(bool enable) = 0;

protected:
    Window();

    virtual ~Window();

    void OnUpdate(UpdateModuleContext& ctx) override;

    /**
     * \~chinese
     * @brief ��ȡ�����¼�
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
