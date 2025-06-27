#pragma once

#include "NzWndBase.h"
#include "AsepriteJsonLoader.h"

#include <wrl/client.h>
#include <d2d1helper.h>

class GameTimer;

namespace sample
{
    class D2DRenderer;
}

class TestMainApp : public NzWndBase
{
public:
    TestMainApp() = default;
    virtual ~TestMainApp() = default;

    bool Initialize();
    void Run();
    void Finalize();

private:

    void UpdateTime();
    void Render();

    void TestJsonLoad();
    void TestJsonAndImageLoad();

    void OnResize(int width, int height) override;
    void OnClose() override;

    std::shared_ptr<sample::D2DRenderer> m_Renderer = nullptr;

    std::shared_ptr<GameTimer> m_GameTimer = nullptr;

    AsepriteData m_spriteData;
};
