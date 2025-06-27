// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"

// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.
// Direct3D & DXGI
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "d3dcompiler.lib")

// Direct2D & DirectWrite
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// WIC (Windows Imaging Component)
#pragma comment(lib, "windowscodecs.lib")