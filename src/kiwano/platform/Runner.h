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
#include <kiwano/core/Time.h>
#include <kiwano/platform/Window.h>
#include <kiwano/render/Color.h>
#include <kiwano/render/Texture.h>
#include <kiwano/utils/Ticker.h>

namespace kiwano
{

class Application;

/**
 * \~chinese
 * @brief ��Ϸ����
 */
struct Settings
{
    WindowConfig window;          ///< ��������
    Color        bg_color;        ///< ����ɫ
    Duration     frame_interval;  ///< ֡���
    bool         vsync_enabled;   ///< ��ֱͬ��
    bool         debug_mode;      ///< ����ģʽ

    Settings()
        : bg_color(Color::Black)
        , frame_interval(0)
        , vsync_enabled(true)
        , debug_mode(false)
    {
    }
};

/**
 * \~chinese
 * @brief ����������
 */
class KGE_API Runner : public ObjectBase
{
public:
    Runner();

    /// \~chinese
    /// @brief ��������������
    /// @param main_window ������
    Runner(const Settings& settings);

    virtual ~Runner();

    /// \~chinese
    /// @brief ��ʼ����ɴ���
    /// @details ���ظú�������Ӧ�ó����ʼ����ɺ��Զ�ִ��
    virtual void OnReady();

    /// \~chinese
    /// @brief Ӧ�ó������ٴ���
    /// @details ���ظú����Դ���Ӧ�ó�������ʱ����Ϊ���������Դ���յ�
    virtual void OnDestroy();

    /// \~chinese
    /// @brief Ӧ�ó���رմ���
    /// @details ���ظú����Դ����û��ر�Ӧ�ó���ʱ����Ϊ���籣���û����ݵ�
    /// @return ����true�����û��رճ��򣬷�����ֹ����ر�
    virtual bool OnClose();

    /// \~chinese
    /// @brief Ӧ�ó�����ѭ��
    /// @param dt ʱ����
    /// @details ���ظú����Կ��Ƴ�����ѭ��
    /// @return ����false�˳���ѭ�����������������ѭ��
    bool MainLoop(Duration dt);

    /// \~chinese
    /// @brief ��ȡ����
    RefPtr<Window> GetWindow() const;

    /// \~chinese
    /// @brief ��ȡ����
    Settings GetSettings() const;

    /// \~chinese
    /// @brief ��ȡ֡��ʱ��
    RefPtr<Ticker> GetFrameTicker() const;

    /// \~chinese
    /// @brief ����֡��ʱ��
    void SetFrameTicker(RefPtr<Ticker> ticker);

protected:
    /// \~chinese
    /// @brief �޸�����
    void SetSettings(Settings settings);

    /// \~chinese
    /// @brief ���ô���
    void SetWindow(RefPtr<Window> window);

private:
    friend class Application;

    void InitSettings();

private:
    Settings       settings_;
    RefPtr<Window> main_window_;
    RefPtr<Ticker> frame_ticker_;
};

inline void Runner::OnReady() {}

inline void Runner::OnDestroy() {}

inline bool Runner::OnClose()
{
    return true;
}

inline RefPtr<Window> Runner::GetWindow() const
{
    return main_window_;
}

inline void Runner::SetWindow(RefPtr<Window> window)
{
    main_window_ = window;
}

inline Settings Runner::GetSettings() const
{
    return settings_;
}

inline void Runner::SetSettings(Settings settings)
{
    settings_ = settings;
}

inline RefPtr<Ticker> Runner::GetFrameTicker() const
{
    return frame_ticker_;
}

inline void Runner::SetFrameTicker(RefPtr<Ticker> ticker)
{
    frame_ticker_ = ticker;
}

}  // namespace kiwano
