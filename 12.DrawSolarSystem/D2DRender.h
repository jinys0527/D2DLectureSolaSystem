#pragma once

#include <wrl/client.h>
#include <d2d1helper.h>
using namespace Microsoft::WRL;

namespace myspace
{
    class D2DRenderer
    {
    public:
        D2DRenderer() = default;

        ~D2DRenderer() { Uninitialize(); }
        
        void Initialize(HWND hwnd);

        void Uninitialize();

        void Resize(UINT width, UINT height);   

        void DrawLine(float x1, float y1, float x2, float y2, const D2D1::ColorF& color);

        void DrawCircle(float x, float y, float radius, const D2D1::ColorF& color);

        void DrawRectangle(float left, float top, float right, float bottom, const D2D1::ColorF& color);

        void DrawBitmap(ID2D1Bitmap1* bitmap, D2D1_RECT_F dest);

        void DrawMessage(const wchar_t* text, float left, float top, float width, float height, const D2D1::ColorF& color);

        void SetTransform(const D2D1_MATRIX_3X2_F tm);

        void RenderBegin();

        void RenderEnd();

        void CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1*& outBitmap);

    private:

        void CreateDeviceAndSwapChain(HWND hwnd);

        void CreateRenderTargets();

        void CreateWriteResource();

        void ReleaseRenderTargets();

    private:
        HWND m_hwnd = nullptr;

        ComPtr<ID3D11Device>           m_d3dDevice;
        ComPtr<IDXGISwapChain1>        m_swapChain;

        ComPtr<ID2D1Bitmap1>           m_targetBitmap; // D2D render target bitmap
        ComPtr<ID2D1SolidColorBrush>   m_brush;
        ComPtr<ID2D1SolidColorBrush>   m_textBrush;
        ComPtr <IDWriteTextFormat>     m_textFormat; 

        ComPtr<ID2D1Device7>           m_d2dDevice;
        ComPtr<ID2D1DeviceContext7>    m_d2dContext;     

        ComPtr<IWICImagingFactory>     m_wicFactory;
    };
}// namespace 
