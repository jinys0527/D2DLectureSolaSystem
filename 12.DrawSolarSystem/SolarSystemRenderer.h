/**
	@file      SolarSystemRenderer.h
	@brief     Transform 과 Sprite 학습을 위한 예제
	@author    skyfish (게임인재원)
	@date      JUNE.2025 
   
**/

#pragma once

#include "SingletonBase.h"
#include "D2DRender.h"

class SolarSystemRenderer : public SingletonBase<myspace::D2DRenderer>
{
	// private/protected 생성자만 허용
	SolarSystemRenderer() { /* 초기화 */ }
	~SolarSystemRenderer() { /* 정리 */ }

	// SingletonBase<MyManager> 가 친구이므로 생성자 호출 가능
	friend class SingletonBase<myspace::D2DRenderer>;

public:

};

