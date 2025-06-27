#include "pch.h"
#include "InputManager.h"
#include "D2DTransform.h"
#include "SolarSystemRenderer.h"
#include "AnimationTestScene.h"

using TestRenderer = myspace::D2DRenderer;
using Vec2 = MYHelper::Vector2F;
// 
// 과제  : 애니메이션 테스트 씬
// 

namespace aniTest
{
    class BoxObject
    {
        BoxObject() = delete;
        BoxObject(const BoxObject&) = delete;
        void operator=(const BoxObject&) = delete;

    public:
        BoxObject(ComPtr<ID2D1Bitmap1>& bitmap, bool isPlayer = false)
        {
            m_isPlayer = isPlayer;
            m_BitmapPtr = bitmap;

            m_renderTM = MYTM::MakeRenderMatrix(true);

            D2D1_SIZE_F size = { m_rect.right - m_rect.left, m_rect.bottom - m_rect.top };

            m_transform.SetPivotPreset(D2DTM::PivotPreset::Center, size);
        }

        ~BoxObject() = default;

        void Update(float deltaTime)
        {

        }

        void Draw(TestRenderer& testRender, D2D1::Matrix3x2F viewTM)
        {
            static  D2D1_RECT_F s_rect = D2D1::RectF(0.f, 0.f, 100.f, 100.f);

            D2D1::Matrix3x2F worldTM = m_transform.GetWorldMatrix();

            D2D1::Matrix3x2F finalTM = m_renderTM * worldTM * viewTM;

            D2D1::ColorF boxColor = D2D1::ColorF::LightGray;

            testRender.SetTransform(finalTM);
            testRender.DrawRectangle(s_rect.left, s_rect.top, s_rect.right, s_rect.bottom, boxColor);

            D2D1_RECT_F dest = D2D1::RectF(s_rect.left, s_rect.top, s_rect.right, s_rect.bottom);

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

        bool IsPlayer() const
        {
            return m_isPlayer;
        }

        D2DTM::Transform* GetTransform()
        {
            return &m_transform;
        }

        void SetParent(BoxObject* parent)
        {
            assert(parent != nullptr);

            if (nullptr != m_transform.GetParent())
            {
                // 이미 부모가 있다면 부모 관계를 해제합니다.
                m_transform.DetachFromParent();
            }

            m_transform.SetParent(parent->GetTransform());
        }

        void DetachFromParent()
        {
            m_transform.DetachFromParent();
        }

        void ToggleSelected()
        {
            m_isSelected = !m_isSelected;
        }

        bool IsSelected() const
        {
            return m_isSelected;
        }

    private:
        D2DTM::Transform m_transform;

        MAT3X2F m_renderTM; // 렌더링 변환 행렬

        D2D1_RECT_F m_rect = D2D1::RectF(0.f, 0.f, 100.f, 100.f);

        bool m_isPlayer = false; // 플레이어 여부
        bool m_isSelected = false;

        ComPtr<ID2D1Bitmap1> m_BitmapPtr;

    };
}//aniTest

using namespace aniTest;

AnimationTestScene::~AnimationTestScene()
{
    for (auto& box : m_BoxObjects)
    {
        delete box;
    }
}

void AnimationTestScene::SetUp(HWND hWnd)
{
    constexpr int defaultGameObjectCount = 100;

    m_BoxObjects.reserve(defaultGameObjectCount);

    m_hWnd = hWnd;

    SetWindowText(m_hWnd, 
    L"[이동키] 플레이어 이동(새) [L-Btn] 고양이생성, [R-Btn] 고양이 선택, [F1] 고양이 클리어");

    std::cout << "카메라가 플레이어를 따라가는 구조 입니다." << std::endl;
 
    
    SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/cat.png", *m_catBitmap.GetAddressOf());
    SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/redbird1.png", *m_birdOneBitmap.GetAddressOf());
   
    // 플레이어 오브젝트 생성
    m_BoxObjects.push_back(new BoxObject(m_catBitmap, true));
    m_BoxObjects.back()->SetPosition(Vec2(-100.f, -100.f)); // 초기 위치 설정

    RECT rc;
    if (::GetClientRect(hWnd, &rc))
    {
        float w = static_cast<float>(rc.right - rc.left);
        float h = static_cast<float>(rc.bottom - rc.top);

        m_UnityCamera.SetScreenSize(w, h);
    }
}

void AnimationTestScene::Tick(float deltaTime)
{
    // 입력  이벤트 처리
    ProcessMouseEvents();
    ProcessKeyboardEvents();

    for (auto& box : m_BoxObjects)
    {
        box->Update(deltaTime);
    }

    // 카메라 업데이트

    MAT3X2F cameraTM = m_UnityCamera.GetViewMatrix();

    MAT3X2F renderTM = MYTM::MakeRenderMatrix(true); // 카메라 위치 렌더링 매트릭스

    MAT3X2F finalTM = renderTM * cameraTM;

    // 렌더링

    static myspace::D2DRenderer& globalRenderer = SolarSystemRenderer::Instance();

    wchar_t buffer[128] = L"";
    MYTM::MakeMatrixToString(cameraTM, buffer, 128);

    globalRenderer.RenderBegin();

    globalRenderer.SetTransform(finalTM);

    // 카메라 위치 표시
    globalRenderer.DrawRectangle(-10.f, 10.f, 10.f, -10.f, D2D1::ColorF::Red);
    globalRenderer.DrawCircle(0.f, 0.f, 5.f, D2D1::ColorF::Red);
    globalRenderer.DrawMessage(buffer, 10.f, 10.f, 100.f, 100.f, D2D1::ColorF::Black);

    for (auto& box : m_BoxObjects)
    {
        box->Draw(globalRenderer, cameraTM);
    }

    globalRenderer.RenderEnd();
}

void AnimationTestScene::OnResize(int width, int height)
{
    // 윈도우 크기 변경 시 카메라의 화면 크기를 업데이트
    m_UnityCamera.SetScreenSize(width, height);
}

void AnimationTestScene::ProcessKeyboardEvents()
{
    // 클리어
    if (InputManager::Instance().GetKeyPressed(VK_F1))
    {
        ClearBoxObjects();
    }

    // 플레이어를 이동
    static const std::vector<std::pair<int, Vec2>> kBoxMoves = {
      { 'D', {  1.f,  0.f } }, // D키로 오른쪽 이동
      { 'A', { -1.f,  0.f } }, // A키로 왼쪽 이동
      { 'W', {  0.f,  1.f } }, // W키로 위로 이동
      { 'S', {  0.f, -1.f } }, // S키로 아래로 이동
    };

    for (const auto& [key, dir] : kBoxMoves)
    {
        if (InputManager::Instance().GetKeyDown(key))
        {
            PlayerMove(dir);
		}
	}
   
}

void AnimationTestScene::OnMouseLButtonDown(D2D1_POINT_2F pos)
{
    AddBoxObjects(pos);
}

void AnimationTestScene::OnMouseRButtonDown(D2D1_POINT_2F pos)
{
    SelectBoxObject(pos);
}

void AnimationTestScene::AddBoxObjects(D2D1_POINT_2F point)
{
    MAT3X2F cameraTM = m_UnityCamera.GetViewMatrix();
    cameraTM.Invert();

    D2D1_POINT_2F worldPt = cameraTM.TransformPoint(point);

    BoxObject* pNewBox = new BoxObject(m_catBitmap);

    pNewBox->SetPosition(Vec2(worldPt.x, worldPt.y));

    m_BoxObjects.push_back(pNewBox);
}

void AnimationTestScene::ClearBoxObjects()
{
    // 첫 번째 (플레이어)만 제외하고 정리
    auto it = std::next(m_BoxObjects.begin());
    while (it != m_BoxObjects.end()) 
    {
        delete* it;
        it = m_BoxObjects.erase(it);
    }

    m_SelectedBoxObjects.clear();
}

void AnimationTestScene::SelectBoxObject(D2D1_POINT_2F point)
{
  
}

void AnimationTestScene::PlayerMove(const Vec2& dir)
{
    BoxObject* playerBox = GetPlayerBoxObject();
    if (playerBox)
    {
        playerBox->Move(dir); // 플레이어 박스 오브젝트를 이동시킴

        m_UnityCamera.Move(dir.x, dir.y); // 카메라도 함께 이동
	}
}


BoxObject* AnimationTestScene::GetPlayerBoxObject()
{
    assert(false == m_BoxObjects.empty());

    return m_BoxObjects.front(); // 첫 번째 박스 오브젝트가 플레이어
}
   