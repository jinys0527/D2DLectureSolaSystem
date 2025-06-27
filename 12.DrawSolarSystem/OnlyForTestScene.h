#pragma once

#include <string>
#include <vector>

class GameObject;

class OnlyForTestScene
{
public:
    OnlyForTestScene() = default;
    virtual ~OnlyForTestScene() = default;

    virtual void SetUp(HWND hWnd) abstract;
   
    virtual void Tick(float deltaTime) abstract;

    virtual void OnResize(int width, int height) abstract;

protected:

    void ProcessMouseEvents();

    virtual void OnMouseLButtonDown(D2D1_POINT_2F pos);
    virtual void OnMouseLButtonUp(D2D1_POINT_2F pos);
    virtual void OnMouseRButtonDown(D2D1_POINT_2F pos);
    virtual void OnMouseRButtonUp(D2D1_POINT_2F pos);
    virtual void OnMouseMove(D2D1_POINT_2F prev, D2D1_POINT_2F cur);

    virtual void ProcessKeyboardEvents() abstract;

    HWND m_hWnd = nullptr;
};


