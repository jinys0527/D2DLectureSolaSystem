/**
	@file      SigletonBase.h
	@brief     Meyers’ Singleton 패턴
	@author    SKYFISH
	@date      JUNE.2023
    @reference https://bitboom.github.io/2019-06-14/meyer-singleton
**/

#pragma once
#include <mutex>

// SingletonBase: 템플릿을 상속받는 T만 유일 인스턴스로 생성.
template<typename T>
class SingletonBase
{
public:
    
    inline static T& Instance()
    {
        static T instance;
        return instance;
    }

protected:
    SingletonBase() = default;
    virtual ~SingletonBase() = default;

private:
    // 복사/이동 금지
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;
    SingletonBase(SingletonBase&&) = delete;
    SingletonBase& operator=(SingletonBase&&) = delete;

    // 오직 T만이 생성·소멸을 제어할 수 있도록 friend 지정
    friend T;
};