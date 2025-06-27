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
	friend class PlanetObject;

	TransformPracticeScene() = default;
	virtual ~TransformPracticeScene();

	void SetUp(HWND hWnd) override;

	void Tick(float deltaTime) override;

	void OnResize(int width, int height) override;

private:

	void ProcessKeyboardEvents();

	UnityCamera m_UnityCamera;

	ComPtr<ID2D1Bitmap1> m_SunBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_EarthBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_MoonBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_MercuryBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_VenusBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_MarsBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_UranusBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_JupiterBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_SaturnBitmapPtr;
	ComPtr<ID2D1Bitmap1> m_NeptuneBitmapPtr;

	std::vector<PlanetObject*> m_PlanetObjects;
};

