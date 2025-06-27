#pragma once

#include <wrl/client.h>
#include <d2d1_1.h>

struct Rect
{
    int m_x;
    int m_y;
    int m_w;
    int m_h;
};

struct SliceKey
{
    int m_frame;
    Rect m_bounds;
    Rect m_pivot;
};

struct Slice
{
    std::string m_name;
    std::vector<SliceKey> m_keys;
};

struct FrameData
{
    std::string m_filename;
    Rect m_frame;
    bool m_rotated;
    bool m_trimmed;
    Rect m_spriteSourceSize;
    Rect m_sourceSize;
    int m_duration;
};

struct Tag
{
    std::string m_name;
    int m_from;
    int m_to;
    std::string m_direction;
};

struct AsepriteData
{
    std::vector<FrameData> m_frames;
    std::vector<Tag> m_frameTags;
    std::vector<Slice> m_slices;

    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_image;
};

bool LoadAsepriteJson(const std::string& filePath, AsepriteData& outData);

namespace sample
{
    class D2DRenderer;
}

