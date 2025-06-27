#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

//git 쪽 소스 반영. 프레임 읽어서 애니메이션 하는 거

// m_ 접두사 네이밍 규칙
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
};

bool LoadAsepriteJson(const std::string& filePath, AsepriteData& outData)
{
    std::ifstream ifs(filePath);
    if (!ifs)
    {
        std::cerr << "Failed to open JSON: " << filePath << '\n';
        return false;
    }

    nlohmann::json root;
    try { ifs >> root; }
    catch (const std::exception& e)
    {
        std::cerr << "JSON parse error: " << e.what() << '\n';
        return false;
    }

    // 1) Frames: object or array 형태 모두 지원
    auto& framesNode = root["frames"];
    if (framesNode.is_object())
    {
        for (auto it = framesNode.begin(); it != framesNode.end(); ++it)
        {
            const auto& f = it.value();
            FrameData fd;
            fd.m_filename = it.key();
            fd.m_frame.m_x = f["frame"]["x"];
            fd.m_frame.m_y = f["frame"]["y"];
            fd.m_frame.m_w = f["frame"]["w"];
            fd.m_frame.m_h = f["frame"]["h"];
            fd.m_rotated = f["rotated"];
            fd.m_trimmed = f["trimmed"];
            fd.m_spriteSourceSize.m_x = f["spriteSourceSize"]["x"];
            fd.m_spriteSourceSize.m_y = f["spriteSourceSize"]["y"];
            fd.m_spriteSourceSize.m_w = f["spriteSourceSize"]["w"];
            fd.m_spriteSourceSize.m_h = f["spriteSourceSize"]["h"];
            fd.m_sourceSize.m_w = f["sourceSize"]["w"];
            fd.m_sourceSize.m_h = f["sourceSize"]["h"];
            fd.m_duration = f["duration"];
            outData.m_frames.push_back(fd);
        }
    }
    else if (framesNode.is_array())
    {
        for (const auto& f : framesNode)
        {
            FrameData fd;
            fd.m_filename = f["filename"];
            fd.m_frame.m_x = f["frame"]["x"];
            fd.m_frame.m_y = f["frame"]["y"];
            fd.m_frame.m_w = f["frame"]["w"];
            fd.m_frame.m_h = f["frame"]["h"];
            fd.m_rotated = f["rotated"];
            fd.m_trimmed = f["trimmed"];
            fd.m_spriteSourceSize.m_x = f["spriteSourceSize"]["x"];
            fd.m_spriteSourceSize.m_y = f["spriteSourceSize"]["y"];
            fd.m_spriteSourceSize.m_w = f["spriteSourceSize"]["w"];
            fd.m_spriteSourceSize.m_h = f["spriteSourceSize"]["h"];
            fd.m_sourceSize.m_w = f["sourceSize"]["w"];
            fd.m_sourceSize.m_h = f["sourceSize"]["h"];
            fd.m_duration = f["duration"];
            outData.m_frames.push_back(fd);
        }
    }
    else {
        std::cerr << "Unsupported 'frames' format" << '\n';
        return false;
    }

    // 2) frameTags (옵션)
    if (root["meta"].contains("frameTags"))
    {
        for (const auto& t : root["meta"]["frameTags"]) {
            Tag tag;
            tag.m_name = t["name"];
            tag.m_from = t["from"];
            tag.m_to = t["to"];
            tag.m_direction = t["direction"];
            outData.m_frameTags.push_back(tag);
        }
    }

    // 3) slices (옵션)
    if (root["meta"].contains("slices"))
    {
        for (const auto& s : root["meta"]["slices"]) {
            Slice slice;
            slice.m_name = s["name"];
            for (const auto& key : s["keys"]) {
                SliceKey sk;
                sk.m_frame = key["frame"];
                sk.m_bounds.m_x = key["bounds"]["x"];
                sk.m_bounds.m_y = key["bounds"]["y"];
                sk.m_bounds.m_w = key["bounds"]["w"];
                sk.m_bounds.m_h = key["bounds"]["h"];
                sk.m_pivot.m_x = key["pivot"]["x"];
                sk.m_pivot.m_y = key["pivot"]["y"];
                slice.m_keys.push_back(sk);
            }
            outData.m_slices.push_back(slice);
        }
    }

    return true;
}

int main()
{
    AsepriteData data;
    std::string path = "../resource/redbirdSheet.json";

    if (!LoadAsepriteJson(path, data))
        return -1;

    std::cout << "Loaded " << data.m_frames.size() << " frames\n";
    for (auto& f : data.m_frames)
        std::cout << f.m_filename << " @ (" << f.m_frame.m_x << "," << f.m_frame.m_y << ")\n";

    return 0;
}
