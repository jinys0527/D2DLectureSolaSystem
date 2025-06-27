#pragma once
#include "OnlyForTestScene.h"
#include "Camera2D.h"
#include <list>
#include <wrl/client.h>
#include <d2d1_1.h>

namespace aniTest
{
	class BoxObject;
}

class AnimationTestScene : public OnlyForTestScene
{
public:
    AnimationTestScene() = default;
    virtual ~AnimationTestScene();
    
    void SetUp(HWND hWnd) override;

    void Tick(float deltaTime) override;

    void OnResize(int width, int height) override;

private:

    void ProcessKeyboardEvents();
    
    void OnMouseLButtonDown(D2D1_POINT_2F point) override;

    void OnMouseRButtonDown(D2D1_POINT_2F point) override;

    void AddBoxObjects(D2D1_POINT_2F point);

    void ClearBoxObjects();

    void SelectBoxObject(D2D1_POINT_2F point);

    void PlayerMove(const MYHelper::Vector2F& dir);

    aniTest::BoxObject* GetPlayerBoxObject();

    UnityCamera m_UnityCamera;

    ComPtr<ID2D1Bitmap1> m_catBitmap;  // Cat
    ComPtr<ID2D1Bitmap1> m_birdOneBitmap; // Bird1
    //ComPtr<ID2D1Bitmap1> m_birdSpriteSheet; // 애니메이션은 별도 클래스로

    std::vector<aniTest::BoxObject*> m_BoxObjects;
    std::list<aniTest::BoxObject*> m_SelectedBoxObjects;

};

