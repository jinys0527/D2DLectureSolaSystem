#include "pch.h"
#include "SolarSystemRenderer.h"
#include "InputManager.h"
#include "DefaultScene.h"

std::wstring& DefaultTestScene::GetTitleString() const
{
    static const std::wstring unity = L"유니티 카메라 모드";
    static const std::wstring d2d = L"D2D 카메라 모드";

    static std::wstring ret;
    ret.clear();
    ret = (m_IsUnityCamera ? unity : d2d);
    ret += L" - ";
    ret += (m_IsRenerTMOn ? L"렌더링 TM 적용" : L"렌더링 TM 미적용");

    return ret;
}

void DefaultTestScene::SetUp(HWND hWnd)
{
    m_hWnd = hWnd;

    SetWindowText(m_hWnd, L"이동 키로 카메라 이동, F1 좌표계 모드 전환, F2 렌더 모드 전환");

    RECT rc;
    if (::GetClientRect(hWnd, &rc))
    {
        float w = static_cast<float>(rc.right - rc.left);
        float h = static_cast<float>(rc.bottom - rc.top);

        m_unityCamera.SetScreenSize(w, h);
    }

}

void DefaultTestScene::Tick(float deltaTime)
{
    // 키보트 이벤트 처리
    
    ProcessKeyboardEvents();

    // 카메라 업데이트

    MAT3X2F cameraTM = GetCameraTransform();

    MAT3X2F renderTM = MAT3X2F::Identity();
        
    if (m_IsRenerTMOn) renderTM = MYTM::MakeRenderMatrix(m_IsUnityCamera);

    MAT3X2F finalTM =  renderTM * cameraTM;

    // 카메라 위치 출력
    wchar_t buffer[128] = L"";
    MYTM::MakeMatrixToString(cameraTM, buffer, 128);

    // 렌더링

    static myspace::D2DRenderer& globalRenderer = SolarSystemRenderer::Instance();;

    globalRenderer.RenderBegin();

    globalRenderer.SetTransform(finalTM);

    globalRenderer.DrawRectangle(-10.f, 10.f, 10.f, -10.f, D2D1::ColorF::Red);
    globalRenderer.DrawCircle(0.f, 0.f, 5.f, D2D1::ColorF::Red);
    globalRenderer.DrawMessage(buffer, 10.f, 10.f, 100.f, 100.f, D2D1::ColorF::Black);

    globalRenderer.RenderEnd();
}

void DefaultTestScene::OnResize(int width, int height)
{
    // 윈도우 크기 변경 시 카메라의 화면 크기를 업데이트
    m_unityCamera.SetScreenSize(width, height);
}

void DefaultTestScene::ProcessKeyboardEvents()
{
    // F1 키를 눌렀을 때 카메라 모드 전환
    if (InputManager::Instance().GetKeyPressed(VK_F1))
    {
        FlipCameraMode();
    }

    // F2 키를 눌렀을 때 렌더링 모드 전환
    if (InputManager::Instance().GetKeyPressed(VK_F2))
    {
        FlipRenderMode();
    }

    // 카메라 이동 처리
    if (InputManager::Instance().GetKeyDown(VK_RIGHT))
    {
        m_d2dCamera.Move(1.f, 0.f);

        m_unityCamera.Move(1.f, 0.f);
    }

    if (InputManager::Instance().GetKeyDown(VK_LEFT))
    {
        m_d2dCamera.Move(-1.f, 0.f);

        m_unityCamera.Move(-1.f, 0.f);
    }

    if (InputManager::Instance().GetKeyDown(VK_UP))
    {
        m_d2dCamera.Move(0.f, 1.f);

        m_unityCamera.Move(0.f, 1.f);
    }

    if (InputManager::Instance().GetKeyDown(VK_DOWN))
    {
        m_d2dCamera.Move(0.f, -1.f);

        m_unityCamera.Move(0.f, -1.f);
    }
}

void DefaultTestScene::FlipCameraMode()
{
    m_IsUnityCamera = !m_IsUnityCamera;

    SetWindowText(m_hWnd, GetTitleString().c_str());
}

void DefaultTestScene::FlipRenderMode()
{
    m_IsRenerTMOn = !m_IsRenerTMOn;

    SetWindowText(m_hWnd, GetTitleString().c_str());
}

MAT3X2F DefaultTestScene::GetCameraTransform()
{
    if (m_IsUnityCamera)
    {
        return m_unityCamera.GetViewMatrix();
    }
 
    return m_d2dCamera.GetViewMatrix();
}
