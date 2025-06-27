#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include "AsepriteJsonLoader.h"


// 일단, 몽땅 읽어 봅니다.

bool LoadAsepriteJson(const std::string& filePath, AsepriteData& outData)
{
    std::ifstream ifs(filePath);

    if (!ifs.is_open()) 
    {
        std::cerr << "파일을 열 수 없습니다.\n";
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    auto parsed = nlohmann::json::parse(content, nullptr, false);  // 마지막 인자 'false'는 예외 비활성화

    if (parsed.is_discarded()) 
    {
        std::cerr << "JSON 파싱 실패: 유효하지 않은 문서입니다.\n";
        return false;
    }
    const auto& root = parsed;

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
    else 
    {
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




