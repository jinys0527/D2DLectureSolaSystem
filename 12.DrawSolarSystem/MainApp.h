#pragma once

#include "NzWndBase.h"
#include "D2DTransform.h"

//class GameTimer;
//class OnlyForTestScene;
#include "GameTimer.h"
#include "OnlyForTestScene.h"

class MainApp : public NzWndBase
{
public:
    MainApp() = default;
    virtual ~MainApp() = default;

    bool Initialize();
    void Run();
    void Finalize();

private:

    void UpdateTime();

    void OnResize(int width, int height) override;
    void OnClose() override;

    std::unique_ptr<GameTimer> m_TimerPtr = nullptr;
    
    std::shared_ptr<OnlyForTestScene> m_TestScenePtr = nullptr;


};
