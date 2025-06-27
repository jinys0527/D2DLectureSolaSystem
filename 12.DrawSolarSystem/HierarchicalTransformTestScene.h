#pragma once
#include "OnlyForTestScene.h"
#include "Camera2D.h"
#include <list>
#include <wrl/client.h>
#include <d2d1_1.h>

class BoxObject;

class HierarchicalTransformTestScene : public OnlyForTestScene
{
public:
    HierarchicalTransformTestScene() = default;
    virtual ~HierarchicalTransformTestScene();
    
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

    void SetBoxSelfRotation();

    void UpdateRelationship();

    UnityCamera m_UnityCamera;

    ComPtr<ID2D1Bitmap1> m_BitmapPtr;

    std::vector<BoxObject*> m_BoxObjects;

    std::list<BoxObject*> m_SelectedBoxObjects; // 선택된 박스 객체들
};

