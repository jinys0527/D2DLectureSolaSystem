#pragma once
#include "TMHelper.h"

using Vec2F = MYHelper::Vector2F;
using MAT3X2F = D2D1::Matrix3x2F;

// ------------------------------------------------------
// 2D 카메라를 위한 기본 클래스 : 회전은 없음.
// ------------------------------------------------------

class Camera2DBase
{
public:
    Camera2DBase() noexcept
        : m_position({ 0,0 })
        , m_zoom(1.0f)
    {
    }

    void SetPosition(const Vec2F& pos) noexcept
    {
        m_position = pos;
    }

    void SetZoom(float zoom) noexcept
    {
        m_zoom = zoom;
    }

    Vec2F GetPosition() const noexcept
    {
        return m_position;
    }

    float GetZoom() const noexcept
    {
        return m_zoom;
    }

    void Move(const float deltaX, const float deltaY) noexcept
    {
        m_position.x += deltaX;
        m_position.y += deltaY;
    }

protected:
    // 파생 카메라가 자체 뷰 매트릭스 계산
    virtual MAT3X2F GetViewMatrix() abstract;

    // 월드에서 카메라 위치
    Vec2F m_position;

    // 확대/축소 (1.0 = 100%)
    float m_zoom;       
};

// ------------------------------------------------------
// 좌상단 기준의 2D 카메라 클래스
// ------------------------------------------------------

class D2DCamera2D : public Camera2DBase
{
public:
    virtual MAT3X2F GetViewMatrix() override
    {
        // 1) 줌
        auto S = MAT3X2F::Scale(m_zoom, m_zoom);
        
        // 2) 카메라 위치만큼 반대로 평행 이동
        auto T = MAT3X2F::Translation(-m_position.x, -m_position.y);

        return S * T;
    }
};

// ------------------------------------------------------
// Unity식 카트시안 좌표계 카메라, 
// 1) 스크린 중심 기준
// 2) y축 반전
// ------------------------------------------------------

class UnityCamera : public Camera2DBase
{
public:
    UnityCamera() = default;

    UnityCamera(float screenWidth,
        float screenHeight) noexcept
        : m_screenWidth(screenWidth)
        , m_screenHeight(screenHeight)
    {
        m_position = { 0, 0 };
        m_zoom = 1.0f;
    }

 
    // World → CameraPan → CenterPivot → Scale+Flip → BackPivot
    MAT3X2F GetViewMatrixLB()
    {
        // 1) 카메라 패닝: 월드 좌표를 카메라 좌표로 이동
        auto pan = MAT3X2F::Translation(
            -m_position.x,
            -m_position.y);

        // 2) 화면 중심을 (0,0)으로 옮기는 Pivot
        auto toCenter = MAT3X2F::Translation(
            -m_screenWidth * 0.5f,
            -m_screenHeight * 0.5f);

        // 3) 줌(scale)과 Y축 반전(flipY)을 한 번에
        auto scaleFlip = MAT3X2F::Scale(
            m_zoom,
            -m_zoom);

        // 4) Pivot 복원 (원래 화면 위치로 이동)
        auto back = MAT3X2F::Translation(
            m_screenWidth * 0.5f,
            m_screenHeight * 0.5f);

        //5) 모든 변환을 결합
        // 순서: pan → toCenter → scaleFlip
        return pan * toCenter * scaleFlip * back;
    }

    // World → CameraPan → Scale+Flip → CenterPivot
    MAT3X2F GetViewMatrixCenter()
    {
        // 1) 카메라 패닝: 월드 좌표를 카메라 좌표로 이동
        auto pan = MAT3X2F::Translation(
            -m_position.x,
            -m_position.y);

        // 2) 줌(scale) + Y축 반전(flipY)
        auto scaleFlip = MAT3X2F::Scale(m_zoom,
            -m_zoom);

        // 3) 화면 정중앙으로 이동
        auto center = MAT3X2F::Translation(
            m_screenWidth * 0.5f,
            m_screenHeight * 0.5f);

        // 순서대로 곱하면: pan → scale+flip → center
        return pan * scaleFlip * center;
    }

    // 카메라 뷰 매트릭스 반환 : 정중앙 기준
    virtual MAT3X2F GetViewMatrix() override
    {
        //return GetViewMatrixLB(); // 좌하단 기준
        return GetViewMatrixCenter(); // 정중앙 기준
    }

    void SetScreenSize(float width, float height) noexcept
    {
        m_screenWidth = width;
        m_screenHeight = height;
    }

private:
    float m_screenWidth = 0.f;    // 렌더 타겟 너비(px)
    float m_screenHeight = 0.f;   // 렌더 타겟 높이(px)
};