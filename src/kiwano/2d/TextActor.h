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
#include <kiwano/2d/Actor.h>
#include <kiwano/render/Color.h>
#include <kiwano/render/TextLayout.h>

namespace kiwano
{

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 文本角色
 */
class KGE_API TextActor : public Actor
{
public:
    TextActor();

    /// \~chinese
    /// @brief 创建文本角色
    /// @param text 文字内容
    TextActor(StringView text);

    /// \~chinese
    /// @brief 创建文本角色
    /// @param text 文字内容
    /// @param style 文本样式
    TextActor(StringView text, const TextStyle& style);

    virtual ~TextActor();

    /// \~chinese
    /// @brief 获取文本
    StringView GetText() const;

    /// \~chinese
    /// @brief 获取文本样式
    TextStyle GetStyle() const;

    /// \~chinese
    /// @brief 获取文本布局
    RefPtr<TextLayout> GetLayout() const;

    /// \~chinese
    /// @brief 获取大小
    Size GetSize() const override;

    /// \~chinese
    /// @brief 获取填充画刷
    RefPtr<Brush> GetFillBrush() const;

    /// \~chinese
    /// @brief 获取描边画刷
    RefPtr<Brush> GetOutlineBrush() const;

    /// \~chinese
    /// @brief 获取描边线条样式
    RefPtr<StrokeStyle> GetOutlineStrokeStyle() const;

    /// \~chinese
    /// @brief 获取字体
    RefPtr<Font> GetFont() const;

    /// \~chinese
    /// @brief 设置文本
    void SetText(StringView text);

    /// \~chinese
    /// @brief 设置文本样式
    void SetStyle(const TextStyle& style);

    /// \~chinese
    /// @brief 设置字体
    void SetFont(RefPtr<Font> font);

    /// \~chinese
    /// @brief 设置文字填充画刷
    void SetFillBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 设置文字填充颜色（默认值为 Color::White）
    void SetFillColor(const Color& color);

    /// \~chinese
    /// @brief 设置文本自动换行的宽度（默认为 0）
    void SetWrapWidth(float wrap_width);

    /// \~chinese
    /// @brief 设置行间距（默认为 0）
    void SetLineSpacing(float line_spacing);

    /// \~chinese
    /// @brief 设置对齐方式（默认为 TextAlign::Left）
    void SetAlignment(TextAlign align);

    /// \~chinese
    /// @brief 设置文字描边画刷
    void SetOutlineBrush(RefPtr<Brush> brush);

    /// \~chinese
    /// @brief 设置文字描边颜色
    void SetOutlineColor(const Color& outline_color);

    /// \~chinese
    /// @brief 设置描边线条样式
    void SetOutlineStrokeStyle(RefPtr<StrokeStyle> stroke);

    /// \~chinese
    /// @brief 设置是否显示下划线（默认值为 false）
    void SetUnderline(bool enable);

    /// \~chinese
    /// @brief 设置是否显示删除线（默认值为 false）
    void SetStrikethrough(bool enable);

    /// \~chinese
    /// @brief 设置文本布局
    void SetTextLayout(RefPtr<TextLayout> layout);

    /// \~chinese
    /// @brief 更新脏文字布局
    /// @details 仅当文字布局脏时更新
    void UpdateDirtyLayout();

    /// \~chinese
    /// @brief 强制更新文字布局
    /// @details 文字布局是懒更新的，手动更新文字布局以更新节点状态
    void ForceUpdateLayout();

    void OnRender(RenderContext& ctx) override;

protected:
    void Update(Duration dt) override;

    bool CheckVisibility(RenderContext& ctx) const override;

    void UpdateCachedTexture();

    /// \~chinese
    /// @brief 设置预渲染模式，在描边等情况下会有更好的性能
    void SetPreRenderEnabled(bool enable);

private:
    bool                  is_cache_dirty_;
    String                content_;
    TextStyle             style_;
    RefPtr<TextLayout>    layout_;
    RefPtr<Brush>         fill_brush_;
    RefPtr<Brush>         outline_brush_;
    RefPtr<StrokeStyle>   outline_stroke_;
    RefPtr<Texture>       texture_cached_;
    RefPtr<RenderContext> render_ctx_;
};

/** @} */

inline StringView TextActor::GetText() const
{
    return content_;
}

inline RefPtr<Font> TextActor::GetFont() const
{
    return style_.font;
}

inline TextStyle TextActor::GetStyle() const
{
    return style_;
}

inline RefPtr<TextLayout> TextActor::GetLayout() const
{
    return layout_;
}

inline RefPtr<Brush> TextActor::GetFillBrush() const
{
    return fill_brush_;
}

inline RefPtr<Brush> TextActor::GetOutlineBrush() const
{
    return outline_brush_;
}

inline RefPtr<StrokeStyle> TextActor::GetOutlineStrokeStyle() const
{
    return outline_stroke_;
}

}  // namespace kiwano
