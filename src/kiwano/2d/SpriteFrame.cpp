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

#include <kiwano/2d/SpriteFrame.h>
#include <kiwano/render/BitmapCache.h>

namespace kiwano
{

SpriteFrame::SpriteFrame() {}

SpriteFrame::SpriteFrame(StringView file_path)
{
    Load(file_path);
}

SpriteFrame::SpriteFrame(const Resource& res)
{
    Load(res);
}

SpriteFrame::SpriteFrame(RefPtr<Bitmap> bitmap)
{
    SetBitmap(bitmap);
}

SpriteFrame::SpriteFrame(StringView file_path, const Rect& crop_rect)
    : SpriteFrame(file_path)
{
    SetCropRect(crop_rect);
}

SpriteFrame::SpriteFrame(const Resource& res, const Rect& crop_rect)
    : SpriteFrame(res)
{
    SetCropRect(crop_rect);
}

SpriteFrame::SpriteFrame(RefPtr<Bitmap> bitmap, const Rect& crop_rect)
    : SpriteFrame(bitmap)
{
    SetCropRect(crop_rect);
}

bool SpriteFrame::Load(StringView file_path)
{
    RefPtr<Bitmap> bitmap = new Bitmap(file_path);
    if (bitmap->IsValid())
    {
        SetBitmap(bitmap);
        return true;
    }
    return false;
}

bool SpriteFrame::Load(const Resource& res)
{
    RefPtr<Bitmap> bitmap = new Bitmap(res);
    if (bitmap->IsValid())
    {
        SetBitmap(bitmap);
        return true;
    }
    return false;
}

void SpriteFrame::SetCropRect(const Rect& crop_rect)
{
    crop_rect_ = crop_rect;
}

void SpriteFrame::SetBitmap(RefPtr<Bitmap> bitmap)
{
    bitmap_ = bitmap;

    Size bitmap_size;
    if (bitmap_)
    {
        bitmap_size = bitmap_->GetSize();
    }
    SetCropRect(Rect(Point(), bitmap_size));
}

Vector<SpriteFrame> SpriteFrame::Split(int cols, int rows, int max_num, float padding_x, float padding_y)
{
    if (cols <= 0 || rows <= 0 || max_num == 0)
        return {};

    if (!IsValid())
        return {};

    float raw_width  = crop_rect_.GetWidth();
    float raw_height = crop_rect_.GetHeight();
    float width      = (raw_width - (cols - 1) * padding_x) / cols;
    float height     = (raw_height - (rows - 1) * padding_y) / rows;

    Vector<SpriteFrame> frames;
    frames.reserve((max_num > 0) ? max_num : (rows * cols));

    int current_num = 0;

    float dty = crop_rect_.GetTop();
    for (int i = 0; i < rows; i++)
    {
        float dtx = crop_rect_.GetLeft();

        for (int j = 0; j < cols; j++)
        {
            frames.emplace_back(bitmap_, Rect{ dtx, dty, dtx + width, dty + height });
            ++current_num;

            dtx += (width + padding_x);

            if (max_num > 0 && current_num >= max_num)
                break;
        }
        dty += (height + padding_y);

        if (max_num > 0 && current_num >= max_num)
            break;
    }
    return frames;
}

}  // namespace kiwano
