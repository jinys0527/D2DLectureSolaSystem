#pragma once
#include "OnlyForTestScene.h"
#include "Camera2D.h"

//
// class DefaultTestScene
// 
// 좌표계에 대해 이해해 봅시다.
// Top-Left Origin (0,0)    Bottom-Left Origin (0,0)    Center Origin (0,0)
// ┌(0,0)───▶ X        ▲ Y                              ▲ Y
// │                       │                                │
// │                       │                            ──└────▶ X
// ▼ Y                (0,0)└────▶ X                    │(0,0)
// 

class DefaultTestScene : public OnlyForTestScene
{
public:
    DefaultTestScene() = default;
    ~DefaultTestScene() override = default;

    void SetUp(HWND hWnd) override;

    void Tick(float deltaTime) override;

    void OnResize(int width, int height) override;

private:

    void ProcessKeyboardEvents() override;

    void FlipCameraMode();
    void FlipRenderMode();

    MAT3X2F GetCameraTransform();

    std::wstring& GetTitleString() const;

    bool m_IsUnityCamera = false;
    bool m_IsRenerTMOn  = false;

    D2DCamera2D m_d2dCamera;
    UnityCamera m_unityCamera;
};


