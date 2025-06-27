#pragma once

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject()
    {
       
    }

    virtual void Update(float deltaTime) = 0;

};
