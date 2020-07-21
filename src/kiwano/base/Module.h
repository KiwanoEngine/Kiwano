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
#include <kiwano/core/Time.h>

namespace kiwano
{
class RenderContext;
class Event;
class Module;

/// \~chinese
/// @brief ģ���б�
typedef Vector<Module*> ModuleList;

/// \~chinese
/// @brief ģ��������
class KGE_API ModuleContext
{
public:
    void Next();

protected:
    ModuleContext(ModuleList& modules);

    virtual ~ModuleContext();

    virtual void Handle(Module* m) = 0;

private:
    int         index_;
    ModuleList& modules_;
};

/// \~chinese
/// @brief ��Ⱦģ��������
class KGE_API RenderModuleContext : public ModuleContext
{
public:
    RenderContext& render_ctx;

    RenderModuleContext(ModuleList& modules, RenderContext& ctx);

    virtual ~RenderModuleContext();

protected:
    void Handle(Module* m) override;
};

/// \~chinese
/// @brief ����ģ��������
class KGE_API UpdateModuleContext : public ModuleContext
{
public:
    Duration dt;

    UpdateModuleContext(ModuleList& modules, Duration dt);

protected:
    void Handle(Module* m) override;
};

/// \~chinese
/// @brief ʱ��ģ��������
class KGE_API EventModuleContext : public ModuleContext
{
public:
    Event* evt;

    EventModuleContext(ModuleList& modules, Event* evt);

protected:
    void Handle(Module* m) override;
};

/**
 * \~chinese
 * @brief ����ģ��
 */
class KGE_API Module : Noncopyable
{
public:
    /// \~chinese
    /// @brief ����ģ��
    virtual void SetupModule();

    /// \~chinese
    /// @brief ����ģ��
    virtual void DestroyModule();

    /// \~chinese
    /// @brief ��Ⱦʱ
    /// @param ctx ��Ⱦ������
    virtual void OnRender(RenderModuleContext& ctx);

    /// \~chinese
    /// @brief ����ʱ
    /// @param ctx ����������
    virtual void OnUpdate(UpdateModuleContext& ctx);

    /// \~chinese
    /// @brief �¼�����
    /// @param ctx �¼�������
    virtual void HandleEvent(EventModuleContext& ctx);

protected:
    Module();
};

}  // namespace kiwano
