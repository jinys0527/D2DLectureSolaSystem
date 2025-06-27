#include "pch.h"
#include "InputManager.h"
#include "D2DTransform.h"
#include "SolarSystemRenderer.h"
#include "TransformPracticeScene.h"

using TestRenderer = myspace::D2DRenderer;
using Vec2 = MYHelper::Vector2F;

class PlanetObject
{
	PlanetObject() = delete;
	PlanetObject(const PlanetObject&) = delete;
	void operator=(const PlanetObject&) = delete;

public:
	PlanetObject(ComPtr<ID2D1Bitmap1>& bitmap, D2D1_SIZE_F size)
	{
		m_BitmapPtr = bitmap;

		m_renderTM = MYTM::MakeRenderMatrix(true);

		m_rect = D2D1::RectF(0.f, 0.f, size.width, size.height);

		m_transform.SetPivotPreset(D2DTM::PivotPreset::Center, size);
	}

	~PlanetObject() = default;

	void Update(float deltaTime)
	{
		if (m_isSelfRotation)
		{
			m_transform.Rotate(deltaTime * 36.f);
		}
	}

	void Draw(TestRenderer& testRender, D2D1::Matrix3x2F viewTM)
	{
		D2D1::Matrix3x2F worldTM = m_transform.GetWorldMatrix();

		D2D1::Matrix3x2F finalTM = m_renderTM * worldTM * viewTM;

		D2D1::ColorF boxColor = D2D1::ColorF::White;

		testRender.SetTransform(finalTM);
		testRender.DrawRectangle(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom, boxColor);

		D2D1_RECT_F dest = D2D1::RectF(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);

		testRender.DrawBitmap(m_BitmapPtr.Get(), dest);
	}

	void SetPosition(const Vec2& pos)
	{
		m_transform.SetPosition(pos);
	}

	void Move(const Vec2& offset)
	{
		m_transform.Translate(offset);
	}

	void Rotate(float angle)
	{
		m_transform.Rotate(angle);
	}

	void ToggleSelfRotation()
	{
		m_isSelfRotation = !m_isSelfRotation;
	}

	D2DTM::Transform* GetTransform()
	{
		return &m_transform;
	}

	void SetParent(PlanetObject* parent)
	{
		assert(parent != nullptr);

		if (nullptr != m_transform.GetParent())
		{
			m_transform.DetachFromParent();
		}

		m_transform.SetParent(parent->GetTransform());
	}

	void DetachFromParent()
	{
		m_transform.DetachFromParent();
	}

private:
	D2DTM::Transform m_transform;

	MAT3X2F m_renderTM;

	D2D_RECT_F m_rect = D2D1::RectF(0.f, 0.f, 100.f, 100.f);

	bool m_isSelfRotation = false;

	ComPtr<ID2D1Bitmap1> m_BitmapPtr;
};

TransformPracticeScene::~TransformPracticeScene()
{
	for (auto& planet : m_PlanetObjects)
	{
		delete planet;
	}
}

