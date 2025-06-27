#pragma once
#include "OnlyForTestScene.h"
#include "Camera2D.h"
#include <list>
#include <vector>
#include <wrl/client.h>
#include <d2d1_1.h>

class PlanetObject;

class TransformPracticeScene : public OnlyForTestScene
{
public:
	TransformPracticeScene() = default;
	virtual ~TransformPracticeScene();

	void SetUp(HWND hWnd) override;

	void Tick(float deltaTime) override;

	void OnResize(int width, int height) override;

private:

	void ProcessKeyboardEvents();

	void SetPlanetSelfRotation();

	UnityCamera m_UnityCamera;

	ComPtr<ID2D1Bitmap1> m_SunBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_EarthBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_MoonBitmapPtr;

	std::vector<PlanetObject*> m_PlanetObjects;
};