void TransformPracticeScene::SetUp(HWND hWnd)
{
	m_hWnd = hWnd;

	SetWindowText(m_hWnd,
		L"가상의 태양계를 만들어 주세요. 물리 법칙은 무시 합니다. ^^;;");

	std::cout << "태양은 자전을 해야 합니다." << std::endl;
	std::cout << "행성들은 자전을 하며 동시에 태양의 자전에 영향을 받아 공전하는 것처럼 보입니다." << std::endl;
	std::cout << "달은 자전을 하면서 동시에 지구의 자전에 영향을 받아 공전하는 것처럼 보입니다." << std::endl;
	std::cout << "회전 속도는 자유롭게 설정하세요." << std::endl;

	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/sun.png", *m_SunBitmapPtr.GetAddressOf());
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/earth.png", *m_EarthBitmapPtr.GetAddressOf());
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/moon.png", *m_MoonBitmapPtr.GetAddressOf());


	MAT3X2F cameraTM = m_UnityCamera.GetViewMatrix();
	cameraTM.Invert();

	D2D1_POINT_2F worldPt = cameraTM.TransformPoint({ -50.f, -50.f });

	PlanetObject* pSun = new PlanetObject(m_SunBitmapPtr, { 100.f, 100.f });

	pSun->SetPosition(Vec2(worldPt.x, worldPt.y));

	m_PlanetObjects.push_back(pSun);

	worldPt = cameraTM.TransformPoint({ 150.f, 150.f });

	PlanetObject* pEarth = new PlanetObject(m_EarthBitmapPtr, { 75.f, 75.f });

	pEarth->SetPosition(Vec2(worldPt.x, worldPt.y));

	pEarth->SetParent(pSun);

	m_PlanetObjects.push_back(pEarth);

	worldPt = cameraTM.TransformPoint({ 250.f, 250.f });

	PlanetObject* pMoon = new PlanetObject(m_MoonBitmapPtr, { 30.f, 30.f });

	pMoon->SetPosition(Vec2(worldPt.x, worldPt.y));

	pMoon->SetParent(pEarth);

	m_PlanetObjects.push_back(pMoon);


	RECT rc;
	if (::GetClientRect(hWnd, &rc))
	{
		float w = static_cast<float>(rc.right - rc.left);
		float h = static_cast<float>(rc.bottom - rc.top);

		m_UnityCamera.SetScreenSize(w, h);
	}
}

void TransformPracticeScene::Tick(float deltaTime)
{
	ProcessKeyboardEvents();

	for (auto& planet : m_PlanetObjects)
	{
		planet->Update(deltaTime);
	}

	MAT3X2F cameraTM = m_UnityCamera.GetViewMatrix();

	MAT3X2F renderTM = MYTM::MakeRenderMatrix(true);

	MAT3X2F finalTM = renderTM * cameraTM;

	static myspace::D2DRenderer& globalRenderer = SolarSystemRenderer::Instance();

	globalRenderer.RenderBegin();

	globalRenderer.SetTransform(finalTM);

	globalRenderer.DrawRectangle(-10.f, 10.f, 10.f, -10.f, D2D1::ColorF::Red);
	globalRenderer.DrawCircle(0.f, 0.f, 5.f, D2D1::ColorF::Red);

	for (auto& planet : m_PlanetObjects)
	{
		planet->Draw(globalRenderer, cameraTM);
	}

	globalRenderer.RenderEnd();
}

void TransformPracticeScene::OnResize(int width, int height)
{
	m_UnityCamera.SetScreenSize(width, height);
}

void TransformPracticeScene::ProcessKeyboardEvents()
{
	if (InputManager::Instance().GetKeyPressed(VK_F2))
	{
		SetPlanetSelfRotation();
	}

	static const std::vector<std::pair<int, Vec2>> kCameraMoves = {
		{VK_RIGHT, {  1.f,  0.f } },
		{VK_LEFT,  { -1.f,  0.f } },
		{VK_UP,    {  0.f,  1.f } },
		{VK_DOWN,  {  0.f, -1.f } },
	};

	for (auto& [vk, dir] : kCameraMoves)
	{
		if (InputManager::Instance().GetKeyDown(vk))
		{
			m_UnityCamera.Move(dir.x, dir.y);
		}
	}

	static const std::vector<std::pair<int, Vec2>> kPlanetMoves = {
		{'D', {  1.f,  0.f } },
		{'A', { -1.f,  0.f } },
		{'W', {  0.f,  1.f } },
		{'S', {  0.f, -1.f } },
	};

	for (auto& [vk, dir] : kPlanetMoves)
	{
		if (InputManager::Instance().GetKeyDown(vk) && !m_PlanetObjects.empty())
		{
			m_PlanetObjects.front()->Move(dir);
		}
	}
}

void TransformPracticeScene::SetPlanetSelfRotation()
{
	for (auto& planet : m_PlanetObjects)
	{
		planet->ToggleSelfRotation();
	}
}

